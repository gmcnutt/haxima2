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

#include "font.h"


int font_init(void)
{
        if (TTF_Init()) {
                fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
                return -1;
        }
        atexit(TTF_Quit);
        return 0;
}

void font_printf(SDL_Renderer * renderer, TTF_Font * font, const char *fmt,
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
        if (!(surface = TTF_RenderText_Blended(font, buf, fg))) {
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

