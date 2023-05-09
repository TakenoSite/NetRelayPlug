#ifndef _G_H_
#define _G_H_

#include "include/hcpy.h"

#define SET_CONF_FORMAT "KEEP_ALIVE_SIGNAL_INTERVAL :: %d\nKEEP_NODE_SIGNAL_INTERVAL :: %d\nFROM_RETURN_PORT :: %d\nSESSION_PORT :: %d\nCMD_SET_PASSWORD :: %s\nCMD_SET_USER :: %s\nCMD_SET_TOKEN :: %s\n"

#define CONF_FORMAT_SIZE (strlen(SET_CONF_FORMAT) + 128)


void showtable(node_table *index_head, char *text);
char *showconfig(char *buf, size_t bsize);
void printconfig(void);


#endif
