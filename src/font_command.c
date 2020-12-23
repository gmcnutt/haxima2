/*
 * font_command.c -- font inspector
 *
 * Copyright 2020 Gordon James McNutt
 */

#include "haxima2.h"
#include "font_command.h"
#include "font.h"

static const SDL_Color BLACK = { 0, 0, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color BLUE = { 0, 0, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color GREEN = { 0, 255, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color CYAN = { 0, 255, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color RED = { 255, 0, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color MAGENTA = { 255, 0, 255, SDL_ALPHA_OPAQUE };
static const SDL_Color YELLOW = { 255, 255, 0, SDL_ALPHA_OPAQUE };
static const SDL_Color WHITE = { 255, 255, 255, SDL_ALPHA_OPAQUE };

static int render_at(font_t * font, const char *text,
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

static void font_test(const char *font_file, int ptsize)
{
        font_t *font;

        font = font_open(font_file, ptsize);

        font_print_info(font);

        int lineheight = font_get_height(font) + 1;
        int space = font_get_glyph_width(font, ' ');
        int y = 0;
        int x = 0;

        /* Show blended mode */
        char *text = str_printf("blended %s", font_file);
        x += render_at(font, text, x, y);
        x += space;
        font_set_fgcolor(font, RED);
        x += render_at(font, text, x, y);
        x += space;
        y += lineheight;
        mem_deref(text);

        /* Show solid mode */
        x = 0;
        font_set_render_method(font, FONT_SOLID);
        text = str_printf("solid %s", font_file);
        font_set_fgcolor(font, GREEN);
        x += render_at(font, text, x, y);
        x += space;
        font_set_fgcolor(font, BLUE);
        x += render_at(font, text, x, y);
        x += space;
        y += lineheight;
        mem_deref(text);

        /* Show shaded mode */
        x = 0;
        font_set_render_method(font, FONT_SHADED);
        font_set_fgcolor(font, CYAN);
        font_set_bgcolor(font, BLACK);
        text = str_printf("shaded %s", font_file);
        x += render_at(font, text, x, y);
        x += space;
        y += lineheight;
        mem_deref(text);

        mem_deref(font);
}


void font_command_exec(int argc, char **argv)
{
        /* Evaluate the command-line args. */
        static const char *help =
            "Usage: haxima2 font [options] <font-file>\n" "Options:\n"
            " -s: point size\n"
            " -h: help\n";
        int c, ptsize = 18;
        while ((c = getopt(argc, argv, "hs:")) != -1) {
                switch (c) {
                case '?':
                        printf("%s", help);
                        exit(EXIT_FAILURE);
                case 's':
                        ptsize = atoi(optarg);
                        break;
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


        /* Run the font test */
        font_sys_init();
        font_test(font_file, ptsize);

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
}
