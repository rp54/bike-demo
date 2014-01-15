/********************************************************************
*
* Bike Demo - Exercise Bike Demonstration
* Scenery
*
* slctr.c - function implementations r-
* elated to "selectors", used to match
* a node in an XML document tree again-
* st, a pattern string (known as a "se-
* lector") where,ehen a match exists
* between the two, the selector is said
* to "select" the node
* 
* Copyright (C) 2007-2014, Russell Potter,
* All rights reserved
*
*********************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/slctr.h>
#include <bikedemo/eval.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "evltr" - the evaluator containing
 * implementations of the selector
 * codes */
struct bdev_evltr *evltr;

/* "prsr" - parser converting sele-
 * ctor text to run-time */
struct bdpr_parser *prsr;

/* "start_name_esc" - lexical analyser
 * transition call-back when starting a
 * name escape.  Returns zero on error,
 * non-zero otherwise */
static int start_name_esc(struct bdlx_tok *tok,
                          int dig,
                          void *opaque,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* set "dig" from "cp" field's
     * hexa-decimal representation */
    sl_tok->u.cp = bdut_from_hex(dig);

    return 1;
}

/* "start_buffer" - lexical analyser
 * transition call-back to start coll-
 * ecting input into "tok"'s buffer .
 * Returns zero on error, non-zero
 * otherwise */
static int start_buffer(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* start "sl_tok"'s buffer */
    return (sl_tok->u.buf = bdbf_init(allocs,
                                     logger,
                                     sizeof(int)))
                                     != NULL;

}

/* "add_to_buffer" - lexical analyser
 * transition call-back to add "cp"
 * tp "tok"'s buffer. Returns zero on
 * error, non-zero otherwise */
static int add_to_buffer(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* ad "cp" to "sl_tok"'s buffer */
    return bdbf_add(sl_tok->u.buf,
    	 	        allocs,
                    logger,
                    (void *) cp);

}

/* "dig_add" - lexical analyser
 * transition call-back to add the
 * code-point "dig" to "tok"'s num-
 * ber.  Returns zero on error, non-
 * zero otherwise */
static int dig_add(struct bdlx_tok *tok,
                   int dig,
                   void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* add "dig" "sl_tok"'s number  */
    sl_tok->u.num += (dig - '0');
    sl_tok->u.num *= 10;
    return 1;
}

/* "start_num" - lexical analyser
 * transition call-back to start
 * "tok"'s number with "num".
 * Returns zero on error, non-
 * zero otherwise */
static int start_num(struct bdlx_tok *tok,
                     int num,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* start "sl_tok"'s number
     * with "num" */
    sl_tok->u.num = (num - '0');
    return 1;
}

/* "neg_num" - lexical analyser
 * transition call-back to negate
 * "tok"'s number.  Returns zero
 * on error, non-zero otherwise */
static int neg_num(struct bdlx_tok *tok,
                   int num,
                   void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* negate "sl_tok"'s number */
    sl_tok->u.num = -sl_tok->u.num;
    return 1;
}


/* "start_name" - lexical analyser
 * transition call-back when starting a
 * name.  Returns zero on error, non-
 * zero otherwise */
static int start_name(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdsl_tok *sl_tok =
              (struct bdsl_tok *) tok;

    /* initalise "tok"'s buffer ... */
    if (!(sl_tok->u.buf = bdbf_init(allocs,
    		                        logger,
    		                        sizeof(int))))
        return 0;

    /* ... and append "cp", the name's
     * first pode-point */
     return bdbf_add(sl_tok->u.buf,
                    allocs,
    		        logger,
    		        (void *) cp);

}

/* "add_name_esc" - lexical analyser
 * transition call-back when adding a
 * a digit to a name escape.  Returns
 * zero on error, non-zero otherwise */
static int add_name_esc(struct bdlx_tok *tok,
                        int dig,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                (struct bdsl_tok *) tok;

     /* add "dig" from "cp" field's
      * hexa-decimal representation */
    sl_tok->u.cp *= 16;
    sl_tok->u.cp += bdut_from_hex(dig);
    return 1;
}

/* "end_name_esc" - selector sub-routine
 * transition call-back for ending a a
 * name escape.  Returns zero on error,
 * non-zero otherwise */
static int end_name_esc(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                (struct bdsl_tok *) tok;

    /* append the just-escaped code-
     * point to "tok"'s buffer field */
    if (!(bdbf_add(sl_tok->u.buf,
                   allocs,
                   logger,
                   (void *)
                       sl_tok->u.cp)))
    	return 0;


    /* then append current code-point
     * to "tok"'s buffer field */
    return bdbf_add(sl_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "end_name" - lexical analyser
 * transition call-back to end a
 * name.  Returns zero on error,
 * non-zero otherwise */
static int end_name(struct bdlx_tok *tok,
                    int cp,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                (struct bdsl_tok *) tok;

    /* create name's code-points by
     * "flattening" buffer field */
    sl_tok->u.name->cps = (int *) bdbf_flttn(
                          sl_tok->u.buf,
                          allocs,
                          logger);

    /* ... and its length from
     * the buffer length */
    sl_tok->u.name.len =
                bdbf_len(sl_tok->u.buf);
    return 1;
}

/* "end_ref" - lexical analyser
 * transition call-back to end a
 * reference name.  Returns zero
 * on error, non-zero otherwise */
static int end_ref(struct bdlx_tok *tok,
                    int cp,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                (struct bdsl_tok *) tok;

    /* create name's code-points by
     * "flattening" buffer field */
    sl_tok->u.ref->cps = (int *) bdbf_flttn(
                          sl_tok->u.buf,
                          allocs,
                          logger);

    /* ... and its length from
     * the buffer length */
    sl_tok->u.ref->len =
                bdbf_len(sl_tok->u.buf);
    return 1;
}

/* "str_end" - lexical analyser
 * transition call-back to end a
 * string literal.  Returns zero
 * on error, non-zero otherwise */
static int str_end(struct bdlx_tok *tok,
                   int cp,
                   void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                (struct bdsl_tok *) tok;

    /* create name's code points by
     * "flattening" buffer field */
    sl_tok->u.str->cps = (int *)
                         bdbf_flttn(
                          sl_tok->u.buf,
                          allocs,
                          logger);

    /* ... and its length from
     * the buffer length */
    sl_tok->u.str->len =
                bdbf_len(sl_tok->u.buf);
    return 1;
}

/* "add_name_cp" - lexical analyser
 * transition call-back when adding
 * the code-point "cp" to "tok"'s
 * name field.  Returns zero on err-
 * or, non-zero otherwise */
static int add_name_cp(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdsl_tok *sl_tok =
                  (struct bdsl_tok *) tok;

    /* append "cp" to "tok"'s
     * buffer field */
    return bdbf_add(sl_tok->u.buf,
                    allocs,
                    logger,
    		        (void *) cp);

}

/* "re_init_tok" - performs one-time
 * initialisation of the selector
 * lexical token in "tok", using the
 * memory allocator and error logger
 * in "allocs" and "logger", respect-
 * ively.  Returns zero on error non-
 * zero otherwise */
static int init_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce ot correct type */
	struct bdsl_tok *sl_tok  =
             (struct bdsl_tok *)
             tok;

	sl_tok->u.num = 0;

    /* initialise token's buffer */
    sl_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));

	return sl_tok->u.buf != NULL;
}

/* "tok_type" - returns the type
 * of the selector token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger _logger)
{
    /* co-erce ot correct type */
	struct bdsl_tok *sl_tok  =
             (struct bdsl_tok *) tok;

	return sl_tok->type; /* return
                          * the type
                          * of the
                          * token */
}

/* "tok_reset" - performs
 * processing required before
 * each read of the selector
 * token in "tok", using the
 * memory allocator and error
 * logger in "allocs" and "lo-
 * gger", respectively */
static int tok_reset(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdsl_tok *sl_tok  =
             (struct bdsl_tok *) tok;

    /* reset the token's buffer */
    return bdbf_reset(sl_tok->u.buf,
                      allocs,
                      logger);
}

/* "tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the selector token in
 * "tok", where "exact" indicates
 * the lebek id exsactness in the
 * content description, using the
 * memory allocator and error logg-
 * er in "allocs" and "logger", res-
 * pectively.  Returns a negative
 * value on error */
static int tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdsl_tok *sl_tok  =
             (struct bdsl_tok *) tok;

	return 0; /* no description here */
}

