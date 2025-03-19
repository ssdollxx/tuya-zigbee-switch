#ifndef ZCL_MULTISTATE_INPUT_H
#define ZCL_MULTISTATE_INPUT_H



/*********************************************************************
 * CONSTANTS
 */

/**
 *  @brief	relative humidity measurement cluster Attribute IDs
 */

#define ZCL_ATTRID_MULTISTATE_INPUT_NUMBER_OF_STATES    0x004A
#define ZCL_ATTRID_MULTISTATE_INPUT_OUT_OF_SERVICE      0x0051
#define ZCL_ATTRID_MULTISTATE_INPUT_PRESENT_VALUE       0x0055
#define ZCL_ATTRID_MULTISTATE_INPUT_STATUS_FLAGS        0x006F

#define ZCL_MULTISTATE_INPUT_FLAG_IN_ALARM              0x01 << 0
#define ZCL_MULTISTATE_INPUT_FLAG_FAULT                 0x01 << 1
#define ZCL_MULTISTATE_INPUT_FLAG_OVERRIDDEN            0x01 << 2
#define ZCL_MULTISTATE_INPUT_FLAG_OUT_OF_SERVICE        0x01 << 3


status_t zcl_multistate_input_register(u8 endpoint, u16 manuCode, u8 attrNum, const zclAttrInfo_t attrTbl[], cluster_forAppCb_t cb);

#endif  /* ZCL_RELATIVE_HUMIDITY_H */
