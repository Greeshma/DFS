#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#include "common.h"

char *server;
int port = 5000;

char *mount_path;

int send_command(int cmd)
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    char sendBuff[1024];
    struct sockaddr_in serv_addr; 

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET, server, &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    memset(sendBuff, 0, MAX_SIZE);
    sprintf(sendBuff, "%d\n", cmd);
    n = write(sockfd, sendBuff, strlen(sendBuff)); 

    memset(recvBuff, 0, MAX_SIZE);
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

    return 0;
}

int parse_response(int cmd, char *resp) {
  char ip[MAX_STR], *buf;
  switch(cmd) {
    case INITIALISE_CLIENT:
      strcpy(ip, resp);
      printf("\nEnter the path to mount the server partition: ");
      mount_path = (char *)malloc(sizeof(char) * MAX_PATH);
      buf = (char *)malloc(sizeof(char) * MAX_PATH);
      scanf("%s", mount_path);
      sprintf(buf, "bin/dclient -S %s %s", ip, mount_path);
      printf("\nCommande executing %s", buf);
      system(buf);
      break;
    case SYNC_MEM_SERVERS:
      printf("\n %s \n", resp);
      break;
    case CLOSE_CLIENT:
      printf("\n %s \n", resp);
      printf("\npath: %s", mount_path);
      sprintf(buf, "fusermount -u %s", mount_path);
      system(buf);
      break;
  }
}

/*
int main(int argc, char **argv) {
  if(argc != 2)
  {
      printf("\n Usage: %s <ip of server> \n",argv[0]);
      return 1;
  } 
  if(argc == 3)
  {
    port = atoi(argv[2]);
  }
  server = argv[1];

  send_command(INITIALISE_CLIENT);
  send_command(SYNC_MEM_SERVERS);
  send_command(CLOSE_CLIENT);

  return 0;
}
*/
