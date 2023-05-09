#ifndef _B_H_
#define _B_H_

#include "include/hcpy.h"

#define BIND_PROTOCOL_FALSE_MSG "PEVONDE+PEJJTkQ+PFJFUz5mYWxzZTwvUkVTPjwvQklORD48L0VONDE+"
#define BIND_PROTOCOL_TRUE_MSG "PEVONDE+PEJJTkQ+PFJFUz50cnVlPC9SRVM+PC9CSU5EPjwvRU40MT4="
#define DELETE_NONE_RESPONSE_MSG "PEVONDE+PERFTEVURT48UkVTPk5vZGUgbm90IHJlZ2lzdGVyZWQ8UkVTPjwvREVMRVRFPjwvRU40MT4="
#define ADD_TABLE_COMPLETE_MSG ""



int node_add_table(struct sockaddr_in addr, int sock, int sport, node_table *table, vec2 *vec2_info);
int node_update_table(struct sockaddr_in addr, int sock, int sport, node_table *table, vec2 *vec2_info);

int node_delete_table(struct sockaddr_in addr, int sock, node_table *index_head, node_table *raw_node_index_head);


#endif
