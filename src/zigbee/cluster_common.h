#ifndef _CLUSTER_COMMON_H_
#define _CLUSTER_COMMON_H_

#define SETUP_ATTR_FOR_TABLE(table, attr_index, attr_id, attr_type, attr_access, attr_data) \
    table[attr_index].id = attr_id; \
    table[attr_index].type = attr_type; \
    table[attr_index].access = attr_access; \
    table[attr_index].data = (u8*)&attr_data;

#define SETUP_ATTR(attr_index, attr_id, attr_type, attr_access, attr_data) \
   SETUP_ATTR_FOR_TABLE(cluster->attr_infos, attr_index, attr_id, attr_type, attr_access, attr_data)

#define DEF_STR(string, name) struct { u8 len; char str[sizeof(string) - 1]; } const name = { sizeof(string) - 1, string }
#define DEF_STR_NON_CONST(string, name) struct { u8 len; char str[sizeof(string) - 1]; } name = { sizeof(string) - 1, string }

#endif

