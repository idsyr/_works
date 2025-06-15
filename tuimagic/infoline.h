#ifndef INFOLINE_H
#define INFOLINE_H

#include "theme.h"
#include "area.h"
#include "widget_header.h"

struct widget_infoline;
typedef struct widget_infoline infoline_t;

typedef struct infoline_theme {
  theme_e label;
  theme_e info;
} infoline_theme_t;

widget_header_t create_infoline( char * label, char * info, infoline_theme_t * theme );
area_t          draw_infoline( infoline_t * infoline, area_t area );
void            destruct_infoline( infoline_t * infoline );

char * const get_label( widget_header_t h );

#endif
