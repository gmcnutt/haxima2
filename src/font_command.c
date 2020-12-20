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

static const SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color BLUE = { 0, 0, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color GREEN = { 0, 255, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color CYAN = { 0, 255, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color RED = { 255, 0, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color MAGENTA = { 255, 0, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color YELLOW = { 255, 255, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color WHITE = { 255, 255, 255, SDL_ALPHA_OPAQUE };

static int render_at(SDL_Renderer * renderer, font_t * font, const char *text,
                     int x, int y)
{
        SDL_Texture *texture = font_render(font, renderer, text);
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect dest = { x, y, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        return w;
}

static int font_test(SDL_Renderer * renderer, const char *font_file)
{
        font_t *font;

        if (!(font = font_open(font_file, 18))) {
                return -1;
        }

        font_print_info(font);

        int lineheight = font_get_height(font) + 1;
        int space = font_get_glyph_width(font, ' ');
        int y = 0;
        int x = 0;

        /* Show blended mode */
        char *text = str_printf("blended %s", font_file);
        x += render_at(renderer, font, text, x, y);
        x += space;
        font_set_fgcolor(font, RED);
        x += render_at(renderer, font, text, x, y);
        x += space;
        y += lineheight;
        mem_deref(text);

        /* Show solid mode */
        x = 0;
        font_set_render_method(font, FONT_SOLID);
        text = str_printf("solid %s", font_file);
        font_set_fgcolor(font, GREEN);
        x += render_at(renderer, font, text, x, y);
        x += space;
        font_set_fgcolor(font, BLUE);
        x += render_at(renderer, font, text, x, y);
        x += space;
        y += lineheight;
        mem_deref(text);

        /* Show shaded mode */
        x = 0;
        font_set_render_method(font, FONT_SHADED);
        font_set_fgcolor(font, CYAN);
        font_set_bgcolor(font, BLACK);
        text = str_printf("shaded %s", font_file);
        x += render_at(renderer, font, text, x, y);
        x += space;
        y += lineheight;
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
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
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
