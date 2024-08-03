/**
 * haxima2.h -- globals, common includes and macros
 *
 * Copyright 2020 Gordon James McNutt
 *
 */


#pragma once

/* Common system includes */
#include <SDL2/SDL.h>
#include <config.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

/* Common application includes */
#include "font.h"
#include "mem.h"
#include "panic.h"
#include "str.h"

/* Common macros */
#define max(a,b)               \
  ({ __typeof__ (a) _a = (a);  \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b)               \
  ({ __typeof__ (a) _a = (a);  \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })


/* Constants */
extern const SDL_Color BLACK;
extern const SDL_Color BLUE;
extern const SDL_Color GREEN;
extern const SDL_Color CYAN;
extern const SDL_Color RED;
extern const SDL_Color MAGENTA;
extern const SDL_Color YELLOW;
extern const SDL_Color WHITE;


/* Globals */
extern SDL_Window *window;
extern SDL_Renderer *renderer;
