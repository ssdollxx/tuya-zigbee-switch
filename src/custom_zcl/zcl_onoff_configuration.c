#include "zcl_include.h"


#ifdef ZCL_ONOFF_CONFIGUATION

/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * LOCAL TYPES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */

_CODE_ZCL_ status_t zcl_onoff_configuration_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb)
{
  return(zcl_registerCluster(endpoint, ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG, manuCode, attrNum, attrTbl, NULL, cb));
}

#endif  /* ZCL_RELATIVE_HUMIDITY */
