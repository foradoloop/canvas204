#ifndef TOOLS_H
#define TOOLS_H

#include <stdbool.h>

#define NUM_TOOLS 8

struct tool {
	Rect bounds;
	bool preview;
	void (*draw_icon)(Rect *);
	void (*on_click)();
	void (*on_motion)();
	void (*on_release)();
};
typedef struct tool Tool;

extern Tool *tools[];

void on_click(Tool *tool);
void on_motion(Tool *tool);
void on_release(Tool *tool);

#endif

