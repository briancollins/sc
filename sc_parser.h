#ifndef SC_PARSER_H
#define SC_PARSER_H

typedef enum {
  SC_TOK_END,
  SC_TOK_OPEN,
  SC_TOK_CLOSE,
  SC_TOK_SYMBOL
} sc_token_type;

sc_val *sc_parse(char *str);

#endif

