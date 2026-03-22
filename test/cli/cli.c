/*
 * Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
 */

#include "../../src/cli.h"
#include "../test.h"

#include <stdlib.h>

/*
 * Unit tests - fixtures
 */

void* test_dummy_cli_allocator(unsigned long x) {
  return NULL;
}

void test_dummy_cli_deallocator(void* x) {
  return;
}

void test_dummy_cli_function(void* opt) {
  return;
}

void test_dummy_cli_function2(void* opt) {
  return;
}

/*
 * Unit tests
 */

void test_cli_setbuf_s(void) {
  // arrange
  cli_route_t buffer[32];

  // act
  void* previous = cli_setbuf_s(buffer, 32);
  void* current = cli_setbuf_s(NULL, 0);

  // assert
  testing(test_IntEqInt(__func__, iexpr(previous), iexpr(NULL)));
  testing(test_IntEqInt(__func__, iexpr(current), iexpr(buffer)));
  test_passed(__func__);
}

void test_cli_addrte_s(void) {
  // arrange
  cli_route_t buffer[8];
  cli_func_t function = test_dummy_cli_function;
  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const unsigned long id_num = 3, param_cnt = 1;

  cli_setbuf_s(buffer, 8);

  // act
  cli_error_t error = cli_addrte_s(id_list, id_num, function);

  // assert
  testing(test_IntEqInt(__func__, iexpr(error), iexpr(NULL)));
  testing(test_UintEqUint(__func__, uexpr(buffer[0].func), uexpr(function)));
  testing(test_UintEqUint(__func__, uexpr(buffer[0].param_cnt), uexpr(param_cnt)));
  testing(test_UintEqUint(__func__, uexpr(buffer[0].id_nmbr), uexpr(id_num)));
  testing(test_UintEqUint(__func__, uexpr(cli_rtecount()), uexpr(1)));
  test_passed(__func__);
}

void test_cli_addrte_s__buffer_null(void) {
  // arrange
  cli_func_t function = test_dummy_cli_function;
  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const unsigned long id_num = 3;

  cli_setbuf_s(NULL, 8); // invalid state

  // act
  cli_error_t error = cli_addrte_s(id_list, id_num, function);

  // assert
  testing(test_IntEqInt(__func__, iexpr(error), iexpr(CLI_ERTEBUFVOID)));
  test_passed(__func__);
}

void test_cli_addrte_s__func_null(void) {
  // arrange
  cli_route_t buffer[8];
  cli_func_t function = NULL; // invalid state
  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const unsigned long id_num = 3;

  cli_setbuf_s(buffer, 8);

  // act
  cli_error_t error = cli_addrte_s(id_list, id_num, function);

  // assert
  testing(test_IntEqInt(__func__, iexpr(error), iexpr(CLI_EFUNCNULL)));
  test_passed(__func__);
}

void test_cli_addrte_s__id_list_null(void) {
  // arrange
  cli_route_t buffer[8];
  cli_func_t function = test_dummy_cli_function;
  const char** id_list = NULL; // invalid state
  const unsigned long id_num = 3;

  cli_setbuf_s(buffer, 8);

  // act
  cli_error_t error = cli_addrte_s(id_list, id_num, function);

  // assert
  testing(test_IntEqInt(__func__, iexpr(error), iexpr(CLI_EIDLISTNULL)));
  test_passed(__func__);
}

void test_cli_clearrte(void) {
  // arrange
  cli_route_t buffer[8];
  cli_setbuf_s(buffer, 8);

  cli_func_t function = test_dummy_cli_function;
  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const unsigned long id_num = 3;

  cli_setbuf_s(buffer, 8);
  cli_addrte_s(id_list, id_num, function);

  // act
  cli_clearrte();
  unsigned long rtecnt = cli_rtecount();

  // assert
  testing(test_UintEqUint(__func__, uexpr(rtecnt), uexpr(0)));
  test_passed(__func__);
}

void test_cli_parseint(void) {
  // arrange
  cli_clearrte();
  const char token[] = "1234";

  // act
  cli_param_t param;
  cli_error_t error = cli_parseint(token, &param);

  // assert
  testing(test_UintEqUint(__func__, uexpr(error), uexpr(NULL)));
  testing(test_IntEqInt(__func__, iexpr(param.u.xint), iexpr(1234)));
  test_passed(__func__);
}

void test_cli_parseint__invalid(void) {
  // arrange
  cli_clearrte();
  const char token[] = "12a4";

  // act
  cli_param_t param;
  cli_error_t error = cli_parseint(token, &param);

  // assert
  testing(test_UintEqUint(__func__, uexpr(error), uexpr(CLI_EINVINTEGER)));
  test_passed(__func__);
}

