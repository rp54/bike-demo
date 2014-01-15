/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* allocs.h - data types and function prototypes for memory
* allocation sand de-allocation.
*
* Such allocation and de-allocation is performed via call-back
* functions designed for that purpose, to allow these functions
* to call out, where appropriate, in different operating
* environments, and for a debug version of the call-backs to
* track memory usage
*
* Copyright (C) 2007-2012, Russell Potter
* All rights reserved
*
*********************************************************************/

#ifndef __BD_ALLOCS
#define __BD_ALLOCS

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <bikedemo/log.h>

struct bd_allocs;
struct bd_logger;


/* bd_allocs" structure - a memry "allocator", conta-
 * ining call-backs to allocate and de-allocate blocks
 * of memory nd an associated opaque value to pass with
 * those calls */
struct bd_allocs {

    void *opaque;                        /* parameter
                                          * pointer pas-
                                          * sed with r-
                                          * outines */

    void *(* alloc)(struct bd_allocs *,  /* allocation */
                    size_t,              /* callback */
                    struct bd_logger *);

    void (* dealloc)(struct bd_allocs *, /* deallocation */
                     void *);            /* call-back */
};

/*** function declarations ***/

/* "bd_alloc" - allocate, using the allocator
 * given in the first parameter, the number of
 * bytes given in the second parameter.   Alloc=
 * ation errors are logged to the logger given
 * in the third
 * parameter */
void *bd_alloc(struct bd_allocs *,
		       size_t,
		       struct bd_logger *);

/* "bd_free" - deallocate, using the allocator
 * given in the first parameter, the memory
 * block pointer given in the second parameter */
void bd_free(struct bd_allocs *,
		     void *);

#ifdef __cplusplus
}
#endif

#endif
