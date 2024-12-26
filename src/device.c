/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "device.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "device.h"
#if ZBHCI_EN
#include "zbhci.h"
#endif

#include "chip_8258/timer.h"
#include "ext_ota.h"
#include "reporting.h"

#include "base_components/millis.h"
#include "peripherals.h"

#include "zigbee/general_commands.h"
#include "zigbee/bdb_callbacks.h"
#include "zigbee/endpoint.h"
#include "zigbee/basic_cluster.h"
#include "zigbee/relay_cluster.h"
#include "zigbee/switch_cluster.h"
#include "zigbee/general.h"

#include "zigbee/endpoint_cfg.h"
#include "custom_zcl/zcl_onoff_configuration.h"


/**********************************************************************
 * GLOBAL VARIABLES
 */
app_ctx_t g_baseAppCtx;

extern ota_callBack_t baseEndpoint_otaCb;

//running code firmware information
ota_preamble_t baseEndpoint_otaInfo = {
	.fileVer 			= FILE_VERSION,
	.imageType 			= IMAGE_TYPE,
	.manufacturerCode 	= MANUFACTURER_CODE_TELINK,
};


//Must declare the application call back function which used by ZDO layer
const zdo_appIndCb_t appCbLst = {
	bdb_zdoStartDevCnf,//start device cnf cb
	NULL,//reset cnf cb
	NULL,//device announce indication cb
	device_leaveIndHandler,//leave ind cb
	device_leaveCnfHandler,//leave cnf cb
	NULL,//nwk update ind cb
	NULL,//permit join ind cb
	NULL,//nlme sync cnf cb
	NULL,//tc join ind cb
	NULL,//tc detects that the frame counter is near limit
};


/**
 *  @brief Definition for bdb commissioning setting
 */
bdb_commissionSetting_t g_bdbCommissionSetting = {
	.linkKey.tcLinkKey.keyType = SS_GLOBAL_LINK_KEY,
	.linkKey.tcLinkKey.key = (u8 *)tcLinkKeyCentralDefault,       		//can use unique link key stored in NV

	.linkKey.distributeLinkKey.keyType = MASTER_KEY,
	.linkKey.distributeLinkKey.key = (u8 *)linkKeyDistributedMaster,  	//use linkKeyDistributedCertification before testing

	.linkKey.touchLinkKey.keyType = MASTER_KEY,
	.linkKey.touchLinkKey.key = (u8 *)touchLinkKeyMaster,   			//use touchLinkKeyCertification before testing

	.touchlinkEnable = 0,												/* disable touch-link */
	.touchlinkChannel = DEFAULT_CHANNEL, 								/* touch-link default operation channel for target */
	.touchlinkLqiThreshold = 0xA0,			   							/* threshold for touch-link scan req/resp command */
};


/*********************************************************************
 * @fn      stack_init
 *
 * @brief   This function initialize the ZigBee stack and related profile. If HA/ZLL profile is
 *          enabled in this application, related cluster should be registered here.
 *
 * @param   None
 *
 * @return  None
 */
void stack_init(void)
{
	zb_init();
	zb_zdoCbRegister((zdo_appIndCb_t *)&appCbLst);
}

zigbee_endpoint main_endpoint = {
	.index = 1,	
};

zigbee_endpoint switch2_endpoint = {
	.index = 2,	
};

zigbee_endpoint relay1_endpoint = {
	.index = 3,	
};

zigbee_endpoint relay2_endpoint = {
	.index = 4,	
};

zigbee_basic_cluster basic_cluster = {
	.deviceEnable = 1,
};

zigbee_relay_cluster relay1_cluster = {
	.relay = &relay1,
};

zigbee_relay_cluster relay2_cluster = {
	.relay = &relay2,
};

zigbee_relay_cluster* relay_clusters[2] = {&relay1_cluster, &relay2_cluster};


zigbee_switch_cluster switch1_cluster = {
	.mode = ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_TOGGLE,
	.action = ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE,
	.relay_mode = ZCL_ONOFF_CONFIGURATION_RELAY_MODE_RISE,
	.relay_index = 1,
	.button = &button_s1,
};

zigbee_switch_cluster switch2_cluster = {
	.mode = ZCL_ONOFF_CONFIGURATION_SWITCH_TYPE_TOGGLE,
	.action = ZCL_ONOFF_CONFIGURATION_SWITCH_ACTION_TOGGLE,
	.relay_mode = ZCL_ONOFF_CONFIGURATION_RELAY_MODE_RISE,
	.relay_index = 2,
	.button = &button_s2,
};


void onResetClicked(void *_) {
	factoryReset();
}


/*********************************************************************
 * @fn      user_app_init
 *
 * @brief   This function initialize the application(Endpoint) information for this node.
 *
 * @param   None
 *
 * @return  None
 */
