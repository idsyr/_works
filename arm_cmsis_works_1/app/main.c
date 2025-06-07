#include <stdbool.h>
#include <string.h>

#include "load.h"
#include "rcc.h"
#include "systick.h"
#include "board.h"
#include "usart.h"

static bool led_toggle_access = true;
const char  hello_world[]     = "Hello World!\n";

int main() {
  load();
  set_optimal_clock();
  systick_setup();
  led_setup();
  init_usart1();
  for ( ;; ) {
    if ( led_toggle_access ) {
      led_toggle();
      led_toggle_access = false;
    } else {
      send_usart1( hello_world, 13 );
      led_toggle_access = true;
    }
    systick_wait_sec();
  }
}

__attribute__( ( used ) ) void handle_usart1( void ) {
  char msg = 0;
  putc_usart1( getc_usart1() );
  for ( int i = 0; i < 255; ++i ) {
    send_usart1( &msg, 1 );
    ++msg;
  }
  msg = 0;
}
