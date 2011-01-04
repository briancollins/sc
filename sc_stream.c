#include <stdlib.h>
#include <string.h>

#include "sc_stream.h"

sc_stream *sc_stream_new(char *str)
{
  sc_stream *s = malloc(sizeof(s));
  sc_stream_init(s, str);
  return s;
}

void sc_stream_init(sc_stream *s, char *str)
{
  s->len = strlen(str);
  s->str = str;
  s->pos = 0;
}

void sc_stream_free(sc_stream *s)
{
  free(s);
}

char sc_stream_peek(sc_stream *s)
{
  if (s->pos >= s->len) 
    return '\0';
  else 
    return s->str[s->pos];
}

char sc_stream_next(sc_stream *s)
{
  if (s->pos >= s->len)
    return '\0';
  else
    return s->str[s->pos++];
}

