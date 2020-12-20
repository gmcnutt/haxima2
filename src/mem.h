/*
 * mem.h - interface to memory allocator
 *
 * A generic reference-counting memory allocator with support for
 * destructors.
 *
 * Copyright (C) 2016 Gordon McNutt
 */

#ifndef mem_h
#define mem_h

#include <stddef.h>

/**
 * Allocate a chunk of memory with an optional destructor and a refcount.
 *
 * If 'fin' is not NULL it will be called on the final mem_deref. The returned
 * chunk will have a refcount of 1.
 */
void *mem_alloc(size_t size, void (*fin) (void *));

/**
 * Increment the refcount on a chunk.
 */
void mem_ref(void *ptr);

/**
 * Decrement the refcount on a chunk.
 *
 * When the refcount falls to zero the chunk will be deallocated.
 */
void mem_deref(void *ptr);

/**
 * Get the number of references on a chunk.
 */
size_t mem_get_refs(void *ptr);

/**
 * Set a function to call when an error occurs.
 */
void mem_set_err_handler(void (*handler) (size_t size));

/**
 * Return a string describing the last error.
 */
const char *mem_get_error(void);

#endif
