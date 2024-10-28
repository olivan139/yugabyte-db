#pragma once

#include "yb/master/master.h"
#include "yb/util/thread.h"
#include "yb/common/sys_metrics_collector.h"

namespace yb {

namespace master {

class SysMetricsTableUpdater {
 public:
  SysMetricsTableUpdater(Master* master, int period) : master_(DCHECK_NOTNULL(master)),  period_(period) {};
  void Init();
  Status Start();
  void Shutdown();
  ~SysMetricsTableUpdater();

 private:
  void CollectMetricsLoop();
  void CreateSysNamespace();
  void CreateSysTable(const NamespaceId& namespace_id);

  Master* master_;
  int period_;
  std::atomic<bool> is_shutdown_{false};
  SysMetricsCollector collector_;
};

} // namespace master
} // namepace yb