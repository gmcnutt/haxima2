/* String handling utilities. */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "mem.h"
#include "str.h"


char *str_printf(const char *fmt, ...)
{
        va_list argp;
        char c, *str;
        size_t len;

        /* Probe the length using vsnprintf, knowing it will return the number
         * of chars it failed to write. */
        va_start(argp, fmt);
        len = vsnprintf(&c, 1, fmt, argp);
        va_end(argp);

        if (len < 1) {
                return NULL;
        }
        len += 2;               /* One for c, one for null */

        /* Now alloc a buffer big enough and do it again. */
        if (!(str = mem_alloc(len, NULL))) {
                return NULL;
        }

        va_start(argp, fmt);
        vsnprintf(str, len, fmt, argp);
        va_end(argp);

        return str;
}


char *str_copy(const char *s)
{
        size_t n = strlen(s);
        char *c = mem_alloc(n + 1, NULL);
        strcpy(c, s);
        return c;
}
