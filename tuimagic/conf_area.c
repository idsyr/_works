#include <ncurses.h>
#include <string.h>

#include "area.h"
#include "conf_area.h"

// TODO: resize problem
area_t get_area_by_name( char * name ) {
  area_t result_area = {
      .h = LINES / 4,
      .w = COLS / 2,
      .y = 0,
      .x = 0,
  };
  if ( strcmp( name, "result" ) == 0 )
    return result_area;

  area_t device_area = {
      .h = LINES / 4,
      .w = COLS / 2 + 1,
      .y = 0,
      .x = COLS / 2,
  };
  if ( strcmp( name, "device" ) == 0 )
    return device_area;

  area_t usb_area = {
      .h = LINES / 4,
      .w = COLS / 2,
      .y = LINES / 4,
      .x = 0,
  };
  if ( strcmp( name, "usb" ) == 0 )
    return usb_area;

  area_t printer_area = {
      .h = LINES / 4,
      .w = COLS / 2 + 1,
      .y = LINES / 4,
      .x = COLS / 2,
  };
  if ( strcmp( name, "printer" ) == 0 )
    return printer_area;

  area_t process_area = {
      .h = LINES / 3,
      .w = COLS,
      .y = ( LINES / 4 ) * 2,
      .x = 0,
  };

  return process_area;
}
