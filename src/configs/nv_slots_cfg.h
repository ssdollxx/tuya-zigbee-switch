

#define MAX_SWITCH_CLUSTER 2
#define MAX_RELAY_CLUSTER 4


#define MIN_RELAY_CLUSTER 3


#define NV_ITEM_ZCL_SWITCH_CONFIG(endpoint)       (NV_ITEM_APP_GP_TRANS_TABLE + endpoint)    // endpoint starts from 1, see sdk/proj/drivers/drv_nv.h

#define NV_ITEM_ZCL_RELAY_CONFIG(endpoint)    (NV_ITEM_ZCL_SWITCH_CONFIG(MAX_SWITCH_CLUSTER) + endpoint - MIN_RELAY_CLUSTER + 1)