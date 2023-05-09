#ifndef _F_H_
#define _F_H_

#include "include/hcpy.h"

extern int KEEP_ALIVE_SIGNAL_INTERVAL;
extern int KEEP_NODE_SIGNAL_INTERVAL;
extern int FROM_RETURN_PORT;

extern int SESSION_PORT;  // to uint_8 

extern char CMD_SET_PASSWORD[BUF_32BYTE];
extern char CMD_SET_USER[BUF_32BYTE];
extern char CMD_SET_TOKEN[BUF_128BYTE];

extern bool SESSION_GATE;

struct SET_INTERVAL_CONFIG{
	int keep_alive_signal_interval;
	int keep_node_signal_interval;
	int from_return_port;
	int session_port;

	bool session_gate;

	char *cmd_set_password;
	char *cmd_set_user;
	char *cmd_set_token;
};

typedef struct SET_INTERVAL_CONFIG SET_INTERVAL_CONFIG;

void set_keep_alive_signal_interval(int t);
void set_keep_node_signal_interval(int t);
void set_from_return_port(int port);
void set_session_port(int port);
void set_session_gate(bool set);
bool set_cmd_set_password(char *valu);
bool set_cmd_set_user(char *valu);
bool set_cmd_token(char *valu);


int set_interval_config(SET_INTERVAL_CONFIG set_conf);
#endif 
