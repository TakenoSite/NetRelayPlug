#ifndef __NODE_TABLE_H_
#define __NODE_TABLE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



// インデックステーブル
typedef struct node_table{
	struct node_table* node;
	struct node_table* next;

} node_table;

// ポート配列
typedef struct node_port{
	int port;
	struct node_port* next;
} node_port;

// ホスト配列
typedef struct node_ipv4{
	char *host;
	struct node_ipv4* next;
} node_ipv4; 

// addr収納
typedef struct vec2{
	char* host;
	int port;
	struct vec2 *next;
}vec2;

typedef struct{
	struct node_table table;
	struct node_port port;
	struct node_ipv4 host; 
	
}TABLE;


bool  insert_index_table(node_table *index_head, vec2 *node, int insert_len);
bool create_addr_array(vec2 *head, int port, char* host);

bool safety_array_access(node_table *n_p, int access_len);
bool push_back(node_table *head, vec2 *addr);
bool push_front(node_table *head, vec2 *addr);
bool placeinsert(node_table *head, vec2* addr, int place);
void raplacement(node_table *head, vec2* addr, int place);
void deletes(node_table *head, int delete_array_place);
void print_array(node_table *head, char* table_name);
int array_len(node_table *head);
void *at(node_table *head, int at_len);
void vector_show(node_table *head, int len);
int table_search(node_table *head, char* target);
size_t get_node_size(node_table *n_p);
void close_array(node_table *head);
#endif
