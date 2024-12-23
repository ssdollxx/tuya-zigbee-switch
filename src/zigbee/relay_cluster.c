#include "tl_common.h"
#include "zb_common.h"
#include "endpoint.h"
#include "relay_cluster.h"
#include "cluster_common.h"


status_t relay_cluster_callback(zigbee_relay_cluster *cluster, zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t relay_cluster_callback_trampoline(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

zigbee_relay_cluster *relay_cluster_by_endpoint[10];

void relay_cluster_add_to_endpoint(zigbee_relay_cluster *cluster, zigbee_endpoint *endpoint) {
    relay_cluster_by_endpoint[endpoint->index] = cluster;
    cluster->endpoint = endpoint->index;

    SETUP_ATTR(0, ZCL_ATTRID_ONOFF, ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, cluster->relay->on);

    zigbee_endpoint_add_cluster(endpoint, 1, ZCL_CLUSTER_GEN_ON_OFF);
    zcl_specClusterInfo_t *info = zigbee_endpoint_reserve_info(endpoint);
    info->clusterId = ZCL_CLUSTER_GEN_ON_OFF;
    info->manuCode = MANUFACTURER_CODE_NONE;
    info->attrNum = 1;
    info->attrTbl = cluster->attr_infos;
    info->clusterRegisterFunc = zcl_onOff_register;
    info->clusterAppCb = relay_cluster_callback_trampoline;
}

status_t relay_cluster_callback_trampoline(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{	
	return relay_cluster_callback(relay_cluster_by_endpoint[pAddrInfo->dstEp], pAddrInfo, cmdId, cmdPayload);
}

status_t relay_cluster_callback(zigbee_relay_cluster *cluster, zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload)
{	
    if(cmdId == ZCL_CMD_ONOFF_ON){
		printf("ON\r\n");
		relay_on(cluster->relay);
	} else if(cmdId == ZCL_CMD_ONOFF_OFF){
		printf("OFF\r\n");
		relay_off(cluster->relay);
	} else if(cmdId == ZCL_CMD_ONOFF_TOGGLE){
		printf("TOGGLE\r\n");
		relay_toggle(cluster->relay);
	} else {
		printf("Unknown command: %d\r\n", cmdId);
	}

	return ZCL_STA_SUCCESS;
}


void relay_cluster_report(zigbee_relay_cluster *cluster)
{	
    if(zb_isDeviceJoinedNwk()){
		printf("Send Report\r\n");

        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        
    	zclAttrInfo_t *pAttrEntry;
    	pAttrEntry = zcl_findAttribute(cluster->endpoint, ZCL_CLUSTER_GEN_ON_OFF, ZCL_ATTRID_ONOFF);
    	zcl_sendReportCmd(cluster->endpoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
    			ZCL_CLUSTER_GEN_ON_OFF, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
    }
}

