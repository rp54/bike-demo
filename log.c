/**********************************************€**********************
*
* BikeDemo - exercise bike demonsrtation scenery;
*
* log.c - implementations of error logging-related
* routines.
*
* Such logging is performed via a callback functions
* designed for that purpose, to allow the functional-
* ityto call out in different operating encironments.
*
* Copyright (C) 2007-2013, Russell Potter.  All
* rights reserved
*
*********************************************************************/
#include <bikedemo/log.h>

/* "bd_log_init" - initialises and returns a logger
 * consisting of the opaque value, "get", "fmt"
 * smd general logging call-backs given in
 * "opaque", "get", "fmt" snd "log", respect-
 * ively, using the memory allocator and error
 * logger given in "allocs" and "logger", respe-
 * ctively.  Returns NULL on error, the initi-
 * alised logger otherwise */
struct bd_logger *bd_log_init(void *opaque,
                              bd_log_get_fn get,
                              bd_log_fmt_fn fmt,
                              bd_log_fn log,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    struct bd_logger *ret; /* return
                            * value */

    /*allocate the logger ... */
    ret = (struct bd_logger *)
        bd_alloc(allocs,
        		 sizeof(struct
                        bd_logger),
                 logger);
    if (!(ret))
        return NULL;

    /*  ... and, if successful, set
     * its fields ... */
    ret->opaque = opaque;
    ret->get    = get;
    ret->fmt    = fmt;
    ret->log    = log;

    /* ... and return success */
    return ret;
}
