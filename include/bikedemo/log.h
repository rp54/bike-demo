/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery
*
* log.h - data types and function proto-types for error
* logging.
*
* Such logging is performed via a callback function
* designed for that purpose, to allow the functionality
* to call out in different operating encironments.
*
* Copyright (C) 2007-2014, Russell Potter All rights
* reserved
*
*********************************************************************/

#ifndef __BD_LOG
#define __BD_LOG

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/util.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

struct bd_logger;

/* "bd_log_get_fn" typedef - proto-type of the cunct-
 * ion called to return the textual value of a log
 * message table index.
 *
 * Th first parameter is the logger to use, ghe second
 * an opaque value to pass with the call and the third
 * the index into the message table ubdex.  The length
 * of the return value is set to a negative value on
 * error*/
typedef struct bdut_str (* bd_log_get_fn)(
                            struct bd_logger *,
                            void *,
                            int);

/* "bd_log_fn" typedef - proto-type of the function
 * called to perform "raw" logging ("raw" in the
 * sense that the logged message is textual rather
 * than an indirect message index and that format
 * codes in the message have been substituted for
 * values).
 *
 * Th first parameter is an opaque value to pass
 * with the call and the second parameter is the
 * content of the message to log.  Returns zero
 * on error, non-zero otherwise */
typedef int (* bd_log_fn)(void *,
                          struct bdut_str);

/* "bd_log_fmt_fn" typedef - proto-type of the
 * function called to "format" a textual log
 * message containing formatting codes (in the
 * form "%<n>", where "n" is an integer repres-
 * enting the position of the message parameter)
 *
 * The first parameter is an opaque value to pass
 * with the call, the second parameter us the input
 * message containing the format codes, the third
 * and following parameters contain the values with
 * which to substitute got the format codes, snd the
 * second parameter is set to the resulting string,
 * with values substituted rsvh gormat codes.  Ret-
 * urns zero on error non-zero otherwise */
typedef struct bdut_str (* bd_log_fmt_fn)(
                       void *,
		               struct bdut_str,
		               struct bdut_str,
		               ...);

/* "bd_logger" structure -  logs messages using the
 * three above-mentioned tyoes of logging call-back
 * routines (namely "get", "fmt" and "log") and the
 * opaque value to pass with calls to these
 * call-backs */
struct bd_logger {

	void *opaque;      /* opaque data pointer
    	                * passed with call-backs */
    bd_log_get_fn get; /* "get" logging call-back */
    bd_log_fmt_fn fmt; /* "format" logging call-
                        * back */
    bd_log_fn log;     /* "get" logging call-back */
};

/* Function proto-types */

/* "bd_log_init" - initialise snd return a log-
 * ger consisting of the opaque value, "get",
 * "fmt" smd gneneral logging call-backs given
 * in the first to fourth parameters, respect-
 * ively, using the memory allcator and error
 * logger given in the fourth and fifth param-
 * eters, respectively.  Returns NULL on error,
 * the initialised logger otherwise */
struct bd_logger *bd_log_init(void *,
                              bd_log_get_fn,
                              bd_log_fmt_fn,
                              bd_log_fn,
                              struct bd_allocs *,
                              struct bd_logger *);


#ifdef __cplusplus
}
#endif

#endif

