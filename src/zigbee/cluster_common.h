#ifndef _CLUSTER_COMMON_H_
#define _CLUSTER_COMMON_H_

#define SETUP_ATTR(attr_index, attr_id, attr_type, attr_access, attr_data) \
    cluster->attr_infos[attr_index].id = attr_id; \
    cluster->attr_infos[attr_index].type = attr_type; \
    cluster->attr_infos[attr_index].access = attr_access; \
    cluster->attr_infos[attr_index].data = (u8*)&attr_data;

#define DEF_STR(string, name) struct { u8 len; char str[sizeof(string) - 1]; } const name = { sizeof(string) - 1, string }
#define DEF_STR_NON_CONST(string, name) struct { u8 len; char str[sizeof(string) - 1]; } name = { sizeof(string) - 1, string }

#endif

