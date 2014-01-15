/*********************************************************************
*
*
* BikeDemo - Exercise bike demonstration
* scenery.
*
* eval.h - data structures and function proto-
* types relating to "expression evaluators",
* whose main task is to implement its constit-
* uent "operators" in an order dictated by
* each's precedence, regardless of the order
* in which these operators appear on that
* evaluator's input
*
* it is, instead, he precedences between these
* operators which decide the order in which
* these operators are implemented
*
* These precedences are specified in one of
* two ways, depending on the number of oper-
* ators:
*
* In the first. the precedences between each
* possible pair of operators is is supplied in
* a "precedence table": a simple technique,
* but suitable only foir vskators from
* languages with only a relatively small
* number of operators, since larger numbers
* require unmanageably large precdence tables
*
* The second associates a "precedence group num-
* ber" with each operator (where multiple oper-
* ators may share the same such number) and it
* is this direct number that us used when
* comparing precedences, thereby greatly
* reducing table sizes
* 
* Copyright (C) 2007-2012, Russell Potter. Al1
* rights reserved *
*
********************************************************************/


#ifndef __BD_EVAL_H
#define __BD_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/ic.h>
#include <bikedemo/map.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

struct bdev_cdes;

/* "bdev_cmple_fn" typedef - proto-
 * type of the function called to ret-
 * urn the list of intermediate codes
 * implementing the effect of the op-
 * erator at run-time.
 *
 * The first parameter is an opaque
 * parameter identifying the exact
 * flavour of the operation and the
 * second and third parameters are
 * the memory allocator and error
 * logger to use, respectively */
typedef struct bdic_icdes *(* bdev_cmple_fn)(
                 void *,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger);

/* "bdev_apply_fn" typedef - proto-
 * type of the function called to
 * execute (at run-time) an evalua-
 * tor "code".
 *
 * The first parameter is an opaque
 * parameter identifying the exact
 * flavour of the operation to per-
 * form, the second the list of
 * codes to which the implementing
 * codes are to be appended and the
 * third and fourth parameters are
 * the memory allocator and error
 * logger to use, respectively.
 * Returns zero on error, non-zero
 * otherwise */
