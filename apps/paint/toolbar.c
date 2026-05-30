#include "paint.h"
#include "canvas.h"
#include "shapes.h"
#include "color.h"
#include "tools.h"
#include "text.h"

#include <stddef.h>

#define SELECTED ROYALBLUE 

#define NUM_COLORS 8
#define NUM_THICKNESS 3

struct palette {
	Rect rect;
	Rect bounds[NUM_COLORS];
	uint32_t colors[NUM_COLORS];
};
typedef struct palette Palette;

Palette palette;

uint32_t palette_colors[NUM_COLORS] = {
	BLACK, WHITE, RED, GREEN, BLUE, YELLOW, CYAN, MAGENTA
};

struct thickness {
	Rect rect;
	Rect bounds[NUM_THICKNESS];
	int nivel[NUM_THICKNESS];
};
typedef struct thickness Thickness;

Thickness thickness;

struct button {
	Rect bounds;
};
typedef struct button Button;

Button new;
Button save;

Rect toolbar_l;
Rect toolbar_b;

int toolbar_l_x;
int toolbar_l_y;
int toolbar_l_w;
int toolbar_l_h;

int toolbar_b_x;
int toolbar_b_y;
int toolbar_b_w;
int toolbar_b_h;

int palette_x;
int palette_y;
int palette_w;
int palette_h;

int thickness_x;
int thickness_y;
int thickness_w;
int thickness_h;

int new_x;
int new_y;
int new_w;
int new_h;

int save_x;
int save_y;
int save_w;
int save_h;

void init_ui_components()
{
	int window_width = app.window_width;
	int window_height = app.window_height;

	toolbar_l_x = 0;
	toolbar_l_y = 0;
	toolbar_l_w = window_width * 0.075f;
	toolbar_l_h = window_height * 8.0f / 9.0f;

	toolbar_b_x = 0;
	toolbar_b_y = toolbar_l_h;
	toolbar_b_w = window_width;
	toolbar_b_h = window_height - toolbar_l_h;

	palette_x = toolbar_b_x;
	palette_y = toolbar_b_y;
	palette_w = toolbar_b_w * 0.5f;
	palette_h = toolbar_b_h;

	thickness_x = palette_x + palette_w;
	thickness_y = toolbar_b_y;
	thickness_w = (toolbar_b_w - palette_w) * 0.5f;
	thickness_h = toolbar_b_h;

	new_x = thickness_x + thickness_w;
	new_y = toolbar_b_y;
	new_w = (toolbar_b_w - palette_w - thickness_w) * 0.5f;
	new_h = toolbar_b_h;

	save_x = new_x + new_w;
	save_y = toolbar_b_y;
	save_w = (toolbar_b_w - palette_w - thickness_w) * 0.5f;
	save_h = toolbar_b_h;

	toolbar_l = (Rect){
		{ toolbar_l_x, toolbar_l_y },
		{ toolbar_l_w, toolbar_l_h }
	};

	toolbar_b = (Rect){
		{ toolbar_b_x, toolbar_b_y },
		{ toolbar_b_w, toolbar_b_h }
	};

	palette.rect = (Rect){
		{ palette_x, palette_y },
		{ palette_w, palette_h }
	};

	thickness.rect = (Rect){
		{ thickness_x, thickness_y },
		{ thickness_w, thickness_h }
	};

	new.bounds = (Rect){
		{ new_x, new_y },
		{ new_w, new_h }
	};

	save.bounds = (Rect){
		{ save_x, save_y },
		{ save_w, save_h }
	};

	thickness.nivel[0] = THIN;
	thickness.nivel[1] = MEDIUM;
	thickness.nivel[2] = THICK;

	for (int i = 0; i < NUM_COLORS; i++) {
		palette.colors[i] = palette_colors[i];
	}

	app.art_width = app.window_width - toolbar_l.size.x;
	app.art_height = app.window_height - toolbar_b.size.y;
	app.art_offset = (Vec2){ toolbar_l.size.x, 0 };
}

void draw_toolbar_left(void)
{
	draw_fill_rect(app.screen, &toolbar_l, DIMGREY);

	float x = toolbar_l.pos.x;
	float y = toolbar_l.pos.y;
	float w = toolbar_l.size.x;
	float h = toolbar_l.size.y;

	float cell = h / 8.0f;

	float mx = 0.05f * w; 
	float my = 0.05f * cell;

	for (int i = 0; i < NUM_TOOLS; i++) {
		Rect btn = {
			.pos = { x + mx, y + my + cell * i },
			.size = { w - mx - mx, cell - my - my }
		};

		draw_fill_rect(app.screen, &btn, (app.tool == tools[i]) ? SELECTED : GREY);
		tools[i]->draw_icon(&btn);
		tools[i]->bounds = btn;
	}
}

void draw_palette(void)
{
	draw_fill_rect(app.screen, &palette.rect, DIMGREY);

	float x = palette.rect.pos.x;
	float y = palette.rect.pos.y;
	float w = palette.rect.size.x;
	float h = palette.rect.size.y;

	float cell = w / NUM_COLORS;

	float mx = 0.1f * cell; 
	float my = 0.1f * h;

	for (int i = 0; i < NUM_COLORS; i++) {
		Rect btn = {
			{ x + mx + cell * i, y + my },
			{ cell - mx - mx, h - my - my }
		};

		draw_fill_rect(app.screen, &btn, (app.color == palette.colors[i]) ? SELECTED : GREY);

		Rect color = {
			{ btn.pos.x + mx, btn.pos.y + my },
			{ btn.size.x - mx - mx, btn.size.y - my - my }
		};

		draw_fill_rect(app.screen, &color, palette.colors[i]);

		palette.bounds[i] = btn;
	}
}

