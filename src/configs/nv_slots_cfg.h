


#define NV_ITEM_ZCL_SWITCH_CONFIG(endpoint)       (NV_ITEM_APP_GP_TRANS_TABLE + endpoint)    // endpoint starts from 1, see sdk/proj/drivers/drv_nv.h

#define NV_ITEM_ZCL_RELAY_CONFIG(endpoint)    (NV_ITEM_ZCL_SWITCH_CONFIG(SWITCH_CLUSTERS) + endpoint - SWITCH_CLUSTERS)