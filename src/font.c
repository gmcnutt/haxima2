/* 
 *  haxima2 - font.c
 *
 *  Copyright 2020 Gordon James McNutt
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <SDL2/SDL_ttf.h>

int font_init(void)
{
        if (TTF_Init()) {
                fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
                return -1;
        }
        atexit(TTF_Quit);
        return 0;
}

void _font_print_info(TTF_Font * font)
{
        static const char *hintings[] = {
                "normal", "light", "mono", "none"
        };

        static const char *styles[] = {
                "normal", "bold", "italic", "underling", "strikethrough"
        };


        printf("family:     %s\n", TTF_FontFaceFamilyName(font));
        printf("style:      %s\n", TTF_FontFaceStyleName(font));
        printf("faces:      %ld\n", TTF_FontFaces(font));
        printf("fixedwidth: %s\n",
               TTF_FontFaceIsFixedWidth(font) ? "yes" : "no");

        int style = TTF_GetFontStyle(font);
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

        printf("hinting:    %s\n", hintings[TTF_GetFontHinting(font)]);
        printf("kerning:    %s\n", TTF_GetFontKerning(font) ? "on" : "off");
        printf("outline:    %d (pixels)\n", TTF_GetFontOutline(font));
        printf("height:     %d (pixels)\n", TTF_FontHeight(font));
        printf("ascent:     %d (pixels)\n", TTF_FontAscent(font));
        printf("descent:    %d (pixels)\n", TTF_FontDescent(font));
        printf("lineskip:   %d (pixels)\n", TTF_FontLineSkip(font));
}

void _font_printf(SDL_Renderer * renderer, TTF_Font * font, const char *fmt,
                  ...)
{
        static char buf[256];
        va_list args;
        SDL_Color fg = { 0, 0, 0, SDL_ALPHA_OPAQUE };

        va_start(args, fmt);
        int n = vsnprintf(buf, sizeof (buf), fmt, args);
        va_end(args);

        if (n < 0) {
                perror("vsnprintf");
                return;
        }

        SDL_Surface *surface;
        if (!(surface = TTF_RenderText_Solid(font, buf, fg))) {
                fprintf(stderr, "TTF_RenderText_Solid: %s\n", TTF_GetError());
                return;
        }

        SDL_Texture *texture;
        if (!(texture = SDL_CreateTextureFromSurface(renderer, surface))) {
                fprintf(stderr, "SDL_CreateTextureFromSurface: %s\n",
                        SDL_GetError());
                goto freesurface;
        }

        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);

        SDL_Rect dest = { 0, 0, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_RenderPresent(renderer);

        SDL_DestroyTexture(texture);

freesurface:
        SDL_FreeSurface(surface);
}

void font_test(SDL_Renderer * renderer)
{
        TTF_Font *font;
        const char *TEST_FONT = "zoftfrakt-eye-fs.ttf";

        if (!(font = TTF_OpenFont(TEST_FONT, 18))) {
                fprintf(stderr, "TTF_OpenFont(%s): %s\n", TEST_FONT,
                        TTF_GetError());
                return;
        }

        _font_print_info(font);
        _font_printf(renderer, font, "solid %s", TEST_FONT);
        SDL_Delay(5000);

closefont:
        TTF_CloseFont(font);
}
