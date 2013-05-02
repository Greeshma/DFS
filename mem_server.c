#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "mem_server.h"
#include "list.h"

mem_serv mem_serv_list;

int init_mem_serv() {
    config_t cfg;
    config_setting_t *servers, *ip;

    config_init(&cfg);
    if( init_mem_serv_list(&mem_serv_list) < 0){
        fprintf(stderr, "\nError: Initialising memory servers list");
        return -1;
    }

    if(! config_read_file(&cfg, "mem_serv.cfg")){
        fprintf(stderr, "Error: Reading from config file\n");
        return -1;
    }

    servers = config_lookup(&cfg, "servers");
    int count = config_setting_length(servers);
    int i;
    const char *ip_addr;

    for(i=0; i<count; i++){
        ip_addr = config_setting_get_string_elem(servers, i);
        printf("\nAdding memory server with ip %s", ip_addr);
        mem_serv_list_append(&mem_serv_list, ip_addr);
    }
}

int get_client_count(mem_serv* curr) {
    return curr->count;
}

void set_client_count(mem_serv* curr, int n) {
    curr->count = n;
    return;
}

void inc_client_count(mem_serv* curr) {
    curr->count++;
    return;
}

void dec_client_count(mem_serv* curr) {
    curr->count--;
    return;
}

void set_is_assigned(mem_serv* curr, int n) {
    curr->is_assigned = n;
    return;
}

void set_is_synced(mem_serv* curr, int n) {
    curr->is_synced = n;
}

int get_is_assigned(mem_serv* curr) {
    return curr->is_assigned;
}

int get_is_synced(mem_serv* curr) {
    return curr->is_synced;
}

void get_mem_serv_by_ip(mem_serv *node, const char *ip) {
    mem_serv *curr, *prev;
    curr = &mem_serv_list;

    while (curr != NULL) {
        prev = curr;
        curr = curr->next;

        if(strcasecmp(ip, curr->ip) == 0) {
            node = curr;
            return;
        }
    }
}
