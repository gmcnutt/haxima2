/**
 * common.h -- standard macros and utilities
 *
 * Copyright 2020 Gordon James McNutt
 *
 */


#pragma once

/* See https://stackoverflow.com/questions/3437404/min-and-max-in-c for why
 * these look so complicated. */
#define max(a,b)               \
  ({ __typeof__ (a) _a = (a);  \
      __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b)               \
  ({ __typeof__ (a) _a = (a);  \
      __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })
