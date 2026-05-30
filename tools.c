#include "shapes.h"
#include "canvas.h"
#include "color.h"
#include <stddef.h>
#include <math.h>
#include "paint.h"
#include "tools.h"

#include <stdlib.h>

extern App app;

static void pencil_icon(Rect *bounds);
static void pencil_on_click();
static void pencil_on_motion();

static void eraser_icon(Rect *bounds);
static void eraser_on_click();
static void eraser_on_motion();

static void line_icon(Rect *bounds);
static void line_on_release();

static void rect_icon(Rect *bounds);
static void rect_on_release();

static void rect_fill_icon(Rect *bounds);
static void rect_fill_on_release();

static void circle_icon(Rect *bounds);
static void circle_on_release();

static void circle_fill_icon(Rect *bounds);
static void circle_fill_on_release();

static void bucket_icon(Rect *bounds);
static void bucket_on_click();

Tool pencil = {
	.preview = false,
	.draw_icon = pencil_icon,
	.on_click = pencil_on_click,
	.on_motion = pencil_on_motion,
	.on_release = NULL
};

Tool eraser = {
	.preview = false,
	.draw_icon = eraser_icon,
	.on_click = eraser_on_click,
	.on_motion = eraser_on_motion,
	.on_release = NULL
};

Tool line = {
	.preview = true,
	.draw_icon = line_icon,
	.on_click = NULL,
	.on_motion = line_on_release,
	.on_release = line_on_release
};

Tool rect = {
	.preview = true,
	.draw_icon = rect_icon,
	.on_click = NULL,
	.on_motion = rect_on_release,
	.on_release = rect_on_release
};

Tool rect_fill = {
	.preview = true,
	.draw_icon = rect_fill_icon,
	.on_click = NULL,
	.on_motion = rect_fill_on_release,
	.on_release = rect_fill_on_release
};

Tool circle = {
	.preview = true,
	.draw_icon = circle_icon,
	.on_click = NULL,
	.on_motion = circle_on_release,
	.on_release = circle_on_release
};

Tool circle_fill = {
	.preview = true,
	.draw_icon = circle_fill_icon,
	.on_click = NULL,
	.on_motion = circle_fill_on_release,
	.on_release = circle_fill_on_release
};

Tool bucket = {
	.preview = false,
	.draw_icon = bucket_icon,
	.on_click = bucket_on_click,
	.on_motion = NULL,
	.on_release = NULL
};

Tool *tools[] = {
	&pencil,
	&eraser,
	&line,
	&rect,
	&rect_fill,
	&circle,
	&circle_fill,
	&bucket
};

void pencil_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float x_pencil = sx + side * 1.0f / 3.0f;
	float y_pencil = sy;
	float w_pencil = side * 1.0f / 3.0f;
	float h_pencil = side;

	for (int i = 0; i < 2.0f / 6.0f * h_pencil; i++) {
		Vec2 a = { x_pencil + w_pencil * 0.5f + i * 0.5f * w_pencil / (2.0f / 6.0f * h_pencil), y_pencil + i };
		Vec2 b = { x_pencil + w_pencil * 0.5f - i * 0.5f * w_pencil / (2.0f / 6.0f * h_pencil), y_pencil + i };

		if (i < 0.5f * 2.0f / 6.0f * h_pencil) {
			draw_line(app.screen, &a, &b, MAROON);
		} else {
			draw_line(app.screen, &a, &b, LIGHTPINK);
		}
	}

	for (int i = 2.0f / 6.0f * h_pencil; i < 5.0f / 6.0f * h_pencil; i++) {
		Vec2 a = { x_pencil, y_pencil + i };
		Vec2 b = { x_pencil + w_pencil, y_pencil + i };

		if (i < 2.0f / 6.0f * h_pencil + 5.0f / 6.0f * h_pencil / 10.0f) {
			draw_line(app.screen, &a, &b, LIGHTGREY);
		} else {
			draw_line(app.screen, &a, &b, DARKORANGE);
		}
	}

	for (int i = 5.0f / 6.0f * h_pencil; i < h_pencil - 1; i++) {
		Vec2 a = { x_pencil, y_pencil + i };
		Vec2 b = { x_pencil + w_pencil, y_pencil + i };

		if (i < 5.0f / 6.0f * h_pencil + 0.5f * h_pencil / 6.0f) {
			draw_line(app.screen, &a, &b, DARKGREY);
		} else {
			draw_line(app.screen, &a, &b, HOTPINK);
		}
	}

	pencil.bounds = *bounds;
}

void pencil_on_click()
{
	if (app.thickness == THIN) {
		draw_point(app.art, &app.art_curr, app.color);
	} else {
		draw_fill_circle(app.art, &(Circle){ app.art_curr, app.thickness * 0.5f }, app.color);
	}
}

