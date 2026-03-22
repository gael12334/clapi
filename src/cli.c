/*
 * Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
 */

#include "cli.h"
#include <ctype.h>
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CLI_INPUT_SIZE 256
#define CLI_TOKEN_NMBR (CLI_INPUT_SIZE >> 1)

typedef cli_error_t (*cli_parser_t)(const char*, cli_param_t*);
typedef unsigned long ulong;

const char CLI_INT_ARG[] = "\x01\x00\x00\x00";  // INTEGER NUMBER
const char CLI_REAL_ARG[] = "\x02\x00\x00\x00"; // REAL NUMBER
const char CLI_STR_ARG[] = "\x03\x00\x00\x00";  // DOUBLE-QUOTE DELIMITED
const char CLI_WORD_ARG[] = "\x04\x00\x00\x00"; // ALPHANUMERICAL SEQUENCE OF CHARS
const char CLI_HEX_ARG[] = "\x05\x00\x00\x00";  // SEQUENCE OF HEXADECIMAL NUMBERS

const char CLI_EIDLISTNULL[] = "Id list was NULL.";
const char CLI_EFUNCNULL[] = "Function was NULL";
const char CLI_EROUTE_NULL[] = "Route was NULL.";
const char CLI_ENUMBERNULL[] = "The id list length was NULL.";
const char CLI_EIDLSTVALID[] = "Id list must be NULL.";
const char CLI_ENMBRVALID[] = "Id list length must be NULL.";
const char CLI_EBUFFERFULL[] = "The module's buffer is full.";
const char CLI_EUNINITED[] = "The module's is uninitialized.";
const char CLI_ERTEBUFVOID[] = "The module's route buffer is empty or NULL.";
const char CLI_EMISSINGRTE[] = "Absence of route assigned to specified id.";
const char CLI_EINVINTEGER[] = "Interger was invalid.";
const char CLI_EINVREALNUM[] = "Real number was invalid.";
const char CLI_EINVSTRLIT[] = "String literal was invalid.";
const char CLI_EINV_WORD[] = "Word was invalid.";
const char CLI_EINVHEXADEC[] = "Hexadecimal sequence was invalid";
const char CLI_E2MANYPARAM[] = "Exceeded number of expected parameters.";
const char CLI_EMALLOCNULL[] = "Memory allocation has failed.";
const char CLI_EINPUTNULL[] = "Input buffer was NULL.";
const char CLI_EINDEXINV[] = "Index is invalid.";
const char CLI_EPARAMNULL[] = "Parameter reference was NULL.";
const char CLI_EIDWASDIFF[] = "Id was diffrent than expected.";
const char CLI_ESTRENDDQ[] = "Missing closing double quotes for string literal.";
const char CLI_ESTRSEPCH[] = "Expected nul or blank char after closing double quotes.";
const char CLI_ETOKLSTNUL[] = "Token list was null.";

const static cli_parser_t parser[] = {
    [0] = NULL,         /**/
    [1] = cli_parseint, /**/
    [2] = cli_parsernb, /**/
    [3] = cli_parsestr, /**/
    [4] = cli_parsewrd, /**/
    [5] = cli_parsehex, /**/
};

static struct {
  struct {
    ulong num;
    ulong cnt;
    cli_route_t* buf;
  } rte;

  struct {
    ulong cnt;
    char* buf[CLI_INPUT_SIZE / 2];
  } tok;

  struct {
    ulong num;
    ulong cnt;
    cli_param_t buf[CLI_INPUT_SIZE / 2];
  } var;

  char input[CLI_INPUT_SIZE];
} cli = {0};

cli_error_t cli_parseint(const char* id, cli_param_t* value) {
  char* end;
  long xint = strtol(id, &end, 10);
  int has_failed = 0;
  has_failed |= (*end == *(id));
  has_failed |= (*end != '\0');
  has_failed |= (errno != 0);

  if (has_failed) {
    return CLI_EINVINTEGER;
  }

  value->u.xint = xint;
  value->size = 0;
  return NULL;
}

