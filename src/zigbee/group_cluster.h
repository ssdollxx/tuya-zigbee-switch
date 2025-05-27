#ifndef _GROUP_CLUSTER_H_
#define _GROUP_CLUSTER_H_

#include "tl_common.h"
#include "zb_common.h"
#include "zcl_include.h"

#include "endpoint.h"

typedef struct
{  
    zclAttrInfo_t attr_infos[1];
} zigbee_group_cluster;

void group_cluster_add_to_endpoint(zigbee_group_cluster *cluster, zigbee_endpoint *endpoint);

#endif
