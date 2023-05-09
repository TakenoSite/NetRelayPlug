#ifndef _E_H_
#define _E_H_

#include "include/hcpy.h"
#include "z.h"

#define CMD_SHOW_TABLE_NONE_RESPONSE	"PEVONDE+PFJFUz48Q01EPjxUWVBFPlNIT1dfVEFCTEU8L1RZUEU+PFJFU09MVkU+Tk9ORTwvUkVTT0xWRT48L0NNRD48L1JFUz48L0VONDE+"
#define CMD_SHOW_TABLE_DATA_RESPONSE	"<EN41><RES><CMD>SHOWTABLE</CMD><VALU><PORT0>%s</PORT0><RAW>%s</RAW></VALU></RES></EN41>"

#define CMD_ERROR_TYPE_1_RESPONSE_MSG	"PEVONDE+PENNRD48UkVTPkVSUk9SMTwvUkVTPjwvQ01EPjwvRU40MT4=" 
#define CMD_ERROR_TYPE_2_RESPONSE_MSG	"PEVONDE+PENNRD48UkVTPkVSUk9SMjwvUkVTPjwvQ01EPjwvRU40MT4="
#define CMD_NONE_RESPONSE_MSG			"PEVONDE+PENNRD48UkVTPk5vbmU8L1JFUz48L0NNRD48L0VONDE+"
#define CMD_NODE_QUERY_RESPONSE			"<EN41><RES><CMD><TYPE>%s<TYPE><QUERY>%s<QUERY><RESOLVE>%s</RESOLVE></CMD></RES></EN41>"
#define CMD_SESSIONCLOSE_RES_MSG		"PEVONDE+PENNRD48UkVTPkNMT1NFPC9SRVM+PC9DTUQ+PC9FTjQxPg=="
#define CMD_CMPLETION_RES_MSG			"PEVONDE+PENNRD48UkVTPkNvbXBsZXRpb248L1JFUz48L0NNRD48L0VONDE+"


struct CMD_SET{
	char cmd_password_bud[BUF_32BYTE];
	char cmd_user_buf[BUF_32BYTE];
	char cmd_token_buf[BUF_32BYTE];
	char cmd_msg_buf[MESSAGE_BUF];
};

struct CMD_ARG
{
	struct sockaddr_in addr;
	int udp_sock;
	char *msg;
	
	/* table */
	node_table *index_head;
	node_table *raw_node_index_head;	
};

struct  CMD_TABLE_SHOW
{
	struct sockaddr_in addr;
	int udp_sock;
	char *args;

// TABLE
	node_table *index_head; // port0 
	node_table * raw_node_index_head;
};

void cmd_set(CMD_ARG cmd_arg);


#endif 
