#include <cstring>
#include <iostream>
#include <sqlite3.h>

#include "sqlite_works.hpp"

int print_all_clbk( void * pack, int columns_n, char ** row, char ** row_names ) {
  for ( int i = 0; i < columns_n; ++i ) {
    std::cout << row_names[i] << " := " << row[i] << std::endl;
  }
  return 0;
}

void select_all_n_print( sqlite3 * db ) {
  const char * query = "select * from content_st;";
  char *       err_msg;
  int          res = sqlite3_exec( db, query, print_all_clbk, NULL, &err_msg );
  if ( res != SQLITE_OK ) {
    std::cout << "SQL error: " << err_msg << std::endl;
  }
}

void insert_or_ignore( sqlite3 * db, std::string image_name ) {
  std::string query = "insert or ignore into content_st (file_name) values ('" + image_name + "');";
  char *      err_msg;
  int         res = sqlite3_exec( db, query.c_str(), NULL, NULL, &err_msg );
  if ( res != SQLITE_OK ) {
    std::cout << "SQL error: " << err_msg << std::endl;
  }
}

void set_image_as_transmitted( sqlite3 * db, unsigned id ) {
  std::string query = "update content_st set send_st = 'yes' where id = " + std::to_string( id ) + ";";
  char *      err_msg;
  int         res = sqlite3_exec( db, query.c_str(), NULL, NULL, &err_msg );
  if ( res != SQLITE_OK ) {
    std::cout << "SQL error: " << err_msg << std::endl;
  }
}

void for_first_unsent( sqlite3 * db, sqlite3_clbk clbk, void * clbk_pack ) {
  std::string query = "select * from content_st where send_st = 'no' limit 1;";
  char *      err_msg;
  int         res = sqlite3_exec( db, query.c_str(), clbk, clbk_pack, &err_msg );
  if ( res != SQLITE_OK ) {
    std::cout << "SQL error: " << err_msg << std::endl;
  }
}

bool is_unsent( int columns_n, char ** row, char ** row_names ) { return ( strcmp( row[2], "no" ) == 0 ); }
