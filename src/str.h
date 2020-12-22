/*
 * str.h - interface to string utilities
 *
 * Copyright (C) 2019 Gordon McNutt
 */

#ifndef str_h
#define str_h

/* Create a string via printf-like syntax. Free using mem_deref(). */
char *str_printf(const char *fmt, ...);

/* Allocate a copy, free using mem_deref(). */
char *str_copy(const char *s);

#endif
