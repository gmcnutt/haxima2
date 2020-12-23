/* 
 * font.h -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon James McNutt
 */

#pragma once

#include <SDL2/SDL.h>


typedef struct font font_t;


/* Initialze the font system. */
int font_sys_init(void);

/* Max height in pixels of rendered text. */
int font_get_height(font_t * font);

/* Width of the char's glyph in pixels. Returns -1 if the char has no glyph. */
int font_get_glyph_width(font_t * font, char c);

/* Create a font from a font file. Free using mem_deref(). */
font_t *font_open(const char *font_file, int ptsize);

/* Print info about a font to stdout. */
void font_print_info(font_t * font);

/* Render text to a texture. */
SDL_Texture *font_render(font_t * font, const char *text);

/* Set the background color for shaded rendering; foreground color for any
 * rendering. */
void font_set_bgcolor(font_t * font, SDL_Color color);
void font_set_fgcolor(font_t * font, SDL_Color color);

/* Set the rendering method. */
typedef enum {
        FONT_BLENDED,           /* good default */
        FONT_SHADED,            /* add a background */
        FONT_SOLID              /* kind of sucks */
} font_render_method_t;
void font_set_render_method(font_t * font, font_render_method_t method);
