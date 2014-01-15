/*****************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* ic.c - function implementations related
* to compiler intermediate codes
*
* Intermediate codes are the "raw" output
* of of a compiler designed to be as arch-
* itecture-neutral as possible, generated
* just tbefore the last phase of the comp-
* iler - object code generation - transforms
* them into architecture-specific machine
code, which may then be directly executed.
*
* intermedate codes perform fumctions such
* as: (among others) pushing finction
* parameters, calling functions (both with
* and without return values), returning
* from functions and procedures, perform-
* ing arithmetic, logical operations,
* and branching (both unconditionally  and
* based on the result of value comparisons)
*
* This implementation uses the triples
* form of intermediate codes (implemented
* using quadruples).
*
* Copyright (C) 2006-2013, Russell Potter,
* All Rights Reserved
*
******************************************************************/

#include <bikedemo/ic.h>
#include <bikedemo/map.h>
#include <bikedemo/buffer.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <stdlib.h>

enum bdic_oprnd_modes;

/* "ptch_val" - writes the data
 * given in "dta" to the intrme-
 * diate code value given in
 * "val".  Returns zero on error,
 * non-zero otherwise  */
static int ptch_val(struct bdic_val *val,
                    void *dta)
{
    switch (val->mode) {

        case bdic_mde_cnst:
            val->val.cnst =
                * (struct bdic_cnst *) dta;
            break;

        case bdic_mde_sym:
            val->val.sym = * (int *) dta;
            break;

        case bdic_mde_indr:
            val->val.indr = * (int *) dta;
            break;
	}

	return 1;
}
/* "cmp_psns" - return a compar-
 * ison between the two intermediate
 * code offsets in "off1" and "off2"
 * as the key comparison function of
 * a code list map */
static int cmp_psns(int *off1,
		            int *off2)
{
    return *off1 - *off2;
}

/* "emit" output the single interm-
 * ediate code in "cde from the list
 * in "cdes", using the memory allo-
 * cator and error logger given "all-
 * ocs" and "logger", respectively.
 * Returns zero error, on-zero non-
 * zero otherwise  */
static int emit(struct bdic_icde *cde,
		        struct bdic_icdes *cdes,
		        struct bd_allocs *allocs,
		        struct bd_logger *logger)
{
    struct bd_map_node *last,      /* last node */
	                   *ins_node;  /* in map  */
	                               /* and insertion
                                    * node */
    int idx;                       /* last ode
                                    * index  */

    last = bd_map_last(cdes->map);
    idx = * (int *) (last->key) + 1;

	ins_node = (struct bd_map_node *)
			bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
	if (!(ins_node))
	    return 0;

	if ((bd_map_insert(&cdes->map,
			ins_node,
			&idx,
			(bd_map_cmp_fn)
			    cmp_psns,
			cde)))
	    return 0;

	return bdbf_add(cdes->u.buf,
			        allocs,
			        logger,
			        cde);
}

/* "bdic_init_cdes" - returns a pointer to
 * an (initially empty) intemediate code
 * list that uses "allocs" and "logger"
 * error logger given in the first and
 * second parameters, respectively */
struct bdic_icdes *bdic_init_cdes(
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdic_icdes *ret; /* return
                             * value */

    /*allocate list's storage */
    ret = (struct bdic_icdes *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdic_icdes),
                   logger);
    if (!(ret))
        return NULL;

	/* initialise the
     * list's buffer */
	ret->u.buf = bdbf_init(allocs,
                          logger,
                          sizeof(struct
                                 bdic_icde));
        if (!(ret->u.buf))
        	return NULL;

    ret->map = NULL;

    /* if we get here, return
     success (a non-NULL list) */
    return ret;
}

/* "bdic_mark" - returns the location
 * given in "loc" within the current
 * end of the intermediate code list
 * given in "cdes" having the argume-
 * nt number given nin "arg", using
 * the memory allocator and error
 * logger given in "allocs" and "logg-
 * er", respectively.  Returns NULL
 * on error */
struct bdic_loc *bdic_mark(int arg,
                           struct bdic_icdes *cdes,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
	struct bdic_loc *ret; /* return
                           * value */

    /* allocate retr value */
    ret = (struct bdic_loc *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdic_loc),
                     logger);
    if (!(ret))
        return NULL;

    /* set its offset to
     * "cde"'s current
     * length and location
     * to the passed value */
    ret->off = bdic_len(cdes);
	ret->arg = arg;
    return ret;
}
/* "bdic_ptch" - "patches" (writes over)
 * with the value given in "val" at the
 * location given in "loc" within the
 * intermediate code list given in "cd-
 * es", using the memory allocator and
 * error logger given in "allocs" and
 * "logger" espectively.  Returns zero
 * on error, non-zero otherwise */
int bdic_ptch(struct bdic_loc *loc,
              void *val,
              struct bdic_icdes *cdes,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bdic_icde *cde;    /* code at
                               * location */
    struct bd_map_node *node; /* "cde"'s
                               * node in
                               * node map */

    /* find the map node of the
     * relevant intermediate code
     * in the code map and get
     * its kry (an intermediate
     * code)*/
    node = bd_map_find(cdes->map,
                       &loc->off,
                       (bd_map_cmp_fn)
                           cmp_psns);
    if (!(node))
        return 0;
    cde = (struct bdic_icde *)
             node->value;

    /* and patch "val" into the
     * operand sopecified in "loc"*/
    ptch_val(cde->oprnds + loc->arg,
             val);

    return 1;
}

/* "bdic_len" - returns the length of the
 * intemediate code list given in the first
 * parameter */
