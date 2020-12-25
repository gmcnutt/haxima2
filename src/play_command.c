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

bool exit_on_keypress(SDL_Event * event)
{
        return (event->type == SDL_KEYDOWN);
}

bool main_menu_event_cb(SDL_Event * event)
{
        if (event->type == SDL_KEYDOWN) {
                if (event->key.keysym.sym == SDLK_q) {
                        return true;
                }
        }
        return false;
}

void run_event_loop(bool(*cb) (SDL_Event * event))
{
        /* Loop until user exits */
        SDL_Event event;
        while (SDL_WaitEvent(&event)) {
                if (event.type == SDL_QUIT) {
                        exit(EXIT_SUCCESS);
                }
                if (cb(&event)) {
                        break;
                }
        }
}


void clear_screen(SDL_Color color)
{
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
}

void splash()
{
        clear_screen(BLACK);
        font_t *font = font_open(MAIN_MENU_FONT_PATH, SPLASH_FONT_PTSIZE);
        font_set_fgcolor(font, CYAN);
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

        run_event_loop(exit_on_keypress);
}

void main_menu()
{
        clear_screen(BLACK);

        /* Render the first option centered below. */
        font_t *font = font_open(MAIN_MENU_FONT_PATH, MAIN_MENU_FONT_PTSIZE);
        font_set_fgcolor(font, YELLOW);
        const char *text = "New Game";
        SDL_Texture *texture = font_render(font, text);
        int rw, rh;
        SDL_GetRendererOutputSize(renderer, &rw, &rh);
        int tw, th;
        SDL_QueryTexture(texture, NULL, NULL, &tw, &th);
        SDL_Rect dest = { (rw - tw) / 2, 0, tw, th };
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);

        /* Render the quit option centered below. */
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

        run_event_loop(main_menu_event_cb);

}

void play_command_exec(int argc, char **argv)
{
        (void)argc;             /* suppress `unused parameter` */
        (void)argv;             /* suppress `unused parameter` */

        splash();
        main_menu();
}
