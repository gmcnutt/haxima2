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

#include "font_command.h"

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
                fprintf(stderr, "%s: invalid command -- %s\n", PACKAGE_NAME, cmd);
                exit(EXIT_FAILURE);
        }

}
