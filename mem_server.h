#ifndef __MEM_SERV_H
#define __MEM_SERV_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef struct _mem_serv {
  char ip[INET_ADDRSTRLEN];
  int is_assigned;
  int is_synced;

  int count;

  struct _mem_serv *next;
  struct _mem_serv *prev;
} mem_serv;

int init_mem_serv();

int get_client_count(mem_serv*);
void set_client_count(mem_serv*, int);
void inc_client_count(mem_serv*);
void dec_client_count(mem_serv*);

void set_is_assigned(mem_serv*, int);
void set_is_synced(mem_serv*, int);

int get_is_assigned(mem_serv*);
int get_is_synced(mem_serv*);

//void get_mem_serv_by_ip(mem_serv *head, const char *ip, mem_serv *node);
void get_mem_serv_by_ip(mem_serv*, const char *ip);

#endif
