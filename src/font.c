/* 
 * font.c -- utility wrapper for SDL_ttf
 *
 * Copyright 2020 Gordon McNutt
 */

#include "font.h"

#include <stdio.h>
#include <SDL2/SDL_ttf.h>

#include "common.h"
#include "mem.h"

struct font {
        TTF_Font *ttf;
        font_render_method_t render_method;
        SDL_Color bg;
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
        font_t *font = (font_t *) ptr;
        if (font->ttf) {
                TTF_CloseFont(font->ttf);
                font->ttf = NULL;
        }
}

int font_get_height(font_t * font)
{
        return TTF_FontHeight(font->ttf);
}

font_t *font_open(const char *font_file, int ptsize)
{
        font_t *font = mem_alloc(sizeof (font_t), _font_fin);

        if (!(font->ttf = TTF_OpenFont(font_file, ptsize))) {
                fprintf(stderr, "TTF_OpenFont(%s): %s\n", font_file,
                        TTF_GetError());
                goto deref;
        }
        return font;
deref:
        mem_deref(font);
        return NULL;
}

void font_print_info(font_t * font)
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
        printf("kerning:    %s\n",
               TTF_GetFontKerning(font->ttf) ? "on" : "off");
        printf("outline:    %d (pixels)\n", TTF_GetFontOutline(font->ttf));
        printf("height:     %d (pixels)\n", TTF_FontHeight(font->ttf));
        printf("ascent:     %d (pixels)\n", TTF_FontAscent(font->ttf));
        printf("descent:    %d (pixels)\n", TTF_FontDescent(font->ttf));
        printf("lineskip:   %d (pixels)\n", TTF_FontLineSkip(font->ttf));
}

void font_render(font_t * font, SDL_Renderer * renderer, const SDL_Rect * dest,
                 const char *buf)
{
        /* Render the string using the font to a surface. */
        SDL_Color fg = { 0, 0, 0, SDL_ALPHA_OPAQUE };
        SDL_Surface *surface;

        if (font->render_method == FONT_BLENDED) {
                if (!(surface = TTF_RenderText_Blended(font->ttf, buf, fg))) {
                        fprintf(stderr, "TTF_RenderText_Blended: %s\n",
                                TTF_GetError());
                        return;
                }
        } else if (font->render_method == FONT_SHADED) {
                if (!
                    (surface =
                     TTF_RenderText_Shaded(font->ttf, buf, fg, font->bg))) {
                        fprintf(stderr, "TTF_RenderText_Shaded: %s\n",
                                TTF_GetError());
                        return;
                }
        } else {
                if (!(surface = TTF_RenderText_Solid(font->ttf, buf, fg))) {
                        fprintf(stderr, "TTF_RenderText_Solid: %s\n",
                                TTF_GetError());
                        return;
                }
        }

        /* Create a texture from the surface. */
        SDL_Texture *texture;
        if (!(texture = SDL_CreateTextureFromSurface(renderer, surface))) {
                fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n",
                        SDL_GetError());
                goto freesurface;
        }

        /* Render the texture. */
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect _dest = *dest;
        _dest.w = min(w, _dest.w);
        _dest.h = min(h, _dest.h);
        SDL_RenderCopy(renderer, texture, NULL, &_dest);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);

freesurface:
        SDL_FreeSurface(surface);
}

void font_set_bgcolor(font_t * font, SDL_Color * color)
{
        font->bg = *color;
}

void font_set_render_method(font_t * font, font_render_method_t method)
{
        font->render_method = method;
}
