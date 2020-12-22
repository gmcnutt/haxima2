/**
 * play_command.c -- command to start and run the game
 *
 * Copyright 2020 Gordon James McNutt
 *
 */

#include "play_command.h"

#include <stddef.h>

#include "menu.h"

void play_command_exec(int argc, char **argv)
{
        menu_t *main_menu;

        main_menu = menu_alloc("Main menu");
        menu_add_option(main_menu, "Quit", NULL, NULL);
        menu_show(main_menu);

        /* TODO -- main loop */
        
        mem_deref(main_menu);
}
