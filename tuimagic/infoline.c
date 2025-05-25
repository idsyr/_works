#include <stdlib.h>
#include <ncurses.h>

#include "widget_header.h"
#include "infoline.h"

typedef struct widget_infoline {
  char *  label;
  char *  info;
  theme_e label_theme;
  theme_e info_theme;
} infoline_t;

void destruct_infoline( infoline_t * infoline ) {
  free( infoline->info );
  free( infoline->label );
  free( infoline );
}

static void draw_label( infoline_t * infoline, area_t area ) {
  attron( COLOR_PAIR( infoline->label_theme ) );
  mvprintw( area.y, area.x, infoline->label );
  attroff( COLOR_PAIR( infoline->label_theme ) );
}

static void draw_info( infoline_t * infoline, area_t area ) {
  attron( COLOR_PAIR( infoline->info_theme ) );
  move( area.y, area.x + area.w - strlen( infoline->info ) - 2 );
  printw( infoline->info );
  attroff( COLOR_PAIR( infoline->info_theme ) );
}

void draw_infoline( infoline_t * infoline, area_t area ) {
  refresh();
  draw_label( infoline, area );
  refresh();
  draw_info( infoline, area );
}

void absorb_infoline( infoline_t * infoline, widget_header_t widget ) { /* do nothing */ }

widget_interface_t infoline_interface = {
    .destruct = ( destruct_f )destruct_infoline,
    .draw     = ( draw_f )draw_infoline,
    .absorb   = ( absorb_f )absorb_infoline,
};

widget_header_t create_infoline( char * label, char * info, infoline_theme_t * theme ) {
  widget_header_t res = {
      .interface = &infoline_interface,
      .widget    = malloc( sizeof( infoline_t ) ),
  };

  *( infoline_t * )res.widget = ( infoline_t ){
      .label       = strncpy( malloc( strlen( label ) ), label, strlen( label ) ),
      .info        = strncpy( malloc( strlen( info ) ), info, strlen( info ) ),
      .label_theme = theme->label,
      .info_theme  = theme->info,
  };

  return res;
}
