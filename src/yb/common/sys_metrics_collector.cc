#include "sys_metrics_collector.h"
#include <fstream>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include "yb/util/logging.h"

namespace yb {

SysMetrics SysMetricsCollector::CollectSysMetrics() {
  SysMetrics metrics;
  std::ostringstream filename;
  filename << "/proc/" << getpid() << "/stat";

  auto stat_values = split(readline(filename.str()));

  LOG(INFO) << "here0";
  double utime_sec = boost::lexical_cast<double>(stat_values[13]) / clk_tck_;
  LOG(INFO) << "here1";
  double stime_sec = boost::lexical_cast<double>(stat_values[14]) / clk_tck_;
  LOG(INFO) << "here2";
  double start_time_sec = boost::lexical_cast<double>(stat_values[21]) / clk_tck_;
  LOG(INFO) << "here3";

  auto uptime_values = split(readline("/proc/uptime"));
  double uptime = boost::lexical_cast<double>(uptime_values[0]);
  LOG(INFO) << "here4";

  metrics.cpu_used = (utime_sec + stime_sec) * 100 / (uptime - start_time_sec);
  metrics.virt_mem_used = boost::lexical_cast<double>(stat_values[22]) / 1024.0;
  LOG(INFO) << "here5";
  metrics.rss_mem_used = boost::lexical_cast<double>(stat_values[23]) * page_size_kb_;
  LOG(INFO) << "here6";

  return metrics;
}

std::vector<std::string> SysMetricsCollector::split(const std::string& line) {
  std::vector<std::string> out;

  size_t last, curr;
  for (last = curr = 0; curr < line.size(); ++curr) {
    if (line[curr] != ' ') {
      continue;
    }

    auto arg = line.substr(last, curr - last);
    if (!arg.empty()) {
      out.emplace_back(std::move(arg));
    }

    last = curr + 1;
  }

  if (last < curr) {
    out.emplace_back(line.substr(last, curr - last));
  }

  return out;
}

std::string SysMetricsCollector::readline(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    LOG(WARNING) << "couldn't open " << filename;
  }

  std::string line;
  if (!std::getline(file, line)) {
    LOG(WARNING) << "couldn't read line from " << filename;
  }

  return line;
}

} // namespace yb