#ifndef INFOBLOCK_H
#define INFOBLOCK_H

#include "theme.h"
#include "area.h"
#include "widget_header.h"

struct widget_infoblock;
typedef struct widget_infoblock infoblock_t;

typedef struct infoblock_theme {
  theme_e border;
  theme_e label;
  theme_e content;
} infoblock_theme_t;

widget_header_t   create_infoblock( char * label, int content_n, infoblock_theme_t * theme );
void              draw_infoblock( infoblock_t * infoblock, area_t area );

#endif
