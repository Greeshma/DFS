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
int sockfd = -1;

char *mount_path;
int is_synced;

int send_command(int cmd) {
    printf("\nline 21 send_command %d", cmd);
    int n = 0;
    char recvBuff[MAX_SIZE];
    char sendBuff[MAX_SIZE];
    struct sockaddr_in serv_addr; 
    printf("\nline 26 send_command %d", cmd);

    if(sockfd < 0) {
        if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            printf("\n Error : Could not create socket \n");
            return 1;
        } 
                
        printf("\n send_command %d", cmd);

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
    }

    memset(sendBuff, 0, MAX_SIZE);
    sprintf(sendBuff, "%d%s", cmd, server);
    printf("\nSending command: %s", sendBuff);
    n = write(sockfd, sendBuff, strlen(sendBuff)); 

    memset(recvBuff, 0, MAX_SIZE);
    n = read(sockfd, recvBuff, sizeof(recvBuff)-1);
    printf("\nReceived response: %s", recvBuff);
    parse_response(cmd, recvBuff);

    close(sockfd);
    sockfd = -1;
    return 0;
}

int parse_response(int cmd, char *resp) {
  char ip[MAX_STR], *buf, ch;
  switch(cmd) {
    case INITIALISE_CLIENT:
      is_synced = 0;
      strcpy(ip, resp);
      printf("\nEnter the path to mount the server partition: ");
      mount_path = (char *)malloc(sizeof(char) * MAX_PATH);
      buf = (char *)malloc(sizeof(char) * MAX_PATH);
      scanf("%s", mount_path);
      printf("\nMounting the file system from %s on %s", ip, mount_path);
      sprintf(buf, "corefs/bin/dclient -S %s %s", ip, mount_path);
      system(buf);
      break;
    case SYNC_MEM_SERVERS:
      printf("\n %s \n", resp);
      is_synced = 1;
      break;
    case CLOSE_CLIENT:
      if(! is_synced) {
        printf("\nThe data has been changed locally. Are you sure you don't want to sync the server? (Y/N) ");
        scanf(" %c", &ch);
        printf("\nReceived char %c \n", ch);
        switch(tolower(ch)) {
          case 'y':
            break;
          case 'n':
            printf("\nReceived no");
            send_command(SYNC_MEM_SERVERS);
            break;
          default:
            fprintf(stderr, "\nInvalid input");
        }
      }
      printf("\nUnmounting the file system from %s", mount_path);
      sprintf(buf, "fusermount -u %s", mount_path);
      system(buf);
      break;
  }
}

int main(int argc, char **argv) {
    char ch;

    if(argc != 2)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
    server = argv[1];
    if(argc == 3)
    {
        port = atoi(argv[2]);
    }

    send_command(INITIALISE_CLIENT);
    printf("\nClient init done. Enter any key to continue: ");
    scanf(" %c", &ch);
    send_command(SYNC_MEM_SERVERS);
    printf("\nSync memeory servers done. Enter any key to continue: ");
    scanf(" %c", &ch);
    send_command(CLOSE_CLIENT);
    printf("\nClient close done.");

    return 0;
}
