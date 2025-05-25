#include "widget_header.h"

void destruct( widget_header_t h ) { h.interface->destruct( h.widget ); }

void draw( widget_header_t h, area_t place ) { h.interface->draw( h.widget, place ); }

void absorb( widget_header_t h_master, widget_header_t h_slave ) { h_master.interface->absorb( h_master.widget, h_slave ); }
