#include <ncurses.h>

#include "theme.h"

void init_themes() {
  use_default_colors();
  start_color();
  init_color( COLOR_WHITE, 1000, 1000, 1000 );
  init_pair( green_text, COLOR_GREEN, -1 );
  init_pair( default_theme, -1, -1 );
  init_pair( black_on_white, COLOR_BLACK, COLOR_WHITE );
  init_pair( contrast, COLOR_YELLOW, COLOR_RED );
  init_pair( green_on_white, COLOR_GREEN, COLOR_WHITE );
  init_pair( white_on_green, COLOR_WHITE, COLOR_GREEN );
  init_pair( error_theme, COLOR_RED, COLOR_WHITE );
  init_pair( infoline_label_theme, COLOR_GREEN, COLOR_WHITE );
}
