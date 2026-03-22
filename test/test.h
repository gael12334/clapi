/*
 * Copyright (c) 2026 Gaël Fortier <gael.fortier.1@ens.etsmtl.ca>
 */

#pragma once
#include <stdio.h>
#include <string.h>

#define str_t const char*
#define uchar_t unsigned char
#define ushort_t unsigned short
#define uint_t unsigned int
#define ulong_t unsigned long

typedef struct test_iexpr {
  long value;
  str_t string;
} test_iexpr_t;

#define iexpr(i)                                                                         \
  (test_iexpr_t) {                                                                       \
    .value = (long)(i), .string = #i                                                     \
  }

typedef struct test_uexpr {
  ulong_t value;
  str_t string;
} test_uexpr_t;

#define uexpr(i)                                                                         \
  (test_uexpr_t) {                                                                       \
    .value = (unsigned long)(i), .string = #i                                            \
  }

typedef struct test_fexpr {
  double value;
  str_t string;
} test_fexpr_t;

#define fexpr(i)                                                                         \
  (test_fexpr_t) {                                                                       \
    .value = (double)(i), .string = #i                                                   \
  }

typedef struct test_sexpr {
  str_t value;
  str_t string;
} test_sexpr_t;

#define sexpr(i)                                                                         \
  (test_sexpr_t) {                                                                       \
    .value = (const char*)(i), .string = #i                                              \
  }

static inline void test_fail_int(str_t f, long a, str_t na, str_t op, long b, str_t nb) {
  printf("[ Failed ] %s : '%s' (=%li) %s '%s' (=%li) is false.\n", f, na, a, op, nb, b);
}

static inline void test_fail_uint(str_t f, ulong_t a, str_t na, str_t op, ulong_t b,
                                  str_t nb) {
  printf("[ Failed ] %s : '%s' (=%lu) %s '%s' (=%lu) is false.\n", f, na, a, op, nb, b);
}

static inline void test_failed_float(str_t f, double a, str_t na, str_t op, double b,
                                     str_t nb) {
  printf("[ Failed ] %s : '%s' (=%lf) %s '%s' (=%lf) is false.\n", f, na, a, op, nb, b);
}

static inline void test_failed_str(str_t f, str_t a, str_t na, str_t op, str_t b,
                                   str_t nb) {
  printf("[ Failed ] %s : '%s' (=%s) %s '%s' (=%s) is false.\n", f, na, a, op, nb, b);
}

static inline void test_passed(const char* func) {
  printf("[ Passed ] %s\n", func);
}

// I forgot what I meant by Spv ...
#define test_SpvOpSpv(f, a, na, op, b, nb)                                               \
  {                                                                                      \
    int r = (a op b);                                                                    \
    if (r == 0) {                                                                        \
      _Generic((a),                                                                      \
          char: test_fail_int(f, (long)a, na, #op, (long)b, nb),                         \
          short: test_fail_int(f, (long)a, na, #op, (long)b, nb),                        \
          int: test_fail_int(f, (long)a, na, #op, (long)b, nb),                          \
          long: test_fail_int(f, (long)a, na, #op, (long)b, nb),                         \
          uchar_t: test_fail_uint(f, (unsigned long)a, na, #op, (unsigned long)b, nb),   \
          ushort_t: test_fail_uint(f, (unsigned long)a, na, #op, (unsigned long)b, nb),  \
          uint_t: test_fail_uint(f, (unsigned long)a, na, #op, (unsigned long)b, nb),    \
          ulong_t: test_fail_uint(f, (unsigned long)a, na, #op, (unsigned long)b, nb),   \
          float: test_failed_float(f, a, na, #op, b, nb),                                \
          double: test_failed_float(f, a, na, #op, b, nb));                              \
    }                                                                                    \
    return r;                                                                            \
  }

#define testing(expr)                                                                    \
  if (expr == 0)                                                                         \
  return

static inline int test_IntEqInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, ==, b.value, b.string);
}

static inline int test_IntNeInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, !=, b.value, b.string);
}

static inline int test_IntLeInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <=, b.value, b.string);
}

static inline int test_IntLtInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <, b.value, b.string);
}

static inline int test_IntGeInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >=, b.value, b.string);
}

static inline int test_IntGtInt(str_t func, test_iexpr_t a, test_iexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >, b.value, b.string);
}

static inline int test_UintEqUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, ==, b.value, b.string);
}

static inline int test_UintNeUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, !=, b.value, b.string);
}

static inline int test_UintLeUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <=, b.value, b.string);
}

static inline int test_UintLtUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <, b.value, b.string);
}

static inline int test_UintGeUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >=, b.value, b.string);
}

static inline int test_UintGtUint(str_t func, test_uexpr_t a, test_uexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >, b.value, b.string);
}

static inline int test_FloatEqFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, ==, b.value, b.string);
}

static inline int test_FloatNeFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, !=, b.value, b.string);
}

static inline int test_FloatLeFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <=, b.value, b.string);
}

static inline int test_FloatLtFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, <, b.value, b.string);
}

static inline int test_FloatGeFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >=, b.value, b.string);
}

static inline int test_FloatGtFloat(str_t func, test_fexpr_t a, test_fexpr_t b) {
  test_SpvOpSpv(func, a.value, a.string, >, b.value, b.string);
}

static inline int test_StrEqStr(str_t func, test_sexpr_t a, test_sexpr_t b) {
  a.value = (a.value == NULL) ? "" : a.value;
  b.value = (b.value == NULL) ? "" : b.value;
  long cmp_r = (long)strcmp(a.value, b.value) == 0;
  if (!cmp_r) {
    test_failed_str(func, a.value, a.string, "==", b.value, b.string);
    return 0;
  }

  return 1;
}

static inline int test_StrNeStr(str_t func, test_sexpr_t a, test_sexpr_t b) {
  a.value = (a.value == NULL) ? "" : a.value;
  b.value = (b.value == NULL) ? "" : b.value;
  long cmp_r = (long)strcmp(a.value, b.value) == 0;
  if (cmp_r) {
    test_failed_str(func, a.value, a.string, "!=", b.value, b.string);
    return 0;
  }

  return 1;
}

#undef str_t
#undef uchar_t
#undef ushort_t
#undef uint_t
#undef ulong_t
