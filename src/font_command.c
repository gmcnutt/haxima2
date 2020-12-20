/*
 * font_command.c -- font inspector
 *
 * Copyright 2020 Gordon James McNutt
 */


#include "font_command.h"

#include <config.h>
#include <unistd.h>

#include "font.h"
#include "mem.h"
#include "str.h"


static int font_test(SDL_Renderer * renderer, const char *font_file)
{
        font_t *font;

        if (!(font = font_open(font_file, 18))) {
                return -1;
        }

        font_print_info(font);

        /* Use dest to track the "cursor", initialize to renderer output
         * dimensions. */
        SDL_Rect dest;
        if (SDL_GetRendererOutputSize(renderer, &dest.w, &dest.h)) {
                fprintf(stderr, "SDL_GetRendererOutputSize: %s",
                        SDL_GetError());
                return -1;
        }

        /* Show blended mode */
        int lineheight = font_get_height(font) + 1;
        char *text = str_printf("blended %s", font_file);
        font_render(font, renderer, &dest, text);
        mem_deref(text);

        /* Advance the line */
        dest.y += lineheight;
        dest.h -= lineheight;

        /* Show solid mode */
        text = str_printf("solid %s", font_file);
        font_set_render_method(font, FONT_SOLID);
        font_render(font, renderer, &dest, text);
        mem_deref(text);

        /* Advance the line */
        dest.y += lineheight;
        dest.h -= lineheight;

        /* Show shaded mode */
        text = str_printf("shaded %s", font_file);
        font_set_render_method(font, FONT_SHADED);
        SDL_Color bg = { 200, 200, 200, SDL_ALPHA_OPAQUE };
        font_set_bgcolor(font, &bg);
        font_render(font, renderer, &dest, text);
        mem_deref(text);

        mem_deref(font);

        return 0;
}


int font_command_exec(int argc, char **argv)
{
        /* Evaluate the command-line args. */
        static const char *help =
            "Usage: haxima2 font [options] <font-file>\n" "Options:\n"
            " -h: help\n";
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

        /* Clear the screen */
        SDL_SetRenderDrawColor(renderer, 128, 128, 128, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        /* Run the font test */
        font_sys_init();
        if (font_test(renderer, font_file)) {
                goto destroy_renderer;
        }

        /* Loop until user exits */
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        break;
                }
                if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_q) {
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
