/**
 * play_command.c -- command to start and run the game
 *
 * Copyright 2020 Gordon James McNutt
 *
 */

#include "haxima2.h"
#include "play_command.h"
#include <SDL2/SDL_ttf.h>

void play_command_exec(int argc, char **argv)
{
        char *path = "";
        TTF_Font *ttf = TTF_OpenFont(path, 18);
        if (!ttf) {
                panic("TTF_OpenFont(%s): %s\n", path, TTF_GetError());
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

        TTF_CloseFont(ttf);
}
