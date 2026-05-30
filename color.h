#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

#define COLOR(r, g, b, a) (((uint32_t)(r) << 24) | ((uint32_t)(g) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(a))

#define BLACK       COLOR(0,   0,   0,   255)
#define WHITE       COLOR(255, 255, 255, 255)
#define RED         COLOR(255, 0,   0,   255)
#define GREEN       COLOR(0,   255, 0,   255)
#define BLUE        COLOR(0,   0,   255, 255)
#define YELLOW      COLOR(255, 255, 0,   255)
#define CYAN        COLOR(0,   255, 255, 255)
#define MAGENTA     COLOR(255, 0,   255, 255)

#define DARKGREY    COLOR(64,  64,  64,  255)
#define GREY        COLOR(128, 128, 128, 255)
#define LIGHTGREY   COLOR(192, 192, 192, 255)
#define DIMGREY     COLOR(105, 105, 105, 255)
#define SLATEGREY   COLOR(112, 128, 144, 255)

#define TAN         COLOR(210, 180, 140, 255)
#define BROWN       COLOR(165, 42,  42,  255)
#define SIENNA      COLOR(160, 82,  45,  255)

#define GOLD        COLOR(255, 215, 0,   255)
#define ORANGE      COLOR(255, 165, 0,   255)
#define DARKORANGE  COLOR(255, 140, 0,   255)

#define LIGHTPINK   COLOR(255, 182, 193, 255)
#define HOTPINK     COLOR(255, 105, 180, 255)
#define PINK        COLOR(255, 192, 203, 255)
#define DEEPPINK    COLOR(255, 20,  147, 255)

#define DARKBLUE    COLOR(0,   0,   139, 255)
#define SKYBLUE     COLOR(135, 206, 235, 255)
#define STEELBLUE   COLOR(70,  130, 180, 255)
#define ROYALBLUE   COLOR(65,  105, 225, 255)

#define DARKGREEN   COLOR(0,   100, 0,   255)
#define LIMEGREEN   COLOR(50,  205, 50,  255)
#define FORESTGREEN COLOR(34,  139, 34,  255)

#define PURPLE      COLOR(128, 0,   128, 255)
#define DARKPURPLE  COLOR(106, 31,  126, 255)
#define VIOLET      COLOR(238, 130, 238, 255)

#define MAROON      COLOR(128, 0,   0,   255)
#define OLIVE       COLOR(128, 128, 0,   255)
#define NAVY        COLOR(0,   0,   128, 255)
#define TEAL        COLOR(0,   128, 128, 255)
#define SILVER      COLOR(192, 192, 192, 255)

#endif

