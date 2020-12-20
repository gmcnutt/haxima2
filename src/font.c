/* 
 * font.c -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon McNutt
 */

#include "font.h"

#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "mem.h"

struct font {
        TTF_Font *ttf;
};

int font_sys_init(void)
{
        if (TTF_Init()) {
                fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
                return -1;
        }
        atexit(TTF_Quit);
        return 0;
}

static void _font_fin(void *ptr)
{
        font_t *font = (font_t *)ptr;
        if (font->ttf) {
                TTF_CloseFont(font->ttf);
                font->ttf = NULL;
        }
}

font_t * font_open(const char *font_file, int ptsize)
{
        font_t *font = mem_alloc(sizeof(font_t), _font_fin);

        if (! (font->ttf = TTF_OpenFont(font_file, ptsize))) {
                fprintf(stderr, "TTF_OpenFont(%s): %s\n", font_file,
                        TTF_GetError());
                goto deref;
        }
        return font;
deref:
        mem_deref(font);
        return NULL;
}

void font_print_info(font_t *font)
{
        static const char *hintings[] = {
                "normal", "light", "mono", "none"
        };

        static const char *styles[] = {
                "normal", "bold", "italic", "underling", "strikethrough"
        };


        printf("family:     %s\n", TTF_FontFaceFamilyName(font->ttf));
        printf("style:      %s\n", TTF_FontFaceStyleName(font->ttf));
        printf("faces:      %ld\n", TTF_FontFaces(font->ttf));
        printf("fixedwidth: %s\n",
               TTF_FontFaceIsFixedWidth(font->ttf) ? "yes" : "no");

        int style = TTF_GetFontStyle(font->ttf);
        printf("mode:      ");
        if (style == TTF_STYLE_NORMAL) {
                printf(" normal");
        } else {
                for (int i = 0; i < 4; i++) {
                        if (style & (1 << i)) {
                                printf(" %s", styles[i + 1]);
                        }
                }
        }
        printf("\n");

        printf("hinting:    %s\n", hintings[TTF_GetFontHinting(font->ttf)]);
        printf("kerning:    %s\n", TTF_GetFontKerning(font->ttf) ? "on" : "off");
        printf("outline:    %d (pixels)\n", TTF_GetFontOutline(font->ttf));
        printf("height:     %d (pixels)\n", TTF_FontHeight(font->ttf));
        printf("ascent:     %d (pixels)\n", TTF_FontAscent(font->ttf));
        printf("descent:    %d (pixels)\n", TTF_FontDescent(font->ttf));
        printf("lineskip:   %d (pixels)\n", TTF_FontLineSkip(font->ttf));
}

void font_render(font_t *font, SDL_Renderer * renderer, const char *buf)
{
        /* Render the string using the font to a surface. */
        SDL_Color fg = { 0, 0, 0, SDL_ALPHA_OPAQUE };
        SDL_Surface *surface;
        if (!(surface = TTF_RenderText_Blended(font->ttf, buf, fg))) {
                fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
                return;
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
}
