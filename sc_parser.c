#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sc.h"
#include "sc_stream.h"
#include "sc_parser.h"

sc_val *sc_parse_next(sc_stream *s, int depth);
sc_val *sc_parse_list(sc_stream *s, int depth);

typedef struct sc_token {
  int len;
  char *str;
} sc_token;

sc_token_type sc_next_token(sc_stream *s, sc_token *t)
{
  t->len = 1;
  char c = sc_stream_next(s);
  while (c == '\n' || c == '\t' || c == ' ') {
    c = sc_stream_next(s);
  }

  switch(c) {
    case '\0':
      return SC_TOK_END;
      break;
    case '\'':
      return SC_TOK_QUOTE;
      break;
    case '(':
      return SC_TOK_OPEN;
      break;
    case ')':
      return SC_TOK_CLOSE;
      break;
    default:
      t->str = s->str + s->pos - 1;
      t->len = 1;
      c = sc_stream_peek(s);

      while (c != '\n' && c != '\t' && c != ' ' && c != '\'' && c != '\0' && c != '(' && c != ')') {
        t->len ++;
        sc_stream_next(s);
        c = sc_stream_peek(s);
      }
      
      return SC_TOK_SYMBOL;
      break;
  }
}

sc_val *sc_symbol_from_token(sc_token *t)
{
  sc_val *result = sc_val_new(SC_SYMBOL);

  char *str = malloc(t->len + 1);
  strncpy(str, t->str, t->len);
  str[t->len] = '\0';
  result->value = str;
  return result;
}


sc_token_type sc_peek_token(sc_stream *s, sc_token *t) {
  int pos = s->pos;
  sc_token_type tt = sc_next_token(s, t);
  s->pos = pos;
  return tt;
}

sc_val *sc_cons(sc_val *first, sc_val *rest) {
  sc_val *n = sc_val_new(SC_CELL);
  n->first = first;
  n->rest = rest;
  return n;
}

sc_val *sc_quote_val(sc_val *v) {
  sc_val *q = sc_val_new(SC_CELL);
  q->first = sc_quote;
  sc_val *rest = sc_val_new(SC_CELL);
  rest->first = v;
  q->rest = rest;
  return q;
}

sc_val *sc_parse_list(sc_stream *s, int depth)
{
  sc_token t;
  sc_token_type tt = sc_peek_token(s, &t);

  if (tt == SC_TOK_END) {
    if (depth > 0)
      sc_raise(SC_UNCLOSED_EX, depth);

    return sc_null;

  } else if (tt == SC_TOK_CLOSE) {
    if (depth == 0)
      sc_raise(SC_TOO_MANY_CLOSING_PARENS_EX, 0);

    sc_next_token(s, &t);
    return sc_null;

  } else if (tt == SC_TOK_QUOTE) {
    sc_next_token(s, &t);
    return sc_cons(sc_quote_val(sc_parse_next(s, depth + 1)), sc_parse_list(s, depth));
  } else {
    return sc_cons(sc_parse_next(s, depth), sc_parse_list(s, depth));
  }
}

sc_val *sc_parse_next(sc_stream *s, int depth)
{
  sc_token t;

  switch (sc_next_token(s, &t)) {
    case SC_TOK_OPEN:
      return sc_parse_list(s, depth + 1);
      break;

    case SC_TOK_SYMBOL:
      return sc_symbol_from_token(&t);
      break;

    case SC_TOK_QUOTE:
      return sc_quote_val(sc_parse_next(s, depth + 1));
      break;

    case SC_TOK_END:
      sc_raise(SC_UNCLOSED_EX, depth);
      return sc_null;

    default:
      sc_raise(SC_UNEXPECTED_EX, 0);
      return sc_null;
      break;
  }
}

sc_val *sc_parse(char *str) 
{
  sc_stream s;
  sc_stream_init(&s, str);
  sc_val *v = sc_parse_list(&s, 0);
  return v;
}
