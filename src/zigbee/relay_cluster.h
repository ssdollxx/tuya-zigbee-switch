#ifndef _RELAY_CLUSTER_H_
#define _RELAY_CLUSTER_H_

#include "tl_common.h"
#include "zb_common.h"
#include "zcl_include.h"

#include "endpoint.h"
#include "base_components/relay.h"

typedef struct {
    u8 endpoint;
    zclAttrInfo_t attr_infos[1];
    relay_t *relay;
} zigbee_relay_cluster;

void relay_cluster_add_to_endpoint(zigbee_relay_cluster *cluster, zigbee_endpoint *endpoint);

void relay_cluster_report(zigbee_relay_cluster *cluster);

#endif

