#include "tl_common.h"
#include "zb_common.h"
#include "zcl_include.h"
#include "endpoint.h"


void print_info(zigbee_endpoint *endpoint) {
    printf("ENDPOINT %d\r\n", endpoint->index);
    printf("ENDPOINT simple_description.app_profile_id %d\r\n", endpoint->simple_description.app_profile_id);
    printf("ENDPOINT simple_description.app_dev_id %d\r\n", endpoint->simple_description.app_dev_id);
    printf("ENDPOINT simple_description.endpoint %d\r\n", endpoint->simple_description.endpoint);
    printf("ENDPOINT simple_description.app_dev_ver %d\r\n", endpoint->simple_description.app_dev_ver);
    printf("ENDPOINT simple_description.reserved %d\r\n", endpoint->simple_description.reserved);
    printf("ENDPOINT simple_description.app_in_cluster_count %d\r\n", endpoint->simple_description.app_in_cluster_count);
    printf("ENDPOINT simple_description.app_out_cluster_count %d\r\n", endpoint->simple_description.app_out_cluster_count);

    printf("ENDPOINT simple_description.app_in_cluster_lst addr %d\r\n", endpoint->simple_description.app_in_cluster_lst);
    printf("ENDPOINT simple_description.app_out_cluster_lst addr %d\r\n", endpoint->simple_description.app_out_cluster_lst);

    for (int index = 0; index < endpoint->simple_description.app_in_cluster_count; index++) {
        printf("ENDPOINT simple_description.app_in_cluster_lst[%d] %d\r\n", index, endpoint->simple_description.app_in_cluster_lst[index]);
    }
    for (int index = 0; index <  endpoint->simple_description.app_out_cluster_count; index++) {
        printf("ENDPOINT simple_description.app_out_cluster_lst[%d] %d\r\n", index, endpoint->simple_description.app_out_cluster_lst[index]);
    }

    printf("ENDPOINT simple_description.cluster_info_cnts %d\r\n", endpoint->cluster_info_cnts);

    for (int index = 0; index <  endpoint->cluster_info_cnts; index++) {
        printf("ENDPOINT simple_description.cluster_infos[%d].clusterId %d\r\n", index, endpoint->cluster_infos[index].clusterId);
        printf("ENDPOINT simple_description.cluster_infos[%d].manuCode %d\r\n", index, endpoint->cluster_infos[index].manuCode);
        printf("ENDPOINT simple_description.cluster_infos[%d].attrNum %d\r\n", index, endpoint->cluster_infos[index].attrNum);
        printf("ENDPOINT simple_description.cluster_infos[%d].clusterRegisterFunc %d\r\n", index, endpoint->cluster_infos[index].clusterRegisterFunc);
        printf("ENDPOINT simple_description.cluster_infos[%d].clusterAppCb %d\r\n", index, endpoint->cluster_infos[index].clusterAppCb);
        for (int attr_index = 0; attr_index < endpoint->cluster_infos[index].attrNum; attr_index++) {
            printf("ENDPOINT simple_description.cluster_infos[%d].attrTbl[%d].id %d\r\n", index, attr_index, endpoint->cluster_infos[index].attrTbl[attr_index].id);
            printf("ENDPOINT simple_description.cluster_infos[%d].attrTbl[%d].type %d\r\n", index, attr_index, endpoint->cluster_infos[index].attrTbl[attr_index].type);
            printf("ENDPOINT simple_description.cluster_infos[%d].attrTbl[%d].access %d\r\n", index, attr_index, endpoint->cluster_infos[index].attrTbl[attr_index].access);
            printf("ENDPOINT simple_description.cluster_infos[%d].attrTbl[%d].data %d\r\n", index, attr_index, endpoint->cluster_infos[index].attrTbl[attr_index].data);
        }
    }
}

void zigbee_endpoint_init(zigbee_endpoint *endpoint) {
    endpoint->simple_description.app_profile_id = HA_PROFILE_ID;
    endpoint->simple_description.app_dev_id = HA_DEV_ONOFF_SWITCH;
    endpoint->simple_description.endpoint = endpoint->index;
    endpoint->simple_description.app_dev_ver = 1;
    endpoint->simple_description.reserved = 0;
    endpoint->simple_description.app_in_cluster_lst = endpoint->in_clusters;
    endpoint->simple_description.app_out_cluster_lst = endpoint->out_clusters;
}


void zigbee_endpoint_register_self(zigbee_endpoint *endpoint) {

    af_endpointRegister(endpoint->index, (af_simple_descriptor_t *)&endpoint->simple_description, zcl_rx_handler, NULL);
	zcl_register(endpoint->index, endpoint->cluster_info_cnts, (zcl_specClusterInfo_t *)endpoint->cluster_infos);

    printf("After register\r\n");
    print_info(endpoint);
}

void zigbee_endpoint_add_cluster(zigbee_endpoint *endpoint, u8 server, u16 cluster_type) {
    if (server) {
        printf("Adding server at %d %d, array addr %d\r\n", endpoint->simple_description.app_in_cluster_count, cluster_type, endpoint->in_clusters);

        u8 index = endpoint->simple_description.app_in_cluster_count;
        endpoint->simple_description.app_in_cluster_lst[index] = (u16)cluster_type;
        index++;
        endpoint->simple_description.app_in_cluster_count = index;
    } else {
        printf("Adding client at %d %d, array addr %d\r\n", endpoint->simple_description.app_out_cluster_count, cluster_type, endpoint->out_clusters);
        u8 index = endpoint->simple_description.app_out_cluster_count;
        endpoint->simple_description.app_out_cluster_lst[index] = (u16)cluster_type;
        index++;
        endpoint->simple_description.app_out_cluster_count = index;
    }
}

zcl_specClusterInfo_t* zigbee_endpoint_reserve_info(zigbee_endpoint *endpoint) {
    zcl_specClusterInfo_t *info = endpoint->cluster_infos + endpoint->cluster_info_cnts;
    endpoint->cluster_info_cnts++;
    return info;
}