void test_cli_parsernb(void) {
  // arrange
  cli_clearrte();
  const char token[] = "1234.32";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsernb(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));
  testing(test_FloatEqFloat(__func__, fexpr(param.u.xfloat), fexpr(1234.32)));
  test_passed(__func__);
}

void test_cli_parsernb__invalid(void) {
  // arrange
  cli_clearrte();
  const char token[] = "1234.32.2";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsernb(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(CLI_EINVREALNUM)));
  test_passed(__func__);
}

void test_cli_parsestr(void) {
  // arrange
  cli_clearrte();
  const char token[] = "\"hello world\"";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsestr(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));
  testing(test_StrEqStr(__func__, sexpr(param.u.uxptr), sexpr(token)));
  test_passed(__func__);
}

void test_cli_parsestr__invalid(void) {
  // arrange
  cli_clearrte();
  const char token0[] = "\"hello world";
  const char token1[] = "hello world\"";
  const char token2[] = "\"hello\"world";

  // act
  cli_param_t param0;
  cli_error_t error0 = cli_parsestr(token0, &param0);
  cli_param_t param1;
  cli_error_t error1 = cli_parsestr(token1, &param1);
  cli_param_t param2;
  cli_error_t error2 = cli_parsestr(token2, &param2);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error0), sexpr(CLI_EINVSTRLIT)));
  testing(test_StrEqStr(__func__, sexpr(error1), sexpr(CLI_EINVSTRLIT)));
  testing(test_StrEqStr(__func__, sexpr(error2), sexpr(CLI_EINVSTRLIT)));
  test_passed(__func__);
}

void test_cli_parsewrd(void) {
  // arrange
  cli_clearrte();
  const char token[] = "Hello123";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsewrd(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));
  testing(test_StrEqStr(__func__, sexpr(param.u.uxptr), sexpr(token)));
  test_passed(__func__);
}

void test_cli_parsewrd__invalid(void) {
  // arrange
  cli_clearrte();
  const char token[] = "Hello!123";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsewrd(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(CLI_EINV_WORD)));
  test_passed(__func__);
}

void test_cli_parsehex(void) {
  // arrange
  cli_clearrte();
  const char token[] = "89504e470D0a1A0a";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsewrd(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));
  testing(test_StrEqStr(__func__, sexpr(param.u.uxptr), sexpr(token)));
  test_passed(__func__);
}

void test_cli_parsehex__invalid(void) {
  // arrange
  cli_clearrte();
  const char token[] = "89504X470D0a1A0Z";

  // act
  cli_param_t param;
  cli_error_t error = cli_parsehex(token, &param);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(CLI_EINVHEXADEC)));
  test_passed(__func__);
}

void test_cli_hasrte_s__int_arg(void) {
  // arrange
  cli_clearrte();

  cli_route_t buffer[8];
  cli_setbuf_s(buffer, 8);

  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const char* id_list2[] = {"test2", "id", CLI_INT_ARG};
  cli_addrte_s(id_list, 3, test_dummy_cli_function);
  cli_addrte_s(id_list2, 3, test_dummy_cli_function2);

  char* input[] = {"test", "id", "200"};
  cli_toklst_t tokens3 = {.buf = input, .cnt = 3};

  // act
  cli_route_t* result;
  cli_error_t error = cli_hasrte_s(&tokens3, &result);

  // assert
  testing(test_UintEqUint(__func__, uexpr(error), uexpr(NULL)));
  testing(test_UintEqUint(__func__, uexpr(result->func), uexpr(test_dummy_cli_function)));
  test_passed(__func__);
}

void test_cli_hasrte_s__no_match(void) {
  // arrange
  cli_route_t buffer[8];
  cli_setbuf_s(buffer, 8);

  const char* id_list[] = {"test", "id", CLI_INT_ARG};
  const char* id_list2[] = {"test2", "id", CLI_INT_ARG};
  cli_addrte_s(id_list, 3, test_dummy_cli_function);
  cli_addrte_s(id_list2, 3, test_dummy_cli_function2);

  char* input[] = {"no", "match", "200"};
  cli_toklst_t tokens3 = {.buf = input, .cnt = 3};

  // act
  cli_route_t* result;
  cli_error_t error = cli_hasrte_s(&tokens3, &result);

  // assert
  testing(test_UintEqUint(__func__, uexpr(error), uexpr(CLI_EIDWASDIFF)));
  test_passed(__func__);
}

