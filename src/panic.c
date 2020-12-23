/**
 * panic.c -- fatal error handler
 *
 * Copyright 2020 Gordon James McNutt
 *
 */


#include "panic.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


void panic(const char *fmt, ...)
{
        va_list ap;
        va_start(ap, fmt);
        vfprintf(stderr, fmt, ap);
        va_end(ap);
        exit(EXIT_FAILURE);
}
