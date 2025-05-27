#include "tl_common.h"
#include "zb_common.h"
#include "endpoint.h"
#include "basic_cluster.h"
#include "cluster_common.h"
#include "device_config/device_config.h"
#include "custom_zcl/zcl_basic_config.h"
#include "base_components/network_indicator.h"
#include "configs/nv_slots_cfg.h"


const u8 groupNameSupport  = 0x0;


status_t group_cluster_callback_trampoline(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{
  return(ZCL_STA_SUCCESS);
}


void group_cluster_add_to_endpoint(zigbee_basic_cluster *cluster, zigbee_endpoint *endpoint)
{
  SETUP_ATTR(0, ZCL_ATTRID_GROUP_NAME_SUPPORT, ZCL_DATA_TYPE_BITMAP8, ACCESS_CONTROL_READ, groupNameSupport);

  zigbee_endpoint_add_cluster(endpoint, 1, ZCL_CLUSTER_GEN_GROUPS);
  zcl_specClusterInfo_t *info = zigbee_endpoint_reserve_info(endpoint);
  info->clusterId           = ZCL_CLUSTER_GEN_GROUPS;
  info->manuCode            = MANUFACTURER_CODE_NONE;
  info->attrNum             = 1;
  info->attrTbl             = cluster->attr_infos;
  info->clusterRegisterFunc = zcl_group_register;
  info->clusterAppCb        = group_cluster_callback_trampoline;
}
