#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "list.h"

#define MAX_PATH 257
#define MAX_LIST_LENGTH 100

int init_mem_serv_list(mem_serv *mem_serv_head) {
  mem_serv_head = malloc(sizeof(mem_serv));
  if(mem_serv_head == NULL) {
    fprintf(stderr, "\nCould not initialise memory servers list");
    return -1;
  }

  mem_serv_head->ip = NULL;
  mem_serv_head->is_assigned  = -1;
  mem_serv_head->is_synced = -1;

  mem_serv_head->next = NULL;
  mem_serv_head->prev = NULL;
  return 1;
}

void free_mem_serv_list(mem_serv *mem_serv_head) {
  mem_serv *temp;
  while(mem_serv_head != NULL) {
    temp = mem_serv_head;
    mem_serv_head = mem_serv_head->next;
    free(temp);
  }
  return;
}

int create_mem_serv_node(mem_serv *node, const char *ip) {
  node = malloc(sizeof(mem_serv));
  if(node == NULL) {
    fprintf(stderr, "\nError in creating a memory server node");
    return -1;
  }

  node->ip = ip;
  node->is_assigned = 0;
  node->is_synced = 0;

  node->prev = NULL;
  node->next = NULL;

  return 1;
}

int mem_serv_list_append(mem_serv *head, const char *ip) {
  mem_serv *curr = head;
  mem_serv *prev;
  while(curr != NULL) {
    prev = curr;
    curr = curr->next;
  }

  mem_serv *temp;
  create_mem_serv_node(temp, ip);
  temp->prev = prev;
  return 1;
}
                   
int remove_mem_serv_node(mem_serv *head, const char *ip) {
  mem_serv *curr = head;
  mem_serv *next = curr->next;
  mem_serv *prev;
  while (curr != NULL) {
    if(strcasecmp(ip, curr->ip) == 0) {
      prev->next = next;
      next->prev = prev;
      free(curr);
      return 1;
    }
  }
  fprintf(stderr, "\nError: Memory server with ip %s not found", ip);
  return -1;
}

int mem_serv_list_remove(mem_serv *head, const char *ip) {
}

