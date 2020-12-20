/*
 * font_command.c -- font inspector
 *
 * Copyright 2020 Gordon James McNutt
 */


#include "font_command.h"

#include <config.h>
#include <unistd.h>

#include "font.h"


static void font_print_info(TTF_Font * font)
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
        printf("fixedwidth: %s\n", TTF_FontFaceIsFixedWidth(font) ? "yes" : "no");

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

static int font_test(SDL_Renderer * renderer, const char *font_file)
{
        TTF_Font *font;

        if (!(font = TTF_OpenFont(font_file, 18))) {
                fprintf(stderr, "TTF_OpenFont(%s): %s\n", font_file, TTF_GetError());
                return -1;
        }

        font_print_info(font);
        font_printf(renderer, font, "solid %s", font_file);

        TTF_CloseFont(font);

        return 0;
}


int font_command_exec(int argc, char **argv)
{
        /* Evaluate the command-line args. */
        static const char *help = "Usage: haxima2 font [options] <font-file>\n" "Options:\n" " -h: help\n";
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
                                        SDL_WINDOWPOS_UNDEFINED, 640 * 2, 480 * 2, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN))) {
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
        font_init();
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
