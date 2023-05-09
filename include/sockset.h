#ifndef _RELAY_SOCKSET_H_
#define _RELAY_SOCKSET_H_

#include <sys/socket.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <string.h>

void addr_in(struct sockaddr_in *addr, char *host, int port);
int sockdg();
int sock_bind(int sock, struct sockaddr_in addr);
int sendTo(int sock, struct sockaddr_in addr, char*msg);
int SockTimeout(int sockfd,int sec, int usec);
int ThreadSockTimeout(int sockfd, int sec, int usec);

int SockSt();
int SockConnect(int sock, struct sockaddr_in addr);
int SendToStream(int sock , char *msg);



#endif
