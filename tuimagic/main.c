#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>
#include <poll.h>
#include <sys/wait.h>

#include "widget_header.h"
#include "darray.h"
#include "infoline.h"
#include "infoblock.h"
#include "theme.h"
#include "conf_area.h"
#include "conf_parser.h"

FILE * call_n_wait_output( char * prg_name ) {
  FILE * fp = popen( prg_name, "r" );
  if ( !fp ) {
    endwin();
    perror( "popen" );
    abort();
  }
  struct pollfd pfd = {
      .fd     = fileno( fp ),
      .events = POLLIN,
  };
  poll( &pfd, 1, -1 );
  return fp;
}

void add_if_same_label( widget_header_t h, void * pack ) {
  parsed_words_t * unpacked = ( parsed_words_t * )pack;
  if ( strcmp( get_label( h ), unpacked->words[0] ) == 0 ) {
    infoline_theme_t theme = {
        .label = ( strcmp( unpacked->words[1], "ERROR" ) == 0 ) ? error_theme : infoline_label_theme,
        .info  = black_on_white,
    };
    theme_e label_theme;
    absorb( h, create_infoline( unpacked->words[1], unpacked->words[2], &theme ) );
  }
}

void draw_display_objs( widget_header_t h, void * pack ) { draw( h, get_area_by_name( get_label( h ) ) ); }

void parse_n_draw_output( FILE * fp, darrayof_widget_header_t * display_objs ) {
  char buffer[256];
  while ( fgets( buffer, sizeof( buffer ), fp ) ) {
    buffer[strcspn( buffer, "\n\r" )] = '\0';
    parsed_words_t words              = some_parse( buffer );
    forall( display_objs, add_if_same_label, &words );
    destruct_parsed_words( words );
  }
  forall( display_objs, draw_display_objs, NULL );
}

int main() {
  initscr();
  cbreak();
  noecho();
  curs_set( 0 );
  init_themes();
  refresh();

  infoblock_theme_t default_infoblock_theme = {
      .border  = green_on_white,
      .content = black_on_white,
      .label   = white_on_green,
  };

  darrayof_widget_header_t * display_objs = create_darrayof_widget_header_t( 21 );
  add( display_objs, create_infoblock( "usb", 10, &default_infoblock_theme ) );
  add( display_objs, create_infoblock( "device", 10, &default_infoblock_theme ) );
  add( display_objs, create_infoblock( "result", 10, &default_infoblock_theme ) );
  add( display_objs, create_infoblock( "printer", 10, &default_infoblock_theme ) );
  add( display_objs, create_infoblock( "process", 10, &default_infoblock_theme ) );

  FILE * fp_usb_monitor = call_n_wait_output( "./usb_monitor.x" );
  parse_n_draw_output( fp_usb_monitor, display_objs );
  pclose( fp_usb_monitor );

  FILE * fp_label_printer = call_n_wait_output( "./label_printer.x" );
  parse_n_draw_output( fp_label_printer, display_objs );
  pclose( fp_label_printer );

  refresh();
  getch();
  destruct_darray( display_objs );
  endwin();
  return 0;
}
