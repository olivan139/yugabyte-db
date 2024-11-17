#pragma once

#include <unordered_map>
#include "tablet_server_interface.h"
#include "yb/util/thread.h"
#include "yb/client/client.h"

namespace yb {

namespace tserver {

class ResourceUtilCache {
 public:
  ResourceUtilCache(TabletServerIf* t_server) : t_server_(DCHECK_NOTNULL(t_server)) {}
  Status Start();

  uint64 GetDiskUsageLeft(const TabletId& tablet_id = "");

  void DEBUG_printMap() {
    for (const auto& unit : disk_usage_map_) {
      LOG(INFO) << "tablet_id: " << unit.first << " disk_left: " << unit.second << "\n";
    }
  }

  void Shutdown();
  ~ResourceUtilCache();

 private:
  void ResourceUtilLoop();

  std::unordered_map<TabletId, uint64> disk_usage_map_;
  std::shared_mutex lock_;
  std::atomic<bool> is_shutdown_ = {false};
  scoped_refptr<yb::Thread> task_thread_;
  TabletServerIf* t_server_;
};

} // namespace tserver
} // namespace yb