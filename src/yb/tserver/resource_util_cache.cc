#include "resource_util_cache.h"
namespace yb {

namespace tserver {

Status ResourceUtilCache::Start() {
  RETURN_NOT_OK(Thread::Create(
      "ResourceCache", "resource_util_cache",
      std::bind(&ResourceUtilCache::ResourceUtilLoop, this), &task_thread_));
  return Status::OK();
}

uint64 ResourceUtilCache::GetDiskUsageLeft(const TabletId& tablet_id) {
  std::shared_lock<std::shared_mutex> lock(lock_);
  auto unit = disk_usage_map_.find(tablet_id);
  if (unit != disk_usage_map_.end()) {
    return unit->second;
  }

  client::YBClient* client = t_server_->client_future().get();
  uint64 disk_space_left = 0;

  auto status = client->GetDiskSpaceLeftByTabletId(tablet_id, disk_space_left);
  if (!status.IsOk()) {
    LOG(INFO) << "unable to execute rpc call NamespaceDiskUsage";
  }

  return disk_space_left;
}

void ResourceUtilCache::ResourceUtilLoop() {
  for (;;) {
    if (is_shutdown_.load()) {
      return;
    }

    {
      std::unique_lock<std::shared_mutex> lock(lock_);
      client::YBClient* client = t_server_->client_future().get();

      auto status = client->GetNamespaceDiskUsage(disk_usage_map_);

      if (!status.IsOk()) {
        LOG(INFO) << "unable to execute rpc call NamespaceDiskUsage";
      }
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

void ResourceUtilCache::Shutdown() {
  VLOG(2) << "shutdown proccess started";
  is_shutdown_.exchange(true);
}

ResourceUtilCache::~ResourceUtilCache() {
  Shutdown();
}

} // namespace tserver
} // namespace yb