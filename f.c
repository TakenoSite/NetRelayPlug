#include "f.h"
#include "z.h"

/* global config */
int KEEP_ALIVE_SIGNAL_INTERVAL;
int KEEP_NODE_SIGNAL_INTERVAL;
int FROM_RETURN_PORT;
int SESSION_PORT;	

char CMD_SET_PASSWORD[BUF_32BYTE];
char CMD_SET_USER[BUF_32BYTE];
char CMD_SET_TOKEN[BUF_128BYTE];

bool SESSION_GATE;

/* body */


void set_keep_alive_signal_interval(int t)
{
	KEEP_ALIVE_SIGNAL_INTERVAL = t;	
}

void set_keep_node_signal_interval(int t)
{
	KEEP_NODE_SIGNAL_INTERVAL = t;
}

void set_from_return_port(int port)
{
	FROM_RETURN_PORT = port;
}

void set_session_port(int port)
{
	SESSION_PORT = port;	
}

void set_session_gate(bool set)
{
	SESSION_GATE = set;
}

bool set_cmd_set_password(char *valu)
{ 
	if(strlen(valu) < BUF_32BYTE)
	{
		strcpy(CMD_SET_PASSWORD, valu);
		return true;
	}
	else
	{
		return false;
	}
}

bool set_cmd_set_user(char *valu)
{
	if(strlen(valu) < BUF_32BYTE)
	{
		strcpy(CMD_SET_USER, valu);
		return true;
	}
	else
	{
		return false;
	}
}

bool set_cmd_token(char *valu)
{
	if(strlen(valu) < BUF_32BYTE)
	{
		strcpy(CMD_SET_TOKEN, valu);
		return true;
	}
	else
	{
		return false;
	}

}

int set_interval_config(SET_INTERVAL_CONFIG set_conf)
{
	int e = 0;

	set_keep_alive_signal_interval(set_conf.keep_alive_signal_interval);
	set_keep_node_signal_interval(set_conf.keep_node_signal_interval);
	set_from_return_port(set_conf.from_return_port);
	set_session_port(set_conf.session_port);

	set_session_gate(set_conf.session_gate);

	if(set_cmd_set_password(set_conf.cmd_set_password))
	{
		e += 0;
	}
	else
	{
		e += 1;
	}
	
	if(set_cmd_set_user(set_conf.cmd_set_user))
	{
		e += 0;
	}
	else
	{
		e += 2;
	}

	if(set_cmd_token(set_conf.cmd_set_token))
	{
		e += 0;
	}
	else
	{
		e += 3;
	}

	return e;
}
