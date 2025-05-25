#ifndef WIDGET_HEADER_H
#define WIDGET_HEADER_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ncurses.h>
#include <unistd.h>

#include "area.h"

struct widget_header;
typedef struct widget_header widget_header_t;

typedef void ( *destruct_f )( void * conrete_is_destructable );
typedef void ( *draw_f )( void * concrete_is_drawable, area_t on_place );
typedef void ( *absorb_f )( void * concrete_is_absorbing, widget_header_t abosrbable_header );

typedef struct widget_interface {
  destruct_f destruct;
  draw_f     draw;
  absorb_f   absorb;
} widget_interface_t;

typedef struct widget_header {
  widget_interface_t const * interface;
  void *                     widget;
} widget_header_t;

void destruct( widget_header_t h );
void draw( widget_header_t h, area_t place );
void absorb( widget_header_t h_master, widget_header_t h_slave );

#endif
