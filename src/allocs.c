/********************************************************************
*
* BikeDemo - Exercise Bike Demonstration
* Scenery
*
* allocs.c - function implementations for
* memory allocation and deallocation.
*
* Copyright (C) 2006-2014, Russell Potter.
* All rights reserved.
*
********************************************************************/

#include <bikedemo/allocs.h>
#include <stdlib.h>

/* "bd_alloc" - allocate, using the allocator "allocs",
 * *the amount of memory given in "amt".  Allocation errors
 * are logged to "logger" */
void *bd_alloc(struct bd_allocs *allocs, /* allocator to use */
		      size_t amt,                /* amount to allocate */
		       struct bd_logger *logger) /* logger to use in
                                          * case of allocation
                                          * failure */
{
    /* only use the allocator to allocate the memory if it
     * is non-NULL. In the case in which it is NULL, use
     * "malloc" to allocate instead */
    if (allocs)
        return allocs->alloc(allocs, amt, logger);
    else
        return malloc(amt);
}

/* "bd_free" - de-allocate, using the allocator
 * "allocs", the data pointer "mem" */
void bd_free(struct bd_allocs *allocs, /* allocator
                                        * to use */
             void *mem)                /* memory block
                                        * to de-alloc-
                                        * ate */
{
    /* only use the allocator to free memory only
     * non-NULL. In that case it is NULL, use the
     * default "free" to de-allocate instead */
    if (allocs)
        allocs->dealloc(allocs, mem);
    else
        free(mem);
}
