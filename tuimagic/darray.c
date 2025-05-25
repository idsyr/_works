#include <stdlib.h>

#include "darray.h"

typedef struct darrayof_widget_header {
  widget_header_t * arr;
  int               sz;
  int               curr;
} darrayof_widget_header_t;

darrayof_widget_header_t * create_darrayof_widget_header_t( int sz ) {
  darrayof_widget_header_t * res = malloc( sizeof( darrayof_widget_header_t ) );
  res->arr                       = malloc( sizeof( widget_header_t ) * sz );
  res->sz                        = sz;
  res->curr                      = 0;
  return res;
}

int add( darrayof_widget_header_t * h, widget_header_t w ) {
  if ( h->curr + 1 < h->sz ) {
    h->arr[h->curr++] = w; // TODO: create on place
  } else {
    return -1;
  }
  return 0;
}

static void forall_no_arg( darrayof_widget_header_t * h, void ( *fun_forall )( widget_header_t h ) ) {
  for ( int i = 0, last_i = h->curr; i < last_i; ++i ) {
    fun_forall( h->arr[i] );
  }
}

void destruct_darray( darrayof_widget_header_t * h ) {
  forall_no_arg( h, destruct );
  free( h->arr );
  free( h );
}

void forall( darrayof_widget_header_t * h, void ( *fun_forall )( widget_header_t h, void * pack ), void * pack ) {
  for ( int i = 0, last_i = h->curr; i < last_i; ++i ) {
    fun_forall( h->arr[i], pack );
  }
}
