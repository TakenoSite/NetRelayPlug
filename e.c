#include "e.h"
#include "z.h"
#include "f.h"
#include "g.h"
#include <stdlib.h>
#include <string.h>

/* config */
/*
#define CMD_SET_PASSWORD	"admin"
#define CMD_SET_USER		"admin"
#define CMD_SET_TOKEN		"admin"
*/

/* tag */

#define CMD_ITEM_PASS_TAG		"PASS"
#define CMD_ITEM_USER_TAG		"USER"
#define CMD_ITEM_TOKEN_TAG		"TOKEN"

#define CMD_ARGS_TAG		"ARGS"
#define CMD_HELP_TAG		"HELP"
#define CMD_NODE_QUERY_TAG	"NODEQUERY" 
#define CMD_TABLE_SHOW_TAG	"SHOWTABLE" 
#define CMD_NODE_DELETE_TAG "NODEDELTE"
#define CMD_SESSION_CLOSE	"SESSIONCLOSE"
#define CMD_SESSION_CONFIG	"SETCONFIG"
#define CMD_SHOW_CONFIG		"SHOWCONFIG"

/* body */


#define HELP_BODY "\
#define CMD_HELP_TAG			: HELP :: user :: - \
\n#define CMD_NODE_QUERY_TAG	: NODEQUERY :: user :: ip\
\n#define CMD_TABLE_SHOW_TAG	: SHOWTABLE :: user :: -\
\n#define CMD_NODE_DELETE_TAG	: NODEDELTE :: root :: ip\
\n#define CMD_SESSION_CLOSE		: SESSIONCLOSE ::root :: -\
\n#define CMD_SESSION_CONFIG	: SETCONFIG ::root :: option\
\n#define CMD_SHOW_CONFIG		: SHOWCONFIG :: user :: - \
"

#define ENCODE_HELP_MSG encode_base64(HELP_BODY)
	
static void cmd_help(struct sockaddr_in addr, int udp_sock)
{
	puts(ENCODE_HELP_MSG);
#ifdef D_LOG 
	log_debug("[+] cmd_help : _");
#endif 
	
	char *tmp_msg = "SGVscCBDb21tYW5k"; // sample
	sendTo(udp_sock, addr, tmp_msg);
	
	return;
}	


static void cmd_showconfig(struct sockaddr_in addr, int udp_sock) // root
{	
#ifdef D_LOG
	log_debug("[+] cmd_showconfig : _");
#endif
	char *buf, *payloads; 
	
	buf = malloc(CONF_FORMAT_SIZE);
	if(buf == NULL)
	{	
#ifdef E_LOG
		log_error("[!] cmd_showconfig : %d", errno);
#endif
		return;
	}
	else
	{
		Memset(buf, CONF_FORMAT_SIZE);
	}
	payloads = showconfig(buf, CONF_FORMAT_SIZE);
	
	sendTo(udp_sock, addr, payloads);
	free(buf);
	return;
}


#define SIGNAL_INTERVAL_TAG				"SIGNAL_INTERVAL"
#define KEEP_NODE_SIGNAL_INTERVAL_TAG	"KEEP_SIGNAL_INTERVAL"
#define FROM_RETURN_PORT_TAG			"FROM_RETURN_PORT"
#define SESSION_PORT_TAG				"SESSION_PORT"
#define CMD_SET_PASSWORD_TAG			"SET_PASSWORD"
#define CMD_SET_USER_TAG				"SET_USER"
#define CMD_SET_TOKEN_TAG				"SET_TOKEN"

struct CMD_SETCONFIG
{
	char valu[BUF_256BYTE];
	int to_int_valu;
};

