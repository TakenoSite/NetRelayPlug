#include "d.h"
#include "b.h"
#include "e.h"
#include "c.h"
#include "z.h"

/* tag */

#define ITEM_BIND_TAG			"BIND"
#define ITEM_NODE_TYPE_TAG		"NODE_TYPE"
#define ITEM_PORT0_NODE_TAG		"PORT0"
#define ITEM_RAW_NODE_TAG		"RAW"
#define ITEM_SPORT_TAG			"SPORT"
#define ITEM_HEADER_TAG			"HEADER"
#define ITEM_DATA_TYPE_TAG		"DATA_TYPE"
#define ITEM_DATA_LEN_TAG		"DATA_LEN"
#define ITEM_SEND_TAG			"SEND"
#define ITEM_HOST_TAG			"HOST"
#define ITEM_MSG_TAG			"MSG"
#define ITEM_UPDATE_TAG			"UPDATE"
#define ITEM_DELETE_TAG			"DELETE"
#define ITEM_IPINFO_TAG			"IPINFO"
#define ITEM_EXPLORATION_TAG	"EXPLORATION"
#define ITEM_SHOST_TAG			"SHOST"
#define ITEM_THOST_TAG			"THOST"
#define ITEM_TTL_TAG			"TTL"
#define ITEM_BULLDOZER_TAG		"BULLDOZER"
#define ITEM_KEEPALIVE_TAG		"KEEPALIVE"
#define ITEM_ACC_KEEPALIVE_TAG	"ACC_KEEPALIVE"
#define ITEM_CMD_TAG			"CMD"

/* body */

/*
struct MSG_PARSE_ARG
{
	char *msg;
	struct sockaddr_in addr;
	int ssock;
	
	node_table *index_head;
	node_table *raw_node_index_head;
	vec2 *raw_node_body_head;
	vec2 *vec2_head;
};
typedef MSG_PARSE MSG_PARSE;
*/



