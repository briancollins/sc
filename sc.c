#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include "sc.h"
#include "sc_parser.h"

sc_val *sc_false;
sc_val *sc_true;
sc_val *sc_null;
sc_ex sc_exception;

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

void sc_raise(sc_exception_t ex, int param) {
  sc_exception.param = param;
  sc_exception.ex = ex;
  longjmp(sc_exception.jmp, 1);
}

int main(void)
{
  char *line;
  char prompt_buf[512];
  char in_buf[2048] = { '\0' };

  char *prompt = "> ";

  sc_init();
  while ((line = readline(prompt))) {
    prompt = "> "; // reset prompt after indentation adjustments
    
    if (*line) {
      if (!setjmp(sc_exception.jmp)) { // try
        add_history(line);
        char *in = line;
        if (in_buf[0]) {
          in = in_buf;
        }
        strncat(in_buf, line, 2048);

        sc_val *input = sc_parse(in);

        sc_print(input, 1);
        printf("\n");

      } else { // catch
        if (sc_exception.ex == SC_UNCLOSED_EX) {
          strncat(in_buf, "\n", 2048);

          int depth = sc_exception.param;
          prompt_buf[0] = '\0';
          for (int i = 0; i < depth; i++) {
            strncat(prompt_buf, "\t", 512);
          }
          prompt = prompt_buf;
          continue;
        } else if (sc_exception.ex == SC_TOO_MANY_CLOSING_PARENS_EX) {
          printf("too many closing parentheses\n");
        } else {
          printf("exception\n");
        }
      }

      in_buf[0] = '\0';
    }

    free(line);
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

