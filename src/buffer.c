 /********************************************************************
 *
 * BikeDemo - Exercise bike demonstration scenery
 *
 * buffer.h - function implementations for the
 * storage, manipulation and maintenance of a li-
 * near array of dynamically re-allocated, "grow-
 * able" units of storage.
 *
 *
 * The buffer memory is designed such that it is
 * allocated in a map of fixed-sized chunks of
 * contiguous memory, allocated as needed as the
 * buffer grows
 *
 * Copyright (C) 2009-2014, Russell Potter.
 * All rights reserved.
 *
 ********************************************************************/

#include <stdlib.h>
#include <memory.h>
#include <bikedemo/buffer.h>
#include <bikedemo/allocs.h>
#include <bikedemo/map.h>
#include <bikedemo/log.h>

/* "cmp_segs" - return the result if s comparison
 * between the buffer memory segment indexes in
 * "idx1" and "idx2", as the key comparison call-
 * back of the buffer's segment map */
static int cmp_segs(int *idx1,
                    int *idx2)
{
	/* return pointer comparison */
	return *idx1 - *idx2;
}

/* "bdbf_init" - allocate memory for, and initialise, 
 * the returned buffer, using the memory allocator,
 * and logger and item size given in the "allocs",
 * "logger", and "sz", respectively. */
struct bdbf_buffer *bdbf_init(struct bd_allocs *allocs,  
                              struct bd_logger *logger,
                              int sz)
{
    struct bdbf_buffer *ret;

    if (!(ret = bd_alloc(allocs,
                         sizeof(struct
                                bdbf_buffer),
                               logger)))
        return NULL;

    ret->segs = NULL;
    ret->last = NULL;
    ret->len  = 0;
    ret->sz   = sz;

    return ret;
}
 
/* "bdbf_cleanup" - reclaims resources sused by
 * the buffer given in the first parameter, usually
 * just before it is about to be destroyed.  Returns
 * zero on error */
int bdbf_cleanup(struct bdbf_buffer *buf,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
	/* free the buffer's
	 * segment map */
    bd_free(allocs,
			buf->segs);
    return 1;
}

/* "bdbf_add" - appends the item, a generic cast of
 * which is given in "item", to the buffer given in
 * "buf", adding memory segments as required, using
 * the memory allocator and error logger given in
 * "allocs" and "logger", respectively.  Returns
 * zero on error */
int bdbf_add(struct bdbf_buffer *buf,
             struct bd_allocs *allocs,
             struct bd_logger *logger, 
             void *item)
{
    struct bd_map_node *ins_node;  /* segment */
                                   /* map inser- */
                                   /* tion node */
    int next;                      /*next segme-
                                    * nt /

   /* if sffomg to the buffer requires
    * another segment ... */
	if (!(buf->len++ % CHNK_SZ)) {

		/* calculate the number
		 * of segments and there-
		 * fore the next segment
		 * index */
		int num_segs = * (int *)
	            (bd_map_last(
				buf->segs))->value,
		new_seg = num_segs + 1;

        /* de-allocate the old buffer
         * memory (if present)  */
        if (buf->segs) {
		    bd_free(allocs, buf->mem);
        }

		/* then insert the segment
		 * at that index */
        ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
        		         sizeof(struct
        		                bd_map_node),
        		         logger);
        if (!(ins_node))
        	return 0;
        if (!(bd_map_insert(&buf->segs,
                            ins_node,
                            (void *) &next,
                            (bd_map_cmp_fn)
                                 cmp_segs,
                            (void *) &new_seg)))
        return 0;

        /* and update "buf"'s last
         * segment node: it is now its
         * previous value's next node  */
        buf->last = bd_map_next(buf->last);

	} else
		/* we don't need to allocate another
		 * segment, so just set the appropr-
		 * iate memory segment index from
		 * "item" */
        memcpy(((char *)(buf->last->value)) +
                        (buf->len * buf->sz),
                        item,
                        buf->sz);


    return 1; /* return success if we get
               * to this point  */
}


/* "bd_buffer_len" - returns the length of
 * "buf",  Returns zero on error */
int bdbf_len(struct bdbf_buffer *buf)
{
    return buf->len; 
}

/* "bdbf_buffer_flttn" - "flattens"  (that is, converts
 * to a contiguous array in memory) and returns a pointer
 * to thr beginning of an array of the items in "buf",
 * using the memory allocator and error logger given in
 * "allocs" and "logger", respectively.  Returns NULL on
 * error */
void **bdbf_flttn(struct bdbf_buffer *buf,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
	int i;                  /* segment */
	                        /* iterator */
	void **ret;              /* item
	                         * array
	                         * to return */
	struct bd_map_node *it; /* iterator
	                         * node */

	/* allocate the code-point array */
	ret = bd_alloc(allocs,
	               buf->len *
	               buf->sz,
	               logger);
    if (!(ret))
        return NULL;

	/* iterte "i" through each segment
	 * index and "j" through each
	 * item in that memory segment */
	for (i = 0, it = bd_map_first(buf->segs);
	            it;
	            i++, it = bd_map_next(it)) {

	    /* set the appropriate item in
	     * the allocated array from the
	     * current memory segment */
        memcpy((char *) (ret + i *
                   CHNK_SZ),
               (char *) it->value,
                   CHNK_SZ);

	    /* then free the memory segment,
	     * since it's not needed once the
	     * buffer is flattened */
	    bd_free(allocs,
	            it);
	}
	return (void **) ret;
}

/* "bdbf_buffer_reset" - "resets" the contents (that
 * is, sets its length to zero) of the buffer "buf",
 * using the memory allocator and error logger given
 * in "allocs" and "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
int bdbf_reset(struct bdbf_buffer *buf,
               struct bd_allocs *allocs,
               struct bd_logger *logger)
{
	buf->len  = 0;
	buf->segs = NULL;

	return 1;
}
