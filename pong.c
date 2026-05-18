#include <SDL2/SDL.h>
#include "canvas.h"
#include "shapes.h"

#define WINDOW_TITLE "Ping Pong"
#define WINDOW_WIDTH 1281
#define WINDOW_HEIGHT 721

#define COLOR(r, g, b, a) ((uint32_t)((r << 24) | (g << 16) | (b << 8) | a))

#define WHITE COLOR(255, 255, 255, 255)
#define BLACK COLOR(0, 0, 0, 255)

Canvas canvas;
SDL_Window *window = NULL;
SDL_Surface *window_surface = NULL;
SDL_Surface *surface = NULL;

struct paddle {
	Rect paddle;
	float vel;
	Vec2 dir;
};
typedef struct paddle Paddle;

struct ball {
	Circle ball;
	float vel;
	Vec2 dir;
};
typedef struct ball Ball;

const Rect screen = {
	.pos = { 0.0f, 0.0f },
	.size = { WINDOW_WIDTH, WINDOW_HEIGHT }
};

Paddle player = {
	.paddle = {
		.pos = { 0.03f * screen.size.x, 0.5f * screen.size.y },
		.size = { 0.01f * screen.size.x, 0.15f * screen.size.y }
	},
	.vel = 5.5f,
};

Paddle enemy = {
	.paddle = {
		.pos = { 0.97f * screen.size.x - 0.01f * screen.size.x, 0.5f * screen.size.y },
		.size = { 0.01f * screen.size.x, 0.15f * screen.size.y }
	},
	.vel = 5.5f,
};

Ball ball = {
	.ball = { { 0.5f * screen.size.x, 0.5f * screen.size.y }, 0.02f * screen.size.x },
	.vel = 4.0f,
	.dir = { 1.0f, -1.0f }
};

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

	bool running = true;
	SDL_Event e;
	const Uint8 *curr_key_states;

	while (running) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT) { running = false; }
		}

		curr_key_states = SDL_GetKeyboardState(NULL);

		// Player update
		if (curr_key_states[SDL_SCANCODE_W]) {
			player.paddle.pos.y -= player.vel;
		}
		if (curr_key_states[SDL_SCANCODE_S]) {
			player.paddle.pos.y += player.vel;
		}

		// Check if player is not out of bounds
		if (player.paddle.pos.y < 0) {
			player.paddle.pos.y = 0;
		} else if (player.paddle.pos.y + player.paddle.size.y >= screen.size.y) {
			player.paddle.pos.y = screen.size.y - player.paddle.size.y;
		}

		// Enemy update (simple AI)
		if (ball.ball.center.y + ball.ball.radius < enemy.paddle.pos.y) {
			enemy.paddle.pos.y -= enemy.vel;
		} else if (ball.ball.center.y - ball.ball.radius > enemy.paddle.pos.y) {
			enemy.paddle.pos.y += enemy.vel; 
		}

		// Check if enemy is not out of bounds
		if (enemy.paddle.pos.y < 0) {
			enemy.paddle.pos.y++;
		} else if (enemy.paddle.pos.y + enemy.paddle.size.y >= screen.size.y) {
			enemy.paddle.pos.y = screen.size.y - enemy.paddle.size.y;
		}

		// Ball update
		ball.ball.center.x += ball.dir.x * ball.vel;
		ball.ball.center.y += ball.dir.y * ball.vel;

		// Check if ball has hit the screen
		if (ball.ball.center.y - ball.ball.radius <= 0) {
			ball.ball.center.y = ball.ball.radius;
			ball.dir.y = -ball.dir.y;
		} else if (ball.ball.center.y + ball.ball.radius >= screen.size.y) {
			ball.ball.center.y = screen.size.y - ball.ball.radius;
			ball.dir.y = -ball.dir.y;
		}  else if (ball.ball.center.x - ball.ball.radius < 0 || ball.ball.center.x + ball.ball.radius > screen.size.x) {
			running = false;
			break;
		}

		// Check if ball has hit any of the players
		if ((ball.ball.center.x - ball.ball.radius) <= player.paddle.pos.x + player.paddle.size.x &&
				(player.paddle.pos.y - ball.ball.radius <= ball.ball.center.y
				 && ball.ball.center.y <= player.paddle.pos.y + player.paddle.size.y + ball.ball.radius)
			) {
			ball.dir.x = -ball.dir.x;
			ball.ball.center.x = player.paddle.pos.x + player.paddle.size.x + ball.ball.radius;
		} else if ((ball.ball.center.x + ball.ball.radius) >= enemy.paddle.pos.x &&
				(enemy.paddle.pos.y - ball.ball.radius <= ball.ball.center.y
				 && ball.ball.center.y <= enemy.paddle.pos.y + enemy.paddle.size.y + ball.ball.radius)
			  ) {
			ball.dir.x = -ball.dir.x;
			ball.ball.center.x = enemy.paddle.pos.x - ball.ball.radius;
		}

		canvas_fill(&canvas, BLACK);
		draw_fill_rect(&canvas, &player.paddle, WHITE);
		draw_fill_rect(&canvas, &enemy.paddle, WHITE);
		draw_fill_circle(&canvas, &ball.ball, WHITE);

		update_window();
	}

	SDL_DestroyWindow(window);
	SDL_FreeSurface(surface);
	SDL_Quit();

	free(pixels);

	return 0;
}

