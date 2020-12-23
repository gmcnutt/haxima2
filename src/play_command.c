/**
 * play_command.c -- command to start and run the game
 *
 * Copyright 2020 Gordon James McNutt
 *
 */

#include "haxima2.h"
#include "play_command.h"
#include "font.h"

const char *MAIN_MENU_FONT_PATH = DATADIR "/" PACKAGE "/avatar.ttf";
const int MAIN_MENU_FONT_PTSIZE = 128;

void play_command_exec(int argc, char **argv)
{
        /* Open the font we want */
        font_t *font = font_open(MAIN_MENU_FONT_PATH, MAIN_MENU_FONT_PTSIZE);

        const char *text = "Haxima2";
        SDL_Texture *texture = font_render(font, text);
        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        SDL_Rect dest = { 0, 0, w, h };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
        
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

        mem_deref(font);
}
