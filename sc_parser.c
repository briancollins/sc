#include "sc.h"
#include "sc_stream.h"
#include "sc_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sc_token {
  int len;
  char *str;
} sc_token;

sc_token_type sc_next_token(sc_stream *s, sc_token *t)
{
  int done = 0, in_symbol = 0;

  while (!done) {
    switch(sc_stream_peek(s)) {
      case ' ':
      case '\t':
      case '\n': 
        sc_stream_next(s);
        if (in_symbol)
          done = 1;
        break;
      case '\0':
        if (in_symbol)
          done = 1;
        else
          return SC_TOK_END;
        break;
      case '(':
        if (in_symbol)
          done = 1;
        else {
          sc_stream_next(s);
          return SC_TOK_OPEN;
        }
        break;
      case ')':
        if (in_symbol)
          done = 1;
        else {
          sc_stream_next(s);
          return SC_TOK_CLOSE;
        }
        break;
      default:
        if (!in_symbol) {
          in_symbol = 1;
          t->str = s->str + s->pos;
          t->len = 0;
        }

        t->len ++;
        in_symbol = 1;
        sc_stream_next(s);
        break;
    }
  }

  return SC_TOK_SYMBOL;
}

sc_val *sc_parse_stream(sc_stream *s)
{
  int done = 0;
  sc_token t;
  sc_val *cur = sc_null;

  while (!done) {
    sc_token_type tt = sc_next_token(s, &t);

    switch (tt) {
      case SC_TOK_END: done = 1; break;
      case SC_TOK_OPEN:
                       cur = sc_val_new(SC_CELL);

                       cur->first = sc_parse_stream(s);
                       cur->rest = sc_parse_stream(s);
                       done = 1;
                       break;
      case SC_TOK_SYMBOL:
                       cur = sc_val_new(SC_CELL);

                       cur->first = sc_val_new(SC_SYMBOL);

                       char *str = malloc(t.len + 1);
                       strncpy(str, t.str, t.len);
                       str[t.len] = '\0';
                       cur->first->value = str;
                       cur->rest = sc_parse_stream(s);
                       done = 1;
                       break;
      case SC_TOK_CLOSE: done = 1; break;
    }
  }

  return cur;
}

sc_val *sc_parse(char *str) 
{
  sc_stream s;
  sc_stream_init(&s, str);
  sc_val *v = sc_parse_stream(&s);
  return v;
}
