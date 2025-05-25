#include <sqlite3.h>
#include <cstring>
#include <iostream>

#include "sqlite_works.hpp"

#include <vector>
#include <string>
#include <iostream>

#include "dirworks.hpp"

int upload_clbk( void * pack, int columns_n, char ** row, char ** row_names ) {
  std::cout << "FIRST_UNSENT: " << row[1] << std::endl;
  sqlite3 * db = ( sqlite3 * )pack;

  if ( !file_exists( std::string{ row[1] } ) )
    ; // rm entry and retry

  set_image_as_transmitted( db, atoi( row[0] ) );

  return 0;
}

int main() {
  sqlite3 * db;
  int       res = sqlite3_open( "test.db", &db );
  if ( res != SQLITE_OK ) {
    sqlite3_close( db );
    return res;
  }

  try {
    std::vector< std::string > list_files = get_list_files( "." );
    for ( std::string & el : list_files )
      insert_or_ignore( db, el );
    for_first_unsent( db, upload_clbk, db );
  } catch ( std::runtime_error & e ) {
    std::cout << e.what() << std::endl;
  }

  sqlite3_close( db );
  return 0;
}
