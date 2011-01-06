#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "sc.h"
#include "sc_parser.h"

sc_val *sc_false;
sc_val *sc_true;
sc_val *sc_null;

void sc_print(sc_val *val, int is_rest)
{
  switch(val->type) {
    case SC_FALSE:
      printf("#f");
      break;
    case SC_TRUE:
      printf("#t");
      break;
    case SC_NULL:
      printf("()");
      break;
    case SC_SYMBOL:
      printf("%s", val->value);
      break;
    case SC_CELL:
      if (!is_rest)
        printf("(");

      sc_print(val->first, 0);
      if (val->rest != sc_null) {
        printf(" ");
        sc_print(val->rest, 1);
      }

      if (!is_rest)
        printf(")");
      break;
    default:
      printf("?");
      break;
  }
}

int main(void)
{
  char *line;
  sc_init();
  while ((line = readline("> "))) {
    if (*line) {
      add_history(line);
      sc_val *input = sc_parse(line);
      sc_print(input, 1);
      printf("\n");
    }
  }

  return 0;
}

sc_val *sc_val_new(sc_type type) 
{
  sc_val *v = malloc(sizeof(sc_val));
  v->type = type;
  if (type == SC_CELL) {
    v->first = sc_null;
    v->rest = sc_null;
  }

  return v;
}

void sc_init(void) 
{
  sc_false = sc_val_new(SC_FALSE);
  sc_true = sc_val_new(SC_TRUE);
  sc_null = sc_val_new(SC_NULL);
}
