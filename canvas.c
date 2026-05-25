#include "canvas.h"
#include <stdio.h>
#include <string.h>

#define CANVAS_AT(canvas, x, y) \
	((canvas)->pixels[(canvas)->width * (y) + (x)])

void canvas_init(Canvas *canvas, uint32_t *pixels, int width, int height)
{
	canvas->pixels = pixels;
	canvas->width = width;
	canvas->height = height;
}

bool canvas_in_bounds(Canvas *canvas, int x, int y)
{
	return 0 <= x && x < canvas->width && 0 <= y && y < canvas->height;
}

void canvas_put_pixel(Canvas *canvas, int x, int y, uint32_t c)
{
	if (!canvas_in_bounds(canvas, x, y)) return;

	CANVAS_AT(canvas, x, y) = c;
}

uint32_t canvas_get_pixel(Canvas *canvas, int x, int y)
{
	if (!canvas_in_bounds(canvas, x, y)) return 0;

	return CANVAS_AT(canvas, x, y);
}

void canvas_fill(Canvas *canvas, uint32_t c)
{
	for (int p = 0; p < canvas->width * canvas->height; p++) {
		canvas->pixels[p] = c;
	}
}	

void canvas_blit(
		Canvas *dst,
		int x0,
		int y0,
		Canvas *src,
		int x1,
		int y1,
		int width,
		int height
		)
{
	for (int h = 0; h < height; h++) {
		uint32_t *dst_row = &CANVAS_AT(dst, x0, y0 + h);
		uint32_t *src_row = &CANVAS_AT(src, x1, y1 + h);

		memcpy(dst_row, src_row, width * sizeof(uint32_t));
	}
}

