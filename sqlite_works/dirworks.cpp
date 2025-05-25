#include <stdexcept>
#include <cstring>

#include <dirent.h>
#include <unistd.h>

#include "dirworks.hpp"

std::vector< std::string > get_list_files( std::string dir_path ) {
  std::vector< std::string > result;
  DIR *                      dir_p = opendir( "." );
  if ( dir_p == NULL )
    throw std::runtime_error( "opendir: " + dir_path );

  struct dirent * dirent_p;
  while ( ( dirent_p = readdir( dir_p ) ) != NULL ) {
    if ( std::strcmp( dirent_p->d_name, "." ) == 0 )
      continue;
    if ( std::strcmp( dirent_p->d_name, ".." ) == 0 )
      continue;
    result.push_back( dirent_p->d_name );
  }
  closedir( dir_p );
  return result;
}

bool file_exists( const std::string & name ) { return ( access( name.c_str(), F_OK ) != -1 ); }
