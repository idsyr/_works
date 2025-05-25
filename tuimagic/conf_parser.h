#pragma once

typedef struct parsed_words {
  char *   words[3];
  unsigned n;
} parsed_words_t;

parsed_words_t some_parse( char * buffer );
void           destruct_parsed_words( parsed_words_t parsed_words );
