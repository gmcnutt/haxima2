/* 
 * font.h -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon James McNutt
 */

#ifndef FONT_H
#define FONT_H

#include <SDL2/SDL.h>

typedef struct font font_t;

/* Initialze the font system. */
int font_sys_init(void);

/* Create a font from a font file. Free using mem_deref(). */
font_t * font_open(const char *font_file, int ptsize);

/* Print info about a font to stdout. */
void font_print_info(font_t *font);

/* Render text using the font. */
void font_render(font_t *font, SDL_Renderer * renderer, const char *text);

#endif
