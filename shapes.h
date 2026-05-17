#ifndef SHAPES_H
#define SHAPES_H

#include "canvas.h"
#include <stdbool.h>

struct vec2 {
	float x;
	float y;
};
typedef struct vec2 Vec2;

struct rect {
	Vec2 pos;
	Vec2 size;
};
typedef struct rect Rect;

struct circle {
	Vec2 center;
	float radius;
};
typedef struct circle Circle;

void draw_point(Canvas *canvas, Vec2 *point, uint32_t color);
void draw_line(Canvas *canvas, Vec2 *a, Vec2 *b, uint32_t color);
void draw_circle(Canvas *canvas, Circle *circle, uint32_t color);
void draw_fill_circle(Canvas *canvas, Circle *circle, uint32_t color);
void draw_rect(Canvas *canvas, Rect *rect, uint32_t color);
void draw_fill_rect(Canvas *canvas, Rect *rect, uint32_t color);

bool point_in_rect(Vec2 *point, Rect *rect);
bool point_in_circle(Vec2 *point, Circle *circle);

#endif

