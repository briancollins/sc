#ifndef SC_H
#define SC_H
#include <setjmp.h>

typedef enum sc_type {
  SC_FALSE = 0,
  SC_TRUE = 1,
  SC_NULL = 2,
  SC_SYMBOL,
  SC_STRING,
  SC_NUMBER,
  SC_CELL
} sc_type;

typedef struct sc_val {
  sc_type type;
  void *value;
  struct sc_val *first;
  struct sc_val *rest;
} sc_val;

typedef enum sc_exception_t {
  SC_UNCLOSED_EX
} sc_exception_t;

typedef struct sc_ex {
  jmp_buf jmp;
  sc_exception_t ex;
  int param;
} sc_ex;



sc_val *sc_val_new(sc_type type);
void sc_init(void);
void sc_raise(sc_exception_t ex, int param);

extern sc_val *sc_null;
extern sc_val *sc_false;
extern sc_val *sc_true;
extern sc_ex sc_exception;

#endif