static void cmd_setconfig(struct sockaddr_in addr, int udp_sock, char *body) // root 
{
#ifdef D_LOG
	log_debug("[+] cmd_setconfig : _");
#endif 
		
	struct CMD_SETCONFIG set_conf;
	size_t ssize = sizeof(set_conf);
	char *rbody = malloc(CONF_FORMAT_SIZE); // pt :: 1
	
	if(rbody == NULL)
	{
#ifdef E_LOG
		log_error("[+] cmd_setconfig : malloc");
#endif 
		return;
	}
	else
	{
		Memset(rbody, CONF_FORMAT_SIZE);
	}

	Memset(&set_conf, ssize);
	//Memset(rbody, CONF_FORMAT_SIZE);

	if(xml_item_search(body, SIGNAL_INTERVAL_TAG) == 0)
	{
		if(
			xml_item_tag(body, SIGNAL_INTERVAL_TAG, set_conf.valu, BUF_256BYTE) == 0 
			&& sscanf(set_conf.valu,"%d",&set_conf.to_int_valu)
			&& set_conf.to_int_valu <= 65536
		){
#ifdef LOG
			log_info("[*] cmd_setconfig : SET SIGNAL_INTERVAL :: %s", set_conf.valu);
#endif
			set_keep_alive_signal_interval(set_conf.to_int_valu);	
			strcat(rbody, "SET SIGNAL_INTERVAL :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");
		}
		
		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, KEEP_NODE_SIGNAL_INTERVAL_TAG) == 0)
	{
		if(
			xml_item_tag(body, KEEP_NODE_SIGNAL_INTERVAL_TAG, set_conf.valu, BUF_256BYTE) == 0
			&& sscanf(set_conf.valu,"%d",&set_conf.to_int_valu)
		  ){
#ifdef LOG
			log_info("[*] cmd_setconfig : SET KEEP_NODE_SIGNAL_INTERVAL :: %s", set_conf.valu);
#endif 
			set_keep_node_signal_interval(set_conf.to_int_valu);
			strcat(rbody, "SET KEEP_NODE_SIGNAL_INTERVAL :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");

		}

		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, FROM_RETURN_PORT_TAG) == 0)
	{
		if(
			xml_item_tag(body, FROM_RETURN_PORT_TAG, set_conf.valu, BUF_256BYTE) == 0
			&& sscanf(set_conf.valu,"%d",&set_conf.to_int_valu)
			&& set_conf.to_int_valu <= 65536
		  ){
#ifdef LOG
			log_info("[*] cmd_setconfig : SET FROM_RETURN_PORT :: %s", set_conf.valu);
#endif
			set_from_return_port(set_conf.to_int_valu);
			strcat(rbody, "SET FROM_RETURN_PORT :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");

		}	

		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, SESSION_PORT_TAG) == 0)
	{
		if(
			xml_item_tag(body, SESSION_PORT_TAG, set_conf.valu, BUF_256BYTE) == 0
			&& sscanf(set_conf.valu,"%d",&set_conf.to_int_valu)
			&& set_conf.to_int_valu <= 65536
		  ){
#ifdef LOG
			log_info("[*] cmd_setconfig : SET SESSION_PORT :: %s", set_conf.valu);
#endif
			set_session_port(set_conf.to_int_valu);
			strcat(rbody, "SET SESSION_PORT :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");

		}	

		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, CMD_SET_PASSWORD_TAG) == 0)
	{		
		if(xml_item_tag(body, CMD_SET_PASSWORD_TAG, set_conf.valu, BUF_256BYTE) == 0)
		{
#ifdef LOG
			log_info("[*] cmd_setconfig : SET CMD_SET_PASSWORD :: %s", set_conf.valu);
#endif
			set_cmd_set_password(set_conf.valu);
			strcat(rbody, "SET CMD_SET_PASSWORD :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");

		}
		
		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, CMD_SET_USER_TAG) == 0)
	{
		if(xml_item_tag(body, CMD_SET_USER_TAG, set_conf.valu, BUF_128BYTE) == 0)
		{
#ifdef LOG 
			log_info("[*] cmd_setconfig : SET CMD_SET_USER :: %s", set_conf.valu);
#endif
			set_cmd_set_user(set_conf.valu);
			strcat(rbody, "SET CMD_SET_USER :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");
	
		}
		
		Memset(&set_conf, ssize);
	}
	if(xml_item_search(body, CMD_SET_TOKEN_TAG) == 0)
	{
		if(xml_item_tag(body, CMD_SET_TOKEN_TAG, set_conf.valu, BUF_128BYTE) == 0)
		{
#ifdef LOG
			log_info("[*] cmd_setconfig : SET CMD_SET_TOKEN :: %s", set_conf.valu);
#endif
			set_cmd_token(set_conf.valu);
			strcat(rbody, "SET CMD_SET_TOKEN :: "); strcat(rbody, set_conf.valu); strcat(rbody, "\n");
		}
	}
	

#ifdef D_LOG
	printf("%s", rbody);
#endif
	
	sendTo(udp_sock, addr, encode_base64(rbody)); // None Format...
	free(rbody); // pt :: 1
	return;
}


