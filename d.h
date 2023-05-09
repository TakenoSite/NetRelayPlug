#ifndef _D_H_
#define _D_H_

#include "a.h"
#include "c.h"
#include "include/hcpy.h"


#define BIND_ERROR_RESPONSE_MSG "PEVONDE+PEJJTkQ+PFJFUz5EdXBsaWNhdGU8L1JFUz48L0JJTkQ+PC9FTjQxPg=="

/* RELAY_SEND_PROTOCOL_MSG */
#define SEND_ERROR_MSG "PEVONDE+PFNFTkQ+PFJFUz5wcm90b2NvbCBlcnJvcjI8UkVTPjxTRU5EPjwvRU40MT4="


/* */
#define UPDATE_OR_RES_MSG "PEVONDE+PFVQREFURTI+PFJFUz5QT1JUMCBvciBSQVc8L1JFUz48L1VQREFURTI+PC9FTjQxPg=="


struct MSG_PARSE{
	char MessageValu[MESSAGE_BUF];
	char *decode_msg;
	char src_msg[MESSAGE_BUF];
	char ipv4[IPV4_BUF];
	char select_ipv4[IPV4_BUF];
	char sport[6]; // 2**16
	int char_to_int;
	char node_type[16];
	char ttl_s[6];
	int ttl;	
	
	// fix later 
	//struct dynamic_valu dyn_valu, *dyn_valu_p;
	//struct static_valu sta_valu, *sta_valu_p;

	// 動的データ専用バッファ 
	char *dynamic_valu;
	char *dynamic_src_msg;
	char data_type[BUF_16BYTE];
	char data_len[BUF_64BYTE];
	int int_data_len;
};

void msg_parse(
		char *msg,
		struct sockaddr_in addr,
		int udp_sock,
		node_table *index_head,
		node_table *raw_node_index_head, 
		vec2 *raw_node_body_head,
		vec2 *vec2_head
		);



#endif 
