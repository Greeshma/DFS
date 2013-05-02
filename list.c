#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "list.h"

#define MAX_PATH 257
#define MAX_LIST_LENGTH 100

/* 
 * create mem server list head
 */
int init_mem_serv_list(mem_serv* mem_serv_head) {
  mem_serv_head = (mem_serv*) malloc(sizeof(mem_serv));
  if(mem_serv_head == NULL) {
    fprintf(stderr, "\nCould not initialise memory servers list");
    return -1;
  }

  //mem_serv_head->ip = NULL;
  strcpy(mem_serv_head->ip, "");
  mem_serv_head->is_assigned  = -1;
  mem_serv_head->is_synced = -1;

  mem_serv_head->count = -1;

  mem_serv_head->next = NULL;
  mem_serv_head->prev = NULL;
  return 0;
}

/* free mem server list */
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

  strcpy(node->ip, ip);
  set_is_assigned(node, 0);
  set_is_synced(node, 0);
  set_client_count(node, 0);

  node->prev = NULL;
  node->next = NULL;

  return 1;
}

/*
 * kln:
 * Assumed the list entries start with the head and the head is not used to keep
 * count of number of elements in the list. It is always traversed till the end
 * of the list.
 *
 * Directly creating a node inside the append instead of using another func call
 * to create_mem_serv_node...just to get rid of the pointer hassel.
 */

int mem_serv_list_append(mem_serv *head, const char *ip) {
    fprintf(stdout, "\nIn mem_serv_list_append with ip %s", ip);
    mem_serv *curr = head;
    mem_serv *prev;
    mem_serv *new;

    if(curr != NULL){//if head/list exists
        /* move to end of list */
        while(curr->next != NULL) {
            curr = curr->next;
        }
    }else{
        fprintf(stdout, "\nseems like the mem serv list is missing!! i cannot proceed further.\n");
        return -1;
    }
    new = (mem_serv *)malloc(sizeof(mem_serv));
    //new->ip = ip; // add new ip
    strcpy(new->ip, ip);
    set_is_assigned(new, 0);
    set_is_synced(new, 0);
    set_client_count(new, 0);

    new->next = NULL; // this is the last in the list
    new->prev = curr; // append to current list
    curr->next = new; 

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