static void cmd_nodequery(struct sockaddr_in addr, int udp_sock, char *saddr, 
		node_table *index_head, node_table *raw_node_index_head)
{	
#ifdef D_LOG
	log_debug("[+] cmd_nodequery : _");
#endif 

	int port0_node_query = 0, raw_node_query = 0;
	char rmsg[MESSAGE_BUF];
	
	port0_node_query = table_search(index_head, saddr);
	raw_node_query = table_search(raw_node_index_head, saddr);
	
	if(port0_node_query < 0 && raw_node_query < 0)
	{
		Format(rmsg, MESSAGE_BUF, CMD_NODE_QUERY_RESPONSE, saddr, CMD_NODE_QUERY_TAG, "FALSE");
	//mFormat_Encoding(rmsg, MESSAGE_BUF, CMD_NODE_QUERY_RESPONSE, saddr, CMD_NODE_QUERY_TAG, "FALSE");
		sendTo(udp_sock, addr, encode_base64(rmsg));
		return;
	}
	else
	{
		Format(rmsg, MESSAGE_BUF, CMD_NODE_QUERY_RESPONSE, saddr, CMD_NODE_QUERY_TAG, "TRUE");
	//mFormat_Encoding(rmsg, MESSAGE_BUF, CMD_NODE_QUERY_RESPONSE, saddr, CMD_NODE_QUERY_TAG, "TRUE");
		sendTo(udp_sock, addr, encode_base64(rmsg));
		return;
	}
}


static void cmd_nodedelete(struct sockaddr_in addr, int udp_sock, char *saddr,  // root
		node_table *index_head, node_table *raw_node_index_head)
{
#ifdef D_LOG
	log_debug("[+] cmd_nodedelete : _");
#endif 
	
	int search_status = 0;
	
	search_status = table_search(index_head, saddr);
	if(search_status < 0)
	{
		goto TABLE2;
	}
	else
	{
		if(safety_array_access(index_head, search_status))
		{
#ifdef LOG
			log_info("[*] cmd_nodedelete : Deletes(normal_index) :: %s", saddr);
#endif
			deletes(index_head, search_status);
			sendTo(udp_sock, addr, CMD_CMPLETION_RES_MSG);
			return;
		}
		else
		{
			goto END;

		}
	}
	
TABLE2:
	search_status = table_search(raw_node_index_head, saddr);
	if(search_status < 0)
	{
		goto END;
	}
	else
	{
		if(safety_array_access(raw_node_index_head, search_status))
		{
#ifdef LOG 
			log_info("[*] cmd_nodedelete : Deletes(raw_node_index) :: %s", saddr);
#endif
			deletes(raw_node_index_head, search_status);
			sendTo(udp_sock, addr, CMD_CMPLETION_RES_MSG);
			return;
		}
		else
		{
			goto END;
		}
	}
	
END:
#ifdef LOG 
	log_info("[*] cmd_nodedelete : None :: %s", saddr);
#endif
	sendTo(udp_sock, addr, CMD_NONE_RESPONSE_MSG);
	return;
}


static void cmd_sessionclose(struct sockaddr_in addr, int udp_sock) // root 
{
#ifdef D_LOG
	log_debug("[*] cmd_sessionclose : _");
#endif
	
	// set_session_gate(false);
	sendTo(udp_sock, addr, CMD_SESSIONCLOSE_RES_MSG);
	return;
}


