/* 
 * font.c -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon McNutt
 */

#include "font.h"

#include <stdio.h>

#include "asprintf.h"

int font_init(void)
{
        if (TTF_Init()) {
                fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
                return -1;
        }
        atexit(TTF_Quit);
        return 0;
}

void font_printf(SDL_Renderer * renderer, TTF_Font * font, const char *fmt, ...)
{
        /* Allocate the printf-d string. */
        va_list args;
        char *buf;
        va_start(args, fmt);
        int ret = vasprintf(&buf, fmt, args);
        va_end(args);
        if (-1 == ret) {
                perror("asnprintf");
                return;
        }

        /* Render the string using the font to a surface. */
        SDL_Color fg = { 0, 0, 0, SDL_ALPHA_OPAQUE };
        SDL_Surface *surface;
        if (!(surface = TTF_RenderText_Blended(font, buf, fg))) {
                fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
                goto freebuf;
        }
        /*
         * SDL_Color bg = { 128, 128, 128, SDL_ALPHA_OPAQUE };
         * if (!(surface = TTF_RenderText_Shaded(font, buf, fg, bg))) {
         * fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
         * return;
         * }
         * if (!(surface = TTF_RenderText_Solid(font, buf, fg))) {
         * fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
         * return;
         * }
         */

        /* Create a texture from the surface. */
        SDL_Texture *texture;
        if (!(texture = SDL_CreateTextureFromSurface(renderer, surface))) {
                fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
                goto freesurface;
        }

        /* Render the texture. */
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect dest = { 0, 0, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);

freesurface:
        SDL_FreeSurface(surface);
freebuf:
        free(buf);
}
