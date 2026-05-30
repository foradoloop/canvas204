#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <stdint.h>

void init_ui_components();
void draw_toolbar_left(void);
void draw_toolbar_bottom(void);

Tool *hit_toolbar_left();
uint32_t *hit_palette();
bool hit_art_area();
int *hit_thickness();
bool hit_new_button();
bool hit_save_button();

#endif