void draw_thickness_option(void)
{
	draw_fill_rect(app.screen, &thickness.rect, DIMGREY);

	float x = thickness.rect.pos.x;
	float y = thickness.rect.pos.y;
	float w = thickness.rect.size.x;
	float h = thickness.rect.size.y;

	float cell = w / NUM_THICKNESS;

	float mx = 0.1f * cell; 
	float my = 0.1f * h;

	for (int i = 0; i < NUM_THICKNESS; i++) {
		Rect btn = {
			{ x + mx + cell * i, y + my },
			{ cell - mx - mx, h - my - my }
		};

		draw_fill_rect(app.screen, &btn, (app.thickness == thickness.nivel[i]) ? SELECTED : GREY);

		float mxt = btn.size.x * 0.1f;

		int r = 0.025f * (i + 1) * btn.size.x;
		Vec2 center1 = { btn.pos.x + r + mxt, btn.pos.y + btn.size.y * 0.5f };
		Vec2 center2 = { btn.pos.x + btn.size.x - r - mxt, btn.pos.y + btn.size.y * 0.5f };

		Circle circle1 = { center1, r };
		Circle circle2 = { center2, r };

		draw_fill_circle(app.screen, &circle1, BLACK);
		draw_fill_circle(app.screen, &circle2, BLACK);

		Rect opt = {
			{ center1.x, center1.y - r },
			{ center2.x - center1.x, 2 * r + 1 }
		};

		draw_fill_rect(app.screen, &opt, BLACK);

		thickness.bounds[i] = btn;
	}
}

void draw_new_button(void)
{
	Rect new = { { new_x, new_y }, { new_w, new_h } };

	float x = new.pos.x;
	float y = new.pos.y;
	float w = new.size.x;
	float h = new.size.y;

	float mx = 0.1f * w;
	float my = 0.1f * h;

	Rect btn = {
		{ x + mx, y + my },
		{ w - mx - mx, h - my - my }
	};

	Rect opt = {
		{ btn.pos.x + 0.1f * btn.size.x, btn.pos.y + 0.1f * btn.size.y },
		{ btn.size.x - 2.0f * 0.1f * btn.size.x, btn.size.y - 2.0f * 0.1f * btn.size.y }
	};

	draw_fill_rect(app.screen, &btn, GREY);
	draw_fill_rect(app.screen, &opt, LIGHTGREY);

	int scale = 2;
	int space = 1;

	put_string_in_rect(
			app.screen,
			&opt,
			"NEW",
			scale,
			space,
			BLACK
			);
}

void draw_save_button(void)
{
	Rect new = { { save_x, save_y}, { save_w, save_h } };

	float x = new.pos.x;
	float y = new.pos.y;
	float w = new.size.x;
	float h = new.size.y;

	float mx = 0.1f * w;
	float my = 0.1f * h;

	Rect btn = {
		{ x + mx, y + my },
		{ w - mx - mx, h - my - my }
	};

	Rect opt = {
		{ btn.pos.x + 0.1f * btn.size.x, btn.pos.y + 0.1f * btn.size.y },
		{ btn.size.x - 2.0f * 0.1f * btn.size.x, btn.size.y - 2.0f * 0.1f * btn.size.y }
	};

	draw_fill_rect(app.screen, &btn, GREY);
	draw_fill_rect(app.screen, &opt, LIGHTGREY);

	int scale = 2;
	int space = 1;

	put_string_in_rect(
			app.screen,
			&opt,
			"SAVE",
			scale,
			space,
			BLACK
			);
}

void draw_toolbar_bottom(void)
{
	draw_fill_rect(app.screen, &toolbar_b, DIMGREY);
	draw_palette();
	draw_thickness_option();
	draw_new_button();
	draw_save_button();
}

Tool *hit_toolbar_left()
{
	if (!point_in_rect(&app.screen_curr, &toolbar_l)) return NULL;

	for (int i = 0; i < NUM_TOOLS; i++) {
		if (point_in_rect(&app.screen_curr, &tools[i]->bounds)) {
			return tools[i];
		}
	}

	return NULL;
}

uint32_t *hit_palette()
{
	if (!point_in_rect(&app.screen_curr, &palette.rect)) {
		return NULL;
	}

	for (int i = 0; i < NUM_COLORS; i++) {
		if (point_in_rect(&app.screen_curr, &palette.bounds[i])) {
			return &palette.colors[i];
		}
	}

	return NULL;
}

bool hit_art_area()
{
	Rect area = {
		{ app.art_offset.x, app.art_offset.y },
		{ app.art->width, app.art->height }
	};

	return point_in_rect(&app.screen_curr, &area);
}

int *hit_thickness()
{
	if (!point_in_rect(&app.screen_curr, &thickness.rect)) {
		return NULL;
	}

	for (int i = 0; i < NUM_THICKNESS; i++) {
		if (point_in_rect(&app.screen_curr, &thickness.bounds[i])) {
			return &thickness.nivel[i];
		}
	}

	return NULL;
}

bool hit_new_button()
{
	return point_in_rect(&app.screen_curr, &new.bounds);
}

bool hit_save_button()
{
	return point_in_rect(&app.screen_curr, &save.bounds);
}

