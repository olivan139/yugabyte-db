#include "resource_manager.h"
#include "yb/rpc/messenger.h"
#include "yb/master/catalog_entity_info.h"

namespace yb {
namespace master {

Status ResourceManager::Start() {
  DEBUG_printCache();
  DEBUG_printMapping();
  RETURN_NOT_OK(Thread::Create(
      "ResourceManager", "resource_manager_scheduler",
      std::bind(&ResourceManager::DiskUsageLoop, this), &task_thread_));
  return Status::OK();
}

void ResourceManager::Shutdown() {
  is_shutdown_.exchange(true);
}

void ResourceManager::DiskUsageLoop() {
  if (is_shutdown_.load()) {
    LOG(INFO) << "disk usage shutdown proccess started";
    return;
  }

  std::this_thread::sleep_for(std::chrono::seconds(1));
  CalculateDiskUsage();
}

void ResourceManager::CalculateDiskUsage() {
  std::vector<scoped_refptr<NamespaceInfo>> namespaces;
  master_->catalog_manager()->GetAllNamespaces(&namespaces, true);

  {
    std::unique_lock lock(lock_);
    for (const auto& ns : namespaces) {
      if (ns->disk_limit() == 0) {
        disk_usage_map_[ns->id()] = INT_MAX;
      } else {
        disk_usage_map_[ns->id()] = ns->disk_limit();
      }

      std::vector<TableInfoPtr> tables =
          master_->catalog_manager()->GetTables(GetTablesMode::kRunning);

      for (const auto& table : tables) {
        if (table->GetTableType() != PGSQL_TABLE_TYPE) {
          continue;
        }

        auto tablets_result = table->GetTablets();
        std::vector<std::shared_ptr<TabletInfo>> tablets;
        Status s = tablets_result.MoveTo(&tablets);
        if (!s.IsOk()) {
          continue;
        }

        for (const auto& tablet : tablets) {
          auto drive_info = tablet->GetLeaderReplicaDriveInfo();
          if (!drive_info.ok()) {
            continue;
          }

          tablet_to_namespace_map_[tablet->id()] = ns->id();
          DiskSizeBytes tablet_total_size =
              drive_info.get().wal_files_size + drive_info.get().sst_files_size;

          if (disk_usage_map_[ns->id()] <= tablet_total_size) {
            disk_usage_map_[ns->id()] = 0;
          } else {
            disk_usage_map_[ns->id()] -= tablet_total_size;
          }
        }
      }
    }
  }

  DiskUsageLoop();
}

Status ResourceManager::GetNamespaceDiskUsage(const GetNamespaceDiskUsageRequestPB* req,
                                              GetNamespaceDiskUsageResponsePB* resp) {

  std::shared_lock lock(lock_);
  for (const auto& unit : tablet_to_namespace_map_) {
    GetNamespaceDiskUsageResponsePB_NamespaceDiskUsageInfo *ns = resp->add_namespaces();
    ns->set_tablet_id(unit.first);
    ns->set_disk_space_left(disk_usage_map_[unit.second]);
  }

  return Status::OK();
}

Status ResourceManager::GetDiskSpaceLeftByTabletId(const GetDiskSpaceLeftByTabletIdRequestPB* req,
                                                   GetDiskSpaceLeftByTabletIdResponsePB* resp) {
  {
    std::shared_lock lock(lock_);
    auto ns_id = tablet_to_namespace_map_.find(req->tablet_id());
    if (ns_id != tablet_to_namespace_map_.end()) {
      resp->set_disk_space_left(disk_usage_map_[req->tablet_id()]);
      return Status::OK();
    }
  }

  auto tablet_info = VERIFY_RESULT(master_->catalog_manager()->GetTabletInfo(req->tablet_id()));
  NamespaceId ns_id = tablet_info->table()->namespace_id();
  auto drive_info = tablet_info->GetLeaderReplicaDriveInfo();
  if (drive_info.ok()) {
    auto unit = disk_usage_map_.find(ns_id);
    uint64 total_size = drive_info->sst_files_size + drive_info->wal_files_size;

    if (unit != disk_usage_map_.end()) {
      if (disk_usage_map_[ns_id] <= total_size) {
        disk_usage_map_[ns_id] = 0;
      } else {
        disk_usage_map_[ns_id] -= total_size;
      }
    } else {
      /// TODO: create GetNamespaceInfo(const NamespaceId& id) in catalog manager interface.
      std::vector<scoped_refptr<NamespaceInfo>> namespaces;
      master_->catalog_manager()->GetAllNamespaces(&namespaces, true);
      for (const auto& ns : namespaces) {
        if (ns->id() == ns_id) {
          tablet_to_namespace_map_[req->tablet_id()] = ns_id;

          if (ns->disk_limit() <= total_size) {
            disk_usage_map_[ns_id] = 0;
          } else {
            disk_usage_map_[ns_id] = ns->disk_limit() - total_size;
          }
          break;
        }
      }
    }
  }

  return Status::OK();
}

ResourceManager::~ResourceManager() {
  Shutdown();
}

} // namespace master
} // namespace yb