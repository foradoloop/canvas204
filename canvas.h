#ifndef CANVAS_H
#define CANVAS_H

#include <stdint.h>
#include <stdbool.h>

struct canvas {
	uint32_t *pixels;
	int width;
	int height;
};
typedef struct canvas Canvas;

void canvas_init(Canvas *canvas, uint32_t *pixels, int width, int height);
bool canvas_in_bounds(Canvas *canvas, int x, int y);
void canvas_put_pixel(Canvas *canvas, int x, int y, uint32_t c);
uint32_t canvas_get_pixel(Canvas *canvas, int x, int y);
void canvas_fill(Canvas *canvas, uint32_t c);
void canvas_line(Canvas *canvas, int x0, int y0, int x1, int y1, uint32_t c);
void canvas_hline(Canvas *canvas, int x0, int x1, int y, uint32_t c);
void canvas_vline(Canvas *canvas, int x, int y0, int y1, uint32_t c);
void canvas_circle(Canvas *canvas, int cx, int cy, int r, uint32_t c);
void canvas_fill_circle(Canvas *canvas, int cx, int cy, int r, uint32_t c);
void canvas_rect(Canvas *canvas, int x, int y, int w, int h, uint32_t c);
void canvas_fill_rect(Canvas *canvas, int x, int y, int w, int h, uint32_t c);

#endif

