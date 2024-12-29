#include "tl_common.h"
#include "zb_common.h"
#include "endpoint.h"
#include "relay_cluster.h"
#include "cluster_common.h"
#include "configs/nv_slots_cfg.h"



status_t relay_cluster_callback(zigbee_relay_cluster *cluster, zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);
status_t relay_cluster_callback_trampoline(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload);

void relay_cluster_on_relay_change(zigbee_relay_cluster *cluster, u8 state);
void relay_cluster_on_write_attr(zigbee_relay_cluster *cluster);

void relay_cluster_store_attrs_to_nv(zigbee_relay_cluster *cluster);
void relay_cluster_load_attrs_from_nv(zigbee_relay_cluster *cluster);
void relay_cluster_handle_startup_mode(zigbee_relay_cluster *cluster);

zigbee_relay_cluster *relay_cluster_by_endpoint[10];

void relay_cluster_callback_attr_write_trampoline(u8 clusterId) {	
    relay_cluster_on_write_attr(relay_cluster_by_endpoint[clusterId]);
}

void relay_cluster_add_to_endpoint(zigbee_relay_cluster *cluster, zigbee_endpoint *endpoint) {
    relay_cluster_by_endpoint[endpoint->index] = cluster;
    cluster->endpoint = endpoint->index;
    relay_cluster_load_attrs_from_nv(cluster);
   
    cluster->relay->callback_param = cluster;
    cluster->relay->on_change = (ev_relay_callback_t)relay_cluster_on_relay_change;
 
    relay_cluster_handle_startup_mode(cluster);

    SETUP_ATTR(0, ZCL_ATTRID_ONOFF, ZCL_DATA_TYPE_BOOLEAN, ACCESS_CONTROL_READ | ACCESS_CONTROL_REPORTABLE, cluster->relay->on);
    SETUP_ATTR(1, ZCL_ATTRID_START_UP_ONOFF, ZCL_DATA_TYPE_ENUM8, ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE, cluster->startup_mode);

    zigbee_endpoint_add_cluster(endpoint, 1, ZCL_CLUSTER_GEN_ON_OFF);
    zcl_specClusterInfo_t *info = zigbee_endpoint_reserve_info(endpoint);
    info->clusterId = ZCL_CLUSTER_GEN_ON_OFF;
    info->manuCode = MANUFACTURER_CODE_NONE;
    info->attrNum = 2;
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
		relay_on(cluster->relay);
	} else if(cmdId == ZCL_CMD_ONOFF_OFF){
		relay_off(cluster->relay);
	} else if(cmdId == ZCL_CMD_ONOFF_TOGGLE){
		relay_toggle(cluster->relay);
	} else {
		printf("Unknown command: %d\r\n", cmdId);
	}

	return ZCL_STA_SUCCESS;
}


void relay_cluster_report(zigbee_relay_cluster *cluster)
{	
    if(zb_isDeviceJoinedNwk()){
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

void relay_cluster_on_relay_change(zigbee_relay_cluster *cluster, u8 state) {
    if (cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE 
        || cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS) {
        relay_cluster_store_attrs_to_nv(cluster);
    }		
}

void relay_cluster_on_write_attr(zigbee_relay_cluster *cluster) {		
    relay_cluster_store_attrs_to_nv(cluster);
}

typedef struct {
    u8   on_off;
    u8 	 startup_mode;
} zigbee_relay_cluster_config;


zigbee_relay_cluster_config nv_config_buffer;


void relay_cluster_store_attrs_to_nv(zigbee_relay_cluster *cluster) {	
    nv_config_buffer.on_off = cluster->relay->on;
    nv_config_buffer.startup_mode = cluster->startup_mode;

    nv_flashWriteNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_RELAY_CONFIG(cluster->endpoint), sizeof(zigbee_relay_cluster_config), (u8*)&nv_config_buffer);
}

void relay_cluster_load_attrs_from_nv(zigbee_relay_cluster *cluster) {	
    nv_sts_t st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_RELAY_CONFIG(cluster->endpoint), sizeof(zigbee_relay_cluster_config), (u8*)&nv_config_buffer);

    if (st != NV_SUCC) {
        return;
    }
    cluster->startup_mode = nv_config_buffer.startup_mode;

}

void relay_cluster_handle_startup_mode(zigbee_relay_cluster *cluster) {
    nv_sts_t st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_RELAY_CONFIG(cluster->endpoint), sizeof(zigbee_relay_cluster_config), (u8*)&nv_config_buffer);
    if (st != NV_SUCC) {
        return;
    }

    u8 prev_on = nv_config_buffer.on_off;
    if (cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TO_OFF) {
        relay_off(cluster->relay);
    }
    if (cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TO_ON) {
        relay_on(cluster->relay);
    }
    if (cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TOGGLE) {
        if (prev_on) {
            relay_off(cluster->relay);
        } else {
            relay_on(cluster->relay);
        }
    }
    if (cluster->startup_mode == ZCL_START_UP_ONOFF_SET_ONOFF_TO_PREVIOUS) {
        if (prev_on) {
            relay_on(cluster->relay);
        } else {
            relay_off(cluster->relay);
        }
    }
}