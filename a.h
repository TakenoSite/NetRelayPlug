#ifndef _A_H_
#define _A_H_

#include "include/hcpy.h"
#include "z.h"

#define LOGN_LINE "-------------------------------------------------------------"

struct CREATE_SESSION
{
	struct sockaddr_in addr, sinfo;
	int udp_sock;
	char recv_msg_buf[RECV_MSG_BUF];
	socklen_t addrlen;
	
	node_table *index_head;
	node_table *raw_node_index_head;
	
	vec2 *raw_node_body_head;
	vec2 *vec2_head;
};


int create_table(vec2 *vec_head, node_table *index_head);
int create_session(CREATE_SESSION *c_s);



#endif