typedef int ( *bdev_apply_fn)(
              void *,
              struct bdev_cdes *,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdev_precs" enumeration - an indi-
 * cation as to whether an operator has
 * greater or lesser precedence than
 * some other operator */
enum bdev_precs {

    bdev_lt, /* lesser precedence */
    bdev_gt  /* greater precedence */
};

/* "bdev_exec_types" enumeration - the
 * possible execution types of an eval-
 * uator: namely, either "compiled"
 * (in which output is in the form of
 * a list if intermediate codes which
 * must be first translated to machine
 * code, vefore becoming executable)
 * "interpreted", in which a layer of
 * indirection lies between the proc-
 * essor and the evaluator's execution,
 * making them slower to execute but
 * more portable across architectures */
enum bdev_exec_types {

    bdev_exec_type_intrp, /* interpreted */
    bdev_exec_type_cmpld  /* vcompiled */
};

/* "bdev_asscs" enumeration - the "ass-
 * ocivity" of an operator: either "left-
 * to-right" or "right-to-left" */
enum bdev_asscs {

    bdev_assc_l2r, /* left-to-right */
    bdev_assc_r2l  /* right-to-left */
};

/* "bdev_res_cdes" enumeration - the
 * "reserved" (predefined) cdes in an
 * evaluator, consisting of a code
 * to push a value onto the evalua-
 * tor's run-time stack */
enum bdev_res_cdes {

    bdev_res_cde_psh,  /* push value */
    bdev_res_cde_pop,  /* pop value */
    bdev_res_cde_none, /* no implemen-
                        * ting code*/
    bdev_res_cdes_num, /* number of
                        * reserved codes */
};

/* "bdev_cdes" structure - a list of
 * the codes emitted from an evaluator,
 * consisting of a buffer of the list's
 * previously emitted codes (for use
 * while the codes are being built),
 * the "flattened" buffer in the
 * form of linear list of the code
 * IDs (for use once the code build is
 * complete) and a reference to the
 * codes's originating evaluator */
struct bdev_cdes {

    struct bdbf_buffer *buf;  /* for use
                               * during build */
    int *cdes;                /* for use
                               * after build */
    struct bdev_evltr *evltr; /* originating
                               * evaluator */
};

/* "bdev_val" structure - a single value in the
 * run-time and operator stacks of an evaluator,
 * consisting of the value's type and an opaque
 * representation of the value itself */
struct bdev_val {

    int type;  /* value's type */
    void *val; /* opaque representation of
                * value */
};

/* "bdev_item" structure - a single entry in the
 * run-time and operator stacks of an evaluator,
 * consisting of the value of the entry,, a poi-
 * nter to the preceding entry smd sbiopaque
 * value */
struct bdev_item {

    struct bdev_val *val;   /* entry's value */
    struct bdev_item *prcd; /* preceding item */
    void *opaque;           /* opaque value */
};

/* "bdev_evltr" structure - an "expression
 * evaluator", holding a set of the types
 * of codes that may "emit"codes and oper-
 * ators that may be "pushed" onto its ope-
 * rator stack,  which will then "emit" in
 * an order dictated by each's relative
 * precedence.  Each code is implemented
 * by an "apply" function at run-time.
 *
 * The evaluator consists of a relative prec-
 * edence table or (if NULL, these are given
 * instead as direct precedences) and, for
 * each operator, its ID, precedence and com-
 * pile call-back, a map of the codes in the
 * evaluator and referefereRnces to the tops
 * of the operator and run-time stacks */
struct bdev_evltr {

	enum bdev_exec_types type; /* execution */
                               /* type */
    enum bdev_precs *precs;    /* precedence
                                * table */
    struct bd_map_node *oprs,  /* map of op- */
                       *cdes;  /* erators and
                                * codes */
    struct bdev_item *op_top,  /* tops of op- */
                     *rt_top;  /* erator and
                                * run-time
                                * stacks */
};

/* "bdev_op" structure - an "operator"
 * within an evaluator, which consists of
 * its associativity, relative precedence
 * or precedence number (depending on the
 * presence of a precedence table), either
 * its "compile" or "apply" call-back
 * (depending the evauator's type) and
 * the code to emit to implement the
 * effect of the operator when executed */
struct bdev_op {

    enum bdev_asscs assc;    /* associativity */
    int prec, emit;          /* precedence
                              *  number and
                              * emit code*/
    union {
        bdev_cmple_fn cmple; /* compile
                              * call-back */
        bdev_apply_fn intrp; /* interpreted
                              * call-back */
    } u;
};

/* "bdev_cde" structure - an "code" (as
 * used in interpreted evaluators) within
 * an evaluator, consisting of an opaque
 * value and an "apply" call-back imple-
 * menting the code */
struct bdev_cde {

    void *opaque;        /*  opaque value */

    bdev_apply_fn apply; /* interpreted
                          * mplementation
                          * call-back */

};

/**  function proto-types ***/

/* "bdev_init" - initialises and returns an (init-
 * ially empty) evaluator  having the execution
 * type, precedence table (represented as a poi-
 * nter to an array of relative precedences, a
 * NULL value indicating that precedence group
 * numers are instead used), memory allocator
 * and error logger given in the first to fourth
 * parameters, respectively.  Returns zero on
 * error, non-zero otherwise */
struct bdev_evltr *bdev_init(enum bdev_exec_types,
                             enum bdev_precs *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdev_cdes_init" - initialises and returns an
 * (init- ially empty) evaluator code list  hav-
 * ing the , evaluator , memory allocator and er-
 * ror logger given in the first to third parame-
 * ters, respectively.  Returns NULL on error,
 * the code list otherwise */
struct bdev_cdes *bdev_cdes_init(struct bdev_evltr *,
                                 struct bd_allocs *,
                                 struct bd_logger *);

/* "bdev_add_op" - add an operator whose ID,
 * compilation and interpretation call-backs,
 * precedence and  associativity are given
 * in the dirst to fifth parameyers, respe-
 * ctively, to the evaluator given in the
 * first parameter, using the memory alloc-
 * ator and error logger given in the sev-
 * enth and eighth parameters, respectively.
 * Returns zero on error, non-zero otherw-
 * ise */
int bdev_add_op(struct bdev_evltr *,
                int,
                bdev_cmple_fn,
                bdev_apply_fn,
                int,
                enum bdev_asscs,
                struct bd_allocs *,
                struct bd_logger *);

/* "bdev_add_cde" - add an evaluator code (rele-
 * vant only to interpreted evaluators) whose
 * ID, "apply" and "compile" call-backs and
 * opaque value are given in the second to fi-
 * gfth parameters, respectively, using the m-
 * emory allocator and error logger given in
 * the fifth and sixth  parameters, respectiv-
 * ely. This routine is usually called at ev-
 * aluator initialisation time.  Returns zero
 * on error, non-zero otherwise */
int bdev_add_cde(struct bdev_evltr *,
                 int,
                 bdev_apply_fn,
                 bdev_cmple_fn,
                 void *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdev_push_op" - "pushes" the operator whose id-
 * entifier and operator opaque value are given in
 * the second and third parameters, respectively,
 * onto the operator stack of the code list given
 * in the first parameter, using the memory alloc-
 * ator and error logger given in the fourth and
 * fifth parameters, respectively.  If this push
 * causes the output of operators (which will occur
 * for othose operators of less precedence on
 * the operator stack) these operators' compile
 * call-backs will be emitted from the evaluator,
 * thereby simulating the effect of calling the
 * operator at run-time.  This routine is usually
 * called at compile time.  Returns zero on error,
 * non-zero otherwise */
int bdev_push_op(struct bdev_cdes *,
		         int,
		         void *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdev_emit" - "emits", by either, depending
 * on the type of the evaluator with which the
 * code list given in the first parameter is
 * associated, executing the "apply" call-back
 * associated with the code ID given in the
 * third parameter (for "interpreted" evaluat-
 * ors) or calling its "compile" call-back
 * (which outputs to the intermediate code
 * list given in the second parameter) assoc-
 * iated with the code ID given in the third
 * parameter (for "compiled" evaluators), us-
 * ing the ID's opaque value, memory alloc-
 * ator and error logger given in the fourth,
 * fifth and sixth parameters, respectively.
 * Returns zero on error, non-zero otherwise */
int bdev_emit(struct bdev_cdes *,
              struct bdic_icdes *,
		      int,
		      void *,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdev_run" - executes the op-code array
 * given in the first parameter until the
 * exit cide whise ID  is given in the sec-
 * ond parameter is encountered, using the
 * memory allocator and err-or logger giv-
 * en in the third and fourth, respectiv-
 * ely.  Returns zero on error, non-zero
 * otherwise */
int bdev_run(struct bdev_cdes *,
             int,
             struct bd_allocs *,
             struct bd_logger *);

/* "bdev_push_val" - "pushes" the content ty-
 * pe and value of which is given in the seco-
 * nd and third parameters, respectively, onto
 * the run-time stack of the evaluator in the
 * first parameter, using the memory allocator
 * and error logger given in the fourth and
 * fifth parameters, respectively.  Returns
 * zero on error, non-zero otherwise */
int bdev_push_val(struct bdev_evltr *,
                  int,
                  void *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdev_pop" - returns the top value
 * of run-time stack of the evaluator
 *  given in the first parameter, ghav-
 * ing the expected type given in the
 * second parameter, then "pops" that
 * top value from the stack, using the
 * memory allocator and error logger
 * given in the third and and fourth
 * parameters, respectively. Returns
 & NULL on error */
void *bdev_pop(struct bdev_evltr *,
               int,
               struct bd_allocs *,
               struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
