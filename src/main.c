/* 
 *  main.c -- program entry point for haxima2 and utilities
 *
 * Copyright 2020 Gordon McNutt
 */
#include <config.h>
#include <stdio.h>
#include <unistd.h>

#include <SDL2/SDL.h>

#include "font_command.h"
#include "play_command.h"

typedef void (*command_t)(int, char**);

static void _print_help()
{
        /* *INDENT-OFF* */
        printf("Usage: %s [options] [command]\n"
               "Options: \n" "  -h: help\n" "  -v: version\n"
               "Commands:\n"
               "  font\n"
               , PACKAGE_NAME);
        /* *INDENT-ON* */
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

        /* Do common initialization. */
        //platform_init();
        //font_sys_init();

        /* Run the player command. */
        argc -= optind;
        argv = &argv[optind];
        optind = 1;
        command(argc, argv);
        
}
