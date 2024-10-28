#pragma once

#include <vector>
#include <string>

#include "yb/gutil/integral_types.h"
#include <unistd.h>

namespace yb {

struct SysMetrics {
  double virt_mem_used;
  double rss_mem_used;
  float cpu_used;
};

class SysMetricsCollector {
 public:
  SysMetricsCollector()
      : clk_tck_(sysconf(_SC_CLK_TCK)), page_size_kb_(sysconf(_SC_PAGE_SIZE) / 1024) {}
  SysMetrics CollectSysMetrics();

 private:
  std::vector<std::string> split(const std::string& line);
  std::string readline(const std::string& filename);

  long clk_tck_;
  long page_size_kb_;
};

} // namespace yb