void pencil_on_motion()
{
	if (app.thickness == THIN) {
		draw_line(app.art, &app.art_curr, &app.art_prev, app.color);
	} else {
		draw_thick_line(app.art, &app.art_curr, &app.art_prev, app.thickness, app.color);
	}
}

void eraser_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	Vec2 p0 = { sx, sy + 0.75f * side };
	Vec2 p1 = { sx + 0.25f * side, sy + side };

	float a = p1.x - sx;
	float b = p0.y - sy;

	Vec2 p2 = { sx + side, sy + side - b };
	Vec2 p3 = { sx + side - a, sy };

	float t = 0.65f;

	Vec2 p4 = { p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y) };
	Vec2 p5 = { p0.x + t * (p3.x - p0.x), p0.y + t * (p3.y - p0.y) };

	Vec2 bottom[] = { p0, p1, p4, p5 };
	Vec2 top[] = { p5, p3, p2, p4 };

	draw_fill_polygon(app.screen, top, 4, PINK);
	draw_fill_polygon(app.screen, bottom, 4, LIGHTGREY);
}

void eraser_on_click()
{
	if (app.thickness == THIN) {
		draw_point(app.art, &app.art_curr, WHITE);
	} else {
		draw_fill_circle(app.art, &(Circle){ app.art_curr, app.thickness * 0.5f }, WHITE);
	}
}

void eraser_on_motion()
{
	if (app.thickness == THIN) {
		draw_line(app.art, &app.art_curr, &app.art_prev, WHITE);
	} else {
		draw_thick_line(app.art, &app.art_curr, &app.art_prev, app.thickness, WHITE);
	}
}

void line_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	Vec2 a = { sx, sy };
	Vec2 b = { sx + side, sy + side };

	draw_thick_line(app.screen, &a, &b, 0.1f * w, BLACK);
	line.bounds = *bounds;
}

void line_on_release()
{
	if (app.thickness == THIN) {
		draw_line(app.art, &app.art_start, &app.art_curr, app.color);
	} else {
		draw_thick_line(app.art, &app.art_start, &app.art_curr, app.thickness, app.color);
	}
}

void rect_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float x_rect = sx;
	float y_rect = sy;
	float w_rect = side;
	float h_rect = side;

	Vec2 pos = { x_rect, y_rect };
	Vec2 size = { w_rect, h_rect };
	Rect r = { pos, size };

	draw_rect(app.screen, &r, BLACK);

	rect.bounds = *bounds;
}

void rect_on_release()
{
	Vec2 *start = &app.art_start;
	Vec2 *curr = &app.art_curr;

	float x_max = start->x > curr->x ? start->x : curr->x;
	float x_min = start->x < curr->x ? start->x : curr->x;
	float y_max = start->y > curr->y ? start->y : curr->y;
	float y_min = start->y < curr->y ? start->y : curr->y;

	float x = x_min;
	float y = y_min;
	float w = x_max - x_min;
	float h = y_max - y_min;

	Vec2 pos = { x, y };
	Vec2 size = { w, h };
	Rect rect = { pos, size };

	draw_thick_rect(app.art, &rect, app.thickness, app.color);
}

void rect_fill_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float x_rect = sx;
	float y_rect = sy;
	float w_rect = side;
	float h_rect = side;

	Vec2 pos = { x_rect, y_rect };
	Vec2 size = { w_rect, h_rect };
	Rect rect = { pos, size };

	draw_fill_rect(app.screen, &rect, BLACK);

	rect_fill.bounds = *bounds;
}

void rect_fill_on_release()
{
	Vec2 *start = &app.art_start;
	Vec2 *curr = &app.art_curr;

	float x_max = start->x > curr->x ? start->x : curr->x;
	float x_min = start->x < curr->x ? start->x : curr->x;
	float y_max = start->y > curr->y ? start->y : curr->y;
	float y_min = start->y < curr->y ? start->y : curr->y;

	float x = x_min;
	float y = y_min;
	float w = x_max - x_min;
	float h = y_max - y_min;

	Vec2 pos = { x, y };
	Vec2 size = { w, h };
	Rect rect = { pos, size };

	draw_fill_rect(app.art, &rect, app.color);
}

void circle_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float cx = sx + side * 0.5f;
	float cy = sy + side * 0.5f;
	float r = (cx > cy) ? cy * 0.5f : cx * 0.5f;

	Vec2 center = { cx, cy };
	Circle c = { center, r };

	draw_circle(app.screen, &c, BLACK);

	circle.bounds = *bounds;
}

