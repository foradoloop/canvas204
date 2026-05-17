#include "canvas.h"
#include "shapes.h"
#include <SDL2/SDL.h>

#define WINDOW_TITLE "Smoke"
#define WINDOW_WIDTH 1281
#define WINDOW_HEIGHT 721

#define PINK ((255 << 24) | (109 << 16) | (194 << 8) | 255)
#define DARKPURPLE ((112 << 24) | (31 << 16) | (126 << 8 ) | 255)
#define DARKGREEN ((0 << 24) | (82 << 16) | (17 << 8) | 255)
#define DARKBLUE ((0 << 24) | (82 << 16) | (172 << 8) | 255)

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

	Vec2 center = { (WINDOW_WIDTH - 1) / 2, (WINDOW_HEIGHT - 1) / 2};
	float radius = (WINDOW_WIDTH > WINDOW_HEIGHT) ? (WINDOW_HEIGHT - 1) / 2 : (WINDOW_WIDTH - 1) / 2;
	Circle circle = { center, radius };

	draw_fill_circle(&canvas, &circle, DARKPURPLE);

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