int bdic_len(struct bdic_icdes *cdes)
{
    /* return the index of the last
     * code- ry in the codes in map
     * the map, which will contain
     * the number of codes */
    return * (int *)
         bd_map_last(cdes->map)->key;
}

/* "bdic_idx" returns the intermediate
 * code located at the index  ,given in
 * "idx", into the intermediate code
 * list given in "icdes", using the mem-
 * ory allocator and ror logger given
 * in "allocs" and "logger", respectively */
struct bdic_icde *bdic_idx(int idx,
		                   struct bdic_icdes *icdes,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* return the intermediate code
     *at the index given in "idx" */
    return icdes->u.arry + idx;
}

/* "bdic_emit" -"emits" (by appending
 * to the intermediate code list given in
 * "cdes") the intermediate code of the
 * type, result symbol number in "rslt",
 * operand modes given in "oprnd1_mde",
 * "oprnd2_mde" and "oprnd3_mde", resp-
 * ectively, and operand values given,
 * "op1", "op2" and "op3" respectively,
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise */
int bdic_emit(struct bdic_icdes *cdes,
              enum bdic_types type,
              int rslt,
              enum bdic_mdes op1_mde,
              void *op1,
              enum bdic_mdes op2_mde,
              void *op2,
              enum bdic_mdes op3_mde,
              void *op3,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bdic_icde *cde; /* intermediate
                            * code to be
                            * created from
                            * parameters */

   cde = (struct bdic_icde *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdic_icde),
                 logger);
    if (!(cde))
        return 0;

    /* set "cde"'s typr sand result ... */
    cde->type = type;
    cde->rslt = rslt;

    /* ... its first operand ... */
    switch (cde->oprnds[0].mode) {

        case bdic_mde_cnst:
            cde->oprnds[0].val.cnst =
                * (struct bdic_cnst *) &op1;
            break;

        case bdic_mde_sym:
            cde->oprnds[0].val.sym =
                 * (int *) op1;
            break;

        case bdic_mde_indr:
            cde->oprnds[0].val.indr =
                 * (int *) op1;
            break;

         case bdic_mde_addr:
            cde->oprnds[0].val.addr =
                 (void *) op1;
            break;
    }

    /*  .. second operand */
    switch (cde->oprnds[1].mode) {

        case bdic_mde_cnst:
            cde->oprnds[1].val.cnst =
                * (struct bdic_cnst *) &op2;
            break;

        case bdic_mde_sym:
            cde->oprnds[1].val.sym =
                 * (int *) op2;
            break;

        case bdic_mde_indr:
            cde->oprnds[1].val.indr =
                 * (int *) op2;
            break;

         case bdic_mde_addr:
            cde->oprnds[1].val.addr =
                 (void *) op2;
            break;
    }

    /* ... and the third operand */
   switch (cde->oprnds[2].mode) {

        case bdic_mde_cnst:
            cde->oprnds[2].val.cnst =
                * (struct bdic_cnst *) &op3;
            break;

        case bdic_mde_sym:
            cde->oprnds[2].val.sym =
                 * (int *) op3;
            break;

        case bdic_mde_indr:
            cde->oprnds[2].val.indr =
                 * (int *) op3;
            break;

         case bdic_mde_addr:
            cde->oprnds[2].val.addr =
                 (void *) op3;
            break;
    }


    /* and append the code to
     * the code list */
    return emit(cde,
                cdes,
                allocs,
                logger);
}

/* "bdic_emit_cde" - "emits" (outputs)
 * the intermediate code given in the
 * second parameter fron thr intermed-
 * iate code list given in the first
 * parameter, using the memory allocat-
 * or and error logger given in the
 * third and fourth parameters, resp-
 * ectively.  Returns  zero on error,
 * non-zero otherwise */
int bdic_emit_cde(struct bdic_icdes *cdes,
                  struct bdic_icde *cde,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    /* call the internal "emit"
     * function */
    return emit(cde,
                cdes,
                allocs,
                logger);
}

/* "bdic_end" -  completes the generation
 * of intermediate codes from the code list
 * given in "cdes" using memory allocatocs"
 * and error logger given in "allocs" and
 * "logger", respectively.  Returns a poin-
 * ter to the beginning an array consisting
 * of its emitted codes */
struct bdic_icde *bdic_end(struct bdic_icdes *cdes,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /*set "cde"'s array to the "flatten-
     * ing" of it's buffer  */
    return cdes->u.arry =
              (struct bdic_icde *)
                  bdbf_flttn(cdes->u.buf,
                             allocs,
                             logger);
}

/* "bdic_end" -  returns the concatenation of
 * the intermediate codes given in "cdes2" to
 * those in "cdes1", using the memory allocator
 * and error logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error */
struct bdic_icdes *bdic_cat(struct bdic_icdes *cdes1,
                            struct bdic_icdes *cdes2,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    struct bdic_icdes *ret; /* return value */
    struct bd_map_node *it; /* "cdes2" iter-
                             * ator */
    struct bdic_icde *cde;  /* virrent code
                             * in "cde2" */

    ret = cdes1; /* return value
                  * starts the same
                  * as "cdes1" */


    /* iterate through the codes in
     * "cdes2" ... */
    for (it = bd_map_first(cdes2->map);
         it;
         it = bd_map_next(it)) {

        /* get the node's value (an inter-
         * mediate code) */
        cde = (struct bdic_icde *)
              it->value;

        if (!(emit(cde,
                   ret,
                   allocs,
                   logger)))
             return NULL;
    }

    return ret;
}
