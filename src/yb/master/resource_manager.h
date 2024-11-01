#pragma once

#include <memory>
#include "yb/master/master.h"
#include "yb/util/thread.h"
#include "yb/rpc/scheduler.h"
#include "yb/master/resource_manager.service.h"

namespace yb {

namespace master {

typedef uint64 DiskSizeBytes;

class ResourceManager {
 public:
  explicit ResourceManager(Master* master)
      : master_(DCHECK_NOTNULL(master)) {}

  Status Start();
  Status GetNamespaceDiskUsage(const GetNamespaceDiskUsageRequestPB* req,
                               GetNamespaceDiskUsageResponsePB* resp);
  Status GetDiskSpaceLeftByTabletId(const GetDiskSpaceLeftByTabletIdRequestPB* req,
                                    GetDiskSpaceLeftByTabletIdResponsePB* resp);
  void Shutdown();
  ~ResourceManager();

  void DEBUG_printCache() {
    for (const auto& unit : disk_usage_map_) {
      std::cout << "namespace_id:" << unit.first << " --- " << "disk_space_left:" << unit.second << "\n";
    }
  }

 private:
  void DiskUsageLoop();
  void CalculateDiskUsage();

  rpc::ScheduledTaskTracker scheduler_;

  std::shared_mutex lock_;
  std::unordered_map<NamespaceId, DiskSizeBytes> disk_usage_map_;
  std::unordered_map<TabletId, NamespaceId> tablet_to_namespace_map_;
  bool DEBUG_has_been_printed{false};

  std::atomic<bool> is_shutdown_ = {false};
  scoped_refptr<yb::Thread> task_thread_;
  Master* master_;
};

}  // namespace master
}  // namespace yb