/* "apply_sbstr" - applies (implements)
 * the "no attribute is sub-string of"
 * code of the evaluator code list in
 * "cdes", using the memory allocator,
 *  error logger and opaque value in
 *  "allocs", "logger" and "opaque",
 * respectively.  This routine serves
 * as the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_sbstr(void *opaque,
                       struct bdev_cdes *cdes,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "apply_ret" - applies (implements)
 * the "return from sub-routine"
 * code of the evaluator code list in
 * "cdes", using the memory allocator,
 *  error logger and opaque value in
 *  "allocs", "logger" and "opaque",
 * respectively.  This routine serves
 * as the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_ret(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_frst" - applies (implements)
 * the "is first child" code of the
 * evaluator code list in "cdes", us-
 * ing the memory allocator, error
 * logger and opaque value in "all-
 * ocs", "logger" and "opaque", res-
 * pectively.  This routine serves
 * as the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_frst(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_lst" - applies (implements)
 * the "is last child" code of the
 * evaluator code list in "cdes", us-
 * ing the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque", respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_lst(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_rt" - applies (implements)
 * the "is root node" code of the
 * evaluator code list in "cdes",
 * using the memory allocator, error
 * logger and opaque value in  "all-
 * ocs", "logger" and "opaque", resp-
 * ectively.  This routine serves as
 * the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_rt(void *opaque,
                    struct bdev_cdes *cdes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_only" - applies (implements)
 * the "is only child" code of the
 * evaluator code list in "cdes", us-
 * ing the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque", respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_only(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdxl_node *nde; /* node to query */

    /* pop needed node from "evltr"'s
     * run-time stack */
    if (!(node = bdev_pop(cdes->evltr,
                          sbdsl_stk_nde,
                          allocs,
                          logger)))
        return 0;


return node->ldrn && bnext && coswde;
}

/* "apply_empty" - applies (implements)
 * the "is empty node" code of the
 * evaluator code list in "cdes", using
 * the memory allocator, error logger
 * and opaque value in "allocs", "logger"
 * and "opaque", respectively.  This
 * routine serves as the selector eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_empty(void *opaque,
                       struct bdev_cdes *cdes,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "apply_or" - applies (implements)
 * the "logical or" code of the eval-
 * uator code list in "cdes", using
 * the memory allocator, error logger
 * and opaque value in "allocs", "log-
 * ger" and "opaque", respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_or(void *opaque,
                    struct bdev_cdes *cdes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_end" - applies (implements)
 * the "exit run-time" code of the
 * evaluator code list in "cdes", us-
 * ing the memory allocator, error
 * logger and opaque value in "allo-
 * cs", "logger" and "opaque", resp-
 * ectively.  This routine serves as
 * the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_end(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_attr" - applies (implements)
 * the "has attribute" code of the
 * evaluator code list in "cdes", us-
 * ing the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque",respectively.
 * This routine serves as the select-
 * or evalutor's "apply" call-back
 * for this code. Returns zero on
 * error, non-zero otherwise */
