#pragma once

#include <string>
#include <sqlite3.h>

typedef int ( *sqlite3_clbk )( void *, int, char **, char ** );

void select_all_n_print( sqlite3 * db );
void insert_or_ignore( sqlite3 * db, std::string image_name );
void insert_or_ignore( sqlite3 * db, std::string image_name );
void set_image_as_transmitted( sqlite3 * db, unsigned id );
void for_first_unsent( sqlite3 * db, sqlite3_clbk clbk, void * clbk_pack );
bool is_unsent( int columns_n, char ** row, char ** row_names );

int print_all_clbk( void * pack, int columns_n, char ** row, char ** row_names );
