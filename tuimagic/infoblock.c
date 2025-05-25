#include <ncurses.h>

#include "widget_header.h"
#include "darray.h"
#include "infoblock.h"

typedef struct widget_infoblock {
  WINDOW *                   win;
  char *                     label;
  darrayof_widget_header_t * content;
  theme_e                    content_theme;
  theme_e                    border_theme;
  theme_e                    label_theme;
} infoblock_t;

void destruct_infoblock( infoblock_t * infoblock ) {
  destruct_darray( infoblock->content );
  delwin( infoblock->win );
  free( infoblock->label );
  free( infoblock );
}

static void clear_last_draw( infoblock_t * infoblock ) {
  werase( infoblock->win );
  wbkgd( infoblock->win, COLOR_PAIR( default_theme ) );
  wrefresh( infoblock->win );
}

static void draw_box( infoblock_t * infoblock ) {
  wattron( infoblock->win, COLOR_PAIR( infoblock->border_theme ) );
  box( infoblock->win, 0, 0 );
  wattroff( infoblock->win, COLOR_PAIR( infoblock->border_theme ) );
}

static void draw_label( infoblock_t * infoblock ) {
  wattron( infoblock->win, COLOR_PAIR( infoblock->label_theme ) | A_BOLD );
  mvwprintw( infoblock->win, 0, 7, " %s ", infoblock->label );
  wattroff( infoblock->win, COLOR_PAIR( infoblock->label_theme ) | A_BOLD ); // TODO: make a watX macro
}

static void draw_with_seq_indent( widget_header_t h, area_t * area ) {
  draw( h, *area ); // TODO: the draw f must return the h->widget size
  ++area->y;        // area->y += widget_size;
}

void draw_infoblock( infoblock_t * infoblock, area_t area ) {
  // clear_last_draw( infoblock ); // TODO: check the need
  delwin( infoblock->win );
  infoblock->win = newwin( area.h, area.w, area.y, area.x );

  draw_box( infoblock );
  wrefresh( infoblock->win ); // TODO: check the need
  draw_label( infoblock );
  wrefresh( infoblock->win ); // TODO: check the need

  wbkgd( infoblock->win, COLOR_PAIR( infoblock->content_theme ) );
  wrefresh( infoblock->win ); // TODO: check the need

  area_t infoblock_area = {
      .y = area.y + 2,
      .x = area.x + 2,
      .h = area.h - 2,
      .w = area.w - 2,
  };

  forall( infoblock->content, ( fun_forall_t )draw_with_seq_indent, &infoblock_area );
  wrefresh( infoblock->win );
}

void absorb_infoblock( infoblock_t * infoblock, widget_header_t h_slave ) { add( infoblock->content, h_slave ); }

widget_interface_t const infoblock_interface = {
    .destruct = ( destruct_f )destruct_infoblock,
    .draw     = ( draw_f )draw_infoblock,
    .absorb   = ( absorb_f )absorb_infoblock,
};

widget_header_t create_infoblock( char * label, int content_n, infoblock_theme_t * theme ) {
  widget_header_t res = {
      .interface = &infoblock_interface,
      .widget    = malloc( sizeof( infoblock_t ) ),
  };

  *( infoblock_t * )res.widget = ( infoblock_t ){
      .label         = strncpy( malloc( strlen( label ) ), label, strlen( label ) ),
      .content_theme = theme->content,
      .border_theme  = theme->border,
      .label_theme   = theme->label,
      .content       = create_darrayof_widget_header_t( content_n ),
  };

  return res;
}

char * const get_label( widget_header_t h ) {
  infoblock_t * infoblock = h.widget;
  return infoblock->label;
}
