#ifndef _BASIC_CLUSTER_H_
#define _BASIC_CLUSTER_H_

#include "tl_common.h"
#include "zb_common.h"
#include "zcl_include.h"

#include "endpoint.h"
#include "base_components/relay.h"

typedef struct {
	u8	deviceEnable;
    zclAttrInfo_t attr_infos[11];
} zigbee_basic_cluster;

void basic_cluster_add_to_endpoint(zigbee_basic_cluster *cluster, zigbee_endpoint *endpoint);

#endif

