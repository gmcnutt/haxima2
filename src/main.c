/* 
 *  haxima2 - main.c
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
#include <config.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "font.h"

void on_keydown(SDL_KeyboardEvent * event, int *quit)
{
        switch (event->keysym.sym) {
        case SDLK_q:
                *quit = 1;
                break;
        default:
                break;
        }
}

int main()
{
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;


        printf("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);

        /* Init SDL */
        if (SDL_Init(SDL_INIT_VIDEO)) {
                fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
                return -1;
        }

        /* Cleanup SDL on exit. */
        atexit(SDL_Quit);


        /* Create the main window */
        if (!(window = SDL_CreateWindow(PACKAGE_STRING, SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 640 * 2,
                                        480 * 2,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)))
        {
                fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
                return -1;
        }

        /* Create the renderer. */
        if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
                fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
                goto destroy_window;
        }

        SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        font_init();
        font_test(renderer);

        SDL_Event event;
        int quit = 0;
        while (!quit) {
                while (SDL_PollEvent(&event)) {
                        switch (event.type) {
                        case SDL_QUIT:
                                quit = 1;
                                break;
                        case SDL_KEYDOWN:
                                on_keydown(&event.key, &quit);
                                break;
                        default:
                                break;
                        }
                }
        }


        SDL_DestroyRenderer(renderer);

destroy_window:
        SDL_DestroyWindow(window);

        return 0;
}
