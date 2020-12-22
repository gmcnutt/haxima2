/**
 * menu.h -- a gui menu
 *
 * Copyright 2020 Gordon James McNutt
 *
 */


#pragma once

typedef struct menu menu_t;

typedef int (*menu_cb_t)(menu_t *menu, void *data);

/* Create a menu, free using mem_deref(). */
menu_t *menu_alloc(const char *title);

/* Add a menu option that will invoke the callback with the data. The title
 * will be copied. */
void menu_add_option(menu_t *menu, const char *title, menu_cb_t cb, void *data);

/* Hide or show the menu, making it active or deactive. */
void menu_hide(menu_t *menu);
void menu_show(menu_t *menu);

/* Pass a click event to the menu. (x, y) are relative to the menu origin. */
void menu_on_mouse_click(menu_t *menu, int x, int y);
void menu_on_mouse_move(menu_t *menu, int x, int y);
