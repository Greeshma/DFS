#ifndef __MEM_SERV_H
#define __MEM_SERV_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct _mem_serv {
//  struct sockaddr_in serv_addr; 
  char ip[INET_ADDRSTRLEN];
  int is_assigned;
  int is_synced;

  struct _mem_serv *next;
  struct _mem_serv *prev;
} mem_serv;

int init_mem_serv();

#endif
