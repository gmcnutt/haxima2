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
#include <unistd.h>

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

static void _print_help()
{
        printf("Usage: %s [options]\n"
               "Options: \n" "  -h: help\n" "  -v: version\n", PACKAGE_NAME);
}

int font_command_exec(int argc, char **argv)
{
        /* Evaluate the command-line args. */
        static const char *help = "Usage: haxima2 font [options] <font-file>\n"
            "Options:\n" " -h: help\n";
        int c;
        while ((c = getopt(argc, argv, "h")) != -1) {
                switch (c) {
                case '?':
                        printf("%s", help);
                        exit(EXIT_FAILURE);
                case 'h':
                default:
                        printf("%s", help);
                        exit(EXIT_SUCCESS);
                        break;
                }
        }

        /* getopt() resets optind to 1 when it matches argc */
        if (optind == argc) {
                printf("%s", help);
                exit(EXIT_FAILURE);
        }

        const char *font_file = argv[optind];

        /* Init SDL */
        if (SDL_Init(SDL_INIT_VIDEO)) {
                fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
                return -1;
        }

        /* Cleanup SDL on exit. */
        atexit(SDL_Quit);


        /* Create the main window */
        SDL_Window *window = NULL;
        if (!(window = SDL_CreateWindow(PACKAGE_STRING, SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, 640 * 2,
                                        480 * 2,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)))
        {
                fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
                return -1;
        }

        /* Create the renderer. */
        SDL_Renderer *renderer = NULL;
        if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
                fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
                goto destroy_window;
        }

        SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        font_init();
        if (font_test(renderer, font_file)) {
                goto destroy_renderer;
        }

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

destroy_renderer:
        SDL_DestroyRenderer(renderer);

destroy_window:
        SDL_DestroyWindow(window);

        return 0;
}

int main(int argc, char **argv)
{
        printf("%s %s\n", PACKAGE_NAME, PACKAGE_VERSION);

        /* Evaluate the command-line args. */
        int c;
        while ((c = getopt(argc, argv, "+vh")) != -1) {
                switch (c) {
                case 'v':
                        printf("%s\n", PACKAGE_VERSION);
                        break;
                case '?':
                        _print_help();
                        exit(EXIT_FAILURE);
                case 'h':
                default:
                        _print_help();
                        exit(EXIT_SUCCESS);
                        break;
                }
        }

        if (optind == argc) {
                exit(EXIT_SUCCESS);
        }

        const char *cmd = argv[optind];
        if (!strcmp(cmd, "font")) {
                argc -= optind;
                argv = &argv[optind];
                optind = 1;
                font_command_exec(argc, argv);
        } else {
                fprintf(stderr, "%s: invalid command -- %s\n", PACKAGE_NAME,
                        cmd);
                exit(EXIT_FAILURE);
        }

}
