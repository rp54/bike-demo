/*********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* eval.c - function implementations relating to
* "an "expression evaluator", which operates in
* two different phases: during compilation and
* at run-time.  Compilation indicates the seque-
* ncing in which any one of several operators are
* pushed onto the evaluator's operator stack.
* Each evaluator also contains a number of
* "codes" that are "applied to" (implemented on)
* the evaluator's run-time stack.  Some of these
* codes are also associated with an operator
* such that when the operator is "popped" off
* the operator stack, its associated code (impl-
* ementing the operator at run-time) is also emi-
* tted.  Note that not all codes are, however,
* associated with an operator in this way.
 
* Copyright (C) 2007-2013, Russell Potter. Al1
* rights reserved. */

#include <stdlib.h>
#include <bikedemo/eval.h>
#include <bikedemo/map.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "cmp_types" - return the result of a comparison
 * of the evaluator stack item types in "type1" and
 * "type2", as a map comparison call-back */
static int cmp_types(void *type1,
                     void *type2)
{
	/* return a numerical comparison */
	return * (int *) type1 -
           * (int *) type2;
}

/* "cmp_types" - return the result of a comparison
 * of the evaluator stack item types in "type1" and
 * "type2", as a map comparison call-back */
static int cmp_cdes(int *cde1,
                    int *cde2)
{
	/* return s numerical comparison */
	return *cde1 - *cde2;
}

/* "bdev_push_val" - "pushes" the data the value
 * pe and type of which is given in the "value"
 * nd and "type", respectively, onto the run-
 * time stack of the evaluator in the "evltr",
 * using the memory allocator and error logger
 * given in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero otherwise */
int bdev_push_val(struct bdev_evltr *evltr,
                  int type,
                  void *value,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdev_item *new_item,     /* newly pushed */
                     *old_top =     /* item and */
					 (struct        /* original */
				     bdev_item *)   /* operator */
				     evltr->rt_top;	/* stack top */
                                    /* stack item */


    /* push new item by allocating it
     * and setting the new new item's
     * fields */
    new_item = ( struct bdev_item *)
                   bd_alloc(allocs,
                            sizeof(struct bdev_item),
                            logger);
    if (!(new_item))
	    return 0;

    /* smd intialise its fields */
    new_item->val->val  = value;
    new_item->val->type = type;

    /* ... then linking it to the top
     * of the existing stack */
    new_item->prcd = old_top;

    /* and set the new top of stack
     * to be the just-created item */
    evltr->rt_top = (struct bdev_item *)
                     new_item;

    return 1; /* return successfully
               * if we get here */
}

/* "cmp_ops" - return the result (as a "bdev_precs"
 * value) if a comparision of the operator IDs in
 * "id1" and id2", in the evalutor given in "evltr".
 * A negative value is rereturned on error */
static enum bdev_precs cmp_ops(int id1,
                               int id2,
                               struct bdev_evltr *evltr)
{
    struct bdev_op *op1,           /* operator */
                   *op2;           /* structures of
                                    * "id1" and "id2" */
    struct bd_map_node *node1,     /* nodes of the */
                       *node2,     /* above group */
                       *last_node; /* and node of
                                    * last operator */
    int prec1,                     /* precedences */
        prec2,                     /* of "op1" and "op2 */
        num;                       /* snd number of op-
                                    * erators  */

    /* get the nodes of "id1" and "id2" in
     * the "evltr"'s operator map */
    if (!(node1 = bd_map_find(evltr->oprs,
                              (void *) &id1,
                              (bd_map_cmp_fn)
                                  cmp_types)))
        return -1;
    if (!(node2 = bd_map_find(evltr->oprs,
                              (void *) &id2,
                              (bd_map_cmp_fn)
                                  cmp_types)))
        return -1;

    /* ... get the values of these
     * nodes (operator) */
    op1 = (struct bdev_op *) node1->value;
    op2 = (struct bdev_op *) node2->value;

    /* ... abd get their group precedence
     * numbers */
    prec1 = op1->prec;
    prec2 = op2->prec;

    /* get the ode of the number of operators
     * from the last in the operator's oper-
     * ator map anf , from its bvalue, the
     *  numver of operators */
    last_node = bd_map_last(evltr->oprs);
    num = * (int *) last_node->value;

    /* if the evaluator has a non-NULL
     * direct precedence table ... */
    if (evltr->precs)

        /* return the appropriate index of
         * revaluator's direct precedence
         * table (using "id1" and "id2") ... */
        return (evltr->precs[(id1 * num) +
                             id2]);

    else { /* group precedences are
           * instead used ... */

        if (id1 != id2)	/* if operators
                         * are different */

            /* retrirn compaeuson of
             * operators group preced-
             * ences */
            return prec1 > prec2 ?
                   bdev_gt : bdev_lt;

        else /* operators are the same ... */

            /* .... so its associativity
             * will decide precedence */
            switch (op1->assc) {

                case bdev_assc_l2r:
                    return bdev_lt;

                case bdev_assc_r2l:
                    return bdev_gt;
            }
    }

