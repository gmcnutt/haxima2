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
#include "mem.h"
#include "panic.h"
#include "str.h"

/* Common macros */


/* Globals */
extern SDL_Window *window;
extern SDL_Renderer *renderer;
