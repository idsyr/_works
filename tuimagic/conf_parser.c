#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <regex.h>

#include "conf_parser.h"

parsed_words_t some_parse( char * buffer ) {
  parsed_words_t map;

  unsigned start_of_word = 0;
  map.n                  = 0;
  char * end_of_word_ptr;
  while ( ( end_of_word_ptr = strchr( buffer + start_of_word, ':' ) ) != NULL && map.n < 3 ) {
    unsigned end_of_word = ( end_of_word_ptr - buffer );
    unsigned word_len    = ( end_of_word - start_of_word );
    map.words[map.n]     = malloc( sizeof( char ) * ( word_len + 1 ) );
    strncpy( map.words[map.n], buffer + start_of_word, word_len );
    map.words[map.n][word_len] = '\0';

    ++map.n;
    start_of_word = end_of_word + 1;
  }
  end_of_word_ptr      = strchr( buffer + start_of_word, '\0' );
  unsigned end_of_word = ( end_of_word_ptr - buffer );
  unsigned word_len    = ( end_of_word - start_of_word ) + 1;
  map.words[map.n]     = malloc( sizeof( char ) * ( word_len ) );
  strncpy( map.words[map.n], buffer + start_of_word, word_len );
  ++map.n;

  return map;
}

void destruct_parsed_words( parsed_words_t parsed_words ) {
  for ( unsigned i = 0, trgt = parsed_words.n; i < trgt; ++i )
    free( parsed_words.words[i] );
}
