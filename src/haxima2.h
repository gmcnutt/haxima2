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


/* Globals */
extern SDL_Window *window;
extern SDL_Renderer *renderer;
