#include "c.h"
#include "z.h"
#include "f.h"

#include <netinet/in.h>
#include <pthread.h>

/* config */
/*
#define KEEP_ALIVE_SIGNAL_INTERVAL 600
#define KEEP_NODE_SIGNAL_INTERVAL 30
#define FROM_RETURN_PORT 41666
*/

/* body */


static int node_alive_check(char *src_host){ // old name is return_node_active_check
	NODE_ALIVE_CHECK node_alive_valu;
	
	Memset(&node_alive_valu, sizeof(node_alive_valu));
	Memset(&node_alive_valu.addr_s, sizeof(node_alive_valu.addr_s));
	
	node_alive_valu.return_port = FROM_RETURN_PORT;
	node_alive_valu.sockfd =  sockdg();
	
	if(node_alive_valu.sockfd < 0){
		close(node_alive_valu.sockfd);
		return -1;
	}
	
	node_alive_valu.addr_s.sin_addr.s_addr = INADDR_ANY;
	node_alive_valu.addr_s.sin_family = AF_INET;
	node_alive_valu.addr_s.sin_port = htons(node_alive_valu.return_port);
	
	if(sock_bind(node_alive_valu.sockfd, node_alive_valu.addr_s) < 0){
		close(node_alive_valu.sockfd);
		return -2;
	}
	
	node_alive_valu.timeout = SockTimeout(node_alive_valu.sockfd, 5, 0);
	if(node_alive_valu.timeout == 0){
		close(node_alive_valu.sockfd);
		return -3;
	}
	
	// msg parcer
	
	recv(node_alive_valu.sockfd
			,node_alive_valu.res_msg_buf,
			sizeof(node_alive_valu.res_msg_buf),
			0);
		

	
	if(strcmp(src_host, node_alive_valu.res_msg_buf) == 0){
		close(node_alive_valu.sockfd);
		return 0;
	}	

	close(node_alive_valu.sockfd);
	return -3;
}



// port0 nodeBへメッセージを転送する
void msg_relay(struct sockaddr_in addr, int sock, char *dsthost, char *msg, char *dtype, node_table *index_head)
{ // old name is relay_send_to

	struct sockaddr_in nodeinfo, *n_i;
	char sendbuf[MESSAGE_BUF];
	int check;
	char srchost[IPV4_BUF];
	
	Memset(&nodeinfo, sizeof(nodeinfo));
	Memset(sendbuf, MESSAGE_BUF);
	Memset(srchost, IPV4_BUF);
	
	n_i = &nodeinfo;

	// srchsot 取得
	inet_ntop(AF_INET, &addr.sin_addr, srchost, IPV4_BUF);

	// dsthsotを検索
	check = table_search(index_head, dsthost);
	if(check == -1){
		sendTo(sock, addr,SEND_RESPONSE_MSG);
		return;
	}
	
	// addr 設定
	node_table *n_t =  at(index_head, check);  
	int port = 0;
	memcpy(&port, &n_t->next->node->next, sizeof(int));
	addr_in(n_i, (char*)n_t->next->node->node, port);	
	
#ifdef LOG
	log_info("[*] relay_send_to -> %d\n", htons(nodeinfo.sin_port));
#endif 
	
	// body 作成
	mFormat_Encoding(sendbuf, MESSAGE_BUF, MSG_SEND_FORMAT, dtype, srchost, msg);
	// bodyを送信
	sendTo(sock, nodeinfo, sendbuf);
	return;
	
}


//	有効なノードを返す
void response_active_node_table(char *payload)
{
	RESPONSE_ACTIVE_NODE_TABLE set_valu;
	Memset(&set_valu, sizeof(set_valu));
	
	set_valu.retrun_port = FROM_RETURN_PORT;
	set_valu.sockfd = sockdg();
	
	if(set_valu.sockfd < 0)
		return;
	
	set_valu.addr_s.sin_addr.s_addr = INADDR_ANY;
	set_valu.addr_s.sin_family = AF_INET;
	set_valu.addr_s.sin_port = htons(set_valu.retrun_port);
	
	sendTo(set_valu.sockfd, set_valu.addr_s, payload);
	close(set_valu.sockfd);
	
	return;
}

