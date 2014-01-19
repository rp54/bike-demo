
/*********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* xpth.c - function implementations related to
* the W3C's XML Path Language (X-PATH) 3.0, a
* mechanis by which an XML node may be located
* via a "path expression" selative to some
* context node.
*
* Copyright (C) 2007-2014, Russell Potter. All
* rights reserved.
* 
***********************************************************************/

#include <bikedemo/sllocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xpath.h>

/* "apply_cma" - implements ("applies") the
 * "comma" operator in the X-PATH evaluator
 * given in "evltr", using the memory alloc-
 * ator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_cma(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_for" - implements ("applies") the
 * "lfor" operator in the X-PATH evaluator
 * given in "evltr", using the memory allo-
 * cator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_for(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_let" - implements ("applies") the
 * "let" operator in the X-PATH evaluator
 * given in "evltr", using the memory allo-
 * cator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_let(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sme" - implements ("applies") the
 * "some" operator in the X-PATH evaluator
 * given in "evltr", using the memory alloc-
 * ator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_sme(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_evry" - implements ("applies") the
 * "every" operator in the X-PATH evaluator
 * given in "evltr", using the memory alloc-
 * ator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_evry(struct bdev_evltr *evltr,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_if" - implements ("applies") the
 * "if" operator in the X-PATH evaluator
 * given in "evltr", using the memory alloc-
 * ator, error logger and opaque parameter
 * given in "allocs", "logger" and "opaque",
 * respectively.  This routine serves as the
 * X-PATH evaluator's "apply" call-back for
 * this operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_if(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_seq" - implements ("applies") the
 * "string equal to" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine ser-
 * ves as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_seq(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sne" - implements ("applies") the
 * "string not equal to" operator in the X-
 * PATH evaluator given in "evltr", using the
 * memory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine ser-
 * ves as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_sne(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_slt" - implements ("applies") the
 * "string less than" operator in the X-PATH
 * evaluator given in "evltr", using the mem-
 * ory allocator, error logger and opaque pa-
 * rameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine ser-
 * ves as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_slt(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sgt" - implements ("applies") the
 * "string greater than" operator in the X-
 * PATH evaluator given in "evltr", using
 * the memory allocator, error logger and
 * opaque parameter given in "allocs", "lo-
 * gger" and "opaque", respectively.  This
 * routine serves as the X-PATH evaluator's
 * "apply" callback for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_sgt(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sle" - implements ("applies") the
 * "string less than or equal to" operator
 * in the X-PATH evaluator given in "evltr",
 * using the memory allocator, error logger
 * and opaque parameter given in "allocs",
 * "logger" and "opaque", respectively.  This
 * routine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.  Re-
 * turns zero on error, non-zero otherwise */
