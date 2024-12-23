#include "tl_common.h"
#include "zb_common.h"
#include "endpoint.h"
#include "switch_cluster.h"
#include "cluster_common.h"
#include "relay_cluster.h"
#include "custom_zcl/zcl_onoff_configuration.h"
#include "zcl_include.h"
#include "base_components/relay.h"


extern zigbee_relay_cluster *relay_clusters[2];


void switch_cluster_on_button_press(zigbee_switch_cluster *cluster);
void switch_cluster_on_button_release(zigbee_switch_cluster *cluster);
void switch_cluster_on_button_long_press(zigbee_switch_cluster *cluster);

zigbee_switch_cluster *switch_cluster_by_endpoint[10];

void switch_cluster_store_attrs_to_nv(zigbee_switch_cluster *cluster);
void switch_cluster_load_attrs_from_nv(zigbee_switch_cluster *cluster);
void switch_cluster_on_write_attr(zigbee_switch_cluster *cluster);


status_t switch_cluster_callback_trampoline(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload) {	
	return ZCL_STA_SUCCESS;
}

void switch_cluster_callback_attr_write_trampoline(u8 clusterId) {	
    switch_cluster_on_write_attr(switch_cluster_by_endpoint[clusterId]);
}

void switch_cluster_add_to_endpoint(zigbee_switch_cluster *cluster, zigbee_endpoint *endpoint) {
    switch_cluster_by_endpoint[endpoint->index] = cluster;
    cluster->endpoint = endpoint->index;
    switch_cluster_load_attrs_from_nv(cluster);

    cluster->button->on_press = (ev_button_callback_t)switch_cluster_on_button_press;
    cluster->button->on_release = (ev_button_callback_t)switch_cluster_on_button_release;
    cluster->button->on_long_press = (ev_button_callback_t)switch_cluster_on_button_long_press;
    cluster->button->callback_param = cluster;

    SETUP_ATTR(0,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_TYPE, 			 ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ,                         cluster->mode);
    SETUP_ATTR(1,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_ACTIONS, 		 ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  cluster->action);
    SETUP_ATTR(2,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_MODE, 		     ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  cluster->mode);
    SETUP_ATTR(3,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_RELAY_MODE,     ZCL_DATA_TYPE_ENUM8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  cluster->relay_mode);
    SETUP_ATTR(4,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_RELAY_INDEX, 	 ZCL_DATA_TYPE_UINT8,    ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  cluster->relay_index);
    SETUP_ATTR(5,  ZCL_ATTRID_ONOFF_CONFIGURATION_SWITCH_LONG_PRESS_DUR, ZCL_DATA_TYPE_UINT16,   ACCESS_CONTROL_READ | ACCESS_CONTROL_WRITE,  cluster->button->long_press_duration_ms);

    zigbee_endpoint_add_cluster(endpoint, 0, ZCL_CLUSTER_GEN_ON_OFF);
    zcl_specClusterInfo_t *info = zigbee_endpoint_reserve_info(endpoint);
    info->clusterId = ZCL_CLUSTER_GEN_ON_OFF;
    info->manuCode = MANUFACTURER_CODE_NONE;
    info->attrNum = 0;
    info->attrTbl = NULL;
    info->clusterRegisterFunc = zcl_onOff_register;
    info->clusterAppCb = switch_cluster_callback_trampoline;

    zigbee_endpoint_add_cluster(endpoint, 1, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG);
    zcl_specClusterInfo_t *info_conf = zigbee_endpoint_reserve_info(endpoint);
    info_conf->clusterId = ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG;
    info_conf->manuCode = MANUFACTURER_CODE_NONE;
    info_conf->attrNum = 6;
    info_conf->attrTbl = cluster->attr_infos;
    info_conf->clusterRegisterFunc = zcl_onoff_configuration_register;
    info_conf->clusterAppCb = switch_cluster_callback_trampoline;
}