/* thread process */

// サブノードの接続を維持する
void *udp_punch_keep(void *arg){
	
	UDP_PUNCH_KEEP_ARGS *entry = (UDP_PUNCH_KEEP_ARGS*)arg;
	pthread_detach(*entry->udppunchkeep_t);

	struct sockaddr_in *keep_node_addr_p, keep_node_addr;
	keep_node_addr_p = &keep_node_addr;
	

	int to_int = 0, index_table_len = 0;
	node_table *n_t;
	node_table *n_p = entry->index_head;
		
	do{
		index_table_len = array_len(n_p);
		for(int i=0; i<index_table_len; i++)
		{
			Memset(&keep_node_addr, sizeof(keep_node_addr));
			if(safety_array_access(n_p, i)) n_t = at(n_p, i);
			else break;
			
			memcpy(&to_int, &n_t->next->node->next, sizeof(int));
#ifdef LOG
			log_info("keep protocol -> %s, %d", (char*)n_t->next->node->node, to_int);
#endif

			addr_in(keep_node_addr_p, (char*)n_t->next->node->node, to_int);
			sendTo(entry->udp_socket, keep_node_addr, KEEP_MSG);
		}
	
		sleep(KEEP_NODE_SIGNAL_INTERVAL);
	}while(true);
		
	return 0;
}


// ノードの有効確認
void *keepalive(void *arg){
	struct sockaddr_in addr_s, *addr_p;
	int to_int;
	int node_status;
	char *src_host;
	char create_msg[MESSAGE_BUF];

	THREAD_KEEPALIVE_ARGS *entry = (THREAD_KEEPALIVE_ARGS*)arg;	
	pthread_detach(*entry->keepalive_t);

	addr_p = &addr_s;
	node_table *src_addr;
	node_table *n_p = entry->index_head;

	do{
		sleep(KEEP_ALIVE_SIGNAL_INTERVAL);
#ifdef LOG 
		log_debug("[+] keepalive -> run it");
#endif
		for(int len=0; len < array_len(n_p); len++){
			if(safety_array_access(n_p, len)){
				src_addr = at(n_p, len);
			}else{
				break;
			}
			
			Memset(&addr_s, sizeof(addr_s));
			Memset(create_msg, MESSAGE_BUF);
			to_int = 0;
			memcpy(&to_int, &src_addr->next->node->next, sizeof(int));
			
			src_host = (char*)src_addr->next->node->node;
			addr_in(addr_p, src_host, to_int);
			
			snprintf(create_msg, MESSAGE_BUF, KEEPALIVE_PAYLOAD, src_host);
			if(sendTo(entry->keepalive_sockets, addr_s, encode_base64(create_msg)) < 0){};

			node_status = node_alive_check(src_host);
			
			switch(node_status){
				case 0:
#ifdef LOG 
					log_debug("[+] keepalive -> acc addr %s:%d", src_host, to_int);
#endif 
					break;
				case -1:
#ifdef LOG 
					log_error("[+] keepalive -> socket generate error!!");
#endif 
					break;
				case -2:
#ifdef LOG 
					log_error("[+] keepalive -> socket bind error !!");
#endif 
					break;
				case -3:
#ifdef LOG 
					log_debug("[+] keepalive -> timeout !!");
#endif 
					if(safety_array_access(n_p, len)){
						deletes(n_p, len);
					}
						
#ifdef LOG 
					// show table
					print_array(n_p, TABLE_NAME_INDEX_HEAD);
#endif 
					break;
			}
		}
		
	}while(true);

	return 0;	
}


void inet_ip_info(struct sockaddr_in addr, int sock)
{
	char sender_host[IPV4_BUF];
	char ghost_info[BUF_128BYTE];
	Memset(sender_host, IPV4_BUF);
	Memset(ghost_info, BUF_128BYTE);
	
	inet_ntop(AF_INET, &addr.sin_addr, sender_host, IPV4_BUF);
	mFormat_Encoding(ghost_info, BUF_128BYTE, GHOST_REPONSE_MSG, sender_host);
	puts(ghost_info);
	sendTo(sock, addr, ghost_info);
	
	return;
}

