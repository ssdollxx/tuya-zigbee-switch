/*
 * reporting.c
 * Created: pvvx
 */
#include "reporting.h"
#include "utility.h"

extern void reportAttr(reportCfgInfo_t *pEntry);

/*********************************************************************
 * @fn      app_chk_report
 *
 * @brief	check if there is report.
 *
 * @param   time from old check in sec
 *
 * @return	NULL
 */
void app_chk_report(u16 uptime_sec) {
	zclAttrInfo_t *pAttrEntry = NULL;
	u16 len;
	bool flg_report, flg_chk_attr;
	if(reportingTab.reportNum) {
		for(u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++){
			reportCfgInfo_t *pEntry = &reportingTab.reportCfgInfo[i];
			if(pEntry->used && (pEntry->maxInterval != 0xFFFF)) {
				// used
				flg_chk_attr = false;
				flg_report = false;
				
				// timer minInterval seconds
				if(pEntry->minIntCnt > uptime_sec)
					pEntry->minIntCnt -= uptime_sec;
				else
					pEntry->minIntCnt = 0;
				// timer maxInterval seconds
				if(pEntry->maxIntCnt > uptime_sec)
					pEntry->maxIntCnt -= uptime_sec;
				else
					pEntry->maxIntCnt = 0;

				if(pEntry->maxIntCnt == 0) {
					flg_report = true;
				} else if(pEntry->minIntCnt == 0) {
					flg_chk_attr = true;
				}
				if(flg_chk_attr || flg_report) {
					pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
					if(!pAttrEntry){
						printf("Failed to get entry!!!\r\n");
						// should not happen.
						ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
						return;
					}
				}
				if(flg_chk_attr) {
					// report pAttrEntry
					if(zcl_analogDataType(pAttrEntry->type)) {
						if(reportableChangeValueChk(pAttrEntry->type, pAttrEntry->data, pEntry->prevData, pEntry->reportableChange)) {
							flg_report = true;
						}
					} else {
						len = zcl_getAttrSize(pAttrEntry->type, pAttrEntry->data);
						len = (len > 8) ? (8): (len);
						if(memcmp(pEntry->prevData, pAttrEntry->data, len) != SUCCESS) {
							flg_report = true;
						}
					}
				}
				if(flg_report) {
					printf("Sending report for %d %d \r\n", pEntry->clusterID, pEntry->attrID);
					pEntry->minIntCnt = pEntry->minInterval;
					pEntry->maxIntCnt = pEntry->maxInterval;
				
					epInfo_t dstEpInfo;
					TL_SETSTRUCTCONTENT(dstEpInfo, 0);

					dstEpInfo.profileId = HA_PROFILE_ID;
					dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
					
					pAttrEntry = zcl_findAttribute(pEntry->endPoint, pEntry->clusterID, pEntry->attrID);
					zcl_sendReportCmd(pEntry->endPoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
							ZCL_CLUSTER_GEN_ON_OFF, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
				}
			}
		}
	} else {
		// no report
	}
}

