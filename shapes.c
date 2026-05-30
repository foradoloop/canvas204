#include "shapes.h"
#include <math.h>

#define ABS(x) (((x) < 0) ? -(x) : (x))
#define SIGN(x) (((x) > 0) - ((x) < 0))

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

void draw_point(Canvas *canvas, Vec2 *point, uint32_t color)
{
	canvas_put_pixel(canvas, point->x, point->y, color);
}

void draw_line(Canvas *canvas, Vec2 *a, Vec2 *b, uint32_t color)
{
	int x0, y0, x1, y1;
	int *major_coord, *minor_coord;
	int major_delta, minor_delta;
	int major_step, minor_step;

	x0 = a->x;
	y0 = a->y;
	x1 = b->x;
	y1 = b->y;

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
		Vec2 point = { x, y };

		draw_point(canvas, &point, color);

		if (err >= 0) {
			*minor_coord += minor_step;
			err -= 2 * major_delta;
		}

		*major_coord += major_step;
		err += 2 * minor_delta;
	}
}

void draw_triangle(Canvas *canvas, Triangle *triangle, uint32_t color)
{
	Vec2 v[3] = { triangle->a, triangle->b, triangle->c };

	draw_line(canvas, &v[0], &v[1], color);
	draw_line(canvas, &v[0], &v[2], color);
	draw_line(canvas, &v[1], &v[2], color);
}

void draw_fill_triangle(Canvas *canvas, Triangle *triangle, uint32_t color)
{
	Vec2 v[3] = { triangle->a, triangle->b, triangle->c };

	v[0].x = roundf(v[0].x); v[0].y = roundf(v[0].y);
	v[1].x = roundf(v[1].x); v[1].y = roundf(v[1].y);
	v[2].x = roundf(v[2].x); v[2].y = roundf(v[2].y);

	if (v[1].y < v[0].y) {
		Vec2 t = v[0];
		v[0] = v[1];
		v[1] = t;
	}
	if (v[2].y < v[0].y) {
		Vec2 t = v[0];
		v[0] = v[2];
		v[2] = t;
	}
	if (v[2].y < v[1].y) {
		Vec2 t = v[1];
		v[1] = v[2];
		v[2] = t;
	}

	float x0 = v[0].x, y0 = v[0].y;
	float x1 = v[1].x, y1 = v[1].y;
	float x2 = v[2].x, y2 = v[2].y;

	float x01 = x1 - x0, y01 = y1 - y0;
	float x02 = x2 - x0, y02 = y2 - y0;
	float x12 = x2 - x1, y12 = y2 - y1;

	float slope01, slope02, slope12;
	float xl, xr;
	float sl, sr;

	if (y01 == 0) {
		slope01 = 0.0f;
		slope02 = x02 / y02;
		slope12 = x12 / y12;

		xl = MIN(x0, x1);
		xr = MAX(x0, x1);

		sl = (x0 < x1) ? slope02 : slope12;
		sr = (x0 < x1) ? slope12 : slope02;
	} else if (y12 == 0) {
		slope01 = x01 / y01;
		slope02 = x02 / y02;
		slope12 = 0.0f;

		xl = x0;
		xr = x0;

		sl = MIN(slope01, slope02);
		sr = MAX(slope01, slope02);
	} else {
		slope01 = x01 / y01;
		slope02 = x02 / y02;
		slope12 = x12 / y12;

		xl = x0;
		xr = x0;

		sl = MIN(slope01, slope02);
		sr = MAX(slope01, slope02);
	}

	for (float y = v[0].y; y < v[1].y; y += 1.0f) {
		Vec2 a = { xl, y };
		Vec2 b = { xr, y };

		draw_line(canvas, &a, &b, color);

		xl += sl;
		xr += sr;
	}

	if (y01 != 0 && y12 != 0) {
		if (sl == slope01) {
			sl = slope12;
		} else {
			sr = slope12;
		}
	}

	for (float y = v[1].y; y <= v[2].y; y += 1.0f) {
		Vec2 a = { xl, y };
		Vec2 b = { xr, y };

		draw_line(canvas, &a, &b, color);

		xl += sl;
		xr += sr;
	}
}

void draw_thick_line(Canvas *canvas, Vec2 *a, Vec2 *b, float thick, uint32_t color)
{
	float x0 = a->x;
	float y0 = a->y;
	float x1 = b->x;
	float y1 = b->y;

	float dx = x1 - x0;
	float dy = y1 - y0;

	Vec2 d = { -dy, dx };

	float len = sqrtf(d.x * d.x + d.y * d.y);

	Vec2 n = { d.x / len, d.y / len };

	float half = thick * 0.5f;

	Vec2 o = { n.x * half, n.y * half };

	Vec2 p0 = { x0 + o.x, y0 + o.y };
	Vec2 p1 = { x0 - o.x, y0 - o.y };
	Vec2 p2 = { x1 + o.x, y1 + o.y };
	Vec2 p3 = { x1 - o.x, y1 - o.y };

	Triangle t[2] = {
		{ p0, p2, p3 },
		{ p0, p1, p3 }
	};

	for (int i = 0; i < 2; i++) {
		draw_fill_triangle(canvas, &t[i], color);
	}
}

