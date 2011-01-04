#ifndef SC_STREAM_H
#define SC_STREAM_H

typedef struct sc_stream {
  int pos;
  int len;
  char *str;
} sc_stream;

sc_stream *sc_stream_new(char *str);
char sc_stream_peek(sc_stream *s);
char sc_stream_next(sc_stream *s);

#endif
