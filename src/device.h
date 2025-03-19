#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "zcl_include.h"

/**********************************************************************
 * CONSTANT
 */


/**********************************************************************
 * TYPEDEFS
 */
typedef struct
{
  u8 keyType;       /* ERTIFICATION_KEY or MASTER_KEY key for touch-link or distribute network
                     *           SS_UNIQUE_LINK_KEY or SS_GLOBAL_LINK_KEY for distribute network */
  u8 key[16];       /* the key used */
}app_linkKey_info_t;

typedef struct
{
  u32                lastReportCheckSec; // report add (sec)

  app_linkKey_info_t tcLinkKey;
}app_ctx_t;


/**********************************************************************
 * GLOBAL VARIABLES
 */
extern app_ctx_t g_baseAppCtx;

extern bdb_commissionSetting_t g_bdbCommissionSetting;


#define zcl_iasZoneAttrGet()    &g_zcl_iasZoneAttrs

/**********************************************************************
 * FUNCTIONS
 */
#if 1
#define pm_wait_ms(t)    cpu_stall_wakeup_by_timer0(t * CLOCK_16M_SYS_TIMER_CLK_1MS);
#define pm_wait_us(t)    cpu_stall_wakeup_by_timer0(t * CLOCK_16M_SYS_TIMER_CLK_1US);
#else
#define pm_wait_ms(t)    sleep_us((t) * 1000);
#define pm_wait_us(t)    sleep_us(t);
#endif


void baseEndpoint_zclCheckInStart(void);

void read_dev_name(void);
void save_dev_name(void);

void scan_task(void);

#endif /* _DEVICE_H_ */
