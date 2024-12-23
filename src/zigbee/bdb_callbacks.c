#include "tl_common.h"
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"
#include "device.h"

#include "peripherals.h"

void device_bdbInitCb(u8 status, u8 joinedNetwork);
void device_bdbCommissioningCb(u8 status, void *arg);
void device_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);

void device_otaProcessMsgHandler(u8 evt, u8 status);


bdb_appCb_t g_deviceBdbCb =
{
	device_bdbInitCb,
	device_bdbCommissioningCb,
	device_bdbIdentifyCb,
	NULL
};

ota_callBack_t baseEndpoint_otaCb =
{
	device_otaProcessMsgHandler,
};


ev_timer_event_t *steerTimerEvt = NULL;
#if REJOIN_FAILURE_TIMER
ev_timer_event_t *deviceRejoinBackoffTimerEvt = NULL;
#endif
/**********************************************************************
 * FUNCTIONS
 */
s32 device_bdbNetworkSteerStart(void *arg){

	bdb_networkSteerStart();

	steerTimerEvt = NULL;
	return -1;
}

#if REJOIN_FAILURE_TIMER

s32 device_rejoinBackoff(void *arg){

	if(zb_isDeviceFactoryNew()){
		deviceRejoinBackoffTimerEvt = NULL;
		return -1;
	}

    zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
    return 0;
}

#endif


void device_bdbInitCb(u8 status, u8 joinedNetwork){
	if(status == BDB_INIT_STATUS_SUCCESS){
		/*
		 * for non-factory-new device:
		 * 		load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
		 *
		 * for factory-new device:
		 * 		steer a network
		 *
		 */
		if(joinedNetwork){
		    ota_queryStart(15 * 60);	// 15 m
		}else{
			u16 jitter = 0;
			do{
				jitter = zb_random() % 0x0fff;
			}while(jitter == 0);

			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}
			///time_soff = 0;
			steerTimerEvt = TL_ZB_TIMER_SCHEDULE(device_bdbNetworkSteerStart, NULL, jitter);
		}
	}
#if REJOIN_FAILURE_TIMER
	else{
		if(joinedNetwork){
		 	zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
		}
	}
#endif
}

/*********************************************************************
 * @fn      device_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void device_bdbCommissioningCb(u8 status, void *arg){
	switch(status){
		case BDB_COMMISSION_STA_SUCCESS:
			led_blink(&led, 500, 500, 7);

			if(steerTimerEvt){
				TL_ZB_TIMER_CANCEL(&steerTimerEvt);
			}
			if(deviceRejoinBackoffTimerEvt){
				TL_ZB_TIMER_CANCEL(&deviceRejoinBackoffTimerEvt);
			}
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
			break;
		case BDB_COMMISSION_STA_IN_PROGRESS:
			break;
		case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
			break;
		case BDB_COMMISSION_STA_NO_NETWORK:
		case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
		case BDB_COMMISSION_STA_TARGET_FAILURE:
			{
				u16 jitter = 0;
				do{
					jitter = zb_random() % 0x0fff;
				}while(jitter == 0);

				if(steerTimerEvt){
					TL_ZB_TIMER_CANCEL(&steerTimerEvt);
				}
#if REJOIN_FAILURE_TIMER
				steerTimerEvt = TL_ZB_TIMER_SCHEDULE(device_bdbNetworkSteerStart, NULL, jitter + 60000);
#else
				steerTimerEvt = TL_ZB_TIMER_SCHEDULE(device_bdbNetworkSteerStart, NULL, jitter);
#endif
			}
			break;
		case BDB_COMMISSION_STA_FORMATION_FAILURE:
			break;
		case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
			break;
		case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
			break;
		case BDB_COMMISSION_STA_NOT_PERMITTED:
			break;
		case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
		case BDB_COMMISSION_STA_PARENT_LOST:
#if REJOIN_FAILURE_TIMER
			device_rejoinBackoff(NULL);
#else
			zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
#endif
			break;
		case BDB_COMMISSION_STA_REJOIN_FAILURE:
			if(!zb_isDeviceFactoryNew()){
#if REJOIN_FAILURE_TIMER
                // sleep for 3 minutes before reconnect if rejoin failed
                deviceRejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(device_rejoinBackoff, NULL, 360 * 1000);
#else
				zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
#endif
			}
			break;
		default:
			break;
	}
}


void device_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime){
	// TODO: maybe handle this with blinking of system LED?
}

void device_otaProcessMsgHandler(u8 evt, u8 status)
{
	printf("OTA message %d %d\r\n", evt, status);
	if(evt == OTA_EVT_COMPLETE){
		if(status == ZCL_STA_SUCCESS){
			ota_mcuReboot();
		}else{
			ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
		}
	}
}


/*********************************************************************
 * @fn      device_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void device_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf)
{
    if(pLeaveCnf->status == SUCCESS){
		if(deviceRejoinBackoffTimerEvt){
			TL_ZB_TIMER_CANCEL(&deviceRejoinBackoffTimerEvt);
		}
    }
}

/*********************************************************************
 * @fn      device_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void device_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    // NO-OP
}
