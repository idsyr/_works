#include "rcc.h"
#include "systick.h"
#include "led.h"


void setup ();
void loop  ();


void main ()
{  setup();
   for(;;) loop();
}

void setup ()
{  set_optimal_clock();
   systick_setup();
   led_setup();
}

void loop ()
{  led_toggle(1);
   systick_wait_sec();
   led_toggle(0);
   systick_wait_sec();
}
