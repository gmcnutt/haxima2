/* 
 *  main.c -- program entry point for haxima2 and utilities
 *
 * Copyright 2020 Gordon McNutt
 */

#include "haxima2.h"

#include "font_command.h"
#include "play_command.h"

/* Globals initialized here */
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

/* Typedef function pointer for subcommands. */
typedef void (*command_t)(int, char**);


static void _print_help()
{
        /* *INDENT-OFF* */
        printf("Usage: %s [options] [command]\n"
               "Options: \n" "  -h: help\n" "  -v: version\n"
               "Commands: font play[default]\n"
               , PACKAGE_NAME);
        /* *INDENT-ON* */
}


int main(int argc, char **argv)
{

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

        /* The default command is to play the game. */
        const char *cmd = "play";
        if (optind != argc) {
                cmd = argv[optind];
        }

        /* Check if the user specified a different command and verify it. */
        command_t command = NULL;
        if (!strcmp(cmd, "play")) {
                command = play_command_exec;
        } else if (!strcmp(cmd, "font")) {
                command = font_command_exec;
        } else {
                fprintf(stderr, "%s: invalid command -- %s\n", PACKAGE_NAME,
                        cmd);
                exit(EXIT_FAILURE);
        }

        /* Init SDL */
        if (SDL_Init(SDL_INIT_VIDEO)) {
                panic("SDL_Init: %s\n", SDL_GetError());
        }

        /* Cleanup SDL on exit. */
        atexit(SDL_Quit);

        
        /* Create the main window */
        if (!(window = SDL_CreateWindow(
                      PACKAGE_STRING,
                      SDL_WINDOWPOS_UNDEFINED,
                      SDL_WINDOWPOS_UNDEFINED,
                      640 * 2,
                      480 * 2,
                      SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN)))
        {
                panic("SDL_CreateWindow: %s\n", SDL_GetError());
        }

        /* Create the renderer. */
        if (!(renderer = SDL_CreateRenderer(window, -1, 0))) {
                panic("SDL_CreateRenderer: %s\n", SDL_GetError());
        }

        font_sys_init();
        
        
        /* Clear the screen */
        SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                               SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
        
        /* Run the player command. */
        argc -= optind;
        argv = &argv[optind];
        optind = 1;
        command(argc, argv);        
}
