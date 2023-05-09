#include "a.h"
#include "b.h"
#include "c.h"
#include "d.h"
#include "e.h"
#include "f.h"
#include "z.h"

#include "include/hcpy.h"



static int set_session_config(void)
{
	SET_INTERVAL_CONFIG set_conf;
	int set_status = 0;
		
	set_conf.keep_alive_signal_interval = 600;
	set_conf.keep_node_signal_interval	= 30;
	set_conf.from_return_port			= 41666; // max is 2 << 8
	set_conf.session_port				= 12345; 

	set_conf.session_gate = true;
	
	set_conf.cmd_set_password = "admin";   // 32byte
	set_conf.cmd_set_user	  =	"admin";   // 32byte
	set_conf.cmd_set_token	  =	"zxcvbnm"; // 128byte
		
	set_status = set_interval_config(set_conf);
	if(set_status != 0)
	{
		return set_status;	
	}

	return 0;	
}

static bool set_node_keepalive(int ssock, node_table *index_head)
{		
	pthread_t keepalive_t, udppunchkeep_t;
	int keepalive_s, udppunchkeep_s;
		
	THREAD_KEEPALIVE_ARGS set_keepalive, *set_keepalive_p; 
	UDP_PUNCH_KEEP_ARGS set_udppunch, *set_udppunch_p;
	Memset(&set_keepalive, sizeof(set_keepalive)); Memset(&set_udppunch, sizeof(set_udppunch));

	set_keepalive_p = &set_keepalive; set_udppunch_p = &set_udppunch;
	
	set_keepalive_p -> keepalive_sockets = ssock; set_udppunch_p -> udp_socket = ssock;
	set_keepalive_p -> index_head = index_head; set_udppunch_p -> index_head = index_head;
	set_keepalive_p -> keepalive_t = &udppunchkeep_t; set_udppunch_p -> udppunchkeep_t = &keepalive_t;

	keepalive_s = pthread_create(&keepalive_t, NULL, keepalive, set_keepalive_p);
	udppunchkeep_s = pthread_create(&udppunchkeep_t, NULL, udp_punch_keep, set_udppunch_p);
	
	if(keepalive_s != 0 && udppunchkeep_s != 0)
	{
		return true;	
	}
	else
	{
		return false;
	}
}


int main()
{
	CREATE_SESSION *set_cs, set_cs_n;	
	set_cs = &set_cs_n;
	
	node_table *index_head, index_array_null,
			   *raw_node_index_head, raw_node_index_null;		
	
	vec2 *vec2_head, vec2_array_null,
		 *raw_node_body_head, raw_node_body_null;
	
	index_head = &index_array_null; raw_node_index_head = &raw_node_index_null;
	vec2_head = &vec2_array_null; raw_node_body_head = &raw_node_body_null;
	
	int create_table_status = 0, session_config_status = 0;
	bool set_keepalive_status;

	/* create table */
	create_table_status = create_table(vec2_head, index_head);
	if(create_table_status != 0)
	{
#ifdef LOG
		log_error("[!] main : error in create_table");
#endif 
		exit(42);
	}

	create_table_status = create_table(raw_node_body_head, raw_node_index_head);
	if(create_table_status != 0)
	{
#ifdef LOG 
		log_error("[!] main : error in create_table");
#endif
		exit(42);
	}



	/* set session config */
	session_config_status = set_session_config();
	if(session_config_status != 0)
	{
#ifdef LOG
		log_error("[!] session_config : status code :: %d", session_config_status);
		switch(session_config_status)
		{
			case 1:
				log_error("[!] set_config_error : set_password");
				break;
			case 2:
				log_error("[!] set_config_error : set_user");
				break;
			case 3:
				log_error("[!] set_config_error : set_passeord and set_user");
				break;
			case 5:
				log_error("[!] set_config_error : set_user and set_token");
				break;
			case 6:
				log_error("[!] set_config_error : set_passeord and set_user and set_token");
				break;
		}
#endif
		
		exit(42);
	}
	
	/* session create */
	set_cs->addr.sin_family = AF_INET;
	set_cs->addr.sin_port = htons(SESSION_PORT);
	set_cs->addr.sin_addr.s_addr = INADDR_ANY;
	
	set_cs->index_head = index_head; 
	set_cs->raw_node_index_head = raw_node_index_head;
	set_cs->vec2_head = vec2_head;
	set_cs->raw_node_body_head = raw_node_body_head;

	set_cs->udp_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
	if(set_cs->udp_sock < 0)
	{
#ifdef LOG
		log_error("[!] mian : socket :: %d", errno);
#endif
		exit(42);
	}

#ifdef RUNITKEEPALIVE
	/* thread session process */
#ifdef LOG 
	log_info("[*] mian : thread session setup..");
#endif 
	set_keepalive_status = set_node_keepalive(set_cs->udp_sock, set_cs->index_head);
	if(set_keepalive_status)
	{
#ifdef LOG
		log_error("[!] main : process keepalive");
#endif 
		exit(42);
	}
	/* */
#endif // RUNITKEEPALIVE


#ifdef RUNITSESSION
	/* run it */

	create_session(set_cs);
#endif // RUNITSESSION
	
	return 0;
}