void msg_parse(
		char *msg,
		struct sockaddr_in addr,
		int udp_sock,
		node_table *index_head,
		node_table *raw_node_index_head, 
		vec2 *raw_node_body_head,
		vec2 *vec2_head)
{
	MSG_PARSE m_p;
	Memset(&m_p, sizeof(m_p));
	
	m_p.decode_msg = decode_base64(msg);
	if(m_p.decode_msg == NULL)
	{
#ifdef E_LOG 
		log_error("[!] msg_parse -> decode error");
#endif
		return;
	}
	
#ifdef D_LOG
	printf("[+] msg_paser : decode_msg :: %s\n", m_p.decode_msg);
#endif
		
	/**/
	if(
		xml_item_tag(m_p.decode_msg, ITEM_BIND_TAG, m_p.MessageValu, sizeof(m_p.MessageValu))		== 0 
		&& xml_item_tag(m_p.MessageValu, ITEM_NODE_TYPE_TAG, m_p.node_type, sizeof(m_p.node_type))	== 0
	){
#ifdef D_LOG
		log_debug("[+] msg_paser : bind");
#endif
		if(strcmp(m_p.node_type, ITEM_PORT0_NODE_TAG) == 0)
		{
			node_add_table(addr, udp_sock, 0, index_head, vec2_head);
			return;
		}
		else if(strcmp(m_p.node_type, ITEM_RAW_NODE_TAG) == 0
				&& xml_item_tag(m_p.MessageValu, ITEM_SPORT_TAG, m_p.select_ipv4, sizeof(m_p.select_ipv4)) == 0
				&& sscanf(m_p.select_ipv4, "%d", &m_p.char_to_int)
				&& m_p.char_to_int < 65536 )
		{
			node_add_table(addr, udp_sock, m_p.char_to_int, raw_node_index_head, raw_node_body_head);
			return;
		}
		else
		{	
#ifdef E_LOG
			// error log 
#endif 
			sendTo(udp_sock, addr, BIND_ERROR_RESPONSE_MSG);
			return;
		}

	/**/
	}
	else if(
			xml_item_tag(m_p.decode_msg, ITEM_HEADER_TAG, m_p.MessageValu, sizeof(m_p.MessageValu))		== 0
			&& xml_item_tag(m_p.MessageValu, ITEM_DATA_TYPE_TAG, m_p.data_type, sizeof(m_p.data_type))	== 0
			&& xml_item_tag(m_p.MessageValu, ITEM_DATA_LEN_TAG, m_p.data_len, sizeof(m_p.data_len))		== 0
			&& sscanf(m_p.data_len, "%d", &m_p.int_data_len)
			&& m_p.int_data_len < RECV_MSG_BUF
	){
#ifdef D_LOG
		log_debug("[+] msg_paser : send");
#endif 
		if((m_p.dynamic_valu = cMalloc(m_p.int_data_len + MESSAGE_BUF)) == NULL)
		{
#ifdef E_LOG 
			// error log
#endif 
			sendTo(udp_sock, addr, SEND_ERROR_MSG);
			return;
		}
		
		if((m_p.dynamic_src_msg = cMalloc(m_p.int_data_len + MESSAGE_BUF)) == NULL)
		{
#ifdef E_LOG 
			// error log
#endif 
			free(m_p.dynamic_valu);
			sendTo(udp_sock, addr, SEND_ERROR_MSG);
			return;
		}
		
		if(
			xml_item_tag(m_p.decode_msg, ITEM_SEND_TAG, m_p.dynamic_valu, (m_p.int_data_len + MESSAGE_BUF))		== 0
			&& xml_item_tag(m_p.dynamic_valu, ITEM_HOST_TAG, m_p.ipv4, sizeof(m_p.ipv4))						== 0
			&& xml_item_tag(m_p.dynamic_valu, ITEM_MSG_TAG, m_p.dynamic_src_msg, MESSAGE_BUF)					== 0
		){
			
			// true
			msg_relay(addr, udp_sock, m_p.ipv4, m_p.dynamic_src_msg, m_p.data_type ,index_head);
			
			free(m_p.dynamic_valu);
			free(m_p.dynamic_src_msg);
#ifdef D_LOG
			log_debug("[+] msg_parse : host %s ::send to msg = %s", m_p.ipv4, m_p.dynamic_src_msg);
#endif 
			return;

		}
		else
		{	
#ifdef D_LOG 
			log_debug("[+] msg_paser : tag_format error");
#endif 
			sendTo(udp_sock, addr, SEND_ERROR_MSG);
			free(m_p.dynamic_valu);
			free(m_p.dynamic_src_msg);
			return;
		}
	
	/**/
	}
	else if(
		xml_item_tag(m_p.decode_msg, ITEM_UPDATE_TAG, m_p.MessageValu, sizeof(m_p.MessageValu))		== 0
		&& xml_item_tag(m_p.decode_msg, ITEM_NODE_TYPE_TAG, m_p.node_type, sizeof(m_p.node_type))	== 0
	){
#ifdef D_LOG 
		log_debug("[+] msg_paser : update");
#endif 
		if(
			strcmp(m_p.node_type, ITEM_RAW_NODE_TAG)													== 0
			&& xml_item_tag(m_p.MessageValu, ITEM_SPORT_TAG, m_p.select_ipv4, sizeof(m_p.select_ipv4))	== 0
			&& sscanf(m_p.select_ipv4, "%d", &m_p.char_to_int)
		){
			// raw 
			node_update_table(addr, udp_sock, m_p.char_to_int, raw_node_index_head, raw_node_body_head);
			return;

		}
		else if(strcmp(m_p.node_type, ITEM_PORT0_NODE_TAG) == 0)
		{
			// port0	
			node_update_table(addr, udp_sock, m_p.char_to_int, index_head, vec2_head);
			return;
		}
		else
		{
			sendTo(udp_sock, addr, UPDATE_OR_RES_MSG );
			return;
		}
	/**/
	}
	else if(xml_item_tag(m_p.decode_msg, ITEM_DELETE_TAG, m_p.MessageValu, sizeof(m_p.MessageValu)) == 0)
	{
		node_delete_table(addr, udp_sock, index_head, raw_node_index_head);
		return;
	/**/
	}
	else if(xml_item_tag(m_p.decode_msg, ITEM_KEEPALIVE_TAG, m_p.src_msg, MESSAGE_BUF) == 0)
	{
#ifdef D_LOG 
		log_debug("[+] msg_parse : keepalive ");
#endif
		sendTo(udp_sock, addr, msg);
		return;
	/**/
	}
	else if(xml_item_tag(m_p.decode_msg, ITEM_ACC_KEEPALIVE_TAG, m_p.src_msg, MESSAGE_BUF) == 0)
	{
#ifdef D_LOG 
		log_debug("[+] msg_parse : active keepalive");
#endif 
		response_active_node_table(m_p.src_msg);
		return;
	/**/
	}
	else if(xml_item_tag(m_p.decode_msg, ITEM_CMD_TAG, m_p.src_msg, MESSAGE_BUF) == 0)
	{
#ifdef D_LOG 
		log_debug("[+] msg_paser : cmd");
#endif 
		CMD_ARG cmd_arg;
		Memset(&cmd_arg, sizeof(cmd_arg));
		
		cmd_arg.addr = addr; 
		cmd_arg.udp_sock = udp_sock;
		cmd_arg.msg = m_p.src_msg;
		cmd_arg.index_head = index_head;
		cmd_arg.raw_node_index_head = raw_node_index_head;
		
		cmd_set(cmd_arg);
		return;
		/**/
	}
	else if(xml_item_tag(m_p.decode_msg, ITEM_IPINFO_TAG, m_p.src_msg, MESSAGE_BUF) == 0)
	{	
#ifdef D_LOG
		log_debug("[+] msg_paser : ipinfo");
#endif 
		inet_ip_info(addr, udp_sock);
		return;
		/**/
	}
#ifdef D_LOG
	log_debug("[+] msg_paser : default");
#endif 
}