void circle_on_release()
{
	Vec2 *start = &app.art_start;
	Vec2 *curr = &app.art_curr;

	Vec2 center = *start;
	float x_diff = start->x - curr->x;
	float y_diff = start->y - curr->y;
	float radius = sqrtf(x_diff * x_diff + y_diff * y_diff);

	Circle circle = { center, radius };
	
	draw_thick_circle(app.art, &circle, app.thickness, app.color);
}

void circle_fill_icon(Rect *bounds)
{

	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float cx = sx + side * 0.5f;
	float cy = sy + side * 0.5f;
	float r = (cx > cy) ? cy * 0.5f : cx * 0.5f;

	Vec2 center = { cx, cy };
	Circle circle = { center, r };

	draw_fill_circle(app.screen, &circle, BLACK);

	circle_fill.bounds = *bounds;
}

void circle_fill_on_release()
{
	Vec2 *start = &app.art_start;
	Vec2 *curr = &app.art_curr;
	
	Vec2 center = *start;
	float x_diff = start->x - curr->x;
	float y_diff = start->y - curr->y;
	float radius = sqrtf(x_diff * x_diff + y_diff * y_diff);

	Circle circle = { center, radius };
	
	draw_fill_circle(app.art, &circle, app.color);
}

static void bucket_icon(Rect *bounds)
{
	float mx = bounds->size.x / 5.0f;
	float my = bounds->size.y / 5.0f;

	float x = bounds->pos.x + mx;
	float y = bounds->pos.y + my;
	float w = bounds->size.x - 2.0f * mx;
	float h = bounds->size.y - 2.0f * my;

	float side = (w < h) ? w : h;
	float sx = x + (w - side) * 0.5f;
	float sy = y + (h - side) * 0.5f;

	float w_bucket = side - 0.15f * side;
	float x_bucket = sx + (side - w_bucket) * 0.5f;
	float h_bucket = 0.75f * side;
	float y_bucket = sy + side - h_bucket; 

	Vec2 p0 = { x_bucket, y_bucket };
	Vec2 p1 = { x_bucket + w_bucket, y_bucket };
	Vec2 p2 = { x_bucket + w_bucket - 0.2f * w_bucket, y_bucket + h_bucket };
	Vec2 p3 = { x_bucket + 0.2f * w_bucket, y_bucket + h_bucket };

	float t = 0.125f;

	Vec2 w0 = { p0.x + t * (p3.x - p0.x), p0.y + t * (p3.y - p0.y) };
	Vec2 w1 = { p1.x + t * (p2.x - p1.x), p1.y + t * (p2.y - p1.y) };

	Vec2 center = { (p0.x + p1.x) * 0.5f, (p0.y + p1.y) * 0.5f };
	float radius = w_bucket * 0.9f * 0.5f;

	Circle handle = { center, radius };

	Vec2 bucket[] = { p0, p1, p2, p3 };
	Vec2 water[]  = { p0, p1, w1, w0 };

	draw_circle(app.screen, &handle, BLACK);
	draw_fill_polygon(app.screen, bucket, 4, LIGHTGREY);
	draw_fill_polygon(app.screen, water, 4, BLUE);
}

static void flood_fill(int x0, int y0, uint32_t new_color)
{
	uint32_t prev_color = canvas_get_pixel(app.art, x0, y0);

	if (!canvas_in_bounds(app.art, x0, y0) || prev_color == new_color) {
		return;
	}

	int capacity = 4;
	int cp = 0;
	Vec2 *stack = malloc(sizeof(Vec2) * capacity);

	stack[cp++] = (Vec2){ x0, y0 };

	canvas_put_pixel(app.art, x0, y0, new_color);

	while (cp > 0) {
		Vec2 p = stack[--cp];

		int x = p.x;
		int y = p.y;

		Vec2 neighbors[] = {
			{ x + 1, y },
			{ x - 1, y },
			{ x, y + 1 },
			{ x, y - 1 }
		};

		for (int i = 0; i < 4; i++) {
			int nx = neighbors[i].x;
			int ny = neighbors[i].y;

			if (canvas_in_bounds(app.art, nx, ny) &&
					canvas_get_pixel(app.art, nx, ny) == prev_color) {
				canvas_put_pixel(app.art, nx, ny, new_color);

				if (cp >= capacity) {
					capacity *= 2;
					stack = realloc(stack, capacity * sizeof(Vec2));
				}

				stack[cp++] = neighbors[i];
			}
		}
	}

	free(stack);
}

void bucket_on_click()
{
	int x = app.art_curr.x;
	int y = app.art_curr.y;

	flood_fill(x, y, app.color);
}

void on_click(Tool *tool)
{
	if (tool && tool->on_click) {
		tool->on_click();
	}
}

void on_motion(Tool *tool)
{
	if (tool && tool->on_motion) {
		tool->on_motion();
	}
}

void on_release(Tool *tool)
{
	if (tool && tool->on_release) {
		tool->on_release();
	}
}

