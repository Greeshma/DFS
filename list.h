#ifndef __LIST_H
#define __LIST_H

#include "mem_server.h"

mem_serv* init_mem_serv_list(void);
void free_mem_serv_list(mem_serv *mem_serv_head);

int create_mem_serv_node(mem_serv *node, const char *ip);
int mem_serv_list_append(mem_serv *head, const char *ip);
int remove_mem_serv_node(mem_serv *head, const char *ip);
int mem_serv_list_remove(mem_serv *head, const char *ip);

#endif