static int apply_attr(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_nmed" - applies (implements)
 * the "attribute has name" code of
 * the evaluator code list in "cdes",
 * using the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque",respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_nmed(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_psdo" - applies (implements)
 * the "has pseudo-attribute" code of
 * the evaluator code list in "cdes",
 * using the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque",respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_psdo(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_if" - applies (implements)
 * the "is on-zero" code of the evalu-
 * ator code list in "cdes", using the
 * memory allocator, error logger and
 * opaque value in "allocs", "logger"
 * and "opaque",respectively.  This
 * routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_if(void *opaque,
                    struct bdev_cdes *cdes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_if_n" - applies (implements)
 * the "is zero" code of the evaluator
 * code list in "cdes", using the mem-
 * ory allocator, error logger and
 * opaque value in "allocs", "logger"
 * and "opaque",respectively.  This
 * routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_if_n(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_starts" - applies (imple-
 * ments) the "is starts with" code
 * of the evaluator code list in "cd-
 * es", using the memory allocator,
 * error logger and opaque value in
 * "allocs", "logger" and "opaque",
 * respectively.  This routine serves
 * as the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_starts(void *opaque,
                        struct bdev_cdes *cdes,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    return 1;
}

/* "apply_ends" - applies (impleme-
 * nts) the "ends with" code of the
 * evaluator code list in "cdes",
 * using the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque", respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_ends(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_and" - applies (impleme-
 * nts) the "node "logical and" code of
 * the evaluator code list in "cdes",
 * using the memory allocator, error
 * logger and opaque value in "allocs",
 * "logger" and "opaque", respectively.
 * This routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_and(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_mtch" - applies (impleme-
 * nts) the "node "lmatch selector"
 * code of the evaluator code list in
 * "cdes", using the memory allocator,
 * error logger and opaque value in
 * "allocs", "logger" and "opaque",
 * respectively. This routine serves
 * as the selector evalutor's "apply"
 * call-back for this code. Returns
 * zero on error, non-zero otherwise */
static int apply_mtch(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_sbr" - applies (impleme-
 * nts) the "node "call subroutine"
 * code of the evaluator code list
 * in "cdes", using the memory allo-
 * cator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively. This ro-
 * utine serves as the selector ev-
 * alutor's "apply" call-back for
 this code. Returns zero on error,
 * non-zero otherwise */
static int apply_sbr(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_class" - applies (impleme-
 * nts) the "node "node has class"
 * code of the evaluator code list
 * in "cdes", using the memory allo-
 * cator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively.  This
 * routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_class(void *opaque,
                       struct bdev_cdes *cdes,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "apply_id" - applies (impleme-
 * nts) the "node "node has ID"
 * code of the evaluator code list
 * in "cdes", using the memory allo-
 * cator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively.  This
 * routine serves as the selector
 * evalutor's "apply" call-back for
 * this code. Returns zero on error,
 * non-zero otherwise */
static int apply_id(void *opaque,
                    struct bdev_cdes *cdes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_ns" - applies (impleme-
 * nts) the "node is in namespace"
 * code of the evaluator code list
 * in "cdes", using the memory allo-
 * cator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively. This rou-
 * tine serves as the selector eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_ns(void *opaque,
                    struct bdev_cdes *cdes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_nns" - applies (impleme-
 * nts) the "node is in no namespace"
 * code of the evaluator code list
 * in "cdes", using the memory allo-
 * cator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively. This rou-
 * tine serves as the selector eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_nns(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_a_ns" - applies (impleme-
 * nts) the attribute is in namesp-
 * ace" code of the evaluator code
 * list in "cdes", using the memory
 * allocator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively. This rou-
 * tine serves as the selector eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_a_ns(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_a_nns" - applies (impleme-
 * nts) the attribute is in no namesp-
 * ace" code of the evaluator code
 * list in "cdes", using the memory
 * allocator, error logger and opaque
 * value in "allocs", "logger" and
 * "opaque", respectively. This rou-
 * tine serves as the selector eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_a_nns(void *opaque,
                       struct bdev_cdes *cdes,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "apply_a_not" - applies (impleme-
 * nts) the "invert match" of the
 * evaluator code list in "cdes",
 * using the memory allocator, err-
 * or logger and opaque value in
 * "allocs", "logger" and "opaque",
 * respectively. This routine ser-
 * ves as the selector evalutor's
 * "apply" call-back for this code.
 * Returns zero on error, non-zero
 * otherwise */
static int apply_not(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* grammar production routines */

/* "reduce_start_grp" - perform the action
 * associated with the recognition of the
 * "selector group -> selector, selector
 * group"  * production, using the output
 * selector codes in "cdes", generic cast
 * of the selector's output codes in "cd-
 * es", generic cast of left-hand-side s-
 * ymbol to contain the result of the re-
 * duction in "lhs", reduce items in "it-
 * ems", the number of which is given in
 * "num" and the memory allocator and er-
 * ror logger given in "allocs" and "lo-
 * gger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_start_grp(void *cdes,
                            void *lhs,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
	/* nothing to emit to start
	 * a selector group */
	return 1;
}

/* "reduce_add_to_grp" - perform the
 * action associated with the recognition
 * of the start of a "selector group" pr-
 * oduction, using the output selector
 * codes in "cdes", generic cast of the
 * left-hand-side symbol to contain the
 * result of the reduction in "lhs", red-
 * uce items in "items", the number of
 * which is given in "num" and the mem-
 * ory allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-zero
 * otherwise */
static int reduce_add_to_grp(void *cdes,
				             void *lhs,
				             union bdgm_reduce_item *items,
                             int num,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
    /* emit a "logical or" of the
     * two selectors in the produ-
     * ction, since either may
     * match for the elector group
     * to match */
    return bdev_emit(cdes,
                     NULL,
                     bdsl_cde_or,
                     NULL,
                     allocs,
                     logger);
}

/* "reduce_start_slctr" - perform the act-
 * ion associated with the recognition of
 * the start of a "selector" production,
 * using the the generic cast of the out-
 * put selector codes in "cdes", gene-
 * ric cast of the left-hand- side symbol
 * to contain the result of the reduction
 * in "lhs", reduce items in "items", the
 * number of which is given in "num" and
 * the memory allocator and error logger
 * given in "allocs" and "logger", respe-
 * ctively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_start_slctr(void *cdes,
                              void *lhs,
                              union bdgm_reduce_item *items,
                              int num,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
	/* nothing to emit to start
	 * a selector */
	return 1;
}

/* "reduce_slctr_grp" - perform the act-
 * ion associated with the recognition of
 * the start of the "selector group" prod-
 * uction, using the the generic cast of
 * the output selector codes in "cdes",
 * generic cast of the left-hand-side sy-
 * mbol to contain the result of the re-
 * duction in "lhs", reduce items in "it-
 * ems", the number of which is given in
 * "num" and the memory allocator and er-
 * ror logger given in "allocs" and "log-
 * ger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_slctr_grp(void *cdes,
                            void *lhs,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
	return 1;
}

/* "reduce_add_to_slctr" - perform the
 * action associated with the recognition
 * of the start of a "selector group"
 * production, using the the generic
 * cast of theoutput selector codes in
 * "cdes", generic cast of th left-ha-
 * nd-side symbol to contain the result
 * of the reduction in "lhs", reduce
 * items in "items", the number of wh-
 * ich is given in "num" and the mem-
 * ory allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-zero
 * otherwise */
static int reduce_add_to_slctr(
                void *cdes,
                void *lhs,
                union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    /* emit a "logical and" of the
     * two simple selectors in the
     * production, since both must
     * match for the elector group
     * to match */
    return bdev_emit(cdes,
                     NULL,
                     bdsl_cde_and,
                     NULL,
                     allocs,
                     logger);
}

/* "reduce_cmbntr_chld" - perform the
 * action associated with the recogn-
 * ition of a selector "child combi-
 * nator" * production, using the out-
 * put selector codes in "cdes", gen-
 * eric cast of the left-hand-side sy-
 * mbol to contain the result of the
 * reduction in "lhs", reduce items in
 * "items", the number of which is gi-
 * ven in "num" and the memory alloc-
 * ator and error logger given in "al-
 * locs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise  */
static int reduce_cmbntr_chld(void *cdes,
                              void *lhs,
                              union bdgm_reduce_item *items,
                              int num,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
	return 1;
}

/* "reduce_cmbntr_dsc" - perform the
 * action associated with the recogn-
 * ition of a selector "descendant
 * combinator" * production, using
 * the output selector codes in "cdes",
 * generic cast of the left-hand-side
 * symbol to contain the result of
 * the reduction in "lhs", reduce items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respect-
 * ively.  Returns zero on error,
 * non-zero otherwise  */
static int reduce_cmbntr_dsc(void *cdes,
                             void *lhs,
                             union bdgm_reduce_item *items,
                             int num,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
	return 1;
}

/* "reduce_cmbntr_grnd" - perform the
 * action associated with the recogn-
 * ition of a selector "grand-parent
 * combinator" * production, using the
 * output selector codes in "cdes",
 * generic cast of the left-hand-side
 * symbol to contain the result of the
 * reduction in "lhs", reduce items in
 * "items", the number of which is gi-
 * ven in "num" and the memory alloca-
 * tor and error logger given in "all-
 * ocs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_cmbntr_grnd(void *cdes,
                              void *lhs,
                              union bdgm_reduce_item *items,
                              int num,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
	return 1;
}

/* "reduce_end" - perform the action
 * associated with the end of a sele-
 * ctor production, using the output
 * selector codes in "cdes", generic
 * cast of the left-hand-side symbol
 * to contain the result of the redu-
 * ction in "lhs", reduce items in
 * "items", the number of which is
 * given in "num" and the memory all-
 * ocator and error logger given in
 * "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-
 * zero otherwise  */
static int reduce_end(void *cdes,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
	return 1;
}

/* "reduce_prp_clss" - perform the
 * action associated with the recogn-
 * ition of a "selector class prop-
 * erty" production, using the output
 * selector codes in "cdes", generic
 * cast of the left-hand-side symbol
 * to contain the result of the redu-
 * ction in "lhs", reduce items in
 * "items", the number of which is
 * given in "num" and the memory all-
 * ocator and error logger given in
 * "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-
 * zero otherwise  */
static int reduce_prp_clss(void *cdes,
                           void *lhs,
                           union bdgm_reduce_item *items,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
	return 1;
}

/* "reduce_elem_nme" - perform the
 * action associated with the recogn-
 * ition of an "element name" product-
 * ion, using the output selector co-
 * des in "cdes", generic cast of the
 * left-hand-side symbol to contain
 * the result of the reduction in "l-
 * hs", reduce items in "items", the
 * number of which is given in "num"
 * and the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * on error, non-zero otherwise  */
static int reduce_elem_nme(void *cdes,
                           void *lhs,
                           union bdgm_reduce_item *items,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
	return 1;
}

/* "reduce_elem_nme" - perform the
 * action associated with the recogn-
 * ition of an "element name" product-
 * ion, using the output selector co-
 * des in "cdes", generic cast of the
 * left-hand-side symbol to contain
 * the result of the reduction in "l-
 * hs", reduce items in "items", the
 * number of which is given in "num"
 * and the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * on error, non-zero otherwise  */
static int reduce_elem_nme_empty(void *cdes,
                                 void *lhs,
                                 union bdgm_reduce_item *items,
                                 int num,
                                 struct bd_allocs *allocs,
                                 struct bd_logger *logger)
{
	return 1;
}

/* "reduce_ns_prfx_elem" - perform the
 * action associated with the recogn-
 * ition of an "element name name-spa-
 * ce prefix" production, using the
 * output selector co- des in "cdes",
 * generic cast of the left-hand-side
 * symbol to contain the result of
 * the reduction in "lhs", reduce
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero
 * on error, non-zero otherwise  */
static int reduce_ns_prfx_elem(void *cdes,
                               void *lhs,
                               union bdgm_reduce_item *items,
                               int num,
                               struct bd_allocs *allocs,
                               struct bd_logger *logger)
{
	return 1;
}

/* "reduce_ns_prfx" - perform the
 * action associated with the recogn-
 * ition of an "element name name-spa-
 * ce prefix separator" production,
 * using the output selector codes in
 * "cdes", generic cast of the left-
 * hand-side symbol to contain the
 * result of the reduction in "lhs",
 * reduce items in "items", the num-
 * ber of which is given in "num"
 * and the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_ns_prfx(void *cdes,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
	return 1;
}

/* "reduce_ns_prfx_nme" - perform the
 * action associated with the recogn-
 * ition of an "element name-space
 * prefix separator" production, us-
 * ing the output selector codes in
 * "cdes", generic cast of the left-
 * hand-side symbol to contain the
 * result of the reduction in "lhs",
 * reduce items in "items", the num-
 * ber of which is given in "num"
 * and the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_ns_prfx_nme(
                      void *cdes,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
	return 1;
}

/* "reduce_type" - perform the act-
 * ion associated with the recogn-
 * ition of an "element name-space
 * type" production, using the out-
 * put selector codes in "cdes",
 * generic cast of the left-hand-
 * side symbol to contain the res-
 * ult of the reduction in "lhs",
 * reduce items in "items", the num-
 * ber of which is given in "num"
 * and the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_type(void *cdes,
                       void *lhs,
                       union bdgm_reduce_item *items,
                       int num,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
	return 1;
}

/* "reduce_unvrsl_ast" - perform
 * the action associated with the
 * recognition of an "unniversal
 * name-space" production, using
 * the output selector codes in
 * "cdes", generic cast of the l-
 * eft-hand-side symbol to cont-
 * ain the result of the reduct-
 * ion in "lhs", reduce items in
 * "items", the number of which
 * is given in "num" and the mem-
 * ory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise  */
static int reduce_unvrsl_ast(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_pre" - perform
 * the action associated with the
 * recognition of an "attribute
 * name starts with" production,
 * using the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce
 * items in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int reduce_attr_pre(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_unvrsl_ns" - perfo-
 * rm the action associated wi-
 * th the recognition of an "an-
 * y name-space" production, us-
 * ing the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce
 * items in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int reduce_unvrsl_ns(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_pst" - perform
 * the action associated with the
 * recognition of an "attribute
 * name ends with" production,
 * using the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce
 * items in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int reduce_attr_pst(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_sub" - perform
 * the action associated with the
 * recognition of an "attribute
 * name is sub-string of" prod-
 * uction, using the output se-
 * lector codes in "cdes", gen-
 * eric cast of the left-hand-
 * side symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in "it-
 * ems", the number of which is
 * given in "num" and the memo-
 * ry allocator and error logg-
 * er given in "allocs" and "l-
 * ogger", respectively.  Retu-
 * rns zero on error, non-zero
 * otherwise */
static int reduce_attr_sub(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_eq" - perform
 * the action associated with
 * the recognition of an "attr-
 * ibute name is equal to" pro-
 * duction, using the output
 * selector codes in "cdes",
 * generic cast of the left-h-
 * and-side symbol to contain
 * the result of the reduction
 * in "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_eq(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_incl" - perform
 * the action associated with
 * the recognition of an "attr-
 * ibute name includes" pro-
 * duction, using the output
 * selector codes in "cdes",
 * generic cast of the left-h-
 * and-side symbol to contain
 * the result of the reduction
 * in "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_incl(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce" - perform
 * the action associated with
 * the recognition of an "ele=
 * ment placement pseudo-clas-
 * s" production, using the ou-
 * tput selector codes in "cd-
 * es", generic cast of the l-
 * eft-hand-side symbol to co-
 * ntain the result of the re-
 * duction in "lhs", reduce i-
 * tems in "items", the number
 * of which is given in "num"
 * and the memory allocator
 * and error logger given in
 * "allocs" and "logger", res-
 * pectively.  Returns zero
 * on error, non-zero otherw-
 * ise */
static int reduce_psdo_plce(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_dm" - perform
 * the action associated with
 * the recognition of an "attr-
 * ibute name dash-matches" pr-
 * oduction, using the output
 * selector codes in "cdes",
 * generic cast of the left-h-
 * and-side symbol to contain
 * the result of the reduction
 * in "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_dm(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_pre" - per-
 * form the action associated
 * with the recognition of an
 * "attribute name-space name
 * starts with" production, us-
 * ing the output selector co-
 * des in "cdes", generic ca-
 * st of the left-hand-side
 * symbol to contain the res-
 * ult of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_pre(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_pst" - per-
 * form the action associated
 * with the recognition of an
 * "attribute name-space bname
 * ends with" production, usi-
 * ng the output selector co-
 * des in "cdes", generic ca-
 * st of the left-hand-side
 * symbol to contain the res-
 * ult of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_pst(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_sub" - per-
 * form the action associated
 * with the recognition of an
 * "attribute name-space name
 * is sub-string of" product-
 * ion, using the output sele-
 * ctor codes in "cdes", gene-
 * ric cast of the left-hand-
 * side symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_sub(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_eq" - per-
 * form the action associated
 * with the recognition of an
 * "attribute name-spacename
 * is equal to" production,
 * using the output selector
 * codes in "cdes", generic
 * cast of the left-hand-side
 * symbol to contain the res-
 * ult of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_eq(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_incl" - pe-
 * rform the action associat-
 * ed with the recognition of
 * an "attribute name-space n-
 * ame includes" production,
 * using the output selector
 * codes in "cdes", generic
 * cast of the left-hand-side
 * symbol to contain the res-
 * ult of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_incl(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_attr_ns_dm" - pe-
 * rform the action associat-
 * ed with the recognition of
 * an "attribute name-space n-
 * ame dash-matches" producti-
 * on, using the output selec-
 * tor codes in "cdes", gener-
 * ic cast of the left-hand-
 * side symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and t-
 * he memory allocator and er-
 * ror logger given in "allo-
 * cs" and "logger", respecti-
 * vely.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_attr_ns_dm(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_type_psdo" -
 * pe-rform the action associated
 * with the recognition of an "el-
 * ement type" pseudo class simp-
 * le.selector" production, using
 * the output selector codes in
 * "cdes", generic cast of the l-
 * eft-hand-side symbol to conta-
 * in the result of the reducti-
 * on in "lhs", reduce items in
 * "items", the number of which
 * is given in "num" and the me-
 * mory allocator and error log-
 * ger given in "allocs" and "l-
 * ogger", respectively.  Retu-
 * rns zero on error, non-zero
 * otherwise */
static int reduce_smple_slctr_type_psdo(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_unvrsl" -
 * pe-rform the action associa-
 * ted with the recognition of
 * the "universal element simp-
 * le selector" production, us-
 * ing the output selector cod-
 * es in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allo-
 * cs" and "logger", respectiv-
 * ely.  Returns zero on error,
 * non-zero otherwise */
static int reduce_smple_slctr_unvrsl(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_type" -
 * pe-rform the action associa-
 * ted with the recognition of
 * the "element type simple se-
 * lector" production, using
 * the output selector codes in
 * "cdes", generic cast of the
 * left-hand-side symbol to co-
 * ntain the result of the red-
 * uction in "lhs", reduce it-
 * ems in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allo-
 * cs" and "logger", respectiv-
 * ely.  Returns zero on error,
 * non-zero otherwise */
static int reduce_smple_slctr_type(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_unvrsl_psdo" -
 * pe-rform the action associated
 * with the recognition of the "un-
 * universal element type pseudo
 * selector" production, using the
 * output selector codes in "cdes",
 * generic cast of the left-hand-
 * side symbol to contain the res-
 * ult of the reduction in "lhs",
 * reduce items in "items", the
 * number of which is given in "n-
 * um" and the memory allocator
 * and error logger given in "al-
 * lo- cs" and "logger", respect-
 * ively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_smple_slctr_unvrsl_psdo(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_type" - perf-
 * orm the action associated with
 * the recognition of the "element
 * of type placement" production,
 * using the output selector codes
 * in "cdes", generic cast of the
 * left-hand-side symbol to conta-
 * in the result of the reduction
 * in "lhs", reduce items in "ite-
 * ms", the number of which is gi-
 * ven in "num" and the memory al-
 * locator and error logger given
 * in "allocs" and "logger", resp-
 * ectively.  Returns zero on er-
 * ror, non-zero otherwise */
static int reduce_psdo_plce_type(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_ntype" - perf-
 * orm the action associated with
 * the recognition of the "nth of
 * element type placement" produc-
 * tion, using the output select-
 * or codes in "cdes", generic ca-
 * st of the left-hand-side symbol
 * to contain the result of the r-
 * eduction in "lhs", reduce items
 * in "items", the number of which
 * is given in "num" and the memo-
 * ry allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_psdo_plce_ntype(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_nltype" - pe-
 * rform the action associated wi-
 * th the recognition of the "nth
 * last of element type placement"
 * production, using the output s-
 * elector codes in "cdes", gene-
 * ric cast of the left-hand-side
 * symbol to contain the result of
 * the reduction in "lhs", reduce
 * items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Retu-
 * rns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_plce_nltype(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_nlevn" - pe-
 * rform the action associated wi-
 * th the recognition of the "ev
 * en element placement" product-
 * ion, using the output selector
 * codes in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number of
 * which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Retu-
 * rns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_plce_evn(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_odd" - perf-
 * orm the action associated wi-
 * th the recognition of the "o-
 * dd element placement" product-
 * ion, using the output selector
 * codes in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number of
 * which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Retu-
 * rns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_plce_odd(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_i" - perf-
 * orm the action associated wi-
 * th the recognition of the
 * "i-th element" production,
 * using the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number of
 * which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Retu-
 * rns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_plce_i(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_n" - perf-
 * orm the action associated wi-
 * th the recognition of the "e-
 * very element" production, us-
 * ing the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number of
 * which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Retu-
 * rns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_plce_n(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_in" - perf-
 * orm the action associated wi-
 * th the recognition of the "e-
 * very element from offset" pr-
 * oduction, using the output s-
 * elector codes in "cdes", ge-
 * neric cast of the left-hand-
 * side symbol to contain the re-
 * sult of the reduction in "lh-
 * s", reduce items in "items",
 * the number of which is given
 * in "num" and the memory all-
 * ocator and error logger given
 * in "allocs" and "logger", re-
 * spectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_psdo_plce_in(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_ino" - perf-
 * orm the action associated wi-
 * th the recognition of the "e-
 * very i-th element from offset"
 * pr- oduction, using the outp-
 * ut selector codes in "cdes",
 * generic cast of the left-hand-
 * side symbol to contain the re-
 * sult of the reduction in "lh-
 * s", reduce items in "items",
 * the number of which is given
 * in "num" and the memory all-
 * ocator and error logger given
 * in "allocs" and "logger", re-
 * spectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_psdo_plce_ino(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_strt" -
 * perform the action associated
 * with the recognition of the
 * "start simple selector" prod-
 * uction, using the output sel-
 * ector codes in "cdes", gener-
 * ic cast of the left-hand-side
 * symbol to contain the result
 * of the reduction in "lhs", r-
 * educe items in "items", the
 * number of which is given in
 * "num" and the memory allocat-
 * or and error logger given in
 * "allocs" and "logger", resp-
 * ectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_smple_slctr_strt(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_smple_slctr_end" -
 * perform the action associated
 * with the recognition of the
 * "end simple selector" prod-
 * uction, using the output sel-
 * ector codes in "cdes", gener-
 * ic cast of the left-hand-side
 * symbol to contain the result
 * of the reduction in "lhs", r-
 * educe items in "items", the
 * number of which is given in
 * "num" and the memory allocat-
 * or and error logger given in
 * "allocs" and "logger", resp-
 * ectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_smple_slctr_end(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_plce_ltype" -
 * perform the action associated
 * with the recognition of the
 * "last of type element lplace-
 * ment" production, using the
 * output selector codes in "cd-
 * es", generic cast of the le-
 * ft-hand-side symbol to conta-
 * in the result of the reduct-
 * ion in "lhs", reduce items
 * in "items", the number of wh-
 * ich is given in "num" and th-
 * e memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Re-
 * turns zero on error, non-ze-
 * ro otherwise */
static int reduce_psdo_plce_ltype(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_lc" - perform
 * the action associated with
 * the recognition of the "last
 * element of class" production,
 * using the output selector c-
 * odes in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allo-
 * cs" and "logger", respectiv-
 * ely.  Returns zero on error,
 * non-zero otherwise */
static int reduce_psdo_lc(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_id" - perform
 * the action associated with
 * the recognition of the "ps-
 * eudo class" production, us-
 * ing the output selector co-
 * des in "cdes", generic cast
 * of the left-hand-side symbol
 * to contain the result of the
 * reduction in "lhs", reduce i-
 * tems in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allo-
 * cs" and "logger", respectiv-
 * ely.  Returns zero on error,
 * non-zero otherwise */
static int reduce_psdo_id(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_id_arg" - per-
 * form the action associated
 * withthe recognition of the
 * "pseudo class with argument"
 * production, using the output
 * selector codes in "cdes", g-
 * eneric cast of the left-han-
 * d-side symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in "ite-
 * ms", the number of which is
 * given in "num" and the mem-
 * ory allocator and error log-
 * ger given in "allocs" and "l-
 * ogger", respectively.  Retur-
 * ns zero on error, non-zero o-
 * therwise */
static int reduce_psdo_id_arg(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_not" - per-
 * form the action associated
 * withthe recognition of the
 * " "not" pseudo class" prod-
 * uction, using the output
 * selector codes in "cdes",
 * generic cast of the left-h-
 * and-side symbol to contain
 * the result of the reduction
 * in "lhs", reduce items in
 * "items", the number of whi-
 * ch is given in "num" and the
 * memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Re-
 * turns zero on error, non-z-
 * ero otherwise */
static int reduce_psdo_not(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_rt" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "roote lement" pseudo cl-
 * ass" production, using the
 * output selector codes in
 * "cdes", generic cast of the
 * left-hand-side symbol to co-
 * ntain the result of the red-
 * uction in "lhs", reduce ite-
 * ms in "items", the number of
 * which is given in "num" and
 * the memory allocator and er-
 * ror logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int reduce_psdo_rt(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_nc" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "roote nth child" pseu-
 * do class" production, us-
 * ing the output selector c-
 * odes in "cdes", generic ca-
 * st of the left-hand-side s-
 * ymbol to contain the result
 * of the reduction in "lhs",
 * reduce items in "items", t-
 * he number of which is given
 * in "num" and the memory al-
 * locator and error logger g-
 * iven in "allocs" and "log-
 * ger", respectively.  Retu-
 * rns zero on error, non-
 * zero otherwise */
static int reduce_psdo_nc(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_nlc" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "roote nth last child" p-
 * seudo class" production, u-
 * sing the output selector c-
 * odes in "cdes", generic ca-
 * st of the left-hand-side s-
 * ymbol to contain the result
 * of the reduction in "lhs",
 * reduce items in "items", t-
 * he number of which is given
 * in "num" and the memory al-
 * locator and error logger g-
 * iven in "allocs" and "log-
 * ger", respectively.  Retu-
 * rns zero on error, non-
 * zero otherwise */
static int reduce_psdo_nlc(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_nt" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "nth of type" pseudo c-
 * lass" production, using
 * the output selector cod-
 * es in "cdes", generic ca-
 * st of the left-hand-side s-
 * ymbol to contain the result
 * of the reduction in "lhs",
 * reduce items in "items", t-
 * he number of which is given
 * in "num" and the memory al-
 * locator and error logger g-
 * iven in "allocs" and "log-
 * ger", respectively.  Retu-
 * rns zero on error, non-
 * zero otherwise */
static int reduce_psdo_nt(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_nlt" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "nth last of type" pse-
 * udo class" production, us-
 * ing the output selector cod-
 * es in "cdes", generic ca-
 * st of the left-hand-side s-
 * ymbol to contain the result
 * of the reduction in "lhs",
 * reduce items in "items", t-
 * he number of which is given
 * in "num" and the memory al-
 * locator and error logger g-
 * iven in "allocs" and "log-
 * ger", respectively.  Retu-
 * rns zero on error, non-
 * zero otherwise */
static int reduce_psdo_nlt(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_fot" - perfo-
 * rm the action associated
 * withthe recognition of the
 * " "first of type" pseudo
 * class" production, using
 * the output selector codes
 * in "cdes", generic cast
 * of the left-hand-side sym-
 * bol to contain the result
 * of the reduction in "lhs",
 * reduce items in "items", t-
 * he number of which is given
 * in "num" and the memory al-
 * locator and error logger g-
 * iven in "allocs" and "log-
 * ger", respectively.  Retu-
 * rns zero on error, non-
 * zero otherwise */
static int reduce_psdo_fot(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_lot" - perf-
 * orm the action associated
 * with the recognition of
 * the " "last of type" pse-
 * udo class" production, u-
 * sing the output selector
 * codes in "cdes", generic
 * cast of the left-hand-si-
 * de symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in "i-
 * tems", the number of which
 * is given in "num" and the
 * memory allocator and error
 * logger given in "allocs"
 * and "logger", respective-
 * ly.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_psdo_lot(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_oc" - perf-
 * orm the action associated
 * with the recognition of
 * the " "only child" pse-
 * udo class" production, u-
 * sing the output selector
 * codes in "cdes", generic
 * cast of the left-hand-si-
 * de symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in "i-
 * tems", the number of which
 * is given in "num" and the
 * memory allocator and error
 * logger given in "allocs"
 * and "logger", respective-
 * ly.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_psdo_oc(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_oot" - perf-
 * orm the action associated
 * with the recognition of
 * the " "only of type" pse-
 * udo class" production, u-
 * sing the output selector
 * codes in "cdes", generic
 * cast of the left-hand-si-
 * de symbol to contain the
 * result of the reduction in
 * "lhs", reduce items in "i-
 * tems", the number of which
 * is given in "num" and the
 * memory allocator and error
 * logger given in "allocs"
 * and "logger", respective-
 * ly.  Returns zero on err-
 * or, non-zero otherwise */
static int reduce_psdo_oot(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_empty" - pe-
 * rform the action associa-
 * ted with the recognition
 * of the " "empty element"
 * pseudo class" productio-
 * n, using the output sele-
 * ctor codes in "cdes", ge-
 * neric cast of the left-h-
 * and-side symbol to conta-
 * in the result of the red-
 * uction in "lhs", reduce
 * items in "items", the nu-
 * mber of which is given in
 * "num" and the memory all-
 * ocator and error logger
 * given in "allocs" and "l-
 * ogger", respectively.  R-
 * eturns zero on error, no-
 * n-zero otherwise */
static int reduce_psdo_empty(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_fc" - perf-
 * orm the action associa-
 * ted with the recognition
 * of the " "first child" p-
 * seudo class" production,
 * using the output select-
 * or codes in "cdes", gen-
 * eric cast of the left-h-
 * and-side symbol to conta-
 * in the result of the red-
 * uction in "lhs", reduce
 * items in "items", the nu-
 * mber of which is given in
 * "num" and the memory all-
 * ocator and error logger
 * given in "allocs" and "l-
 * ogger", respectively.  R-
 * eturns zero on error, no-
 * n-zero otherwise */
static int reduce_psdo_fc(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}
/* "reduce_psdo_nm" - perf-
 * orm the action associa-
 * ted with the recognition
 * of the " "nth match ps-
 * eudo class" production,
 * using the output select-
 * or codes in "cdes", gen-
 * eric cast of the left-h-
 * and-side symbol to conta-
 * in the result of the red-
 * uction in "lhs", reduce
 * items in "items", the nu-
 * mber of which is given in
 * "num" and the memory all-
 * ocator and error logger
 * given in "allocs" and "l-
 * ogger", respectively.  R-
 * eturns zero on error, no-
 * n-zero otherwise */
static int reduce_psdo_nm(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_psdo_nlm" - per-
 * form the action associa-
 * ted with the recognition
 * of the " "nth last match
 * pseudo class" production,
 * using the output select-
 * or codes in "cdes", gen-
 * eric cast of the left-h-
 * and-side symbol to conta-
 * in the result of the red-
 * uction in "lhs", reduce
 * items in "items", the nu-
 * mber of which is given in
 * "num" and the memory all-
 * ocator and error logger
 * given in "allocs" and "l-
 * ogger", respectively.  R-
 * eturns zero on error, no-
 * n-zero otherwise */
static int reduce_psdo_nlm(
                   void *cdes,
                   void *lhs,
                   union bdgm_reduce_item *items,
                   int num,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	return 1;
}

/* "reduce_ns_prfx_any" - perform the
 * action associated with the recogn-
 * ition of an "any name-space prefix"
 * production, using the output sele-
 * ctor codes in "cdes", generic cast
 * of the left-hand-side symbol to
 * contain the result of the reduction
 * in "lhs", reduce items in "items",
 * the number of which is given in
 * "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_ns_prfx_any(void *cdes,
                              void *lhs,
                              union bdgm_reduce_item *items,
                              int num,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
	return 1;
}

/* "reduce_ns_prfx_none" - perform the
 * action associated with the recogn-
 * ition of an "no name-space prefix"
 * production, using the output sele-
 * ctor codes in "cdes", generic cast
 * of the left-hand-side symbol to
 * contain the result of the reduction
 * in "lhs", reduce items in "items",
 * the number of which is given in
 * "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_ns_prfx_none(void *cdes,
                               void *lhs,
                               union bdgm_reduce_item *items,
                               int num,
                               struct bd_allocs *allocs,
                               struct bd_logger *logger)
{
	return 1;
}

/* "reduce_type_elem_nme" - perform the
 * action associated with the recogn-
 * ition of an "element name type"
 * production, using the output sele-
 * ctor codes in "cdes", generic cast
 * of the left-hand-side symbol to
 * contain the result of the reduction
 * in "lhs", reduce items in "items",
 * the number of which is given in
 * "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise  */
static int reduce_type_elem_nme(void *cdes,
                                void *lhs,
                                union bdgm_reduce_item *items,
                                int num,
                                struct bd_allocs *allocs,
                                struct bd_logger *logger)
{
	return 1;
}

/* "reduce_prp_id" - perform the act-
 * ion associated with the recognit-
 * ion of a "selector ID property"
 * production, using the output sel-
 * ector codes in "cdes", generic
 * cast of the left-hand-side symbol
 * to contain the result of the redu-
 * ction in "lhs", reduce items in
 * "items", the number of which is
 * given in "num" and the memory all-
 * ocator and error logger given in
 * "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-
 * zero otherwise  */
static int reduce_prp_id(void *cdes,
                         void *lhs,
                         union bdgm_reduce_item *items,
                         int num,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
	return 1;
}

/* "reduce_cmbntr_adj" - perform the
 * action associated with the recogn-
 * ition of a selector "adjacent sib-
 * ling combinator" * production, us-
 * ing the output selector codes in
 * "cdes", generic cast of the left-
 * hand-side symbol to contain the
 * result of the reduction in "lhs",
 * reduce items in "items", the num-
 * ber of which is given in "num"
 * and the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_cmbntr_adj(void *cdes,
                             void *lhs,
                             union bdgm_reduce_item *items,
                             int num,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
	return 1;
}

/* "reduce_cmbntr_sbl" - perform the
 * action associated with the recogn-
 * ition of a selector "general sib-
 * ling combinator" production, using
 * the output selector codes in "cdes",
 * generic cast of the left-hand-side
 * symbol to contain the result of the
 * reduction in "lhs", reduce items in
 * "items", the number of which is giv-
 * en in "num" and the memory allocat-
 * or and error logger given in "allo-
 * cs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_cmbntr_sbl(void *cdes,
                             void *lhs,
                             union bdgm_reduce_item *items,
                             int num,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
	return 1;
}

/* "reduce_add_smple_slctr_prp" - per-
 * form the action associated with the
 * recognition of the selector "add sim-
 * ple selector property" production,
 * using the output selector codes in
 * "cdes", generic cast of the left-
 * hand-side symbol to contain the res-
 * ult of the reduction in "lhs", red-
 * uce items in "items", the number of
 * which is given in "num" and the mem-
 * ory allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-zero
 * otherwise */
static int reduce_add_smple_slctr_prp(
                     void *cdes,
                     void *lhs,
                     union bdgm_reduce_item *items,
                     int num,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* emit a "logical and" of the
     * two simple selectors prpert-
     * ies in the production, since
     * both must match for the prop-
     * erty sequence   to match */
    return bdev_emit(cdes,
                     NULL,
                     bdsl_cde_and,
                     NULL,
                     allocs,
                     logger);
}

/* "reduce_start_smple_slctr_prps" -
 * perform the action associated with
 * the recognition of a start simple
 * selector property list" product-
 * ion, using the output selector
 * codes in "cdes", generic cast of
 * the left-hand-side symbol to con-
 * tain the result of the reduction
 * in "lhs", reduce items in "items",
 * the number of which is given in
 * "num" and the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise  */
static int reduce_start_smple_slctr_prps(
                     void *cdes,
                     void *lhs,
                     union bdgm_reduce_item *items,
                     int num,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
	/* nothing to emit to
	 * start a simple sel-
	 * ector property
	 * sequence  */
	return 1;
}

/* "ld_prsr" initialisation function to load
 * the lexical rules and grammar productions
 * of ixof the parser "prsr" to output an
 * evaluator programme (the evaluator's run-
 * time) which generates a match if the
 * "prsr"'s input (known as a "selector")
 * "selects" the current element of a docu-
 * ment using the memory allocator  */
static int ld_prsr(struct bdpr_parser *prsr,
                   void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdev_cdes *cdes =          /* output */
              bdev_cdes_init(evltr,   /* code list */
                             allocs,
                             logger);

    /* allocate a block of sequentially
     * increasing allocated states, bet-
     * ween which the analyser will tra-
     * nsition, from the analyser's pool
     * of available states */
    int name_start_state   = bdlx_new_state(prsr->lex),
        add_name_state     = bdlx_new_state(prsr->lex),
        end_name_state     = bdlx_new_state(prsr->lex),
        ref_start_state    = bdlx_new_state(prsr->lex),
        add_ref_state      = bdlx_new_state(prsr->lex),
        end_ref_state      = bdlx_new_state(prsr->lex),
        start_id_state     = bdlx_new_state(prsr->lex),
        add_id_state       = bdlx_new_state(prsr->lex),
        start_dq_str_state = bdlx_new_state(prsr->lex),
        add_dq_str_state   = bdlx_new_state(prsr->lex),
        start_sq_str_state = bdlx_new_state(prsr->lex),
        add_sq_str_state   = bdlx_new_state(prsr->lex),
        start_esc_state    = bdlx_new_state(prsr->lex),
        start_num_state    = bdlx_new_state(prsr->lex),
        name_add_esc_state = bdlx_new_state(prsr->lex),
        start_dig_state    = bdlx_new_state(prsr->lex),
        add_dig_state      = bdlx_new_state(prsr->lex),
        neg_num_state      = bdlx_new_state(prsr->lex),
        add_esc_state      = bdlx_new_state(prsr->lex);

        /* load the selector token */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdsl_tok),
                        (bdlx_tok_init_fn)
                            init_tok,
                        (bdlx_tok_type_fn)
                            tok_type,
                        (bdlx_tok_reset_fn)
                            tok_reset,
                        (bdlx_tok_name_fn)
                            tok_name)))
        return 0;

    /* ... initialise evaluator */
    if (!(evltr = bdev_init(bdev_exec_type_intrp,
                            NULL,
                            allocs,
                            logger)))
        return 0;

    /* [populate evaluator's codes */
    if (!(bdev_add_cde(evltr,
                       bdsl_cde_attr,
                       apply_attr,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_nmed,
                       apply_nmed,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_psdo,
                       apply_psdo,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_if,
                       apply_if,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_if_n,
                       apply_if_n,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_or,
                       apply_or,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
         return 0;

    if (!(bdev_add_cde(NULL,
                       bdsl_cde_and,
                       apply_and,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
         return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_not,
                       apply_not,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
         return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_ns,
                       apply_ns,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_nns,
                       apply_nns,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_a_ns,
                       apply_a_ns,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_a_nns,
                       apply_a_nns,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_id,
                       apply_id,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_clss,
                       apply_class,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_strts,
                       apply_starts,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_ends,
                       apply_ends,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_sbstr,
                       apply_sbstr,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_frst,
                       apply_frst,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_lst,
                       apply_lst,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_rt,
                       apply_rt,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_only,
                       apply_only,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_empty,
                       apply_empty,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_sbr,
                       apply_sbr,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_ret,
                       apply_ret,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_end,
                       apply_end,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdsl_cde_mtch,
                       apply_mtch,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    /* add the lexical analyser
     * definitions and rules */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "hex",
                       "[a-fA-F0-9]")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "nasc",
                       "[\\x200-\\x377]")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "name_strt",
                       "[_a-z]|{nasc}")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "name_chr",
                           "[_a-z0-9\\-]"
                           "|{nasc}")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "sqstr_chr",
                           "[^\\n\\r\\"
                           "f\'\\\\]"
                           "|{nasc}")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "dqstr_chr",
                           "[^\\n\\r\\"
                           "f\"\\\\]"
                           "|{nasc}")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start}",
                        bdlx_start_state,
                        name_start_state,
                        (bdlx_trans_fn)
                            start_name,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "/{name_start}",
                        bdlx_start_state,
                        ref_start_state,
                        (bdlx_trans_fn)
                            start_ref,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        start_id_state,
                        add_id_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        ref_start_state,
                        add_ref_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        start_name_state,
                        add_name_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        start_ref_state,
                        add_ref_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        add_ref_state,
                        add_ref_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}/",
                        add_ref_state,
                        0,
                        (bdlx_trans_fn)
                            end_ref,
                        NULL,
                        1,
                        bdsl_tok_ref)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_chr}",
                        add_id_state,
                        add_id_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_chr}",
                        add_id_state,
                        0,
                        (bdlx_trans_fn)
                            end_name,
                        NULL,
                        1,
                        bdsl_tok_id)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_char}",
                        name_start_state,
                        add_name_state,
                        add_name_cp,
                        NULL,
                        0,
                        9)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\\\",
                        name_start_state,
                        start_esc_state,
                        start_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\\\",
                        add_name_state,
                        start_esc_state,
                        start_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        start_esc_state,
                        add_esc_state,
                        start_name_esc,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        name_add_esc_state,
                        name_add_esc_state,
                        add_name_esc,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^hex}",
                        start_esc_state,
                        add_name_state,
                        end_name_esc,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_chr}",
                        add_name_state,
                        end_name_state,
                        NULL,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\(",
                        add_name_state,
                        0,
                        (bdlx_trans_fn)
                            end_name,
                        NULL,
                        1,
                        bdsl_tok_fn)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\(]",
                        add_name_state,
                        0,
                        (bdlx_trans_fn)
                            end_name,
                        NULL,
                        1,
                        bdsl_tok_nme)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[\\t\\r\\n\\f]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_ws)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        start_dq_str_state,
                        (bdlx_trans_fn)
                            start_buffer,
                            NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        start_sq_str_state,
                        (bdlx_trans_fn)
                        start_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "(",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_lpr)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dqstr_chr}",
                        start_dq_str_state,
                        add_dq_str_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{sqstr_chr}",
                        start_sq_str_state,
                        add_sq_str_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dqstr_chr}",
                        add_dq_str_state,
                        add_dq_str_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{sqstr_chr}",
                        add_sq_str_state,
                        add_sq_str_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        start_dq_str_state,
                        0,
                        (bdlx_trans_fn)
                            str_end,
                        NULL,
                        1,
                        bdsl_tok_str)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        start_sq_str_state,
                        0,
                        (bdlx_trans_fn)
                            str_end,
                            NULL,
                        1,
                        bdsl_tok_str)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        add_dq_str_state,
                        0,
                        (bdlx_trans_fn)
                            str_end,
                        NULL,
                        1,
                        bdsl_tok_str)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        add_sq_str_state,
                        0,
                        (bdlx_trans_fn)
                            str_end,
                        NULL,
                        1,
                        bdsl_tok_str)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        bdlx_start_state,
                        start_num_state,
                        (bdlx_trans_fn)
                            start_num,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        start_num_state,
                        add_dig_state,
                        (bdlx_trans_fn)
                            dig_add,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        add_dig_state,
                        add_dig_state,
                        (bdlx_trans_fn)
                            dig_add,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^0-9]",
                        add_dig_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_int)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ")",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_rpr)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_lsq)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_rsq)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "#",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_hsh)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\.",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_clss)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "~=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_incl)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "^=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_pre)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "$=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_pst)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "*=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_sub)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\*$\\^|]=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_eq)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\|[^\\|\\=]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_ppe)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\|=",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_incl)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ">",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_gt)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "+[^0-9]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_plus)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "+[0-9]",
                        bdlx_start_state,
                        start_dig_state,
                        start_num,
                        NULL,
                        1,
                        bdsl_tok_plus)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "-[^0-9]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_sub)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "-[0-9]",
                        bdlx_start_state,
                        neg_num_state,
                        (bdlx_trans_fn)
                            neg_num,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        NULL,
                        neg_num_state,
                        start_num_state,
                        start_num,
                        NULL,
                        0,
                        0)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ",",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_cma)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ":",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_cln)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "~",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_tlde)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\*",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_ast)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "n[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_n)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "i[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_i)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                       "root[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_rt)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "not[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nt)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "nth\\-child[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nc)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "nth\\-last\\-"
                            "child[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nlc)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "nth\\-last"
                        	"\\-of\\-"
                        	"type[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nlt)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "first\\-"
                        	"of\\-type"
                            "[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_fot)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "last\\-"
                        	"of\\-type",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_lot)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "only\\-"
                        	"child",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_oc)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "only\\-"
                        	"of\\-type",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_oot)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "empty"
                            "[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_empty)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "nth\-"
                            "match"[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nm)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "nth\-"
                            "last-"
                            "match"[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_nlm)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "matches"[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_mtchs)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "even[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_evn)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "odd[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_odd)))
       return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "lang[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdsl_tok_kwd_lng)))
       return 0;

    /* ...a nd add the parser's const-
     * ituent grammar productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_slctr_grp,
                        1,
                        reduce_start_grp,
                        bdsl_nt_slctr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_slctr_grp,
                        1,
                        reduce_slctr_grp,
                        bdsl_nt_slctr,
                        bdsl_nt_slctr_end
                        )))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_slctr_end,
                        0,
                        reduce_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_slctr_grp,
                        3,
                        reduce_add_to_grp,
                        bdsl_nt_slctr,
                        bdsl_tok_cma,
                        bdsl_nt_slctr_grp)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_slctr,
                        1,
                        reduce_start_slctr,
                        bdsl_nt_smple_slctr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_slctr,
                        3,
                        reduce_add_to_slctr,
                        bdsl_nt_smple_slctr,
                        bdsl_nt_cmbntr,
                        bdsl_nt_slctr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_ws_list,
                        1,
                        NULL,
                        bdsl_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_ws_list,
                        0,
                        NULL)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ws_list,
                        2,
                        NULL,
                        bdsl_tok_ws,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        2,
                        reduce_cmbntr_chld,
                        bdsl_tok_plus,
                        bdsl_nt_ws_list)))
         return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        2,
                        reduce_cmbntr_sbl,
                        bdsl_tok_tlde,
                        bdsl_nt_ws_list)))
         return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        2,
                        reduce_cmbntr_grnd,
                        bdsl_tok_gt,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        2,
                        reduce_cmbntr_adj,
                        bdsl_tok_tlde,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        1,
                        reduce_cmbntr_dsc,
                        bdsl_nt_ws_seq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_cmbntr,
                        1,
                        reduce_cmbntr_ref,
                        bdsl_nt_ws_seq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr_prp_seq,
                        2,
                        reduce_add_smple_slctr_prp,
                        bdsl_nt_smple_slctr_prp,
                        bdsl_nt_smple_slctr_prp_seq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cdes,
                        bdsl_nt_smple_slctr_prp_seq,
                        1,
                        reduce_start_smple_slctr_prps,
                        bdsl_nt_smple_slctr_prp)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr_prp,
                        2,
                        reduce_prp_id,
                        bdsl_tok_hsh,
                        bdsl_tok_id)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr_prp,
                        2,
                        reduce_prp_clss,
                        bdsl_tok_clss,
                        bdsl_tok_id)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ns_prfx,
                        1,
                        reduce_ns_prfx,
                        bdsl_tok_ppe)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ns_prfx,
                        2,
                        reduce_ns_prfx_elem,
                        bdsl_tok_id,
                        bdsl_tok_ppe)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_elem_nme,
                        1,
                        reduce_elem_nme,
                        bdsl_tok_id)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_elem_nme,
                        0,
                        reduce_elem_nme_empty)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ns_prfx,
                        2,
                        reduce_ns_prfx_any,
                        bdsl_tok_ast,
                        bdsl_tok_ppe)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ns_prfx,
                        1,
                        reduce_ns_prfx_none,
                        bdsl_tok_ppe)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_ns_prfx,
                        2,
                        reduce_ns_prfx_nme,
                        bdsl_tok_id,
                        bdsl_tok_ppe)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_type,
                        2,
                        reduce_type_elem_nme,
                        bdsl_nt_ns_prfx,
                        bdsl_nt_elem_nme)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_type,
                        1,
                        reduce_type,
                        bdsl_nt_elem_nme)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_unvrsl,
                        2,
                        reduce_unvrsl_ns,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_ast)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_unvrsl,
                        1,
                        reduce_unvrsl_ast,
                        bdsl_tok_ast)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_pre,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pre,
                        bdsl_nt_ws_list,
                        bdsl_tok_str,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_pst,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_str,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_sub,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_sub,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_eq,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_eq,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_incl,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_incl,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        9,
                        reduce_attr_dm,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_dm,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_pre,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pre,
                        bdsl_nt_ws_list,
                        bdsl_tok_str,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_pst,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_str,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_sub,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_sub,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_eq,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_eq,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_incl,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_incl,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_attr,
                        10,
                        reduce_attr_ns_dm,
                        bdsl_tok_lsq,
                        bdsl_nt_ws_list,
                        bdsl_nt_ns_prfx,
                        bdsl_tok_id,
                        bdsl_nt_ws_list,
                        bdsl_tok_pst,
                        bdsl_nt_ws_list,
                        bdsl_tok_dm,
                        bdsl_nt_ws_list,
                        bdsl_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr,
                        5,
                        reduce_smple_slctr_type_psdo,
                        bdsl_nt_smple_slctr_strt,
                        bdsl_nt_type,
                        bdsl_nt_smple_slctr_prp_seq,
                        bdsl_nt_psdo,
                        bdsl_nt_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr,
                        4,
                        reduce_smple_slctr_unvrsl,
                        bdsl_nt_smple_slctr_strt,
                        bdsl_nt_unvrsl,
                        bdsl_nt_smple_slctr_prp_seq,
                        bdsl_nt_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr,
                        4,
                        reduce_smple_slctr_type,
                        bdsl_nt_smple_slctr_strt,
                        bdsl_nt_type,
                        bdsl_nt_smple_slctr_prp_seq,
                        bdsl_nt_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr,
                        6,
                        reduce_smple_slctr_unvrsl_psdo,
                        bdsl_nt_smple_slctr_strt,
                        bdsl_nt_unvrsl,
                        bdsl_nt_smple_slctr_prp_seq,
                        bdsl_tok_cln,
                        bdsl_nt_psdo,
                        bdsl_nt_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr,
                        6,
                        reduce_smple_slctr_type_psdo,
                        bdsl_nt_smple_slctr_strt,
                        bdsl_nt_type,
                        bdsl_nt_smple_slctr_prp_seq,
                        bdsl_tok_cln,
                        bdsl_nt_psdo,
                        bdsl_nt_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_type,
                        3,
                        reduce_psdo_plce_type,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_ntype,
                        3,
                        reduce_psdo_plce_ntype,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_nltype,
                        3,
                        reduce_psdo_plce_nltype,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_evn,
                        bdsl_nt_ws_list,
                        bdsl_tok_kwd_evn,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_odd,
                        bdsl_nt_ws_list,
                        bdsl_tok_kwd_odd,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_i,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_n,
                        bdsl_nt_ws_list,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        4,
                        reduce_psdo_plce_in,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        6,
                        reduce_psdo_plce_ino,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr_strt,
                        0,
                        reduce_smple_slctr_strt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_smple_slctr_end,
                        1,
                        reduce_smple_slctr_end)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_id,
                        bdsl_tok_id)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        4,
                        reduce_psdo_id_arg,
                        bdsl_tok_fn,
                        bdsl_nt_expr,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        4,
                        reduce_psdo_not,
                        bdsl_tok_lpr,
                        bdsl_tok_kwd_not,
                        bdsl_nt_smple_slctr,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_rt,
                        bdsl_tok_kwd_rt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        2,
                        reduce_psdo_nc,
                        bdsl_tok_kwd_nc,
                        bdsl_nt_psdo_plce_ntype)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        2,
                        reduce_psdo_nlc,
                        bdsl_tok_kwd_nlc,
                        bdsl_nt_psdo_plce_nltype)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        2,
                        reduce_psdo_nt,
                        bdsl_tok_kwd_nt,
                        bdsl_nt_psdo_plce_type)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        2,
                        reduce_psdo_nlt,
                        bdsl_tok_kwd_nlt,
                        bdsl_nt_psdo_plce_ltype)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_fot,
                        bdsl_tok_kwd_fot)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_lot,
                        bdsl_tok_kwd_lot)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_oc,
                        bdsl_tok_kwd_oc)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_oot,
                        bdsl_tok_kwd_oot)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_empty,
                        bdsl_tok_kwd_mty)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_fc,
                        bdsl_tok_kwd_fc)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_nm,
                        bdsl_tok_kwd_nm)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_nlm,
                        bdsl_tok_kwd_nlm)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_lot,
                        bdsl_tok_kwd_lot)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo,
                        1,
                        reduce_psdo_lc,
                        bdsl_tok_kwd_lc)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_evn,
                        bdsl_nt_ws_list,
                        bdsl_tok_kwd_evn,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        3,
                        reduce_psdo_plce_i,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        4,
                        reduce_psdo_plce_in,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        4,
                        reduce_psdo_plce_n,
                        bdsl_nt_ws_list,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce,
                        6,
                        reduce_psdo_plce_ino,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_tok_kwd_n,
                        bdsl_nt_ws_list,
                        bdsl_tok_int,
                        bdsl_nt_ws_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_type,
                        4,
                        reduce_psdo_plce,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_ntype,
                        4,
                        reduce_psdo_plce_ntype,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        prsr,
                        bdsl_nt_psdo_plce_ltype,
                        3,
                        reduce_psdo_plce_ltype,
                        bdsl_tok_lpr,
                        bdsl_nt_psdo_plce,
                        bdsl_tok_rpr)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         prsr,
                         bdsl_nt_psdo_plce_nltype,
                         3,
                         reduce_psdo_plce_nltype,
                         bdsl_tok_lpr,
                         bdsl_nt_psdo_plce,
                         bdsl_tok_rpr);
}

/* "bdsl_init" - load the selector system
 * with its constituent lexical rules and
 * grammar productions using the memory
 * allocator and error logger given in
 * "allocs" and "logger", respectively
 * Returns zero on error, non-zero other-
 * wise */
int bdsl_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    prsr = bdpr_init(allocs,
                     logger,
                     (bdpr_init_fn)
                           ld_prsr);

    return prsr != NULL;
}

/* "bdsl_select" - sets "spcfty" to a non-
 * NULL value if the selector text given in
 * "slctr" matches theelement given in "elem"
 * (with the fields of "spcfty" indicating
 * the specifity of a successful match),
 * using the memory allocator and error log-
 * ger given in "allocs" and "logger" respe-
 * ctively.  "cse" indicates whether the
 * selector string is case-sensitive.  Ret-
 * urns zero on error, non-zero otherwise */
int dbsl_select(struct bd_allocs *allocs,
		        struct bd_logger *logger,
                struct bdut_str slctr,
                int cse,
		        struct bdxl_node *nde,
                struct bdsl_spcfty *spcfty)
{
	struct bdsl_rt *rt =      /* selector */
	       (struct bdsl_rt *) /* parser's */
	       prsr->rt;          /* run-time */
    struct bdev_val val = {   /* node to be */
	                          /* pushed onto */
	    bdsl_stk_nde,         /* run-time's */
	    nde                   /* evaluator */
    };                        /* run-time */
                              /* stack */

    /* start by pushing the element
     * to select onto the run-time
     * evaluator's erun-time stack */
    if (!(bdev_emit(rt->cdes,
                    NULL,
                    bdev_res_cde_psh_vl,
                    &val,
                    allocs,
                    logger)))
        return 0;


    /* run the selector with the
     * parser */
    if (!(bdpr_run(allocs,
                   logger,
                   prsr,
			       (void *) rt,
			       slctr)))
        return 0;

    /* then execute the gener-
     * ated run-time's mmain
     * evaluator codes */
    if (!(bdev_run(rt->cdes,
                   bdsl_cde_end,
                   allocs,
                   logger)))
        return 0;

    /* and, if the element
     * is not selected (in which
     * case the top of the eval-
     * uator's run-time stack
     * will be zero), set the
     * specifity to NULL */
    if (!((int) (rt->cdes->
                 evltr->rt_top->
                 val)->val))
		spcfty = NULL;

    /* or, otherwise, set the
     * specifity from the run-
     * time's specifity */
    else
        spcfty = rt->spcfty;
}

/* ""bdsl_prse" - convert and return
 * an internal representation (a
 * selector run-time) of the selector
 * string given in "slctr", using the
 * memory allocator and error logger
 * in "allocs" snd "logger", respect-
 * ively,  The internal representat-
 * ion so created may then be repeat-
 * edly executed (during the node sel-
 * ection process) more efficiently
 * Returns NULL
 * on error */
struct bdsl_rt *dbsl_prse(
          struct bd_allocs *allocs,
          struct bd_logger *logger,
          char *slctr)
{
    struct bdsl_rt *ret; /* run-ime to
                          * return */
    struct bdut_str str; /* selector as
                          * code-point
                          * string */

    /* generate tun-time by pars-
     * ing skwect string  */
    bdpr_run(allocs,
             logger,
             prsr,
             ret,
             str);

    return ret;
}

/* "bdsl_exec" - "executes" (by
 * running its evaluator against
 * the XML node in "nde") the
 * pre-parsed selector run-time
 * in "rt", and sets "spcfty"
 * to the run-time's specifity
 * after execution (or to NULL
 * if "nde" does not match the
 * selector), using the memory
 * allocator and error logger
 * given in "allocs" and "log-
 * ger", respectively.  This
 * routine allows repeated mat-
 * ching of selectors against
 * nodes without the over-head
 * of prior parsing.  Returns
 * zero on error, non-zero oth-
 * erwise */
int bdsl_exec(struct bd_allocs *allocs,
              struct bd_logger *logger,
              struct bdsl_rt *rt,
              struct bdxl_node *nde,
              struct bdsl_spcfty *spcfty)
{
    return bdev_exec(rt->cdes,
                     bdsl_cde_end,
                     allocs,
                     logger);
}
