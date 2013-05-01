#include <stdio.h>
#include <stdlib.h>

#include "client.c"
#include "common.h"

int main(int argc, char **argv) {
  char resp[MAX_STR];
  int i;
  strcpy(resp, "localhost");

  printf("\ntest.c 11");
  parse_response(INITIALISE_CLIENT, resp);
  scanf("%d", &i);
  parse_response(CLOSE_CLIENT, resp);

  return 0;
}
