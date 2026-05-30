#ifndef PAINT_H
#define PAINT_H

#include "canvas.h"
#include "shapes.h"
#include "color.h"
#include "tools.h"

struct app {
	int window_width;
	int window_height;
	int art_width;
	int art_height;
	Canvas *screen;
	Canvas *art;
	Canvas *buffer;
	Tool *tool;
	Vec2 screen_start, screen_prev, screen_curr;
	Vec2 art_start, art_prev, art_curr;
	Vec2 art_offset;
	uint32_t color;
	int thickness;
};
typedef struct app App;

extern App app;

enum {
	THIN =  1,
	MEDIUM = 5,
	THICK = 15
};

#endif

