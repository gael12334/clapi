/*
 * Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
 */

#include "cli.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define countof(x) (sizeof(x) / sizeof(x[0]))

int should_close = 0;

const char* command_echo_id[] = {"echo", CLI_STR_ARG};
const char* command_quit_id[] = {"quit"};

void commmand_echo(unsigned long argc, void* argv) {
  cli_param_t* params = argv;
  const char* text = params[1].u.uxptr;
  printf("%s\n", text);
}

void command_quit(unsigned long argc, void* argv) {
  should_close = 1;
}

int main(int argc, char** argv) {
  cli_route_t buffer[1];
  cli_setalloc(0x0);
  cli_setdeall(0x0);
  cli_addrte_s(command_echo_id, countof(command_echo_id), commmand_echo);
  cli_addrte_s(command_quit_id, countof(command_quit_id), command_quit);

  char input_buffer[1024];
  char** tokens = NULL;
  unsigned long a = 0;

  while (should_close == 0) {
    fwrite("$ ", 1, sizeof("$ "), stdout);
    fgets(input_buffer, sizeof(input_buffer), stdin);

    unsigned long length = strnlen(input_buffer, sizeof(input_buffer));
    if (length < 2) {
      continue;
    }

    input_buffer[length - 2] = '\0'; // remove the \r\n
    length -= 2;

    if (length == 0) {
      continue;
    }

    unsigned long i = 0;
    unsigned long c = 0;

    while (i < length - 1) {
      // ignore leading whitespace
      i = i + strspn(&input_buffer[i], " \f\v\t\n\r");
      if (input_buffer[i] == '\0') {
        continue;
      }

      // add more memory
      if (c == a) {
        tokens = realloc(tokens, sizeof(char*) * (a + 1) * 2);
        a = (a + 1) * 2;
      }

      // save start of token
      tokens[c] = &input_buffer[i];

      // string literal
      if (input_buffer[i] == '\"') {
        i = i + strcspn(&input_buffer[i], "\"");
        input_buffer[++i] = '\0';
        i++;
      }

      // otherwise split at whitespace
      else if (input_buffer[i] != '\0') {
        i = i + strcspn(&input_buffer[i], " \f\v\t\n\r");
        input_buffer[++i] = '\0';
        i++;
      }
    }

    cli_route_t* rte;
    cli_hasrte_s((const char**)tokens, c, &rte);
    if (rte != NULL) {
      rte->func(rte->param_cnt, rte->param_list);
    }
  }

  free(tokens);
  cli_clearrte();
  return 0;
}
