#ifndef DARRAY_H
#define DARRAY_H

#include "widget_header.h"

struct darrayof_widget_header;
typedef struct darrayof_widget_header darrayof_widget_header_t;

typedef void ( *fun_forall_t )( widget_header_t h, void * pack );

darrayof_widget_header_t * create_darrayof_widget_header_t( int sz );
int                        add( darrayof_widget_header_t * h, widget_header_t w );
void                       destruct_darray( darrayof_widget_header_t * h );
void                       forall( darrayof_widget_header_t * h, fun_forall_t fun_forall, void * pack );

#endif
