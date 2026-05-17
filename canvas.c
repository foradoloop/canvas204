#include "canvas.h"
#include <stdio.h>

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SIGN(x) (((x) >= 0) ? 1 : -1)

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

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
	if (!canvas_in_bounds(canvas, x, y)) {
		fprintf(stderr, "Canvas out of bounds: (%d, %d) not in [0, %d) x [0, %d)\n",
				x, y,
				canvas->width,
				canvas->height);
		return;
	}

	CANVAS_AT(canvas, x, y) = c;
}

uint32_t canvas_get_pixel(Canvas *canvas, int x, int y)
{
	if (!canvas_in_bounds(canvas, x, y)) {
		fprintf(stderr, "Canvas out of bounds: (%d, %d) not in [0, %d) x [0, %d)\n",
				x, y,
				canvas->width,
				canvas->height);
		return 0;
	}

	return CANVAS_AT(canvas, x, y);
}

void canvas_fill(Canvas *canvas, uint32_t c)
{
	for (int p = 0; p < canvas->width * canvas->height; p++) {
		canvas->pixels[p] = c;
	}
}	

void canvas_line(Canvas *canvas, int x0, int y0, int x1, int y1, uint32_t c)
{
	int *major_coord, *minor_coord;
	int major_delta, minor_delta;
	int major_step, minor_step;

	int dx = x1 - x0;
	int dy = y1 - y0;

	int x = x0;
	int y = y0;

	if (ABS(dx) > ABS(dy)) {
		major_coord = &x;
		minor_coord = &y;
		major_delta = ABS(dx);
		minor_delta = ABS(dy);
		major_step = SIGN(dx);
		minor_step = SIGN(dy);
	} else {
		major_coord = &y;
		minor_coord = &x;
		major_delta = ABS(dy);
		minor_delta = ABS(dx);
		major_step = SIGN(dy);
		minor_step = SIGN(dx);
	}

	int err = 2 * minor_delta - major_delta;

	for (int step = 0; step <= major_delta; step++) {
		canvas_put_pixel(canvas, x, y, c);

		if (err >= 0) {
			*minor_coord += minor_step;
			err -= 2 * major_delta;
		}

		*major_coord += major_step;
		err += 2 * minor_delta;
	}
}

void canvas_hline(Canvas *canvas, int x0, int x1, int y, uint32_t c)
{
	int x_min = MIN(x0, x1);
	int x_max = MAX(x0, x1);

	for (int x = x_min; x <= x_max; x++) {
		canvas_put_pixel(canvas, x, y, c);
	}
}

void canvas_vline(Canvas *canvas, int x, int y0, int y1, uint32_t c)
{
	int y_min = MIN(y0, y1);
	int y_max = MAX(y0, y1);

	for (int y = y_min; y <= y_max; y++) {
		canvas_put_pixel(canvas, x, y, c);
	}
}

void canvas_circle(Canvas *canvas, int cx, int cy, int r, uint32_t c)
{
	int x = 0;
	int y = r;

	while (x <= y) {
		int y_mid = 2 * y - 1;

		if (4 * x * x + y_mid * y_mid > 4 * r * r) {
			y--;
		}

		canvas_put_pixel(canvas, cx + x, cy + y, c);
		canvas_put_pixel(canvas, cx + x, cy - y, c);
		canvas_put_pixel(canvas, cx - x, cy + y, c);
		canvas_put_pixel(canvas, cx - x, cy - y, c);

		canvas_put_pixel(canvas, cx + y, cy + x, c);
		canvas_put_pixel(canvas, cx + y, cy - x, c);
		canvas_put_pixel(canvas, cx - y, cy + x, c);
		canvas_put_pixel(canvas, cx - y, cy - x, c);

		x++;
	}
}

void canvas_fill_circle(Canvas *canvas, int cx, int cy, int r, uint32_t c)
{
	int x = 0;
	int y = r;

	while (x <= y) {
		int y_mid = 2 * y - 1;

		if (4 * x * x + y_mid * y_mid > 4 * r * r) {
			y--;
		}

		canvas_hline(canvas, cx + x, cx - x, cy + y, c);
		canvas_hline(canvas, cx + x, cx - x, cy - y, c);

		canvas_hline(canvas, cx + y, cx - y, cy + x, c);
		canvas_hline(canvas, cx + y, cx - y, cy - x, c);

		x++;
	}
}

void canvas_rect(Canvas *canvas, int x, int y, int w, int h, uint32_t c)
{
	canvas_hline(canvas, x, x + w - 1, y, c);
	canvas_hline(canvas, x, x + w - 1, y + h - 1, c);

	canvas_vline(canvas, x, y, y + h - 1, c);
	canvas_vline(canvas, x + w - 1, y, y + h - 1, c);
}

void canvas_fill_rect(Canvas *canvas, int x, int y, int w, int h, uint32_t c)
{
	for (int i = 0; i < h; i++) {
		canvas_hline(canvas, x, x + w - 1, y + i, c);
	}
}

