#include "yb/master/catalog_manager.h"
#include "yb/master/resource_manager.service.h"
#include "yb/master/master_service.h"
#include "yb/master/master_service_base.h"
#include "yb/master/master_service_base-internal.h"
#include "yb/master/test_async_rpc_manager.h"
#include "yb/master/ysql_backends_manager.h"
#include "yb/util/flags.h"
namespace yb {
namespace master {

namespace {

class MasterResourceManagerServiceImpl : public MasterServiceBase, public MasterResourceManagerIf {
 public:
  explicit MasterResourceManagerServiceImpl(Master* master)
      : MasterServiceBase(master), MasterResourceManagerIf(master->metric_entity()) {}

  void GetNamespaceDiskUsage(
      const ::yb::master::GetNamespaceDiskUsageRequestPB* req,
      ::yb::master::GetNamespaceDiskUsageResponsePB* resp, rpc::RpcContext rpc) override {
    HANDLE_ON_LEADER_WITHOUT_LOCK(ResourceManager, GetNamespaceDiskUsage);
  };

  void GetDiskSpaceLeftByTabletId(
      const ::yb::master::GetDiskSpaceLeftByTabletIdRequestPB* req,
      ::yb::master::GetDiskSpaceLeftByTabletIdResponsePB* resp, rpc::RpcContext rpc) override {
    HANDLE_ON_LEADER_WITHOUT_LOCK(ResourceManager, GetDiskSpaceLeftByTabletId);
  };

};
} // namespace
std::unique_ptr<rpc::ServiceIf> MakeMasterResourceManagerService(Master* master) {
  return std::make_unique<MasterResourceManagerServiceImpl>(master);
}

} // namespace master
} // namespace yb