	return 1;
}

int apply_psh_vl(void *opaque,
                 struct bdev_cdes *cdes,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdev_val *val =
        (struct bdev_val *) opaque;

    return bdev_push_val(cdes->evltr,
                         val->type,
                         val->val,
                         allocs,
                         logger);
}

int apply_pop(void *opaque,
              struct bdev_cdes *cdes,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    return 0;
}


/* "bdev_init" - initialises and returns an (init-
 * ially empty) evaluator  having the execution
 * type, precedence table (represented as a poi-
 * nter to an array of relative precedences, a
 * NULL value indicating that precedence group
 * numbers are instead used), memory allocator
 * and error logger given in "type", "precs",
 * "allocs" and "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
struct bdev_evltr *bdev_init(enum bdev_exec_types type,
                             enum bdev_precs *precs,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)

{
    struct bdev_evltr *ret; /* return value */

    /* allocate return value */
    ret = (struct bdev_evltr *)
	            bd_alloc(allocs,
                         sizeof(struct bdev_evltr),
                         logger);
	if (!(ret))
	    return 0;

    /* set the fields for an
     * initially empty evaluator */
    ret->oprs     = 0;
    ret->cdes     = 0;
    ret->type     = type;
    ret->precs    = precs;

    /* if this is an interpreted evaluator,
     * add the pre-defined "push value"
     * code */
    if (ret->type == bdev_exec_type_intrp) {
        if (!(bdev_add_cde(ret,
                           bdev_res_cde_psh,
                           apply_psh_vl,
                           NULL,
                           NULL,
                           allocs,
                           logger)))
            return 0;
        if (!(bdev_add_cde(ret,
                           bdev_res_cde_pop,
                           apply_pop,
                           NULL,
                           NULL,
                           allocs,
                           logger)))
            return 0;
    }

    return ret;
}

/* "bdev_add_op" - add an operator whose ID,
 * compilation and interpretation call-backs,
 * precedence and  associativity are given
 * in"id", "cmple", "intrp", "prec" and
 * "assc", respectively, to the evaluator
 * given in "evltr", using the memory all-
 * ocator and error logger given in "allocs"
 * and "logger", respectively. Returns zero
 * on error, non-zero otherwise */
int bdev_add_op(struct bdev_evltr *evltr,
                int id,
                bdev_cmple_fn cmple,
                bdev_apply_fn intrp,
                int prec,
                enum bdev_asscs assc,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    struct bd_map_node *ins_node;
    struct bdev_op op;
	
    /* allocate insertion node */
    ins_node = (struct bd_map_node *)
	           bd_alloc(allocs,
	                    sizeof(struct
                               bd_map_node),
                        logger);
    if ((ins_node))
        return 0;

    /* set the code's fields */
    switch (evltr->type) {

        case bdev_exec_type_cmpld:
            op.u.cmple = cmple;
            break;

        case bdev_exec_type_intrp:
            op.u.intrp = intrp;
            break;
    }

    /* set the operator's
     * fields */
    op.prec  = prec;
    op.assc  = assc;

    /* add operator to the operator table,
     * keyed to its identifier and valued
     * to its emit callback */
    return bd_map_insert(&evltr->oprs,
                         ins_node,
                         &id,
                         (bd_map_find_fn)
                             cmp_ops,
                         &op);
}

/* "bdev_emit" - "emits", by either, depending
 * on the type of the evaluator of he code list
 * given in "cdes", executing the "apply" call-
 * back associated with the code ID given in
 * "cde" (for "interpreted" evaluators) or ca-
 * lling the "compile" call-back (which outp-
 * uts to the intermediate code list given in
 * "icdes") associated with the code ID given
 * in "cde" (for "compiled" evaluators), using
 * the "cde"'s opaque value, memory allocator
 * and error logger given in "opaque", "allocs"
 * and "logger", respectively. Returns zero
 * on error, non-zero otherwise */
