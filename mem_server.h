#ifndef __MEM_SERV_H
#define __MEM_SERV_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct _mem_serv {
  struct sockaddr_in serv_addr; 
  int is_assgned;
  int is_synced;
} mem_serv;

int init_mem_serv();

#endif
