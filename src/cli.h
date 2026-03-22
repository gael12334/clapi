/*
 * Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
 */

#pragma once
#include <stdio.h>

extern const char CLI_INT_ARG[];
extern const char CLI_REAL_ARG[];
extern const char CLI_STR_ARG[];
extern const char CLI_WORD_ARG[];
extern const char CLI_HEX_ARG[];

extern const char CLI_EIDLISTNULL[];
extern const char CLI_EFUNCNULL[];
extern const char CLI_EBUFFERFULL[];
extern const char CLI_EUNINITED[];
extern const char CLI_ERTEBUFVOID[];
extern const char CLI_EMISSINGRTE[];
extern const char CLI_EID_IS_NULL[];
extern const char CLI_EVALUENULL[];
extern const char CLI_EINVINTEGER[];
extern const char CLI_EINVREALNUM[];
extern const char CLI_EINVSTRLIT[];
extern const char CLI_EINV_WORD[];
extern const char CLI_EINVHEXADEC[];
extern const char CLI_E2MANYPARAM[];
extern const char CLI_EMALLOCNULL[];
extern const char CLI_EINPUTNULL[];
extern const char CLI_EINDEXINV[];
extern const char CLI_EPARAMNULL[];
extern const char CLI_EIDWASDIFF[];
extern const char CLI_ESTRENDDQ[];
extern const char CLI_ESTRSEPCH[];
extern const char CLI_ETOKLSTNUL[];

typedef unsigned long cli_tokcnt_t;
typedef char** cli_tokbuf_t;
typedef const char* cli_error_t;
typedef void (*cli_func_t)(void*);

typedef struct cli_param {
  union {
    long xint;
    double xfloat;
    const void* uxptr;
  } u;

  unsigned long size;
} cli_param_t;

typedef struct cli_toklst {
  cli_tokcnt_t cnt;
  cli_tokbuf_t buf;
} cli_toklst_t;

typedef struct cli_route {
  const char** id_list;
  unsigned long id_nmbr;
  unsigned long param_cnt;
  cli_func_t func;
} cli_route_t;

void* cli_setbuf_s(cli_route_t* buffer, unsigned long number);
void cli_clearrte(void);
unsigned long cli_rtecount(void);
cli_error_t cli_addrte_s(const char* id_list[], unsigned long id_num, cli_func_t func);
cli_error_t cli_parseint(const char* id, cli_param_t* value);
cli_error_t cli_parsernb(const char* id, cli_param_t* value);
cli_error_t cli_parsestr(const char* id, cli_param_t* value);
cli_error_t cli_parsewrd(const char* id, cli_param_t* value);
cli_error_t cli_parsehex(const char* id, cli_param_t* value);
cli_error_t cli_cmpidlist(cli_route_t* rte, cli_tokbuf_t in, cli_tokcnt_t num);
cli_error_t cli_hasrte_s(cli_toklst_t* tokens, cli_route_t** out);
cli_error_t cli_usrinput(FILE* stream, cli_toklst_t* out);
cli_error_t cli_getparam(unsigned long index, cli_param_t* param);
unsigned long cli_paramcnt(void);
unsigned long cli_dbgtoknb(void);
const char** cli_dbgtokbf(void);
