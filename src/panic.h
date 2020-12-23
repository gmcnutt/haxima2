/**
 * panic.h -- fatal error handler
 *
 * Copyright 2020 Gordon James McNutt
 *
 */


#pragma once

/* Print a message to stderr and exit with a failure code. */
void panic(const char *fmt, ...);
