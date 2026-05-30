#include <SDL2/SDL.h>
#include <string.h>
#include "canvas.h"
#include "shapes.h"
#include "color.h"
#include "text.h"

#define WINDOW_TITLE "Text"
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define FONT_WIDTH 8
#define FONT_HEIGHT 8

Canvas canvas;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
SDL_Surface *surface = NULL;

void update_window(void)
{
	SDL_BlitSurface(surface, NULL, window_surface, NULL);
	SDL_UpdateWindowSurface(window);
}

int main(void)
{
	uint32_t *pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));

	canvas_init(&canvas, pixels, WINDOW_WIDTH, WINDOW_HEIGHT);

	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(
			WINDOW_TITLE,
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			SDL_WINDOW_SHOWN
			);

	window_surface = SDL_GetWindowSurface(window);

	surface = SDL_CreateRGBSurfaceFrom(
			pixels,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			32,
			WINDOW_WIDTH * 4,
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
			);

	canvas_fill(&canvas, DARKBLUE);

	Rect rect = { { 200, 220 }, { 400, 160 } };
	draw_fill_rect(&canvas, &rect, LIGHTGREY);

	const char *str = "Hello World!";
	int scale = 3;
	int space = 2;

	int len = strlen(str);
	int text_w = len * FONT_WIDTH * scale + (len - 1) * space;
	int text_h = FONT_HEIGHT * scale;

	int x = rect.pos.x + (rect.size.x - text_w) * 0.5f;
	int y = rect.pos.y + (rect.size.y - text_h) * 0.5f;

	put_string_in_rect(&canvas, &rect, str, scale, space, BLACK);

	update_window();

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}
		}
	}

	SDL_DestroyWindow(window);
	SDL_FreeSurface(surface);
	SDL_Quit();

	free(pixels);

	return 0;
}

