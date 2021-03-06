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
  
int port = 5000;

int sync_in_process = 0;

void process_command(char *, char *);

void get_mem_serv_ip(char *ip) {
    // todo: use load balancer algo to get correct ip
    printf("\nUsing the load balancer to get ip of the memory server");
    get_best_mem_serv_ip(ip);
    printf("\nReturning the ip address of memory server %s", ip);
    return;
}

void *handle_client(void *arg) 
{
  char sendBuff[MAX_STR];
  char recvBuff[MAX_STR];
  time_t ticks; 
  int n = 0;
  int connfd;
  connfd = *((int *) arg);

  printf("\nClient accepted. Now to handle the client request and respond. ");

  bzero(recvBuff, MAX_SIZE);
  n = read(connfd, recvBuff, sizeof(recvBuff)-1);
  printf("\nReceived request: %s\n", recvBuff, n);

  memset(sendBuff, 0, sizeof(sendBuff)); 
  process_command(sendBuff, recvBuff);
  n = write(connfd, sendBuff, strlen(sendBuff)); 
  printf("\nWriting response: %s", sendBuff);

  close(connfd);
}

void init_client(char *resp) {
    printf("\nInitialising client");

    char ip[INET_ADDRSTRLEN + 1];
    fprintf(stdout, "\nObtaining mem server ip...\n");
    get_mem_serv_ip(ip);

    fprintf(stdout, "Obtained mem server : %s\n", ip);

    mem_serv *curr_mem_serv;
    fprintf(stdout, "Requesting for mem-server with ip: %s\n", ip);
    curr_mem_serv = get_mem_serv_by_ip(ip);
    
    fprintf(stdout, "Received a mem_server count :%d\n", curr_mem_serv->count);

    inc_client_count(curr_mem_serv);
    printf("\nClient count of memory server %s is %d", ip, get_client_count(curr_mem_serv));

    strcpy(resp, ip);
    return;
}

void sync_mem_serv(char *resp, char *ip) {
    printf("\nTrying to sync memory servers");

    FILE *fp;
    fp = fopen(FILEPATH, "a+");

    printf("\nFinding all non-synced servers");
    find_all_not_synced_servers(fp);

    char buf[MAX_STR];
    printf("\nUsing Unison to sync the servers");
    sprintf(buf, "./synchronize.sh %s", ip);
    system(buf);

    fclose(fp);

    strcpy(resp, "Sync completed\n");
    return;
}

void close_client(char *resp, char *ip) {
    mem_serv *curr_mem_serv;
    fprintf(stdout, "Initiated connection closing for %s\n", ip);
    curr_mem_serv = get_mem_serv_by_ip(ip);
    fprintf(stdout, "Found mem server allocated for %s with count %d\n", ip, curr_mem_serv->count);
    dec_client_count(curr_mem_serv);
    printf("\nCLosing client. Ip address of the attached server: %s and number of clients attached is %d ", ip, get_client_count(curr_mem_serv));

    strcpy(resp, "\nBye bye!! ^_^\n");
    printf("\n____________________________________________________________________________\n");
    return;
}

void process_command(char *resp, char *req) {
    char ch = req[0];
    int cmd = atoi(&ch);
    char *ip = req+1;
    printf("\nProcess command function: cmd: %d, ip: %s", cmd, ip);
    switch (cmd) {
        case INITIALISE_CLIENT:
            init_client(resp);
            break;
        case SYNC_MEM_SERVERS:
            sync_mem_serv(resp, ip);
            break;
        case CLOSE_CLIENT:
            close_client(resp, ip);
            break;
        default:
            strcpy(resp, "Error: Unknown command");
            fprintf(stderr, "Unknown command recieved: %d\n", cmd);
    }
}

int main(int argc, char *argv[])
{
  int listenfd = 0, connfd = 0;
  struct sockaddr_in serv_addr; 

  pthread_t thread;

  if(argc > 1)
    port = atoi(argv[1]);

  if(init_mem_serv() < 0 ) {
    return 0;
  }

  printf("\nCreating a socket for listening to clients on port %d ", port);
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset(&serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port); 

  bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)); 

  listen(listenfd, 10); 
  printf("\nListening on port %d", port);

  while(1)
  {
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL); 

    printf("\nAccepted client. Spawning a new thread for the client... \n");
    pthread_create(&thread, NULL, handle_client, (void *)&connfd);
    sleep(1);
   }
}
