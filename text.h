#ifndef TEXT_H
#define TEXT_H

#include "canvas.h"

void put_string(
		Canvas *canvas,
		int x,
		int y,
		const char *str,
		int scale,
		int space,
		uint32_t color
	       );

void put_string_in_rect(
		Canvas *canvas,
		Rect *rect,
		const char *str,
		int scale,
		int space,
		uint32_t color
		);

#endif

