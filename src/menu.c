/**
 * menu.c -- a gui menu
 *
 * Copyright 2020 Gordon James McNutt
 *
 */

#if 0

#include "menu.h"

#include <stdbool.h>

#include "list.h"
#include "mem.h"
#include "str.h"

struct menu {
        char *title;
        struct list options;
        bool hidden;
};

struct menu_option {
        struct list sibs;
        const char *title;
        menu_cb_t cb;
        void *data;
};


static void _menu_fin(void *ptr)
{
        menu_t *menu = (menu_t *) ptr;
        if (menu->title) {
                mem_deref(menu->title);
        }
        /* TODO cleanup options */
}


menu_t *menu_alloc(const char *title)
{
        menu_t *menu = mem_alloc(sizeof (menu_t), _menu_fin);
        list_init(&menu->options);
        menu->title = str_copy(title);
        return menu;
}

void menu_add_option(menu_t * menu, const char *title, menu_cb_t cb, void *data)
{
        /* TODO */
}

void menu_hide(menu_t * menu)
{
        /* TODO */
}

void menu_show(menu_t * menu)
{
        /* TODO */
}

void menu_on_mouse_click(menu_t * menu, int x, int y)
{
        /* TODO */
}

void menu_on_mouse_move(menu_t * menu, int x, int y)
{
        /* TODO */
}

#endif
