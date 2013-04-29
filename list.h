#ifndef __LIST_H
#define __LIST_H

#include "mem_server.h"

int init_mem_serv_list(mem_serv *);
void free_mem_serv_list(mem_serv *);

int mem_serv_list_append(mem_serv *head, const char *ip);

/*
int insert_info_ptr(list_head *, list_tail **, const char* addr,
                    unsigned int addr_length, void * data);
int delete_info(list_head *, list_tail **, const char * addr,
                unsigned int addr_info);
int remove_info(list_head *,  list_tail **, const char * addr,
                unsigned int addr_length);
int remove_info_ptr(list_head *,  list_tail **, void *);
int change_info_ptr(list_head *, list_tail **, const char * addr,
                    unsigned int addr_legnth, void * data);
int get_info_ptr(list_head *,  list_tail **, const char * addr,
                 unsigned int addr_length, void **pointer_to_data);

*/

#endif