//static void cmd_table_show(struct sockaddr_in addr, int udp_sock, char *args, node_table *index_head)
static void cmd_table_show(CMD_TABLE_SHOW opt)
{
#ifdef D_LOG
	log_debug("[+] cmd_table_show : _");
#endif 
	int port0_addr_t_len=0, raw_addr_t_len=0, nport=0;
	size_t total_body_size=0, raw_total_body_size=0, resbuf_size=0; 
	char *addr_table_buffer, *raw_addr_table_buffer, *resbuf;
	char rawaddrbuf[BUF_32BYTE];
	
	node_table *port0_a = opt.index_head;
	node_table *raw_a	= opt.raw_node_index_head;
	

	port0_addr_t_len = array_len(opt.index_head);
	raw_addr_t_len = array_len(opt.raw_node_index_head);
	if(raw_addr_t_len == 0 && port0_addr_t_len == 0)
	{
		sendTo(opt.udp_sock, opt.addr, CMD_SHOW_TABLE_NONE_RESPONSE);
		return;
	}
	
	total_body_size = (IPV4_BUF * port0_addr_t_len) + 8;
	raw_total_body_size = (ADDR_BUF * raw_addr_t_len) + 8;

	addr_table_buffer = malloc(total_body_size); // m:1:st
	if(addr_table_buffer == NULL)
	{	
#ifdef E_LOG
		log_error("[!] cmd_table_show : malloc");
#endif	
		sendTo(opt.udp_sock, opt.addr, CMD_ERROR_TYPE_2_RESPONSE_MSG);
		return;
	}
	else
	{
		Memset(addr_table_buffer, total_body_size);
	}
	

	
	raw_addr_table_buffer = malloc(raw_total_body_size); // m:2:st
	if(raw_addr_table_buffer == NULL)
	{
#ifdef E_LOG
		log_error("[!] cmd_table_show : malloc");
#endif	
		free(addr_table_buffer); // m:1:st
		sendTo(opt.udp_sock, opt.addr, CMD_ERROR_TYPE_2_RESPONSE_MSG);
		return;
	}
	else
	{
		Memset(raw_addr_table_buffer, raw_total_body_size);
	}
	
	
	if(port0_addr_t_len != 0)
	{
		for(port0_a=port0_a->next; port0_a!=NULL; port0_a=port0_a->next)
		{	
			strcat(addr_table_buffer, (char*)port0_a->node->node); strcat(addr_table_buffer, "\r\n");
		}
	}
	else
	{
		strcpy(addr_table_buffer, "None");
	}
	
	if(raw_addr_t_len != 0)
	{
		for(raw_a=raw_a->next; raw_a!=NULL; raw_a=raw_a->next)
		{
			Memset(rawaddrbuf, BUF_32BYTE);
			memcpy(&nport, &raw_a->node->next, sizeof(int));
			sprintf(rawaddrbuf, "%s:%d", (char*)raw_a->node->node, nport);
			strcat(raw_addr_table_buffer, rawaddrbuf); strcat(raw_addr_table_buffer, "\r\n");
		}
	}
	else
	{
		//strcat(raw_addr_table_buffer, "None");
		strcpy(raw_addr_table_buffer, "None");
	}
	

#ifdef D_LOG 
	log_debug("[+] cmd_table_show : table :: %s", addr_table_buffer);
#endif 
	
		
	resbuf_size = port0_addr_t_len + raw_addr_t_len + strlen(CMD_SHOW_TABLE_DATA_RESPONSE)+256;
	resbuf = malloc(resbuf_size); // m:3:st
	if(resbuf == NULL)
	{
#ifdef E_LOG
		log_error("[!] cmd_table_show : malloc");
#endif 
		free(addr_table_buffer);		// m:1:st
		free(raw_addr_table_buffer);	// m:2:st
		
		return;
	}
	else
	{
		Memset(resbuf, resbuf_size);
	}
	

	Format(resbuf, resbuf_size, CMD_SHOW_TABLE_DATA_RESPONSE, addr_table_buffer, raw_addr_table_buffer);
	//mFormat_Encoding(resbuf, resbuf_size, CMD_SHOW_TABLE_DATA_RESPONSE, addr_table_buffer, raw_addr_table_buffer);
	
#ifdef D_LOG
	printf("debug :::: %s\n", resbuf);
#endif
	
	sendTo(opt.udp_sock, opt.addr, encode_base64(resbuf));
	
	free(addr_table_buffer);	 // m:1:st
	free(raw_addr_table_buffer); // m:2:st
	free(resbuf);				 // m:3:st
	
	return;
}


