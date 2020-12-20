/*
 * haxima2 - asprintf.c
 *
 *  Copyright 2020 Gordon James McNutt
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/* Based on code from "21st Century C" by Ben Klemens */

#include "asprintf.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>


int asprintf(char **str, const char *fmt, ...)
{
        /* Use vsnprintf to test the length of the final string. */
        va_list args;
        va_start(args, fmt);
        char buf[1];
        int n = vsnprintf(buf, 1, fmt, args);
        if (n < 1) {
                return n;
        }
        va_end(args);

        /* Allocate a string that size (plus one for the NULL). */
        if (!(*str = malloc(n + 1))) {
                return -1;
        }

        /* Now vsnprintf for real into the allocated string. */
        va_start(args, fmt);
        n = vsnprintf(*str, n + 1, fmt, args);
        va_end(args);

        return n;
}

int vasprintf(char **str, const char *fmt, va_list ap)
{
        va_list args;
        va_copy(args, ap);

        char buf[1];
        int n = vsnprintf(buf, 1, fmt, args);
        if (n < 1) {
                return n;
        }
        va_end(args);

        /* Allocate a string that size (plus one for the NULL). */
        if (!(*str = malloc(n + 1))) {
                return -1;
        }

        /* Now vsnprintf for real into the allocated string. */
        va_copy(args, ap);
        n = vsnprintf(*str, n + 1, fmt, args);
        va_end(args);

        return n;
}
