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

/* Max height in pixels of rendered text. */
int font_get_height(font_t * font);

/* Create a font from a font file. Free using mem_deref(). */
font_t *font_open(const char *font_file, int ptsize);

/* Print info about a font to stdout. */
void font_print_info(font_t * font);

/* Render text using the font to the target rect. Text will be clipped to fit
 * the dest if necessary. */
void font_render(font_t * font, SDL_Renderer * renderer, const SDL_Rect * dest,
                 const char *text);

/* Set the background color for shaded rendering. */
void font_set_bgcolor(font_t * font, SDL_Color * color);

/* Set the rendering method. */
typedef enum {
        FONT_BLENDED,
        FONT_SHADED,
        FONT_SOLID
} font_render_method_t;
void font_set_render_method(font_t * font, font_render_method_t method);

#endif