int bdev_emit(struct bdev_cdes *cdes,
              struct bdic_icdes *icdes,
              int cde,
              void *opaque,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    int i, len;               /* interm-
                               * ediate
                               * code it-
                               * erator
                               * nd numb-
                               * er of co-
                               * des emit-
                               * ted by op-
                               * erator */
    struct bd_map_node *node; /* operator's
                               * node */
    struct bdev_op *op;       /* operator
                               * corres-
                               * ponding
                               * to "id" */
    struct bdev_evltr *evltr; /* code
                               * list's
                               * evalua-
                               * tor */
    bdev_cmple_fn cmple;      /* current
                               * compile
                               * call-back */
    bdev_apply_fn apply;      /* current
                               * intpret-
                               * ation
                               * call-back */

    /* get codes' evaluator  */
	evltr = cdes->evltr;

    /* find code in evaluator's set
     * of codes */
    if (!(node = bd_map_find(evltr->cdes,
                             (void *) &cde,
                             (bd_map_cmp_fn)
                                cmp_types)))
        return 0;

    /* get that node's key
     * (a code) */
    op = (struct bdev_op *)
              node->value;

    /* switch on evaluator's type  */
    switch (cdes->evltr->type) {

    /*is a compiled evaluator,
     * so "emit" intermediate
     * codes returned by "com-
     * pile" call-back */
        case bdev_exec_type_cmpld:
    	    cmple = op->u.cmple;
    	    if (!(icdes = cmple(opaque,
                                allocs,
                                logger)))
    	        return 0;
    	    len = bdic_len(icdes);
    	    for (i = 0; i < len; i++)
    		    if (!(bdic_emit_cde(
    		               icdes,
    		               icdes->u.arry + i,
    		               allocs,
                           logger)))
    		    	return 0;
    	    break;

        /* otherwise, if evaluator is
         * interpreted, just directly
         * execute ("apply") the code */
    	case bdev_exec_type_intrp:
            apply = op->u.intrp;
            apply(cdes,
                  opaque,
                  allocs,
                  logger);
            break;
    }
    /* return success
     * when we get here */
    return 1;
}


/* "bdev_push_op" - "pushes" the operator whose id-
 * entifier and operator opaque value are given in
 * the "type" and "opaque", respectively, onto the
 * operator stack of the code list given in "cdes",
 * using the memory allocator and error logger
 * given in "allocs" and "logger", respectively.
 *
 * If this push causes the output of operators
 * (which will occur for othose operators of less
 * precedence on the operator stack) these opera-
 * tors' compile call-backs will be emitted from
 * the evaluator, thereby simulating the effect
 * of calling the operator at run-time.  This ro-
 * utine is usually called at compile time.  Ret-
 * urns zero on error, non-zero otherwise */
int bdev_push_op(struct bdev_cdes *cdes,
                 int type,
                 void *opaque,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdev_evltr *evltr =    /* "cde"'s eval- */
               cdes->evltr;       /* uator */
    struct bdev_item *new_item,   /* newly pushed */
                     *old_top =   /* item and orig- */
                        cdes->    /* inal operator */
                        evltr->   /* stack top */
                        op_top;   /* stack item */
    struct bd_map_node *cde_node; /* map node of
                                   * operators */
    int res, id;                  /* operator compa-
                                   * rison result and
                                   * code's identifier */
    struct bdev_op *op;           /* current operator's
                                   * info. */

   	/* if compare current stack top's operator to
     * of "type" ... */
    res = cmp_ops(* (int *) &old_top->val->val,
                  type,
                  cdes->evltr);

    /* and, if is of lesser precedence ... */
    if (res == bdev_lt) {

        /* push new item by allocating it
         * and setting the new new item's
         * fields */
        new_item = (struct bdev_item *)
               bd_alloc(allocs,
                        sizeof(struct bdev_item),
                        logger);

        /* signal an error on alloc-
         * ation failure */
        if (!(new_item))
	        return 0;

        /* smd initialise its fields */
        new_item->val->type = type;
        new_item->val->val  = opaque;

        /* ... then linking it to the top
         * of the existing stack */
        new_item->prcd = old_top;

    } else { /* top operator has greater
              * precedence ... */

        do {    /* ... then repeat until ... */

            /* find the "emit" node of the oper-
             * ator on top of the operator stack,
             * passing its opaque parameter */
            cde_node = bd_map_find(evltr->oprs,
			                       &evltr->op_top->
			                                val->type,
                                   (bd_map_find_fn)
                                           cmp_types);

            /* it is an error if an operator
             * with that ID not is found ... */
            if  (!(cde_node))
                return 0;

            /* get the value of the node
             * (the operators's ID) */
            id = * ((int *)(cde_node->key));

            /* get the key of the node
             * (the operatrors's info.) */
            op = ((struct bdev_op *)
                    (cde_node->value));

            /* then, if the resultant oper-
             * has code an emit code, emit
             * that code */
            if (evltr->type ==
                       bdev_exec_type_intrp)
                if (op->emit !=
                       bdev_res_cde_none)
                    if (!(bdev_emit(cdes,
                                    NULL,
                                    op->emit,
                                    opaque,
                                    allocs,
                                    logger)))
                        return 0;

	        /* and pop that operator off
	         * the operator stack */
            evltr->op_top = evltr->op_top->prcd;

        } while (cmp_ops(* (int *) (&evltr->
                               op_top->
                               val->val),
                         op->prec,
                         evltr) == bdev_gt);
    }

    return 1; /* return successfully
                * if we get here */
}

