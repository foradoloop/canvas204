
#include "font8x8_basic.h"
#include "text.h"
#include <string.h>

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

void put_string(
		Canvas *canvas,
		int x,
		int y,
		const char *str,
		int scale,
		int space,
		uint32_t color
	       )
{
	for (int i = 0; str[i]; i++) {
		unsigned char *font = font8x8_basic[(int)str[i]];

		for (int h = 0; h < FONT_HEIGHT; h++) {
			unsigned char row = font[h];

			for (int b = 0; b < 8; b++) {
				unsigned char bit = (row >> b) & 0x01;

				if (bit) {
					Vec2 point = {
						x + i * ((FONT_WIDTH * scale) + space) + b * scale,
						y + h * scale
					};

					Rect rect = { point, { scale, scale } };

					draw_fill_rect(canvas, &rect, color);
				}
			}
		}
	}
}

void put_string_in_rect(
		Canvas *canvas,
		Rect *rect,
		const char *str,
		int scale,
		int space,
		uint32_t color
		)
{
	int len = strlen(str);

	float x = rect->pos.x;
	float y = rect->pos.y;
	float w = rect->size.x;
	float h = rect->size.y;

	float w_str = len * (FONT_WIDTH * scale) + space * (len - 1);
	float h_str = FONT_HEIGHT * scale;

	float mx = (w - w_str) * 0.5f;
	float my = (h - h_str) * 0.5f;

	put_string(
		canvas,
		x + mx,
		y + my,
		str,
		scale,
		space,
		color
		);
}