/*
void cmd_set(struct sockaddr_in addr, int udp_sock, char *msg, 
		node_table *index_head, node_table *raw_node_index_head)
*/
void cmd_set(CMD_ARG cmd_arg)
{
#ifdef D_LOG 
	printf("[+] cmd_session : body::%s\n", cmd_arg.msg);
#endif 
	

	CMD_SET c_s;
	Memset(&c_s, sizeof(c_s));
	
	// 認証
	if(
		xml_item_tag(cmd_arg.msg, CMD_ITEM_PASS_TAG, c_s.cmd_password_bud, BUF_32BYTE)		== 0
		&& xml_item_tag(cmd_arg.msg, CMD_ITEM_USER_TAG, c_s.cmd_user_buf, BUF_32BYTE)		== 0
		&& xml_item_tag(cmd_arg.msg, CMD_ITEM_TOKEN_TAG, c_s.cmd_token_buf, BUF_128BYTE)	== 0
	  ){
#ifdef AUTH_LOG
		log_info("%s", "[*] cmd_set : auth it");
#endif 
	
		if(
			strcmp(c_s.cmd_password_bud, CMD_SET_PASSWORD)	== 0
			&& strcmp(c_s.cmd_user_buf, CMD_SET_USER)		== 0
			&& strcmp(c_s.cmd_token_buf, CMD_SET_TOKEN)		== 0
		  ){
			/* root command.. */
#ifdef LOG 
			log_info("[*] cmd_set : root");
#endif 
			if(xml_item_tag(cmd_arg.msg, CMD_SESSION_CONFIG, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
			{
				// cmd :: SETCONFIG
				cmd_setconfig(cmd_arg.addr, cmd_arg.udp_sock, c_s.cmd_msg_buf);
				return;
			}
			else if(xml_item_tag(cmd_arg.msg, CMD_SESSION_CLOSE, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
			{
				// cmd :: SESSIONCLOSE	
				cmd_sessionclose(cmd_arg.addr, cmd_arg.udp_sock);
				return;
			}
			else if(xml_item_tag(cmd_arg.msg, CMD_SHOW_CONFIG, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
			{
				// cmd :: SHOWCONFIG
				cmd_showconfig(cmd_arg.addr, cmd_arg.udp_sock);
				return;
			}
			else if(xml_item_tag(cmd_arg.msg, CMD_NODE_DELETE_TAG, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
			{
				// cmd :: NODEDELTE
				cmd_nodedelete(cmd_arg.addr, cmd_arg.udp_sock, 
						c_s.cmd_msg_buf, cmd_arg.index_head, cmd_arg.raw_node_index_head);
				return;
			}
			else
			{	
				// default
				sendTo(cmd_arg.udp_sock, cmd_arg.addr, CMD_ERROR_TYPE_2_RESPONSE_MSG);
				return;
			}
			
		}
		else
		{
			sendTo(cmd_arg.udp_sock, cmd_arg.addr, CMD_ERROR_TYPE_1_RESPONSE_MSG);
			return;
		}
	}
		
	/* user commamd */
#ifdef LOG 
	log_info("[*] cmd_set : user");
#endif 
	
	if(xml_item_search(cmd_arg.msg, CMD_HELP_TAG) == 0)
	{
		// cmd :: HELP
		cmd_help(cmd_arg.addr, cmd_arg.udp_sock);
		return;
	}
	else if(xml_item_search(cmd_arg.msg, CMD_NODE_QUERY_TAG) == 0)
	{
		// cmd :: NODE_QUERY
		if(xml_item_tag(cmd_arg.msg, CMD_NODE_QUERY_TAG, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
		{
			if(strlen(c_s.cmd_msg_buf) > IPV4_BUF)
			{
				sendTo(cmd_arg.udp_sock, cmd_arg.addr, CMD_ERROR_TYPE_2_RESPONSE_MSG);
				return;	
			}
			
			cmd_nodequery(cmd_arg.addr, cmd_arg.udp_sock, c_s.cmd_msg_buf, cmd_arg.index_head, cmd_arg.raw_node_index_head);
			return;
		}
	}
	else if(xml_item_search(cmd_arg.msg, CMD_TABLE_SHOW_TAG) == 0)
	{
		// cmd :: SHOW_TABLE
		if(xml_item_tag(cmd_arg.msg, CMD_TABLE_SHOW_TAG, c_s.cmd_msg_buf, MESSAGE_BUF) == 0)
		{
			//cmd_table_show(cmd_arg.addr, cmd_arg.udp_sock, c_s.cmd_msg_buf, cmd_arg.index_head);
			CMD_TABLE_SHOW set;
			Memset(&set, sizeof(set));

			set.addr = cmd_arg.addr;	set.udp_sock = cmd_arg.udp_sock;
			set.args = c_s.cmd_msg_buf;	set.index_head = cmd_arg.index_head; set.raw_node_index_head = cmd_arg.raw_node_index_head;
			
			cmd_table_show(set);
			return;
		}
	}
	else
	{
		// cmd :: default
		sendTo(cmd_arg.udp_sock, cmd_arg.addr, CMD_ERROR_TYPE_1_RESPONSE_MSG);
		return;
	}
}