void draw_circle(Canvas *canvas, Circle *circle, uint32_t color)
{
	int cx = circle->center.x;
	int cy = circle->center.y;
	int r = circle->radius;

	int x = 0;
	int y = r;

	while (x <= y) {
		int y_mid = 2 * y - 1;

		if (4 * x * x + y_mid * y_mid > 4 * r * r) {
			y--;
		}

		Vec2 points[] = {
			{ cx + x, cy + y },
			{ cx + x, cy - y },
			{ cx - x, cy + y },
			{ cx - x, cy - y },
			{ cx + y, cy + x },
			{ cx + y, cy - x },
			{ cx - y, cy + x },
			{ cx - y, cy - x }
		};

		for (int i = 0; i < 8; i++) {
			draw_point(canvas, &points[i], color);
		}

		x++;
	}
}

void draw_thick_circle(Canvas *canvas, Circle *circle, float thick, uint32_t color)
{
	for (float t = 0; t <= thick; t += 1.0f) {
		Circle c = { circle->center, circle->radius - t };
		draw_circle(canvas, &c, color);
	}
}

void draw_fill_circle(Canvas *canvas, Circle *circle, uint32_t color)
{
	int cx = circle->center.x;
	int cy = circle->center.y;
	int r = circle->radius;

	int x = 0;
	int y = r;

	while (x <= y) {
		int y_mid = 2 * y - 1;

		if (4 * x * x + y_mid * y_mid > 4 * r * r) {
			y--;
		}

		Vec2 lines[4][2] = {
			{ { cx + x, cy + y}, { cx - x, cy + y } },
			{ { cx + x, cy - y}, { cx - x, cy - y } },
			{ { cx + y, cy + x}, { cx - y, cy + x } },
			{ { cx + y, cy - x}, { cx - y, cy - x } }
		};

		for (int i = 0; i < 4; i++) {
			draw_line(canvas, &lines[i][0], &lines[i][1], color);
		}

		x++;
	}
}

void draw_rect(Canvas *canvas, Rect *rect, uint32_t color)
{
	draw_thick_rect(canvas, rect, 0.0f, color);
}

void draw_thick_rect(Canvas *canvas, Rect *rect, float thick, uint32_t color)
{
	int x = rect->pos.x;
	int y = rect->pos.y;
	int w = rect->size.x;
	int h = rect->size.y;
	float half = thick * 0.5f;

	Vec2 lines[4][2] = {
		{ { x - half, y }, { x + w - 1 + half, y } },
		{ { x - half, y + h - 1 }, { x + w - 1 + half, y + h - 1 } },
		{ { x, y - half }, { x, y + h - 1 + half } },
		{ { x + w - 1, y - half }, { x + w - 1, y + h - 1 + half } }
	};

	for (int i = 0; i < 4; i++) {
		draw_thick_line(canvas, &lines[i][0], &lines[i][1], thick, color);
	}
}

void draw_fill_rect(Canvas *canvas, Rect *rect, uint32_t color)
{
	float x = rect->pos.x;
	float y = rect->pos.y;
	float w = rect->size.x;
	float h = rect->size.y;

	Vec2 a = { x, y };
	Vec2 b = { x + w - 1, y };
	Vec2 c = { x + w - 1, y + h - 1 };
	Vec2 d = { x, y + h - 1 };

	Triangle t1 = { a, b, c };
	Triangle t2 = { a, c, d };

	draw_fill_triangle(canvas, &t1, color);
	draw_fill_triangle(canvas, &t2, color);
}

void draw_polygon(Canvas *canvas, Vec2 *points, int num_points, uint32_t color)
{
	for (int i = 0; i < num_points; i++) {
		draw_line(canvas, &points[i], &points[(i + 1) % num_points], color);
	}
}

void draw_fill_polygon(Canvas *canvas, Vec2 *points, int num_points, uint32_t color)
{
	for (int i = 1; i < num_points - 1; i++) {
		Triangle t = { points[0], points[i], points[i + 1] };
		draw_fill_triangle(canvas, &t, color);
	}
}

bool point_in_rect(Vec2 *point, Rect *rect)
{
	return (
			rect->pos.x <= point->x && point->x < (rect->pos.x + rect->size.x) &&
			rect->pos.y <= point->y && point->y < (rect->pos.y + rect->size.y)
	       );
}

bool point_in_circle(Vec2 *point, Circle *circle)
{
	float x_diff = point->x - circle->center.x;
	float y_diff = point->y - circle->center.y;
	float r_square = circle->radius * circle->radius;

	return x_diff * x_diff + y_diff * y_diff <= r_square;
}

