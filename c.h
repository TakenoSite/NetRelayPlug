#ifndef _C_H_
#define _C_H_

#include "z.h"
#include "include/hcpy.h"
#include <pthread.h>

#define TABLE_NAME_INDEX_HEAD	"INDEX_HEAD"
#define KEEPALIVE_PAYLOAD		"<EN41><ACC_KEEPALIVE>%s</ACC_KEEPALIVE></EN41>"
#define SEND_RESPONSE_MSG		"PEVONDE+PFNFTkQ+PFJFUz5Ob2RlIG5vdCByZWdpc3RlcmVkPC9SRVM+PC9TRU5EPjwvRU40MT4="
#define KEEP_MSG				"PEVONDE+PEtFRVBOT0RFPjwvS0VFUE5PREU+PC9FTjQxPg=="
#define GHOST_REPONSE_MSG		"<EN41><RES><GHOST>%s</GHOST></RES></EN41>"
#define MSG_SEND_FORMAT			"<EN41><HEADER><DATA_TYPE>%s</DATA_TYPE><SRCHOST>%s</SRCHOST></HEADER><MSG><BODY>%s</BODY></MSG></EN41>"



struct THREAD_KEEPALIVE_ARGS{
	node_table *index_head;
	pthread_t *keepalive_t;
	int keepalive_sockets;

};

struct UDP_PUNCH_KEEP_ARGS{
	node_table *index_head;
	pthread_t *udppunchkeep_t;
	int udp_socket;

};

struct RESPONSE_ACTIVE_NODE_TABLE{
	int sockfd;
	struct sockaddr_in addr_s;
	int retrun_port;
};


struct NODE_ALIVE_CHECK{
	int sockfd;
	struct sockaddr_in addr_s;
	int return_port;
	
	char res_msg_buf[MESSAGE_BUF];
	int timeout;
};


void *keepalive(void *args);
void *udp_punch_keep(void *args);
void msg_relay(struct sockaddr_in addr, int sock, char *dsthost, char *msg, char *dtype, node_table *index_head);
//void msg_relay(struct sockaddr_in addr, int sock, char *dsthost, char *msg, node_table *index_head);


void response_active_node_table(char *payload);
void inet_ip_info(struct sockaddr_in addr, int sock);


#endif