void test_cli_usrinput(void) {
  // arrange
  char str[] = "  hello \"world!\"";
  FILE* input = fmemopen(str, sizeof(str), "r");
  testing(test_UintNeUint(__func__, uexpr(input), uexpr(NULL)));

  // act
  cli_toklst_t tokens;
  cli_error_t error = cli_usrinput(input, &tokens);
  fclose(input);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));
  testing(test_UintEqUint(__func__, uexpr(tokens.cnt), uexpr(2)));
  testing(test_StrEqStr(__func__, sexpr(tokens.buf[0]), sexpr("hello")));
  testing(test_StrEqStr(__func__, sexpr(tokens.buf[1]), sexpr("\"world!\"")));
  test_passed(__func__);
}

void test_cli_usrinput_ending_dq(void) {
  // arrange
  char str[] = "  hello \"world!";
  FILE* input = fmemopen(str, sizeof(str), "r");
  testing(test_UintNeUint(__func__, uexpr(input), uexpr(NULL)));

  // act
  cli_toklst_t tokens;
  cli_error_t error = cli_usrinput(input, &tokens);
  fclose(input);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(CLI_ESTRENDDQ)));
  test_passed(__func__);
}

void test_cli_usrinput_no_blank_after_dq(void) {
  // arrange
  char str[] = "  hello \"world!\"abc";
  FILE* input = fmemopen(str, sizeof(str), "r");
  testing(test_UintNeUint(__func__, uexpr(input), uexpr(NULL)));

  // act
  cli_toklst_t tokens;
  cli_error_t error = cli_usrinput(input, &tokens);
  fclose(input);

  // assert
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(CLI_ESTRSEPCH)));
  test_passed(__func__);
}

/*
 * Integration tests - data & routes
 */

int test_route_result = 0;

void test_route_cli_success(void* opt) {
  unsigned long argc = cli_paramcnt();
  if (argc != 1) {
    test_route_result = 1;
    return;
  }

  cli_param_t p;
  cli_getparam(0, &p);
  if (p.u.xint != 10) {
    test_route_result = 2;
    return;
  }

  return;
}

/*
 * Integration tests
 */

void test_cli_success(void) {
  // clean
  test_route_result = 0;
  cli_clearrte();
  cli_setbuf_s(NULL, 0);

  // set buffer
  const ulong routenumber = 5;
  cli_route_t routebuffer[routenumber];
  void* prevbuffer = cli_setbuf_s(routebuffer, routenumber);
  testing(test_UintEqUint(__func__, uexpr(prevbuffer), uexpr(NULL)));

  // add route
  const char* routeids[] = {"test", CLI_INT_ARG};
  const ulong routeidsnum = 2;
  cli_error_t error = cli_addrte_s(routeids, routeidsnum, test_route_cli_success);
  testing(test_StrEqStr(__func__, sexpr(error), sexpr(NULL)));

  // input
  char inputstring[] = "test 10";
  FILE* stream = fmemopen(inputstring, sizeof(inputstring), "r");
  testing(test_UintNeUint(__func__, uexpr(stream), uexpr(NULL)));

  cli_toklst_t tokens;
  cli_error_t error1 = cli_usrinput(stream, &tokens);
  for (int i = 0; i < tokens.cnt; i++) {
    printf("%s\n", tokens.buf[i]);
  }
  testing(test_StrEqStr(__func__, sexpr(error1), sexpr(NULL)));
  testing(test_UintEqUint(__func__, uexpr(tokens.cnt), uexpr(2)));
  fclose(stream);

  // get route
  cli_route_t* route;
  cli_error_t error2 = cli_hasrte_s(&tokens, &route);
  testing(test_StrEqStr(__func__, sexpr(error2), sexpr(NULL)));
  testing(test_UintNeUint(__func__, uexpr(route), uexpr(NULL)));

  // call route
  route->func(NULL);
  testing(test_IntEqInt(__func__, iexpr(test_route_result), iexpr(0)));
  test_passed(__func__);
}

/*
 * Entry point
 */

int main(int argc, char** argv) {
  test_cli_setbuf_s();
  test_cli_addrte_s();
  test_cli_addrte_s__id_list_null();
  test_cli_addrte_s__func_null();
  test_cli_addrte_s__buffer_null();
  test_cli_clearrte();
  test_cli_parseint();
  test_cli_parseint__invalid();
  test_cli_parsernb();
  test_cli_parsernb__invalid();
  test_cli_parsestr();
  test_cli_parsestr__invalid();
  test_cli_parsewrd();
  test_cli_parsewrd__invalid();
  test_cli_parsehex();
  test_cli_parsehex__invalid();
  test_cli_hasrte_s__int_arg();
  test_cli_usrinput();
  test_cli_usrinput_ending_dq();
  test_cli_usrinput_no_blank_after_dq();
  test_cli_success();
  return 0;
}
