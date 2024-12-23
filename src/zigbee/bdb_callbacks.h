

#ifndef _BDB_CALLBACKS_H_
#define _BDB_CALLBACKS_H_

#include "tl_common.h"
#include "zcl_include.h"
#include "bdb.h"



extern bdb_appCb_t g_deviceBdbCb;


void device_otaProcessMsgHandler(u8 evt, u8 status);

void device_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf);
void device_leaveIndHandler(nlme_leave_ind_t *pLeaveInd);


#endif

