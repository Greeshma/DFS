#ifndef __COREFS_H
#define __COREFS_H

#include "corefs_api.h"

void client_op_init(corefs_client_operations *op);
int corefs_main(int argc, char *argv[], const corefs_client_operations *op);

#endif