void user_app_init(void)
{
	printf("User app init\r\n");
	printf("Hello OTA world - 4!\r\n");
	led_init(&led);

	button_on_board.long_press_duration_ms = 3500; // 3.5 seconds
	button_on_board.on_long_press = onResetClicked;

	af_powerDescPowerModeUpdate(POWER_MODE_RECEIVER_COMES_WHEN_STIMULATED);

    /* Initialize ZCL layer */
	/* Register Incoming ZCL Foundation command/response messages */
	zcl_init(device_zclProcessIncomingMsg);

	printf("Configuring endpoints\r\n");

	zigbee_endpoint_init(&main_endpoint);
	zigbee_endpoint_init(&switch2_endpoint);
	zigbee_endpoint_init(&relay1_endpoint);
	zigbee_endpoint_init(&relay2_endpoint);

	basic_cluster_add_to_endpoint(&basic_cluster, &main_endpoint);
	switch_cluster_add_to_endpoint(&switch1_cluster, &main_endpoint);
	switch_cluster_add_to_endpoint(&switch2_cluster, &switch2_endpoint);
	relay_cluster_add_to_endpoint(&relay1_cluster, &relay1_endpoint);
	relay_cluster_add_to_endpoint(&relay2_cluster, &relay2_endpoint);

	zigbee_endpoint_add_cluster(&main_endpoint, 0, ZCL_CLUSTER_OTA);

	printf("Registering endpoints\r\n");

	zigbee_endpoint_register_self(&main_endpoint);
	zigbee_endpoint_register_self(&switch2_endpoint);
	zigbee_endpoint_register_self(&relay1_endpoint);
	zigbee_endpoint_register_self(&relay2_endpoint);

    ota_init(OTA_TYPE_CLIENT, (af_simple_descriptor_t *)&main_endpoint.simple_description, &baseEndpoint_otaInfo, &baseEndpoint_otaCb);

}

void app_task(void)
{
	millis_update();
	periferals_update();
	if(bdb_isIdle()) {
		// report handler
		if(zb_isDeviceJoinedNwk()) {
			if (zb_isDeviceFactoryNew()) {
				zb_deviceFactoryNewSet(false);
			}

			if(g_baseAppCtx.lastReportCheckSec != seconds()) {
				app_chk_report(seconds() - g_baseAppCtx.lastReportCheckSec);
				g_baseAppCtx.lastReportCheckSec = seconds();
			}
		} else { // Device not Joined
			if (led.blink_times_left != LED_BLINK_FOREVER) {
				led_blink(&led, 500, 500, LED_BLINK_FOREVER);
			}
		}
	}	
}


/*********************************************************************
 * @fn      user_init
 *
 * @brief   User level initialization code.
 *
 * @param   isRetention - if it is waking up with ram retention.
 *
 * @return  None
 */
void user_init(bool isRetention)
{
	if(!isRetention){

		/* Populate properties with compiled-in values */

		/* Initialize Stack */
		stack_init();

		/* Initialize user application */
		user_app_init();

		printf("User app init done\r\n");

		/* User's Task */
		ev_on_poll(EV_POLL_IDLE, app_task);

		/* Load the pre-install code from flash */
		if(bdb_preInstallCodeLoad(&g_baseAppCtx.tcLinkKey.keyType, g_baseAppCtx.tcLinkKey.key) == RET_OK){
			g_bdbCommissionSetting.linkKey.tcLinkKey.keyType = g_baseAppCtx.tcLinkKey.keyType;
			g_bdbCommissionSetting.linkKey.tcLinkKey.key = g_baseAppCtx.tcLinkKey.key;
		}
		/* Set default reporting configuration */
		u32 reportableChange = 0;
        bdb_defaultReportingCfg(
			relay1_endpoint.index,
			HA_PROFILE_ID,
			ZCL_CLUSTER_GEN_ON_OFF,
			ZCL_ATTRID_ONOFF,
			0,
			60,
			(u8 *)&reportableChange
		);

		bdb_defaultReportingCfg(
			relay2_endpoint.index,
			HA_PROFILE_ID,
			ZCL_CLUSTER_GEN_ON_OFF,
			ZCL_ATTRID_ONOFF,
			0,
			60,
			(u8 *)&reportableChange
		);
	
		/* Initialize BDB */
		u8 repower = drv_pm_deepSleep_flag_get() ? 0 : 1;
		bdb_init((af_simple_descriptor_t *)&main_endpoint.simple_description, &g_bdbCommissionSetting, &g_deviceBdbCb, repower);
	} else {
		/* Re-config phy when system recovery from deep sleep with retention */
		mac_phyReconfig();
	}
}