static int apply_sle(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sge" - implements ("applies") the
 * "string greater than or equal to" opera-
 * tor in the X-PATH evaluator given in  "ev-
 * ltr", using the memory allocator, error
 * logger and opaque parameter given in "al-
 * locs", "logger" and "opaque", respectiv-
 * ely.  This routine serves as the X-PATH
 * evaluator's "apply" call-back for this op-
 * erator.  Returns zero on error, non-zero
 * otherwise */
static int apply_sge(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_eq" - implements ("applies") the
 * "equal to" operator in the X-PATH evalua-
 * tor given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_eq(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_ne" - implements ("applies") the
 * "not equal to" operator in the X-PATH ev-
 * aluator given in "evltr", using the mem-
 * ory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_ne(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_lt" - implements ("applies") the
 * "less than" operator in the X-PATH ev-
 * aluator given in "evltr", using the mem-
 * ory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_lt(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_gt" - implements ("applies") the
 * "greater than" operator in the X-PATH ev-
 * aluator given in "evltr", using the mem-
 * ory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_gt(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_le" - implements ("applies") the
 * "less than or equal to" operator in the
 * X-PATH evaluator given in "evltr", using
 * the memory allocator, error logger and
 * opaque parameter given in "allocs", "log-
 * ger" and "opaque", respectively.  This
 * routine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_le(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_ge" - implements ("applies") the
 * "greater than or equal to" operator in
 * the X-PATH evaluator given in "evltr",
 * using the memory allocator, error logg-
 * er and opaque parameter given in "allo-
 * cs", "logger" and "opaque", respectively.
 * This routine serves as the X-PATH evalu-
 * ator's "apply" call-back for this opera-
 * tor.  Returns zero on error, non-zero
 * otherwise */
static int apply_ge(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_lsh" - implements ("applies") the
 * "bit left shift" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This rout-
 * ine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_lsh(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_rsh" - implements ("applies") the
 * "bit right shift" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This rout-
 * ine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_rsh(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_lor" - implements ("applies") the
 * "logical or" operator in the X-PATH eva-
 * luator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This rout-
 * ine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_lor(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_to" - implements ("applies") the
 * "to" operator in the X-PATH evaluator
 * given in "evltr", using the memory all-
 * ocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "app-
 * ly" call-back for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_to(struct bdev_evltr *evltr,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "apply_add" - implements ("applies") the
 * "add" operator in the X-PATH evaluator
 * given in "evltr", using the memory all-
 * ocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "app-
 * ly" call-back for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_add(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sub" - implements ("applies") the
 * "subtract" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "app-
 * ly" call-back for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_sub(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_unn" - implements ("applies") the
 * "union" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "app-
 * ly" call-back for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_unn(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_sunn" - implements ("applies") the
 * "set union" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and "op-
 * aque", respectively.  This routine
 * serves as the X-PATH evaluator's "app-
 * ly" call-back for this operator.  Ret-
 * urns zero on error, non-zero otherwise */
static int apply_sunn(struct bdev_evltr *evltr,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_isct" - implements ("applies") the
 * "set intersect" operator in the X-PATH ev-
 * aluator given in "evltr", using the memory
 * allocator, error logger and opaque parame-
 * ter given in "allocs", "logger" and "opa-
 * que", respectively.  This routine serves
 * as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_isct(struct bdev_evltr *evltr,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_excpt" - implements ("applies") the
 * "set except" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque parame-
 * ter given in "allocs", "logger" and "opa-
 * que", respectively.  This routine serves
 * as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_excpt(struct bdev_evltr *evltr,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "apply_mul" - implements ("applies") the
 * "mutiply" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque parame-
 * ter given in "allocs", "logger" and "opa-
 * que", respectively.  This routine serves
 * as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_mul(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_mod" - implements ("applies") the
 * "modulus" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque parame-
 * ter given in "allocs", "logger" and "opa-
 * que", respectively.  This routine serves
 * as the X-PATH evaluator's "apply" call-
 * back for this operator.  Returns zero on
 * error, non-zero otherwise */
static int apply_mod(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_div" - implements ("applies") the
 * "divide" operator in the X-PATH evaluat-
 * or given in "evltr", using the memory al-
 * locator, error logger and opaque param-
 * eter given in "allocs", "logger" and "op-
 * aque", respectively.  This routine ser-
 * ves as the X-PATH evaluator's "apply" ca-
 * ll-back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_div(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_instnce" - implements ("applies")
 * the "instance of" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine ser-
 * ves as the X-PATH evaluator's "apply" ca-
 * ll-back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_instnce(struct bdev_evltr *evltr,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    return 1;
}

/* "apply_idiv" - implements ("applies") the
 * "integer divide" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_idiv(struct bdev_evltr *evltr,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_trt" - implements ("applies") the
 * "treat as" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_trt(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_cst" - implements ("applies") the
 * "cast as" operator in the X-PATH evalu-
 * ator given in "evltr", using the memory
 * allocator, error logger and opaque para-
 * meter given in "allocs", "logger" and
 * "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_cst(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_cstble" - implements ("applies")
 * the "castable as" operator in the X-PATH
 * evaluator given in "evltr", using the
 * memory allocator, error logger and opaq-
 * ue parameter given in "allocs", "logger"
 * and "opaque", respectively.  This routine
 * serves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_cstble(struct bdev_evltr *evltr,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    return 1;
}

/* "apply_pos" - implements ("applies") the
 * "make positive" operator in the X-PATH
 * evaluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine se-
 * rves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_pos(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_neg" - implements ("applies")
 * the "negate" operator in the X-PATH ev-
 * aluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger" and
 * "opaque", respectively.  This routine se-
 * rves as the X-PATH evaluator's "apply"
 * call-back for this operator.  Returns
 * zero on error, non-zero otherwise */
static int apply_neg(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_map" - implements ("applies")
 * the "map" operator in the X-PATH ev-
 * aluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This rout-
 * ine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_map(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_pth" - implements ("applies")
 * the "path" operator in the X-PATH ev-
 * aluator given in "evltr", using the me-
 * mory allocator, error logger and opaque
 * parameter given in "allocs", "logger"
 * and "opaque", respectively.  This rout-
 * ine serves as the X-PATH evaluator's
 * "apply" call-back for this operator.
 * Returns zero on error, non-zero other-
 * wise */
static int apply_pth(struct bdev_evltr *evltr,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_arry" - implements ("applies")
 * the "take array index" operator in the
 * X-PATH evaluator given in "evltr", us-
 * ing the memory allocator, error logger
 * and opaque parameter given in "allocs",
 * "logger" and "opaque", respectively.
 * This routine serves as the X-PATH eval-
 * uator's "apply" call-back for this op-
 * erator.  Returns zero on error, non-
 * zero otherwise */
static int apply_arry(struct bdev_evltr *evltr,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "init_tok" - performs one-time
 * initialisation of the X-PATH
 * token in "tok", using the memory
 * allocator and error logger in
 * "allocs" and "logger", respect-
 * ively.  Returns zero on error,
 * non-zero otherwise */
static void init_tok(struct bdlx_tok *tok,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxp_tok *xp_tok  =
             (struct bdxp_tok *)
             tok;

    /* reset the token's
     * number and string  */
    xp_tok->u.num = 0;
    xp_tok->u.str.len = 0;

    /* initialise token's buffer */
    xp_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));
}

/* "mv_tok_type" - returns the type
 * of the token in "tok", using the
 * memory allocator and error log-
 ( ger in "allocs" and "logger",
 * respectively.  Returns a nega-
 * tive value on error */
static int tok_type(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxp_tok *xp_tok  =
             (struct bdxp_tok *) tok;

    return xp_tok->type; /* return
                          * the type
                          * of the
                          * token */
}

/* "re_tok_reset" - performs
 * processing required before
 * each read of the JavaScript
 * regular expression token in
 * "tok", using the memory all-
 * ocator and error logger in
 * "allocs" and "logger", resp-
 * ectively */
static int re_tok_reset(
                 struct bdlx_tok *tok,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxp_tok *xp_tok  =
             (struct bdxp_tok *) tok;

    /* reset the token's
     * number and string  */
    xp_tok->u.num = 0;
    xp_tok->u.str.len = 0;

    /* and reset the token's
     * buffer */
    return bdbf_reset(xp_tok->u.buf,
                      allocs,
                      logger);
}

/* "tok_name"  - returns the message
 * table index of the name of the
 * token given in "tok", using the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  "exct" indicates
 * whether the name should indicate
 * the exact content if the token,
 * rather than its general name.
 * Returns a negative on an error */
tint tok_name(struct bdlx_tok *tok,
              int exct,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    return 0; /* do nothing here */
}

/* "reduce_xpth" - perform the action
 * associated with the recognition of
 * the "xpath -> expr" X-PATH production
 * on the generic cast of the parser out-
 * put in "out", generic cast of the le-
 * ft-hand-side symbol in "lhs" to conta-
 * in the result of the parser, "reduce"
 * items in "items", the number of which
 * is given in "num", and the memory all-
 * ocator and error logger give in "allo-
 * cs" and "logger", respectively.  Ret-
 * urns zero on error, non-zero otherw-
 * ise */
static int reduce_xpth(
                    void *out,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    return 1;
}

/* "ld_prsr" - populate, using the memory
 * allocator, error logger and opaque ara-
 * meter given in "allocs", "logger" and
 * "opaque", respectively, the JavaScript
 * parser "js" with its constituent lexi-
 * cal rules and grammar productions */
static int ld_prsr(struct bdpr_parser *js,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
           struct bdxp_prg *prg = bdev_init_cdes(allocs, 
                                                 logger);

    /* a block of sequentially increasing
     * allocated lexical states, between
     * which the analyser will transition,
     * allocated from the analyser's pool
     * of available states */
    int start_state            = bdlx_new_state(prsr->lex),
        ws_state               = bdlx_new_state(prsr->lex);
        num_start_state        = bdlx_new_state(prsr->lex),
        num_add_state          = bdlx_new_state(prsr->lex),
        num_at_frac_state      = bdlx_new_state(prsr->lex),
        num_frac_add_state     = bdlx_new_state(prsr->lex),
        num_at_exp_state       = bdlx_new_state(prsr->lex),
        num_exp_add_state      = bdlx_new_state(prsr->lex),
        num_end_state          = bdlx_new_state(prsr->lex),
        str_start_state        = bdlx_new_state(prsr->lex),
        str_add_state          = bdlx_new_state(prsr->lex);


    /* load the token */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdjs_tok),
                        (bdlx_tok_init_fn) init_tok,
                        (bdlx_tok_type_fn) tok_type,
                        (bdlx_tok_reset_fn) reset_tok,
                        (bdlx_tok_name_fn) tok_name)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_cma,
                      NULL,
                      apply_cma,
                      bdxp_op_prec_cma,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_for,
                      NULL,
                      apply_for,
                      bdxp_op_prec_for,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sme,
                      NULL,
                      apply_sme,
                      bdxp_op_prec_for,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_evry,
                      NULL,
                      apply_evry,
                      bdxp_op_prec_for,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_let,
                      NULL,
                      apply_let,
                      bdxp_op_prec_for,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_if,
                      NULL,
                      apply_if,
                      bdxp_op_prec_for,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_eq,
                      NULL,
                      apply_eq,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_ne,
                      NULL,
                      apply_ne,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_lt,
                      NULL,
                      apply_lt,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_le,
                      NULL,
                      apply_le,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_gt,
                      NULL,
                      apply_gt,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_ge,
                      NULL,
                      apply_ge,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_seq,
                      NULL,
                      apply_seq,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sne,
                      NULL,
                      apply_sne,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_slt,
                      NULL,
                      apply_slt,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sle,
                      NULL,
                      apply_ale,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sgt,
                      NULL,
                      apply_sgt,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sge,
                      NULL,
                      apply_sge,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_lsh,
                      NULL,
                      apply_lsh,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_rsh,
                      NULL,
                      apply_rsh,
                      bdxp_op_prec_rel,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_lor,
                      NULL,
                      apply_lor,
                      bdxp_op_prec_lor,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_to,
                      NULL,
                      apply_to,
                      bdxp_op_prec_to,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_add,
                      NULL,
                      apply_to,
                      bdxp_op_prec_add,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sub,
                      NULL,
                      apply_sub,
                      bdxp_op_prec_add,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_mul,
                      NULL,
                      apply_mul,
                      bdxp_op_prec_mul,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_div,
                      NULL,
                      apply_div,
                      bdxp_op_prec_mul,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_idiv,
                      NULL,
                      apply_idiv,
                      bdxp_op_prec_mul,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_mod,
                      NULL,
                      apply_imod,
                      bdxp_op_prec_mul,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_unn,
                      NULL,
                      apply_unn,
                      bdxp_op_prec_unn,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_sunn,
                      NULL,
                      apply_sunn,
                      bdxp_op_prec_unn,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_isct,
                      NULL,
                      apply_isct,
                      bdxp_op_prec_isct,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_excpt,
                      NULL,
                      apply_excpt,
                      bdxp_op_prec_isct,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_excpt,
                      NULL,
                      apply_excpt,
                      bdxp_op_prec_isct,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_instnce,
                      NULL,
                      apply_instnce,
                      bdxp_op_prec_instnce,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_trt,
                      NULL,
                      apply_trt,
                      bdxp_op_prec_trt,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_cstble,
                      NULL,
                      apply_cstble,
                      bdxp_op_prec_cstble,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_cst,
                      NULL,
                      apply_cst,
                      bdxp_op_prec_cst,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_pos,
                      NULL,
                      apply_pos,
                      bdxp_op_prec_pos,
                      bdev_assc_r2l,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_neg,
                      NULL,
                      apply_neg,
                      bdxp_op_prec_pos,
                      bdev_assc_r2l,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_map,
                      NULL,
                      apply_map,
                      bdxp_op_prec_map,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_pth,
                      NULL,
                      apply_pth,
                      bdxp_op_prec_pth,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdxp_op_arry,
                      NULL,
                      apply_pth,
                      bdxp_op_prec_arry,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    /* ... and add the grammar prod-
     * uctions  */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxp_nt_xpth,
                        1,
                        (bdgm_reduce_fn)
                            reduce_xpth,
                        bdxp_nt_expr)))
        return 0;



}

/* "bdxp_slct" - returns the node
 * selected by the X-PATH express-
 * ion given in "expr", using the
 * document fragment the root of
 * which is given in "frag", us-
 * ing the memory allocator and
 * error logger given in "allocs"
 * and "logger" eters, respecti-
 * vely.  Returns NULL on error */
struct bdxl_node *bdxp_slct(
                       struct bdut_str *expr,
                       struct bdxl_node *frag,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdxl_node *ret; /* return
                            * value */

    /* the parser's run-time consists
     * of the document fragment of the
     * X-PATH expression */
    (struct bdxl_node *)
             prsr->rt = frag;

    /* run the parser iusing "expr"
     * as input, yielding the selected
     * node */
    if (!(bdpr_run(allocs,
                   logger,
                   prsr,
                   ret,
                   expr)))
        return NULL;
    return ret;
}
