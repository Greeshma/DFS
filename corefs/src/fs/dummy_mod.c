#include <string.h>
#include "common.h"
//#include <fuse.h>


/* corefs_server_operations myop; */
/* int temp_read(COMMCTX* ctx, corefs_packet* cmd){ */
/*  return myop.handle_read(ctx,cmd); */
  
/* } */
/* int temp_write(COMMCTX* ctx, corefs_packet* cmd){ */
/*   return myop.handle_write(ctx, cmd); */
  
/* } */

/* dummy upper layer module */


int my_check_access(COMMCTX * ctx, const user_info * u, int * status, const char * path, const char * path1, int op){
  /* Do nothing */
  *status = 0;
  return PROCEED;
}



void my_get_user_info(user_info * u, const char * path, const char * opt_path)
{
  u->uid = fuse_get_context()->uid;
  u->gid = fuse_get_context()->gid;
  u->num_sgids = 0;
}

void my_goodbye(COMMCTX* ctx) {
  return;
}

int my_receive(COMMCTX* ctx, char* buf, int len) {
  /* note "len" here is the buffer size.  we want one packet of any
   * length up to "len". then return the actual size received. */
  
  corefs_header *header=(corefs_header*)buf;
  int psize;
  
  if (phy_receive(ctx, buf, header_size) < 0) {
  
    return -1;
  }
  
  /* We need to decap the header to understand the file size */
  char temp_buf[header_size];
  memcpy(temp_buf,buf,header_size);
  decap_corefs_header(temp_buf, (corefs_packet*)buf); /* Just to be
                                                       * safe lets use
                                                       * different
                                                       * buffers */
  
   psize=header->payload_size;
  
  if (phy_receive(ctx, buf+header_size, psize) < 0) {
    return -1;
  }
  return header_size + psize;
}

int my_send(COMMCTX* ctx, char* buf, int len) {
	return phy_send(ctx, buf, len);
}

int setup(COMMCTX* ctx) {
  
  ctx->receive = my_receive;
  ctx->send = my_send;
  return PROCEED;
}

int server_hello(char * client, char * server, COMMCTX * ctx) { 
  
  return setup(ctx); 
}

int client_hello(char * client, char * server, COMMCTX * ctx){
  
  return setup(ctx);
  
}


int my_parse_arguments(char * argstr, int argc, char** argv){
  return PROCEED;
}


int up_client_init(corefs_client_operations * op){
  op->up_new_server = client_hello;
  op->up_eof_connection = my_goodbye;
  op->up_parse_arguments = my_parse_arguments;
  op->up_get_user_info = my_get_user_info;
  return PROCEED;
  
}



int up_server_init(corefs_server_operations * op){
  op->up_new_client = server_hello;
 
  op->up_eof_connection = my_goodbye;
  op->up_parse_arguments = my_parse_arguments;
  op->up_check_access = my_check_access;
  return PROCEED;
  
}
