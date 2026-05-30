#include <SDL2/SDL.h>
#include "paint.h"
#include "canvas.h"
#include "shapes.h"
#include "color.h"
#include "toolbar.h"

#define WINDOW_TITLE "Paint"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

Canvas screen;
Canvas art;
Canvas buffer;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
SDL_Surface *screen_surface = NULL;
SDL_Surface *art_surface = NULL;

App app;

void update_window(void)
{
	canvas_blit(
			app.screen,
			app.art_offset.x,
			app.art_offset.y,
			app.art,
			0,
			0,
			app.art->width,
			app.art->height
		  );

	SDL_BlitSurface(screen_surface, NULL, window_surface, NULL);
	SDL_UpdateWindowSurface(window);
}

void new_canvas()
{
	canvas_fill(app.art, WHITE);
}

void save_bmp()
{
	SDL_SaveBMP(art_surface, "output.bmp");
}

int main(void)
{
	app.window_width = WINDOW_WIDTH;
	app.window_height = WINDOW_HEIGHT;
	app.screen = &screen;
	app.art = &art;
	app.buffer = &buffer;
	app.tool = tools[0];
	app.color = BLACK;
	app.thickness = THIN;

	init_ui_components();

	uint32_t *screen_pixels = malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(uint32_t));
	uint32_t *art_pixels = malloc(app.art_width * app.art_height * sizeof(uint32_t));
	uint32_t *buffer_pixels = malloc(app.art_width * app.art_height * sizeof(uint32_t));

	canvas_init(&screen, screen_pixels, WINDOW_WIDTH, WINDOW_HEIGHT);
	canvas_init(&art, art_pixels, app.art_width, app.art_height);
	canvas_init(&buffer, buffer_pixels, app.art_width, app.art_height);

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

	screen_surface = SDL_CreateRGBSurfaceFrom(
			screen_pixels,
			WINDOW_WIDTH,
			WINDOW_HEIGHT,
			32,
			WINDOW_WIDTH * 4,
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
			);

	art_surface = SDL_CreateRGBSurfaceFrom(
			app.art->pixels,
			app.art->width,
			app.art->height,
			32,
			app.art->width * 4,
			0xFF000000,
			0x00FF0000,
			0x0000FF00,
			0x000000FF
			);

	canvas_fill(app.screen, WHITE);
	canvas_fill(app.art, WHITE);

	draw_toolbar_left();
	draw_toolbar_bottom();

	update_window();

	bool motion = false;
	bool pressed = false;
	bool released = false;
	bool drawing = false;
	bool update_toolbar_left = false;
	bool update_toolbar_bottom = false;

	Vec2 curr = { 0, 0 }, prev = { 0, 0 }, start = { 0, 0 };

	bool running = true;
	SDL_Event e;
	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) {
				running = false;
			}

			int x, y;
			Uint32 flags = SDL_GetMouseState(&x, &y);
			bool left = flags & SDL_BUTTON_LMASK;

			Vec2 mouse = { x, y };
			prev = curr;
			curr = mouse;


			app.screen_prev = prev;
			app.screen_curr = curr;

			app.art_prev = (Vec2){ app.screen_prev.x - app.art_offset.x, app.screen_prev.y - app.art_offset.y };
			app.art_curr = (Vec2){ app.screen_curr.x - app.art_offset.x, app.screen_curr.y - app.art_offset.y };

			motion = false;
			pressed = false;
			released = false;

			switch (e.type) {
				case SDL_MOUSEMOTION:
					motion = true;
					break;
				case SDL_MOUSEBUTTONDOWN:
					pressed = true;
					break;
				case SDL_MOUSEBUTTONUP:
					released = true;
					break;
				default: break;
			}

			if (left) {
				if (hit_art_area()) {
					if (pressed) {
						if (e.button.button != SDL_BUTTON_LEFT) continue;

						drawing = true;

						start = curr;
						app.screen_start = start;
						app.art_start = (Vec2){ app.screen_start.x - app.art_offset.x, app.screen_start.y - app.art_offset.y };

						app.art_prev = app.art_curr;
						app.screen_prev = app.screen_curr;

						if (app.tool->preview) {
							canvas_blit(
									app.buffer,
									0,
									0,
									app.art,
									0,
									0,
									app.buffer->width,
									app.buffer->height
								   );
						}

						on_click(app.tool);
					} else if (motion && drawing) {
						if (app.tool->preview) {
							canvas_blit(
									app.art,
									0,
									0,
									app.buffer,
									0,
									0,
									app.art->width,
									app.art->height
								   );
						}

						on_motion(app.tool);
					}
				}
			}

			if (released) {
				Tool *tool;
				uint32_t *color;
				int *thick;

				if (e.button.button != SDL_BUTTON_LEFT) continue;

				if (drawing) {
					drawing = false;

					if (app.tool->preview) {
						canvas_blit(
								app.art,
								0,
								0,
								app.buffer,
								0,
								0,
								app.art->width,
								app.art->height
							   );
					}

					on_release(app.tool);
					continue;
				}

				tool = hit_toolbar_left();
				if (tool) {
					app.tool = tool;
					update_toolbar_left = true;
					continue;
				}

				color = hit_palette();
				if (color) {
					app.color = *color;
					update_toolbar_bottom = true;
					continue;
				}

				thick = hit_thickness();
				if (thick) {
					app.thickness = *thick;
					update_toolbar_bottom = true;
					continue;
				}

				if (hit_new_button()) {
					new_canvas();
					continue;
				}

				if (hit_save_button()) {
					save_bmp();
					continue;
				}
			}
		}

		if (update_toolbar_left) {
			draw_toolbar_left();
			update_toolbar_left = false;
		} else if (update_toolbar_bottom) {
			draw_toolbar_bottom();
			update_toolbar_bottom = false;
		}

		update_window();
	}

	SDL_DestroyWindow(window);
	SDL_FreeSurface(screen_surface);
	SDL_FreeSurface(art_surface);
	SDL_Quit();

	free(screen_pixels);
	free(art_pixels);
	free(buffer_pixels);

	return 0;
}

