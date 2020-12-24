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
const int SPLASH_FONT_PTSIZE = 128;
const int MAIN_MENU_FONT_PTSIZE = 64;

void clear_screen()
{
        SDL_SetRenderDrawColor(renderer, 255, 255, 255,
                               SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
}

void splash()
{
        clear_screen();
        font_t *font = font_open(MAIN_MENU_FONT_PATH, SPLASH_FONT_PTSIZE);
        const char *text = "Haxima2";
        SDL_Texture *texture = font_render(font, text);
        int rw, rh;
        SDL_GetRendererOutputSize(renderer, &rw, &rh);
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        SDL_Rect dest = { (rw - tw) / 2, (rh - th) / 2, tw, th };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
        mem_deref(font);

        SDL_RenderPresent(renderer);

        /* Loop until user presses a button */
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        exit(EXIT_SUCCESS);
                }
                if (event.type == SDL_KEYDOWN) {
                        break;
                }
        }
        
}

void main_menu()
{
        clear_screen();
        
        /* Render the first option centered below. */
        font_t *font = font_open(MAIN_MENU_FONT_PATH, MAIN_MENU_FONT_PTSIZE);
        const char *text = "New Game";
        SDL_Texture *texture = font_render(font, text);
        int rw, rh;
        SDL_GetRendererOutputSize(renderer, &rw, &rh);
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        SDL_Rect dest = { (rw - tw) / 2, 0, tw, th };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
        mem_deref(font);

        /* Render the quit option centered below. */
        font = font_open(MAIN_MENU_FONT_PATH, MAIN_MENU_FONT_PTSIZE);
        text = "Quit";
        texture = font_render(font, text);
        dest.y += th;
        SDL_GetRendererOutputSize(renderer, &rw, &rh);
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        dest.x = (rw - tw) / 2;
        dest.w = tw;
        dest.h = th;
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
        mem_deref(font);
        
        SDL_RenderPresent(renderer);

        
        /* Loop until user exits */
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        exit(EXIT_SUCCESS);
                }
                if (event.type == SDL_KEYDOWN) {
                        if (event.key.keysym.sym == SDLK_q) {
                                break;
                        }
                }
        }
        
}

void play_command_exec(int argc, char **argv)
{
        splash();
        main_menu();
}