cli_error_t cli_parsernb(const char* id, cli_param_t* value) {
  char* end;
  double xfloat = strtod(id, &end);
  int has_failed = 0;
  has_failed |= (*end == *(id));
  has_failed |= (*end != '\0');
  has_failed |= (errno != 0);

  if (has_failed) {
    return CLI_EINVREALNUM;
  }

  value->u.xfloat = xfloat;
  value->size = 0;
  return NULL;
}

cli_error_t cli_parsestr(const char* id, cli_param_t* value) {
  if (*id != '\"') {
    return CLI_EINVSTRLIT;
  }

  char* ending = strchr(id + 1, '\"');
  if (ending == NULL) {
    return CLI_EINVSTRLIT;
  }
  if (*(ending + 1) != '\0') {
    return CLI_EINVSTRLIT;
  }

  value->u.uxptr = id;
  value->size = ending - id;
  return NULL;
}

cli_error_t cli_parsewrd(const char* id, cli_param_t* value) {
  static const char accept[] = "0123456789"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz";
  ulong len = strspn(id, accept);

  if (id[len] != '\0') {
    return CLI_EINV_WORD;
  }

  value->u.uxptr = id;
  value->size = len;
  return NULL;
}

cli_error_t cli_parsehex(const char* id, cli_param_t* value) {
  static const char accept[] = "0123456789"
                               "ABCDEF"
                               "abcdef";
  ulong len = strspn(id, accept);

  if (id[len] != '\0') {
    return CLI_EINVHEXADEC;
  }

  value->u.uxptr = id;
  value->size = len;
  return NULL;
}

cli_error_t cli_cmpidlist(cli_route_t* rte, cli_tokbuf_t in, cli_tokcnt_t num) {
  ulong isparam = 1;
  ulong prmndx = 0;    // param index
  ulong psrndx;        // parser index
  cli_parser_t psrfnc; // parser function
  cli_param_t* param;
  cli_error_t error;

  for (ulong i = 0; i < num; i++) {
    psrndx = *rte->id_list[i];
    isparam = 1;
    isparam &= prmndx < rte->param_cnt;
    isparam &= psrndx >= CLI_INT_ARG[0];
    isparam &= psrndx <= CLI_HEX_ARG[0];

    if (isparam) {
      psrfnc = parser[psrndx];
      param = &cli.var.buf[prmndx];
      if ((error = psrfnc(in[i], param)))
        return error;
      prmndx++;
    }

    else if (strcmp(in[i], rte->id_list[i]) != 0) {
      return CLI_EIDWASDIFF;
    }

    continue;
  }

  cli.var.cnt = prmndx;
  return NULL;
}

void* cli_setbuf_s(cli_route_t* buffer, ulong number) {
  void* retval = cli.rte.buf;
  cli.rte.buf = buffer;
  cli.rte.num = number;
  cli.rte.cnt = 0UL;
  return retval;
}

void cli_clearrte(void) {
  cli_route_t* rte = cli.rte.buf;
  ulong* cnt = &cli.rte.cnt;
  memset(rte, 0, sizeof(cli_route_t) * (*cnt));
  *cnt = 0;
}

ulong cli_rtecount(void) {
  return cli.rte.cnt;
}

cli_error_t cli_addrte_s(const char* id_list[], ulong id_num, cli_func_t func) {
  if (id_list == NULL)
    return CLI_EIDLISTNULL;
  if (func == NULL)
    return CLI_EFUNCNULL;
  if (cli.rte.buf == NULL)
    return CLI_ERTEBUFVOID;
  if (cli.rte.cnt >= cli.rte.num)
    return CLI_EBUFFERFULL;

  ulong* rtecnt = &cli.rte.cnt;
  ulong prmcnt = 0;
  ulong isparam;
  for (ulong i = 0; i < id_num; i++) {
    isparam = 1;
    isparam &= (id_list[i][0] >= CLI_INT_ARG[0]);
    isparam &= (id_list[i][0] <= CLI_HEX_ARG[0]);
    prmcnt += isparam;
  }

  cli.rte.buf[*rtecnt] = (cli_route_t){
      .id_list = id_list,
      .id_nmbr = id_num,
      .param_cnt = prmcnt,
      .func = func,
  };

  (*rtecnt)++;
  return NULL;
}

