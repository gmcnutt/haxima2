/* 
 * font.h -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon James McNutt
 */

#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int font_init(void);
void font_printf(SDL_Renderer * renderer, TTF_Font * font, const char *fmt, ...);

#endif
