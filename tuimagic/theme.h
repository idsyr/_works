#ifndef THEME_H
#define THEME_H

typedef enum theme {
  green_text = 1,
  default_theme,
  black_on_white,
  contrast,
  green_on_white,
  white_on_green,
  error_theme,
  infoline_label_theme,
} theme_e;

void init_themes();

#endif
