#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <pthread.h>

#include "common.h"
#include "mem_server.h"

void *handle_client(void *arg) 
{
  char sendBuff[MAX_STR];
  char recvBuff[MAX_STR];
  time_t ticks; 
  int n = 0;
  int connfd;
  connfd = *((int *) arg);

  bzero(recvBuff, MAX_SIZE);
  n = read(connfd, recvBuff, sizeof(recvBuff)-1);
  printf("\nserver received: %s\n", recvBuff, n);

  memset(sendBuff, 0, sizeof(sendBuff)); 
  process_command(sendBuff, atoi(recvBuff));
  n = write(connfd, sendBuff, strlen(sendBuff)); 

  close(connfd);
}

void init_client(char *resp) {
  strcpy(resp, "Init client\n");
  return;
}

void sync_mem_serv(char *resp) {
  strcpy(resp, "sync\n");
  return;
}

void close_client(char *resp) {
  //todo: clean up
  strcpy(resp, "Bye bye!! ^_^\n");
  return;
}

void process_command(char *resp, int command) {
  switch (command) {
    case INITIALISE_CLIENT:
      init_client(resp);
      break;
    case SYNC_MEM_SERVERS:
      sync_mem_serv(resp);
      break;
    case CLOSE_CLIENT:
      close_client(resp);
      break;
    default:
     strcpy(resp, "Error: Unknown command");
  }
}

int main(int argc, char *argv[])
{
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 
  int port = 5000;

  pthread_t thread;

  if(argc > 1)
    port = atoi(argv[1]);

  if(init_mem_serv() < 0 ) {
    return 0;
  }

  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 

  while(1)
  {
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

    printf("\naccept\n");
    pthread_create(&thread, NULL, handle_client, (void *)&connfd);
    sleep(1);
   }
}
