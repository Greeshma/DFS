#ifndef _COREFS_TYPES_H
#define _COREFS_TYPES_H

#define PROCEED 0xABCD
#define STOP -1

int up_new_server(char * client, char * server, COMMCTX * ctx);
int up_new_client(char * client, char * server, COMMCTX * ctx);
int up_parse_arguments(char * argstr, int argc, char** argv);

#endif