/*
int insert_info_ptr(list_head * head,  list_tail ** tail, const char* addr,
                    unsigned int addr_length, void * data)
{

  server_list * node = NULL;
#ifdef ACCOUNT_DEBUG
  fprintf(stderr, "INSERT: inserting file %s with key size %u \n", addr, alength);
#endif

  if(addr_length > MAX_PATH){
    fprintf(stderr,"Insert: file name %s exceeds length limit %d\n", addr,MAX_PATH);
    return -1;
  }
  
  if(head == NULL){
    fprintf(stderr,"Insert: head of the list is NULL - initialize it!\n");
    return -1;
  }
  
  
  if(head->alength == MAX_LIST_LENGTH){
    
    if(tail == NULL){ // this case should never happen, but just in case
      fprintf(stderr,"INSERT: tail of the list is NULL - potential error while initializing list head\n");
      return -1;
    }
    node = *tail;
    if(node->alength < addr_length){
      free(node->server_addr);
      node->server_addr = (char *) calloc(addr_length, 1);
      if(node->server_addr == NULL) {
        fprintf(stderr, "INSERT: out of memory errno[%i]\n",errno);
        return -1;
      }
    }

    memcpy(node->server_addr,addr,addr_length);
    node->alength = addr_length;

    node->data=data;
    
    (*tail)->next = NULL;
    if(MAX_LIST_LENGTH  == 1)
      return 0;
    *tail = (*tail)->prev;
  }
  else {
    node = (server_list*)calloc(1,sizeof(server_list));
    if(node == NULL) {
      fprintf(stderr, "out of memory errno[%i]\n",errno);
      return -1;
    }
    
    node->server_addr = (char *) calloc(addr_length, 1);
    if(node->server_addr == NULL) {
      fprintf(stderr, "INSERT out of memory errno[%i]\n",errno);
      return -1;
    }
    
    memcpy(node->server_addr,addr, addr_length);
    node->alength = addr_length;

    node->data=data;
    
    if(head->alength == 0) // this is the first item in the list
      *tail = node;
    head->alength += 1;
  }
  node->next = head->next;
  if(head->next)
    head->next->prev = node;
  node->prev = head;
  head->next = node;

  return 0;
}


int get_info_ptr(list_head * head,  list_tail ** tail, const char *addr,
                 unsigned int addr_length, void **pointer_to_data)
{

  server_list * node;
  if(head == NULL || addr == NULL)
    return -1;

  
#ifdef ACCOUNT_DEBUG
  fprintf(stderr, "GET_INFO: for addr %s\n",addr);
#endif

  node = head->next;
  while(node){
    
    if(node->alength == addr_length &&
       memcmp(node->server_addr,addr,addr_length) == 0){
      // remove node from the list
      if(node->prev != head){
        node->prev->next = node->next;
        if(node->next)
          node->next->prev = node->prev;
        else // this is the last element
          *tail = node->prev;
        //insert node at the front
        node->next = head->next;
        node->next->prev = node;
        head->next = node;
        node->prev = head;
      }
      *pointer_to_data=node->data;
      return node->alength;
    }
    node = node->next;
  }
  return -1;
}



int change_info_ptr(list_head * head,  list_tail ** tail, const char * addr,
                unsigned int addr_length, void * data)
{
  server_list * node;
  if(head == NULL || addr == NULL)
    return -1;

  node = head->next;
  while(node){
    if(node->alength == addr_length &&
       memcmp(node->server_addr,addr, addr_length) == 0){
      if(node->prev != head){
        // remove node from the list
        node->prev->next = node->next;
        if(node->next)
          node->next->prev = node->prev;
        else // this is the last element
          *tail = node->prev;
        
        //insert node at the front
        node->next = head->next;
        node->next->prev = node;
        head->next = node;
        node->prev = head;
      }
      node->data=data;
      return node->alength;
    }
    node = node->next;
  }
  return -1;
  
}



int delete_info(list_head * head,  list_tail ** tail, const char * addr,
                unsigned int addr_length){
  server_list * node;
  if(head == NULL || addr == NULL)
    return -1;

  node = head->next;
  //print_list(head);
  while(node){
    if(memcmp(node->server_addr,addr,addr_length) == 0){
      // remove node from the list
      node->prev->next = node->next;
      if(node->next)
        node->next->prev = node->prev;
      else // this is the last element
        *tail = node->prev;
      if(node->server_addr) free(node->server_addr);
      if(node->data) free(node->data);
      if(node) free(node);
      head->alength -= 1;

      //print_list(head);
      
      return 0;
    }
    node = node->next;
  }
  return -1;
}

int remove_info(list_head * head,  list_tail ** tail, const char * addr,
                unsigned int addr_length){
  server_list * node;
  if(head == NULL || addr == NULL)
    return -1;

  node = head->next;
  //print_list(head);
  while(node){
    if(memcmp(node->server_addr,addr,addr_length) == 0){
      // remove node from the list
      node->prev->next = node->next;
      if(node->next)
        node->next->prev = node->prev;
      else // this is the last element
        *tail = node->prev;
      head->alength -= 1;

      //print_list(head);
      
      return 0;
    }
    node = node->next;
  }
  return -1;
}

int remove_info_ptr(list_head * head,  list_tail ** tail, void *ptr){
  server_list * node;
  if(head == NULL || ptr == NULL)
    return -1;

  node = head->next;
  while(node){
    if(node->data == ptr){
      // remove node from the list
      node->prev->next = node->next;
      if(node->next)
        node->next->prev = node->prev;
      else // this is the last element
        *tail = node->prev;
      head->alength -= 1;
    }
    node = node->next;
  }
  return -1;
}


void free_list(list_head * head)
{
  if(head == NULL)
    return;
  server_list * node = head->next;
  while(node){
    head->next = node->next;
    if(node->server_addr) free(node->server_addr);
    if(node->data) free(node->data);
    if(node) free(node);
    node = head->next;
  }
  head->alength = 0;
  head->next = NULL;
  head->prev = NULL;
}
*/
