#ifndef SC_H
#define SC_H

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
  struct sc_val *first;
  struct sc_val *rest;
} sc_val;


sc_val *sc_parse(char *str);

sc_val *sc_val_new(sc_type type);
void sc_init(void);

extern sc_val *sc_null;
extern sc_val *sc_false;
extern sc_val *sc_true;

#endif
