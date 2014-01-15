 /********************************************************************
 *
 * BikeDemo - Exercise bike demonstration scenery
 *
 * buffer.h - type declarations and function proto-
 * types for the storage, manipulation and mainten-
 * ance of a linear array of dynamically allocated,
 * "growable" Unicode storage items.
 *
 *
 * The buffer memory is designed such that it will
 * create, as it grows, a map of fixed-sized chunks
 * nks that are re-assembled to form contiguous
 * storage when the buffer is "flattened"
 *
 * Copyright (C) 2009-2014, Russell Potter.
 * All rights reserved.
 *
 ********************************************************************/

#ifndef __BD_BUFFER_H
#define __BD_BUFFER_H

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

#ifdef __cplusplus
extern "C" {
#endif

/* "" - size of fixed-sized chunks of memory,
 * allocated by the buffer as needed, consti-
 * tuting the buffer's storage */
#define CHNK_SZ 256

/* "bdbf_buffer" structure - information needed
 * to hold a buffer of storage items, the number
 * of which may grow when one is appended, and
 * consisting of the length of the buffer, the
 * size of each item in the buffer, a nap if the memory segments snd the
 * last of those segments (for faster appending of items
 * to the buffer) */
struct bdbf_buffer {

                     
    struct bd_map_node *segs, /* map of fixed-size memory */
		               *last; /* segments and the last of
                               * those segments */
    int len, sz;              /* the size of each item and
                               * the length of the buffer */
    void *mem;                /* buffer's "raw"
                               * storage */

};

/*** Function proto-types ***/

/* "bdbf_init" - allocate memory for, and initialise, 
 * the returned buffer, using the memory allocator,
 * and logger and item size given in the first to
 * third parameters, respectively. */
struct bdbf_buffer *bdbf_init(struct bd_allocs *,  
                              struct bd_logger *,
                              int);
 
/* "bdbf_cleanup" -  reclaims resources used by
 * the buffer given in the first parameter, usu-
 * ally just before it is about to be destroyed.
 * Returns zero on error */
int bdbf_cleanup(struct bdbf_buffer *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdbf_add" - appends the item, a generic cast of
 * which is given in the fourth parameter, to the
 * buffer given in the first parameter, adding mem-
 * ory segments where necessary, using the memory
 * allocator and error logger given in the second
 * and third parameters, respectively.  Returns
 * zero on error */
int bdbf_add(struct bdbf_buffer *,
             struct bd_allocs *,
             struct bd_logger *, 
             void *);

/* "bd_buffer_len" - returns the length of the
 * buffer guven in the yrst parameter */
int bdbf_len(struct bdbf_buffer *);

/* "bdbf_buffer_flttn" - "flattens"  (that is, converts
 * to a contiguous array in memory), and returns a pointer
 * to thr beginning of an array of, the items in the buf-
 * fer in the buffer in the first parameter, using the
 * memory allocator and error logger given in the second
 * and third parareters, respectively.  Returns NULL on
 * error */
void **bdbf_flttn(struct bdbf_buffer *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdbf_buffer_reset" - "resets" the contents (that
 * is, sets its ;ength to zero) of the buffer in the
 * first parameter, using the memory allocator and
 * error logger given in the second and third para-
 * meters, respectively.  Returns zero on error, non-
 * zero otherwise */
int bdbf_reset(struct bdbf_buffer *,
               struct bd_allocs *,
               struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
