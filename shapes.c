#include "shapes.h"

void draw_point(Canvas *canvas, Vec2 *point, uint32_t color)
{
	canvas_put_pixel(canvas, point->x, point->y, color);
}

void draw_line(Canvas *canvas, Vec2 *a, Vec2 *b, uint32_t color)
{
	canvas_line(canvas, a->x, a->y, b->x, b->y, color);
}

void draw_circle(Canvas *canvas, Circle *circle, uint32_t color)
{
	canvas_circle(canvas, circle->center.x, circle->center.y, circle->radius, color);
}

void draw_fill_circle(Canvas *canvas, Circle *circle, uint32_t color)
{
	canvas_fill_circle(canvas, circle->center.x, circle->center.y, circle->radius, color);
}

void draw_rect(Canvas *canvas, Rect *rect, uint32_t color)
{
	canvas_rect(canvas, rect->pos.x, rect->pos.y, rect->size.x, rect->size.y, color);
}

void draw_fill_rect(Canvas *canvas, Rect *rect, uint32_t color)
{
	canvas_fill_rect(canvas, rect->pos.x, rect->pos.y, rect->size.x, rect->size.y, color);
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

