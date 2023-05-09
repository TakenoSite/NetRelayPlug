#ifndef _HCPY_H_
#define _HCPY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <pthread.h>
#include <err.h>
#include <errno.h>


/* util */
#include "table.h"
#include "xml_item.h"
#include "base64.h"
#include "sockset.h"
#include "multi_tool.h"


/* log */
#include "print_log.h"


/* buf */
#define MESSAGE_BUF		1024
#define RECV_MSG_BUF	2048
#define IPV4_BUF		17
#define ADDR_BUF		22
#define TTL_BUF			16

#define BUF_16BYTE	16
#define BUF_32BYTE  32
#define BUF_64BYTE  64
#define BUF_128BYTE 128 
#define BUF_256BYTE 256


/* data type */
typedef unsigned int UINT;
typedef uint8_t UINT8;
typedef uint16_t UINT16;
typedef short int SINT;


#endif
