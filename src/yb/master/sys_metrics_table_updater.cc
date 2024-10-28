#include "sys_metrics_table_updater.h"
#include "yb/util/thread.h"
#include "yb/master/master_ddl.pb.h"
#include "yb/client/table.h"
#include "yb/client/table_creator.h"
#include "yb/client/schema.h"

namespace yb {

namespace master {

void SysMetricsTableUpdater::Init() {
  // CreateSysNamespace();
  collector_ = SysMetricsCollector();
}

void SysMetricsTableUpdater::CreateSysNamespace() {
  CreateNamespaceRequestPB req;
  CreateNamespaceResponsePB resp;

  req.set_name("sys_table");
  req.set_database_type(yb::YQL_DATABASE_PGSQL);
  req.set_colocated(true);
  req.set_creator_role_name("yugabyte");

  Status s = master_->catalog_manager()->CreateNamespace(&req, &resp, nullptr, master_->catalog_manager()->GetLeaderEpochInternal());
  if (!s.IsAlreadyPresent()) {
    LOG(WARNING) << "unable to create database for sys metrics: " << s.ToString();
  }
}

//void SysMetricsTableUpdater::CreateSysTable(const NamespaceId& namespace_id) {
//  CreateTableRequestPB req;
//  CreateTableResponsePB resp;
//
//  client::YBSchema schema;
//  client::YBSchemaBuilder b;
//  b.AddColumn(kKeyColumnName)->Type(DataType::INT32)->NotNull()->HashPrimaryKey();
//  b.AddColumn(kValueColumnName)->Type(DataType::INT32)->NotNull();
//  RETURN_NOT_OK(b.Build(&schema));
//
//  std::unique_ptr<client::YBTableCreator> table_creator(client_->NewTableCreator());
//  RETURN_NOT_OK(table_creator->table_name(kYbTableName).schema(&schema).wait(true).Create());
//  RETURN_NOT_OK(client_->OpenTable(kYbTableName, &table_));
//
//  req.set_creator_role_name("yugabyte");
//  req.set_name("sys_metrics");
//  req.set_allocated_namespace_()
//}

void SysMetricsTableUpdater::CollectMetricsLoop() {
  for (;;) {
    if (is_shutdown_.load()) {
      return;
    }

    auto metrics = collector_.CollectSysMetrics();
    LOG(INFO) << "cpu usage: " << metrics.cpu_used << " ram: " << metrics.rss_mem_used << " virt: " << metrics.virt_mem_used;
    std::this_thread::sleep_for(std::chrono::seconds(period_));
  }
}

Status SysMetricsTableUpdater::Start() {
  scoped_refptr<yb::Thread> thread;
  RETURN_NOT_OK(yb::Thread::Create("master", "metrics collection",
                                   &SysMetricsTableUpdater::CollectMetricsLoop, this, &thread));
  return Status::OK();
}

void SysMetricsTableUpdater::Shutdown() {
    is_shutdown_.exchange(true);
}

SysMetricsTableUpdater::~SysMetricsTableUpdater() {
  Shutdown();
}

} // namespace master
} // namespace yb