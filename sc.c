#include <stdio.h>
#include <stdlib.h>

#include "sc.h"
#include "sc_stream.h"

#define INPUT_LEN 512

sc_val *sc_false;
sc_val *sc_true;
sc_val *sc_null;

int main(void)
{
  char line[INPUT_LEN];
  sc_init();
  printf("> ");
  while (fgets(line, INPUT_LEN, stdin) != NULL) {
    sc_val *input = sc_parse(line);
    printf("> ");
  }

  return 0;
}

sc_val *sc_val_new(sc_type type) 
{
  sc_val *v = malloc(sizeof(sc_val));
  v->type = type;
  return v;
}

void sc_init(void) 
{
  sc_false =sc_val_new(SC_FALSE);
  sc_true = sc_val_new(SC_TRUE);
  sc_null = sc_val_new(SC_NULL);
}


sc_val *sc_parse(char *str) 
{
  return sc_null;
}

