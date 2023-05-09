#include "g.h"
#include "f.h"

void showtable(node_table *index_head, char *text)
{	
	print_array(index_head, text);
	return;
}

void printconfig(void)
{	
	printf("KEEP_ALIVE_SIGNAL_INTERVAL	:: %d\n", KEEP_ALIVE_SIGNAL_INTERVAL);
	printf("KEEP_NODE_SIGNAL_INTERVAL	:: %d\n", KEEP_NODE_SIGNAL_INTERVAL);
	printf("FROM_RETURN_PORT			:: %d\n", FROM_RETURN_PORT);
	printf("SESSION_PORT				:: %d\n", SESSION_PORT);
	
	printf("CMD_SET_PASSWORD			:: %s\n", CMD_SET_PASSWORD);
	printf("CMD_SET_USER				:: %s\n", CMD_SET_USER);
	printf("CMD_SET_TOKEN				:: %s\n", CMD_SET_TOKEN);
	
	return;
}

char* showconfig(char *buf, size_t bsize)
{	
#ifdef D_LOG
	printf("KEEP_ALIVE_SIGNAL_INTERVAL	:: %d\n", KEEP_ALIVE_SIGNAL_INTERVAL);
	printf("KEEP_NODE_SIGNAL_INTERVAL	:: %d\n", KEEP_NODE_SIGNAL_INTERVAL);
	printf("FROM_RETURN_PORT			:: %d\n", FROM_RETURN_PORT);
	printf("SESSION_PORT				:: %d\n", SESSION_PORT);
	
	printf("CMD_SET_PASSWORD			:: %s\n", CMD_SET_PASSWORD);
	printf("CMD_SET_USER				:: %s\n", CMD_SET_USER);
	printf("CMD_SET_TOKEN				:: %s\n", CMD_SET_TOKEN);
#endif 
	
	snprintf(buf, bsize, SET_CONF_FORMAT,
			KEEP_ALIVE_SIGNAL_INTERVAL,
			KEEP_NODE_SIGNAL_INTERVAL,
			FROM_RETURN_PORT,
			SESSION_PORT,
			CMD_SET_PASSWORD,
			CMD_SET_USER,
			CMD_SET_TOKEN);
	

	return encode_base64(buf); 
}	