cli_error_t cli_hasrte_s(cli_toklst_t* tokens, cli_route_t** out) {
  if (tokens == NULL)
    return CLI_ETOKLSTNUL;
  if (out == NULL)
    return CLI_EROUTE_NULL;
  if (cli.rte.buf == NULL)
    return CLI_ERTEBUFVOID;

  cli_error_t error;
  for (cli_route_t* route = cli.rte.buf; route < cli.rte.buf + cli.rte.cnt; route++) {
    error = cli_cmpidlist(route, tokens->buf, tokens->cnt);
    if (error == NULL) {
      *out = route;
      return NULL;
    }
  }

  return CLI_EIDWASDIFF;
}

cli_error_t cli_usrinput(FILE* stream, cli_toklst_t* out) {
  if (out == NULL)
    return CLI_ETOKLSTNUL;

  if (stream == NULL)
    stream = stdin;

  // zeroing
  cli.tok.cnt = 0;
  memset(cli.tok.buf, 0, sizeof(cli.tok.buf));
  memset(cli.input, 0, sizeof(cli.input));

  fgets(cli.input, sizeof(cli.input) - 1, stream);
  ulong length = strcspn(cli.input, "\r\n");
  cli.input[length] = '\0';

  for (ulong i = 0; i < length; i++) {
    // skip leading whitespaces
    while (isblank((cli.input[i]))) {
      cli.input[i] = '\0';
      i++;
    }

    // start parsing token
    if (cli.input[i] == '\"') {
      // printf("-------------str\n");
      char* starting = &cli.input[i];
      ulong length = strcspn(&starting[1], "\"") + 1;
      char* ending = &starting[length];

      if (*ending == '\0') {
        // printf("%i\n", *ending);
        return CLI_ESTRENDDQ;
      }

      if (!isblank(ending[1])) {
        if (ending[1] != '\0') {
          // printf("%c\n", ending[1]);
          return CLI_ESTRSEPCH;
        }
      }

      i += length;
      ending[1] = '\0';
      cli.tok.buf[cli.tok.cnt] = starting;
      cli.tok.cnt++;
    }

    // anything else than string literal
    else {
      // printf("-------------word\n");
      char* starting = &cli.input[i];
      char* ending = starting;
      while (!isblank(*ending) && i < length) {
        ending++;
        i++;
      }

      *ending = '\0';
      // printf("len: %zu\n", ending - starting);
      if (ending - starting > 1) {
        cli.tok.buf[cli.tok.cnt] = starting;
        cli.tok.cnt++;
      }
    }
    // printf("i: %lu\ntok.cnt: %lu\n", i, cli.tok.cnt);
    // printf("tok: %s\n", cli.tok.buf[cli.tok.cnt - 1]);
  }

  *out = (cli_toklst_t){.buf = cli.tok.buf, .cnt = cli.tok.cnt};
  return NULL;
}

cli_error_t cli_getparam(ulong index, cli_param_t* param) {
  if (index >= cli.var.cnt)
    return CLI_EINDEXINV;
  if (param == NULL)
    return CLI_EPARAMNULL;

  *param = cli.var.buf[index];
  return NULL;
}

unsigned long cli_paramcnt(void) {
  return cli.var.cnt;
}

unsigned long cli_dbgtoknb(void) {
  return cli.tok.cnt;
}

const char** cli_dbgtokbf(void) {
  return (const char**)cli.tok.buf;
}
