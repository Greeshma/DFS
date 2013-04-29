#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "mem_server.h"
#include "list.h"

mem_serv *mem_serv_list;

int init_mem_serv() {
  config_t cfg;
  config_setting_t *servers, *ip;
  
  config_init(&cfg);
  init_mem_serv_list(mem_serv_list);

  if(! config_read_file(&cfg, "mem_serv.cfg")){
    fprintf(stderr, "Error: reading from config file\n");
    return -1;
  }

  servers = config_lookup(&cfg, "servers");
  int count = config_setting_length(servers);
  int i;
  const char *ip_addr;

  for(i=0;i<count;i++){
    ip_addr = config_setting_get_string_elem(servers, i);

    mem_serv_list_append(mem_serv_list, ip_addr);
  }
}