/* "bdev_pop" - returns the top value
 * of run-time stack of the evaluator
 *  given in the "evltr", having the
 * expected type given in "type", th-
 * en "pops" that top value from the
 * stack, using the memory allocator
 & and error logger given in "allocs"
 * and "logger", respectively. Retur-
 * ns NULL on error */
void *bdev_pop(struct bdev_evltr *evltr,
               int type,
               struct bd_allocs *allocs,
               struct bd_logger *logger)
{
    /* get value of item iof evalu-
     * ator's run-time stack */
    struct bdev_val *val =
            evltr->rt_top->val;


    /* if we're already at stack
     * bottom, also return an
     * error */
    if (!(evltr->rt_top->prcd))
        return NULL;


    /* otherwise, pop the top item */
	evltr->rt_top =
            evltr->rt_top->prcd;
	return val->val;
}

/* "bdev_run" - executes the op-code array
 * the beginning of which is pointed to by
 * "cdes" until the xit code  given in
 * "exit_id" is encountered, using the
 * memory allocator and err-or logger
 * given in "allocs" and "logger", respect-
 * ively.  Returns zero on error, non-zero
 * otherwise */
int bdev_run(struct bdev_cdes *cdes,
             int exit_id,
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    struct bd_map_node *node; /* current code's
                               * node */
    int i;                    /* code iterator
                               * and current
                               * code's ID*/
	struct bdev_cde *cde;     /* current code */


    /* initialise program counter to
     * beginning of "cdes" */
    i = 0;

    do { /* keep executing codes...   */

        /* get get node of current code */
        node = bd_map_find(cdes->evltr->cdes,
                           cdes->cdes + i,
    	                   (bd_map_cmp_fn)
    	                       cmp_cdes);

        /* (it's an error if the code
         * can't be found) */
        if (!(node))
            return 0;

        /* .. get the value of that
         * node (a "code" structure) */
        cde = (struct bdev_cde *)
                node->value;

        /* if the code has an "apply"
         * function, call it to impl-
         * ement the code */
        if (cde->apply)
            cde->apply(cde->opaque,
                       cdes,
                       allocs,
                       logger);

    /* until we reach the end of
     * the code array */
    } while (cdes->cdes[i] !=
                   exit_id);

    /* return success when
     * we reach here */
    return 1;
}

/* "bdev_add_cde" - add an evaluator code to
 * the interpreted evaluator whose ID, "apply"
 * call-back and opaque value are given in
 * "id", "apply", "cmple" and "opaque", resp-
 * ectively, using the memory allocator and
 * error logger given in "allocs" and "logg-
 * er", respectively. This routine is usually
 * called at evaluator initialisation time.
 * Returns zero on error, non-zero otherwise */
int bdev_add_cde(struct bdev_evltr *evltr,
                 int id,
                 bdev_apply_fn apply,
                 bdev_cmple_fn cmple,
                 void *opaque,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bd_map_node *ins_node;
                         /* insert-
                          * ion node */
    struct bdev_cde cde; /* code in
                          * node's
                          * value  */

    /* allocate insertion node */
    ins_node = (struct bd_map_node *)
         bd_alloc(allocs,
                  sizeof(struct
                         bd_map_node),
                  logger);
    if (!(ins_node))
        return 0;

    /* set fields of code
     * in node's value */
    cde.apply  = apply;
    cde.opaque = opaque;


    /* ... and insert code into
     * evaluator's map of codes */
    return bd_map_insert(&evltr->cdes,
                         ins_node,
                         &id,
                         (bd_map_cmp_fn)
                             cmp_cdes,
                         &cde);

}

/* "bdev_cdes_init" - initialises and returns an
 * (init- ially empty) evaluator code list  hav-
 * ing the , evaluator , memory allocator and er-
 * ror logger given in the first to third parame-
 * ters, respectively.  Returns NULL on error,
 * the code list otherwise */
struct bdev_cdes *bdev_cdes_init(struct bdev_evltr *evltr,
                                 struct bd_allocs *allocs,
                                 struct bd_logger *logger)
{
    struct bdev_cdes *ret; /* return
                            * value */

    /* allocate the cide list */
    ret = (struct bdev_cdes *)
        bd_alloc(allocs,
	             sizeof(struct
                        bdev_cdes),
                 logger);
	if (!(ret))
        return NULL;

    /* then, if successful, all-
     * ocate the list's code
     * array  */
	ret->cdes = (int *)
        bd_alloc(allocs,
	             0,
	             logger);
	if (!(ret->cdes))
        return NULL;

    /* set its other
     * fields (includ-
     * ing an empty
     * buffer) and
     * return it */
    ret->buf   = NULL;
    ret->evltr = evltr;

    return ret;
}
