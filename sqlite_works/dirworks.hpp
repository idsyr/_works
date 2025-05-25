#pragma once

#include <vector>
#include <string>

std::vector< std::string > get_list_files( std::string dir_path );
bool                       file_exists( const std::string & name );