void switch_cluster_on_button_press(zigbee_switch_cluster *cluster) {		
    printf("switch_cluster_on_button_press for endpoint %d\r\n", cluster->endpoint);

    zigbee_relay_cluster *relay_cluster = relay_clusters[cluster->relay_index - 1];

	printf("Relay addr used %d\r\n", relay_cluster);

	if (
        cluster->mode == ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_TOGGLE 
        || cluster->mode == ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_MOMENTARY
    ) {
		if (cluster->relay_mode == ZCL_ONOFF_CONFIGURATION_RELAY_MODE_RISE) {
            switch(cluster->action){
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_ONOFF:
                    printf("Action ONOFF\r\n");
                    printf("Relay endpoint %d\r\n", relay_cluster->endpoint);
                    relay_on(relay_cluster->relay);
                    break;
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_OFFON:
                    printf("Action OFFON\r\n");
                    printf("Relay endpoint %d\r\n", relay_cluster->endpoint);
                    relay_off(relay_cluster->relay);
                    break;
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE:
                    printf("Action TOGGLE\r\n");
                    printf("Relay endpoint %d\r\n", relay_cluster->endpoint);
                    relay_toggle(relay_cluster->relay);
                    break;
            }
            relay_cluster_report(relay_cluster);
		}
	}


    if(zb_isDeviceJoinedNwk()) {
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        switch(cluster->action){
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_ONOFF:
                zcl_onOff_onCmd(cluster->endpoint, &dstEpInfo, FALSE);
                break;
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_OFFON:
                zcl_onOff_offCmd(cluster->endpoint, &dstEpInfo, FALSE);
                break;
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE:
                zcl_onOff_toggleCmd(cluster->endpoint, &dstEpInfo, FALSE);
                break;
        }
    }
}

void switch_cluster_on_button_release(zigbee_switch_cluster *cluster) {		
    zigbee_relay_cluster *relay_cluster = relay_clusters[cluster->relay_index - 1];

	if (
        cluster->mode == ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_TOGGLE
    ) {
		if (cluster->relay_mode == ZCL_ONOFF_CONFIGURATION_RELAY_MODE_RISE) {
            switch(cluster->action){
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_ONOFF:
                    relay_on(relay_cluster->relay);
                    break;
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_OFFON:
                    relay_off(relay_cluster->relay);
                    break;
                case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE:
                    relay_toggle(relay_cluster->relay);
                    break;
            }
            relay_cluster_report(relay_cluster);
		}
	}

    if(zb_isDeviceJoinedNwk()) {
        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
        switch(cluster->action){
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_ONOFF:
                zcl_onOff_offCmd(cluster->endpoint, &dstEpInfo, FALSE);
                break;
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_OFFON:
                zcl_onOff_onCmd(cluster->endpoint, &dstEpInfo, FALSE);
                break;
            case ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE:
                if (cluster->mode != ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_MOMENTARY) {
                    zcl_onOff_toggleCmd(cluster->endpoint, &dstEpInfo, FALSE);
                }
                break;
        }
    }
}


void switch_cluster_on_button_long_press(zigbee_switch_cluster *cluster) {		
    if (
        cluster->mode != ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_MOMENTARY
    ) {
        return;
    }

    zigbee_relay_cluster *relay_cluster = relay_clusters[cluster->relay_index - 1];

    if (cluster->relay_mode == ZCL_ONOFF_CONFIGURATION_RELAY_MODE_LONG) {
        relay_toggle(relay_cluster->relay);
        relay_cluster_report(relay_cluster);
    }

    // TODO: support reporting
}


void switch_cluster_on_write_attr(zigbee_switch_cluster *cluster) {		
    switch_cluster_store_attrs_to_nv(cluster);
}


#define NV_ITEM_ZCL_SWITCH_CONFIG(endpoint)       (NV_ITEM_APP_GP_TRANS_TABLE + endpoint)    // endpoint starts from 1, see sdk/proj/drivers/drv_nv.h


zigbee_switch_cluster_config nv_config_buffer;


void switch_cluster_store_attrs_to_nv(zigbee_switch_cluster *cluster) {	
    nv_config_buffer.action = cluster->action;
    nv_config_buffer.mode = cluster->mode;
    nv_config_buffer.relay_index = cluster->relay_index;
    nv_config_buffer.relay_mode = cluster->relay_mode;
    nv_config_buffer.button_long_press_duration = cluster->button->long_press_duration_ms;

    nv_flashWriteNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_SWITCH_CONFIG(cluster->endpoint), sizeof(zigbee_switch_cluster_config), (u8*)&nv_config_buffer);
}

void switch_cluster_load_attrs_from_nv(zigbee_switch_cluster *cluster) {	
    nv_sts_t st = nv_flashReadNew(1, NV_MODULE_ZCL,  NV_ITEM_ZCL_SWITCH_CONFIG(cluster->endpoint), sizeof(zigbee_switch_cluster_config), (u8*)&nv_config_buffer);

    if (st != NV_SUCC) {
        return;
    }
    cluster->action = nv_config_buffer.action;
    cluster->mode = nv_config_buffer.mode;
    cluster->relay_index = nv_config_buffer.relay_index;
    cluster->relay_mode = nv_config_buffer.relay_mode;
    cluster->button->long_press_duration_ms = nv_config_buffer.button_long_press_duration;
}