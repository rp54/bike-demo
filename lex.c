/*****************************************************************
*
* BikeDemo - Exercise bike demonstration scenery
*
* lex.c - function implementations for a lexical anal-
* yser, used to "tokenise" (that is, convert into a list
* of "tokens", each of which might be, for example, a
* number or a string literal) the characters encountered
* on a stream of input.
*
* The analyser is able to recognise, at its most basic
* level, "definitions" , which associate regular
* expressions, or patterns, in the input stream with
* names (so that the patterns may referred by the names
*
* Patterns of These definitions forms a "rule" which,
* when recognised, calls an associated call-back function
* for the rule specifying the rule as being that found
* on the input stream.
*
* Copyright (C) 2008-2013, Russell Potter, All Rights
* Reserved
*
******************************************************************/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include <bikedemo/lex.h>
#include <bikedemo/eval.h>
#include <bikedemo/map.h>
#include <bikedemo/util.h>
#include <bikedemo/eval.h>
#include <bikedemo/grammar.h>
#include <bikedemo/parser.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "re_prec_table" - regular expression
 * operator precedence table:, in which,
 * "op1" being the operator on top of the
 * parser's operator stack and "op2"  the
 * operator on the parser's input, then a
 * value of "bdev_gt" at the index into
 * the table given by "(op1 * num_ops) +
 * op2" indicates "op1" has greater prec-
 * edence than "op2", while a value of
 * "bdev_gt" in this index indicates "op1"
 * has lower precedence than "op2" */
static enum bdev_precs re_prec_table[
                        bdlx_re_op_num *
                        bdlx_re_op_num] = {


 /*           [rep,    alt,     seq,
               lpar,   rpar] */


 /* [rep]  */ bdev_gt, bdev_gt, bdev_gt,
              bdev_lt, bdev_gt,

 /* [alt]  */ bdev_lt, bdev_lt, bdev_lt,
              bdev_lt, bdev_gt,

 /* [seq]  */ bdev_lt, bdev_gt, bdev_gt,
              bdev_lt, bdev_gt,

 /* [lpar] */ bdev_lt, bdev_lt, bdev_lt,
              bdev_lt, bdev_lt,

 /* [rpar] */ bdev_gt, bdev_gt, bdev_gt,
              bdev_gt, bdev_gt
};

/* "apply_rep" - "applies" (implements)
 * the "bit and" (repeat a  a regular
 * expression one or more times)  in the
 * opaque value in "opaque" and evaluat-
 * or codes in "cdes", respectively, usi-
 * ng the memory allocator and error lo-
 * gger in "allocs" and "logger", respe-
 * ctively.  This routine serves as the
 * regular expression evalutor's "apply"
 * call-back for this code. Returns zero
 * on error, non-zero otherwise */
static int apply_rep(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_chce" - "applies" (implements)
 * the "bit and" (alternate btweeb alter-
 * nate regular expressions in the opaq-
 * ue value in "opaque" and evaluator
 * code list in "cdes", respectively,
 * using the memory allocator and error
 * logger in "allocs" and "logger", re-
 * spectively.  This routine serves as
 * the regular expression  parser eval-
 * utor's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_chce(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "apply_end" - "applies" (implemen-
 * ts) the "sequence" (end regular ex-
 * pression) code in the opaque value
 * in "opaque" and regular expression
 * parser's evaluator code list in
 * "cdes", respectively, using the
 * memory allocator and error logger
 * in "allocs" and "logger", respect-
 * ively.  This routine serves as the
 * regular expression parser evaluat-
 * or's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_end(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    return 1;
}

/* "apply_dfn_ref" - "applies" (imple-
 * ments) the "sequence" (reference a
 * definition) code in the opaque val-
 * ue in "opaque" and regular expres-
 * sion parser's evaluator code list
 * in "cdes", respectively, using the
 * memory allocator and error logger
 * in "allocs" and "logger", respect-
 * ively.  This routine serves as the
 * regular expression parser evalut-
 * or's "apply" call-back for this
 * code. Returns zero on error, non-
 * zero otherwise */
static int apply_dfn_ref(void *opaque,
                         struct bdev_cdes *cdes,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    return 1;
}

/* "cmp_states" - compare two NFA states, as
 * part of a map key comparison */
static int cmp_states(int *state1,
                      int *state2)
{
   return *state2 - /* subtract  numerical */
		  *state1;  /* values of states */

}

/* "cmp_trans" - compares two NFA transitions in
 * "trans1" and "trans2", as part of a map key
 * comparison call-back */
static int cmp_trans(struct bdlx_nfa_trans *trans1,
                     struct bdlx_nfa_trans *trans2)
{
    /* return comparison of each transitions'
     * fields */
    return trans1->from - trans2->from &&
           trans1->to   - trans2->to   &&
           trans1->mn   - trans2->mn   &&
           trans1->mx   - trans2->mx   &&
           trans1->acc  - trans2->acc  &&
           (int) trans1 - (int) trans2->fn;
}

/* "cmp_ranges" - compares two ranges in
 * "rnge1" and "rnge2", as a map key
 * comparison vall-back */
static int cmp_ranges(struct bdlx_range *rnge1,
                      struct bdlx_range *rnge2)
{
    /* return comparison of upper and
     * lower limits */
	return rnge1->upper - rnge2->upper &&
           rnge1->lower - rnge2->lower;
}

/* "cmp_sets" - compare two state sets in
 * "set1" and "set2", as part of a map key
 * comparison */
static int cmp_sets(struct bd_map_node *set1,
                    struct bd_map_node *set2)
{
    struct bd_map_node *it1, *it2; /* set ite-
                                    * rators */
     int *state1,                  /* current */
         *state2,                  /* states */
         diff;                     /* state diff-
                                    * erence */

    /* start both iterators at
     * beginning of their sets */
    it1 = bd_map_first(set1);
    it2 = bd_map_first(set2);

    /* a ... and, while either
     * iterator isn't exhausted */
    while (it1 && it2) {

        /* get difference between
         * iterator keys */
        state1 = (int *) it1->key; 
        state2 = (int *) it2->key;

        diff = *state1 - *state2;

        /* if there is a differ-
         * ence, the sets are non-
         * identical, so return */
        if (diff)
            return diff;

        /* move to next set pos-
         * itions */
	    it1 = bd_map_next(it1);
        it2 = bd_map_next(it2);

        /* sets are only identical (ret-
         * urning zero) if both iterat-
         * ors are simultaneously exhau-
         * sted */

    }
    return (!(!it2 && !it2));
}

/* "cat_nfas" - sets "*ret" to the concatenation of
 * the NFAsiven in "nfa1" and "nfa2", such that the
 * length of the resulting NFA is then length(nfa1) +
 * length(nfa2), using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns zero on error, non-zero
 * otherwise */
static int cat_nfas(struct bdlx_nfa *nfa1,
                    struct bdlx_nfa *nfa2,
                    struct bdlx_nfa **ret,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bd_map_node *node,     /* insertion */
		               *ins_node; /* node */
    struct bdlx_nfa_trans *trans; /* "nfa2"s trans-
                                   * itions */


    *ret = (struct bdlx_nfa *)
           bd_alloc(allocs,
                    sizeof(struct bdlx_nfa),
                    logger);

	if (!(ret))
        return 0;

    /* set number of states in
     * resulting NFA */
    (*ret)->num = nfa1->num + nfa2->num;

    /* cycle through transitions in "nfa2" ... */
    for (node = bd_map_first(nfa2->trns);
         node;
         node = bd_map_next(node)) {

        /* get current transition as node's
         * key */
        trans = (struct bdlx_nfa_trans *)
                          node->key;

        /* correct state numbers in "nfa2" by
         * incrementing by the number of states
         * in "nfa1" */
        if (trans->from > nfa1->num)
            trans->from += nfa1->num;
        if (trans->to > nfa1->num)
            trans->to += nfa1->num;

       /* allocate insertion node .... */
       ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                bd_map_node),
                         logger);
        if ((ins_node))
            return 0;

        /* return the status of the
         * insertion of the transit-
         * ion into the concatenated
         * NFA */
        return bd_map_insert((&(*ret)->trns),
                             ins_node,
                             trans,
                             (bd_map_cmp_fn)
                                cmp_trans,
                             trans);
	}
    return 1;
}

/* "init_nfa" - returns an NFA with no transit-
 * ions and "num" number of states, using the
 * memory allocator and error logger igven in
 * "allocs" and "logger", respectively.  Ret-
 * urns zero on error, non-zero otherwise */
static int init_nfa(int num,
                    struct bdlx_nfa **ret,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* allocated return value */
	(*ret) = (struct bdlx_nfa *)
               bd_alloc(allocs,
                        sizeof(struct
                               bdlx_nfa),
                        logger);
    if (!(*ret))
    	return 0;

    /* ... initialise it and its
     * fields with appropriate
     * values */
    (*ret)->trns = NULL;
    (*ret)->num  = num;

    return 1; /* return success
               * if we get to
               * this point */
}

/* "in_sym" - return non-zero if "sym"
 * lies in the range between the "mn"
 * and "mx" fields of "trns", using the 
 * memory allocator and error logger
 * given in "allocs" and "logger", res-
 * pectively, as the "sym" call-back,
 * as the "process_nfa" pass setting
 * the transition call-backs of a DFA */
static int in_sym(
        struct bdlx_nfa_trans *trans,
         int sym,
        struct bd_allocs *allocs,
        struct bd_logger *logger)
{
	/* retm non-aero if "sym" lies bet-
     * ween "trns->mn" and "trns->mx" */
    return sym >= trans->mn &&
           sym <= trans->mx;
}

/* "get fn" - returns the transit-
 * ion function of "trns", using
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively, as
 * the "val" call-back for the
 * "process_nfa" pass setting
 * the transition call-backs of a
 * DFA */
static void *get_fn(
        struct bdlx_nfa_trans *trans,
        struct bd_allocs *allocs,
        struct bd_logger *logger)
{
	/* inficte interest (non-
	 * zero return ) id the
     * transition has a trans-
     * ition function */
	return trans->fn;
}

/* "set_fn" sets the "fn" (and
 * "opaque" fields of "entry",
 * using the the NfA transition
 * in "trns", using the memory
 * allocator and error logger
 * given in "allocs" and "log-
 * ger", respectively, as the
 * "entry" call-back for the
 * "process_nfa" pass setting
 * the transition call-backs of
 * a DFA */
static int set_fn(
         struct bdlx_dfa_entry *entry,
         struct bdlx_nfa_trans *trns,
         void *fn,
         struct bd_allocs *allocs,
         struct bd_logger *logger)
{
	/* set the entry's transition
     * function from the transiti-
     * on's function */
    entry->fn =
       * (bdlx_trans_fn *) &fn;

    /* ... the function's opaque
     * value from that of the tran-
     * sition function */
    entry->opaque = trns->opaque;
    return 1;
}


/* ""has_fn" - returns non-zero if
 * "trans" has a transition funct-
 * ion, using the memory allocator
 * and error logger given in "all-
 * ocs" and "logger", respectively,
 * as the "cond" call-back for the
 * "process_nfa" pass setting the
 * transition call-backs of a DFA */
static int has_fn(
       struct bdlx_nfa_trans *trans,
       struct bd_allocs *allocs,
       struct bd_logger *logger)
{
	/* indicates interest (non-zero
	 * return) if transition has a
	 * transition function */
	return trans->fn != NULL;
}

/* "apply_seq" - applies (implements) the
 * "sequence" (concatenation) operator of
 * the expression evaluator in "evltr",
 * using the memory allocator and error
 * logger in "allocs" and "logger", resp-
 * ectively.  This routine serves as the
 * regular expression evalutor's "apply"
 * call-back for this operator. Returns
 * zero on error, non-zero otherwise */
static int apply_seq(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* concatenate NfAs in top two stack
     * items ... */
    return cat_nfas((struct bdlx_nfa *)
                    cdes->evltr->rt_top->val,
		            (struct bdlx_nfa *)
		            cdes->evltr->rt_top->prcd->opaque,
                    (struct bdlx_nfa **)
                    &cdes->evltr->rt_top->opaque,
                    allocs,
                    logger);
}

/* "empty_closure" given a set of input states
 * "in" of the NFA "nfa", sets "clsre" to the
 * set of states that can be reached from "in"
 * without reading an input (that is, using only
 * empty transitions).  Returns zero on error,
 * non-zero otherwise */
static int empty_closure(struct bd_map_node *in,
                         struct bd_map_node **clsre,
                         struct bdlx_nfa *nfa,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    struct bd_map_node *state_node,  /* state iterator */
                       *trans_node,  /* and insertion */
                       *ins_node;    /* node */
                                     /* transition
                                      * iterator */
    int cont = 1;                    /* continue flag */
    struct bdlx_nfa_trans *trans;    /* current transit-
                                      * ion in NFA */ 

    /* iterate through nodes in input states
     * map */
    for (state_node = bd_map_first(in);
                state_node;
                state_node = bd_map_next(state_node)) {

        /* get state of node  */
        int state = * (int *) &state_node->key;

        /* iterate through transitions in NFA */
        for (trans_node = bd_map_first(nfa->trns);
                        trans_node;
                        trans_node = bd_map_next(
                                     trans_node)) {


            /* get current transition  */
            trans = (struct bdlx_nfa_trans *)
                            trans_node->key;

            while (cont) { /* continue for now ... */

                cont = 0;  /* but initially we're
                            * not continuing  */


                    /* if this is an empty transi-
                     * tion from the current state
                     * to a state not in "ret" ... */
                if (trans->type ==
                           bdlx_sym_type_empty &&
                           trans->from == state &&
                           (!(bd_map_find(in,
                                          &state,
                                          (bd_map_cmp_fn)
                                          cmp_states)))) {
 
                    cont = 1; /* indicate we are
                               * continuing ...  */

                 /* ... and insert state into
                  * "ret" */
                 ins_node = (struct bd_map_node *)
                     bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger);
                 if (!(ins_node))
                     return 0;
                 if (!(bd_map_insert(clsre,
                                     ins_node,
                                     &state,
                                     (bd_map_cmp_fn)
                                          cmp_states,
                               &state)))
                     return 0;
                }
            }
        }                     
    }
    return 1; /* return success
               * if we get here */
}

/* "merge" - sets "*mrgd" to a "merge" of
 * the states in "set1" and "set2", such
 * that a state is in "mrgd" if it us in
 * either "set1" or "set2". returns zero
 * on error, non-zero otherwise */
static int merge(struct bd_map_node *set1,
                 struct bd_map_node *set2,
                 struct bd_map_node **mrgd,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
	struct bd_map_node *ins_node,  /* insert- */
                       *it = set1; /* ion node, */
                                   /* set iter */
                                   /* ators */
    int *state;                    /* current
                                    * state */

    /* the merged set is
     * initially just "set1" */
    *mrgd = set1;

    /* cycle hrough states in "set2" */
    for (it = bd_map_first(set2);
         it;
         it = bd_map_next(it)) {

        /* get the node key
         * (a state pointer) ... */
        state = (int *)
            it->key;

        /* if the state isn't in
         * "set1" ... */
        if (!(bd_map_find(set1,
                          state,
                          (bd_map_cmp_fn)
                          cmp_states))) {

            /* ... then add it to
             * the return set */
		    ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                bd_map_node),
                         logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(mrgd,
                                ins_node,
                                state,
                                (bd_map_cmp_fn)
                                    cmp_states,
                                state)))
                return 0;
        }
	}
    return 1;  /* return success if
                * we get to here */
}

/* "cmbne_states" given a set of sets
 * of NFA states in "sets", sets "dfas"
 * to the set of DFA states containing
 * the NFA state "fnd_state", using the
 * memory allocator and error logger
 * given in "allocs" and "logger", res-
 * pectively */
static int cmbne_states(struct bd_map_node *sets,
		                struct bd_map_node **dfas,
		                int fnd_state,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bd_map_node *node,       /* iterator, */
                       *set,        /* insert- */
                       *ins_node,   /* ion node, */
                       *set_node,   /* state set */
                       *state_node; /* in curre-
                                    ** nt node
                                     * and node
                                     * of curre-
                                     * nt state */
    int state;                      /* current
                                     * state */

    /* the set of DFA states is
     * initially empty */
    *dfas = NULL;

    /* iterate through the nodes
     * in "sets" */
    for (node = bd_map_first(sets);
                node;
                node = bd_map_next(node)) {

        /* get the node's value, itself
         * a map of NFA states */
        set = (struct bd_map_node *)
                       node->value;

        /* iterate through the nodes
         * in "set" */
        for (state_node = bd_map_first(set);
             state_node;
             state_node = bd_map_next(state_node)) {

            state = (int)
            (&(state_node->key));
        }
/* ... and, if state is
         * in that map ... */
        if (!(bd_map_find(*dfas,
                          &state,
	                      (bd_map_cmp_fn)
                              cmp_states))) {

            /* ... then add the DFA
             * (the node's key) to
             * "dfas" */
            ins_node =
               (struct bd_map_node *)
               bd_alloc(allocs,
                        sizeof(struct
                               bd_map_node),
                        logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(dfas,
                                ins_node,
                                &state,
                                (bd_map_cmp_fn)
	                                cmp_states,
                                &state)))
                return 0;
        }
    }
    return 1; /* return success if
               * we get to here */
}

/* "process_dfa" - perform a call-back routine
 * on the entries of DFA "*dfa", whose input
 * symbol indeces are determined by by calls
 * to the "sym_fn" call-back, which indicates
 * (by returning non-zero), that the current
 * input dymbol requires processing, and the
 * result of a call to the "cond_fn" call-back,
 * similarly indicating, by returning non-zero,
 * that the NFA (from which this DFA was gener-
 * ated) passed to the callback matches a cert-
 * ain condition.
 *
 * The "val_fn" call-back contains the value of
 * the NFA transition for use in processing
 * the entry, while "entry_fn" specifies the
 * processing operation to be performed on the
 * appropriate DFA entries.
 *
 * "map" contains a relation between each uni-
 * que set of NFA states and each's correspond-
 * ing DFA state.
 *
 * Returns zero on error, non-zero otherwise */
static int process_dfa(struct bdlx_dfa **dfa,
	                   struct bdlx_nfa *nfa,
                       bdlx_sym_fn sym_fn,
                       bdlx_val_fn val_fn,
                       bdlx_entry_fn entry_fn,
                       bdlx_cond_fn cond_fn,
                       struct bd_map_node *map,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdlx_nfa_trans *trns;     /* NFA trans- */
    struct bd_map_node *trns_node,   /* ition, its */
	                   *row_node;    /* node and */
	                                 /* current row
	                                 /* node */
	int *row;                        /* DFA row */
	struct bd_map_node *rows, *all = /* set of DFA */
                              NULL;  /* rows for
                                      * this transi-
                                      * tion and all
                                      * DFA rows */
    int sym;                         /* current sym-
                                      * bol */
    struct bdlx_dfa_entry *entry;    /* current DFA
                                      * entry */
    void *val;                       /* relevant
                                      * value in cur-
                                      * rent entry */

    /* iterate through each transition in
     * the NFA from which this DFA was
     * created */
    for (trns_node = bd_map_first(nfa->trns);
         trns_node;
         trns_node = bd_map_next(trns_node)) {

        /* get the transition node's key
         * (an NFA transition) */
        trns = (struct bdlx_nfa_trans *)
                   trns_node->key;

        /* ... and iterate through
         * each possible symbol */
        for (sym = 0; sym < 0xFF; sym++) {

            
            /* if this transition is relevant
             * to this processing ... */
            if (cond_fn(trns,
                        allocs,
                        logger)) {

                /* then set "rows" to the set of
                 * DFA rows corresponding to eith-
                 * er the transition's source state
	             * - if the "sym" call-back is non-
                 * NULL, indicating processing is
                 * performed on a per-symbol basis,
                 * or the destination state if the
                 * "sym" callback is NULL, indicat-
                 * ing processing uses only the
                 * acceptance state of the DFA row */
                if (!(cmbne_states(map,
                                   &rows,
	                               sym ?
                                       trns->from :
                                       trns->to,
                                   allocs,
                                   logger)))
                    return 0;

                /* merge the DFA row set for
                 * this transition into that
                 * of all transitions rele-
                 * vant to this processing */
                if (!(merge(rows,
                            all,
                            &rows,
                            allocs,
		                    logger)))
                    return 0;

                /* iterate through each
                 * DFA row for this trans-
                 * ition  */
                for (row_node = bd_map_first(rows);
                     row_node;
                     row_node = bd_map_next(
                         row_node)) {

                    /* get the row's key (an
                     * NFA state number) */
                    row = (int *) row_node->key;

                    /* and, if the "sym"
                     * callback is non-NULL ... */
                    if (sym_fn) {

                        /* ... and if the
                         * current symbol
                         * is relevant to
                         * this processing
                         * ... */
	                    if (sym_fn(trns,
                                   sym,
                                   allocs,
                                   logger)) {

                            /* get the
                             * current entry */
                            entry = (*dfa)->states
                                       [*row].
                                       ins + sym;

                            /* get the rele-
                             * vant value in
                             * the entry for
                             * this proessing */
                            if (!(val_fn(trns,
                                         &val,
                                         allocs,
                                         logger)))
                                return 0;

	                        /* and set that
                             * value in the
                             * entry */
                            if (!(entry_fn(entry,
                                           trns,
                                           val,
                                           allocs,
                                           logger)))
                                return 0;

                        } else /* "sym" call-
                                * back is NULL
                                * ... */

                            /* ... so just set
                             * the DFA row's acc-
                             * eptance status */
                            (*dfa)->states[*row].
                                      acc = 1;
                    }
                }
            }
        }
    }
    return 1; /* return success if
                * we get to here */
}
 
/* "calc_to_set" - ests *tos" to the set of sta-
 * tes reachable from the set of states "in"
 * using transitions on the symbol "sym" in the
 * NFA "nfa", using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively. Returns zero on error, non-
 * zero otherwise */
static int calc_to_set(struct bd_map_node *in,
		               struct bd_map_node **tos,
                       struct bdlx_nfa *nfa,
                       int sym,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bd_map_node *trns_node, /* transition */
                       *ins_node;  /* node itera-
                                    * tor  */
    struct bdlx_nfa_trans *trans;  /* current
                                    * transition */

    /* bycle through transitions in "nfa" */
    for (trns_node = bd_map_first(nfa->trns);
	     trns_node;
         trns_node = bd_map_next(trns_node)) {

        /* get transition node's key
         * (a transition) */
        trans = (struct bdlx_nfa_trans *)
					trns_node->key;

               
        /* if "sym" is in the
         * transition's range,
         * its source state is
         * in "in" ... */
        if (trans->mn >= sym &&
            trans->mx <= sym &&
            bd_map_find(in,
                        &trans->from,
                        (bd_map_find_fn)
                        cmp_states)) {

            /* then add the trans-
             * ition's destination
             * state to the "to"
             * map */
            ins_node =
                 (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(tos,
                                ins_node,
                                &trans->to,
                                (bd_map_cmp_fn)
                                    cmp_states,
                                &trans->to)))
                      return 0;
		}
    }
    return 1; /* return success
               * if we get to here */
}


/* "build_next_states" - sets "nexts" to an
 * array (an index for each DFA state) of
 * arrays (an index for each input symbol)
 * containing the next DFA states for the
 * DFA, the number of states of which is
 * set in "num", generated from the NFA
 * "nfa", using the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.
 *
 * during the build process, "map" is also
 * built to contain a mapping between each
 * unique set of NFA sets and a correspond-
 * ing DFA state */
static int build_next_states(struct bdlx_nfa *nfa,
		                     int **nexts,
                             int *num,
                             struct bd_map_node **map,
                             struct bd_allocs *allocs,

                             struct bd_logger *logger)
{
    struct bd_map_node *unmarked =  /* yet-to-be- */
                           NULL,    /* processed
                                     * ("unmarked")
                                     * sets */
                       **tos,        /* array of NFA
                                     * sets for each
                                     * input symbol
                                     * (for current
                                     *  DFA state) */
                        *ret_map,   /* return value
                                     * as map of NFA
                                     * states for each
                                     * input symbol
                                     * (a map node for
                                     * each DFA sta-
                                     * te) */
                        *ret_node,  /* current node in
                                     * above map */
                        *next_node, /* map node of
                                     * urrent next
                                     * inddex*/
                       *next,       /* next set to */
                                    /* process */
                       *ins_node,   /* insertion
                                     * node and */
                       **to_map,    /* destination
                                     * map */
                       *start_set = /* start_set */
                            NULL;   /* return state */
    int start = 0,                  /* start state, */
        *ret,                       /* return array, */
        sym,                        /* symbol, value */
        *val,                       /* array and iter- */
        i;                          /* ator */


    /* initialise map  */
    *map = NULL;

    /* insert the start state into the
	 * start set as the it's only member */
    ins_node = (struct bd_map_node *)
          bd_alloc(allocs,
                   sizeof(struct
                          bd_map_node),
                   logger);
    if (!(ins_node))
        return 0;
    if (!(bd_map_insert(&start_set,
                        ins_node,
	                    &start,
                        (bd_map_cmp_fn)
                            cmp_states,
                        &start)))
        return 0;

    /* now allocate yet another ins-
     * ertion node, thus time to use
     * this start set as its only
     * set */
    ins_node = (struct bd_map_node *)
               bd_alloc(allocs,
                       sizeof(struct
                              bd_map_node),
                       logger);
    if (!(ins_node))
        return 0;
    /* make start set empty closure
	 * of that set  */
    if (!(empty_closure(start_set,
                        &start_set,
                        nfa,
                        allocs,
                        logger)))
        return 0;
    /* ... and insert this
     * newly-created set into
     * the returned set */
    if (!(bd_map_insert(&unmarked,
                        ins_node,
                        start_set,
                        (bd_map_cmp_fn)
                            cmp_sets,
                        start_set)))
        return 0;

    *num = 0; /* initialise number
               * of DFA states */
	
	/* continue while we have
	 * unmarked sets ... */
    while (unmarked) {

        /* select an unmarked set, in
         * this case the first unmarked
         * set ... */
        next = bd_map_first(unmarked);

        /* ... remove that set */
        if (!(bd_map_remove(&unmarked,
                            next)))
            return 0;


        /* allocate array of next NFA
         * set for each input symbol */
        tos = (struct bd_map_node **)
               bd_alloc(allocs,
                        sizeof(struct
                               bd_map_node *) *
                                    0xFF,
                        logger);
        if (!(tos))
            return 0;

        /*  then, for each possible
         * input symbol */
        for (i = 0; i < 0xFF; i++) {

            /* set "tos" index for that
             * symbol to the set of NFA

             * sets reachable by transi-
             * tions on symbol */
            if (!(calc_to_set(
                    next,
                    (struct bd_map_node **)
                         tos + i,
                   (struct bdlx_nfa *)
                         nfa,
                    i,
	                allocs,
                    logger)))
                return 0;

            /* then, if any of the sets
             * in an index of  "tos"
             * setin an indeces aren't
             * in "nfa2dfa_map" ... */
            if (!(bd_map_find(*to_map,
                              (void *) tos[i],
                              (bd_map_cmp_fn)
                                  cmp_sets))) {

                /* insert it ... */
                ins_node = (struct bd_map_node *)
                          bd_alloc(allocs,
                                   sizeof(struct
                                          bd_map_node),
                                   logger);
                if (!(ins_node))
                    return 0;

	            if (!(bd_map_insert(map,
                                    ins_node,
                                    tos[i],
                                    (bd_map_cmp_fn)
                                        cmp_sets,
					                &num)))
	                return 0;

                /* ... make the inserted
                 * set unmarked ... */
                ins_node = (struct bd_map_node *)
                          bd_alloc(allocs,
                                   sizeof(struct
                                          bd_map_node),
                                   logger);
                if (!(ins_node))
		            return 0;
                if (!(bd_map_insert(&unmarked,
                                    ins_node,
                                    tos[i],
                                    (bd_map_cmp_fn)
                                        cmp_sets,
			  		                tos[i])))
                    return 0;

                /* ... and insert the 
                 * just-created "to"
                 * set into "ret_map
                 * version of the ret-
                 * urn array */
                ins_node = (struct bd_map_node *)
                          bd_alloc(allocs,
                                   sizeof(struct
                                          bd_map_node),
                                   logger);
                if (!(ins_node))
		            return 0;
                if (!(bd_map_insert(&ret_map,
                                    ins_node,
                                    num,
                                    (bd_map_cmp_fn)
                                        cmp_sets,
					                tos)))
                    return 0;

                /* and indicate there is
                 * now one more NFA state to
                 * add */        
				(*num)++;
            }
        }
    }

    /* at this point, the number
     * of DFA states is known, so
     * the return array can now
     * be allocated */
    ret = (int *) bd_alloc(allocs,
                           sizeof(int) *
                               *num,
                           logger);
    if (!(ret))
        return 0;

    /* iterate through both the return
     * value's indeces, as well as the
     * return map's nodes */
    for (ret_node = bd_map_first(ret_map),
                i = 0;
                ret_node && i < *num;
                i++, ret_node = bd_map_next(
                              ret_node)) {

        /* get the value of "ret_node"
         * which contains the NFA "to"
         * set for each symbol) */
        val = (int *) ret_node->value;
 
        /* then cycle through each
         * symbol ... */
        for (sym = 0; sym < 0xFF; sym++)

            /* converting each NFA set at
             * each symbol index to its equ-
             * ivalent DFA next state */
           if (!(next_node = bd_map_find(
                              *to_map,
	                          (void *)
	                              val[sym],
	                          (bd_map_cmp_fn)
	                              cmp_sets)))
               return 0;

        /* gset the current index
         * from the found node's key */
        val[sym] = * (int *)
        		  next_node->key;
    }

    return 1; /* when we get to this
               * point, return success */
}

/* "comile_re" - sets "nfa" to the
 * NFA able to input strings matching
 * the regular expression given in "re",
 * using the memory allocator and err-
 * or logger given in "allocs and
 * "logger" respectively.  return zero
 * on error, non-zero otherwise */
static int compile_re(char *re,
                      struct bdpr_parser *prsr,
                      struct bdlx_nfa *nfa,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
	struct bdut_str *cps; /* code-point
	                       * string of
	                       * regular
	                       * expression */
    int len = strlen(re); /* regular expr-
    ession length */

	if (!(bdut_from_utf8(cps,
                         re,
                         len,
                         allocs,
                         logger)))
        return 0;

	/* run the regular
	 * expression parser
	 * on "cps", which will
	 * generate "nfa" as
	 * its output */
	return bdpr_run(allocs,
	                logger,
	                prsr,
	                nfa,
	                *cps);
}

/* "alloc_dfa" - returns a DFA containing
 * the array of array of next DFA states
 * given in "nexts" and having the number
 * of states given in "num" */
static int alloc_dfa(int **nexts,
                     int num,
                     struct bdlx_dfa **dfa,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    int i, j;
    struct bdlx_dfa_state *ret;

	*dfa = (struct bdlx_dfa *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdlx_dfa),
                logger);

    /* signal an error if allocation
     * fails */
    if (!(*dfa))
         return 0;

    /* set number of states and
     * allocate states */
    (*dfa)->num = num;
    (*dfa)->states = (struct
	         bdlx_dfa_state *)
               alloc(allocs,
                     sizeof(struct
                         bdlx_dfa_state) *
                         num,
                     logger);

    /* if states allocation fails,
     * free the states */
    if (!((*dfa)->states)) {
    	bd_free(allocs,
    			*dfa);
        return 0;
    }

    /* iterate through the DFA
     * states ... */
    for (i = 0; i < (*dfa)->num; i++) {

        /* ... and allocate each state */
        (*dfa)->states[i].ins =
                (struct bdlx_dfa_entry *)
                bd_alloc(allocs,
                         sizeof(struct
                                bdlx_dfa_entry) *
                                0xFF,
                         logger);

        /* signal an error (and free
         * all allocations) if alloc-
         * ation fails */
        if (!((*dfa)->states[i].ins)) {

            for (j = 0; j < (*dfa)->num; j++)
                bd_free(allocs,
                        (*dfa)->states + j);
            bd_free(allocs,
                    (*dfa));
            return 0;
        }

        /* set each entry's fields and
         * "next" array */
        (*dfa)->states[i].acc = 0;
        (*dfa)->states[i].ins =
                (struct bdlx_dfa_entry *)
                bd_alloc(allocs,
                         sizeof(struct
                                bdlx_dfa_entry) *
                                0xFF,
                         logger);

        /* ... and set each
         * entry from its equivalent
         * "next" index */
        for (j = 0; j < 0xFF; j++)
            (*dfa)->states[i].ins[j].next =
                        nexts[i][j];
    }
}

/* "nfa2dfa" - returns a DFA that accepts exactly the
 * same sentences as the NFA "nfa" - in other words,
 * the equivalent DFA to "nfa", using the memory all-
 * ocator and error logger in "allocs" and "logger",
 * respectively */
static int nfa2dfa(struct bdlx_nfa *nfa,
		           struct bdlx_dfa **dfa,
                   struct bd_allocs *allocs,
			       struct bd_logger *logger)
{
    int **nexts;                          /* arrary of array
                                           * of next DFA
                                           * states*/
    int num;                              /* number of
	                                       * DFA states */
	struct bd_map_node *map;              /* NFA sets to
                                           * each DFA state */
    
    /* build the next states for the
     * DFA, in the process establishing
     * the number of those states */
    if (!(build_next_states(nfa,
    		                nexts,
                            &num,
                            &map,
                            allocs,
                            logger)))
        return 0;

	/* ... allocate DFA from next
     * stetes array */
    if (!(alloc_dfa(nexts,
                    num,
                    dfa,
                    allocs,
                    logger)))
        return 0;

    /* set the acceptance status of
     * each from that of each NFA
     * transition */
    if (!(process_dfa(dfa,
                      nfa,
                      (bdlx_sym_fn)
                            NULL,
                      (bdlx_val_fn)
                            NULL,
                      (bdlx_entry_fn)
                            NULL,
                      (bdlx_cond_fn)
                            NULL,
                      map,
                      allocs,
                      logger)))
        return 0;

    /* ... and set each DFA entry's
     * transition function from each
     * NFA transition's function */
    return process_dfa(dfa,
                       nfa,
                       (bdlx_sym_fn)
                            in_sym,
                       (bdlx_val_fn)
	                        get_fn,
                       (bdlx_entry_fn)
                            set_fn,
                       (bdlx_cond_fn)
                            has_fn,
                       map,
                       allocs,
                       logger);
 }

/* "add_trans" - add the transition
 * represented by "type", "mn", "mx",
 * extending from "from" to "to" and
 * using transition function "fn",
 * with opaque parameter "opaque" to
 * the NFA given in "nfa", using the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int add_trans(struct bdlx_nfa *nfa,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger,
                     enum bdlx_sym_types type,
                     int mn,
                     int mx,
                     int from,
                     int to,
                     int acc,
                     bdlx_trans_fn fn,
                     void *opaque)

{	
    struct bdlx_nfa_trans trans = { /* transition
                                     * to insert */
        type,
        mn,
        mx,
        from,
        to,
        acc,
        fn,
        opaque
    };

    struct bd_map_node *ins_node = /* insertion
                                    * node */
            (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);

    if (!(ins_node))
        return 0;

    /* return the status of insert-
     * ing the transition into
     * "nfa" */
    return bd_map_insert(&nfa->trns,
                         ins_node,
                         &trans,
                         (bd_map_cmp_fn)
                             cmp_trans,
                         &trans);
}

/* "add_range" - add the range repres-
 * ented by by "uppr" and "lwr" into the
 * "ranges" map.  Returns zero on error,
 * non-zero otherwise */
static int add_ranges(struct bd_map_node *ranges,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger,
                      int uppr,
                      int lwr)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */
	struct bdlx_range range = {
        uppr,
        lwr
	};

    /* allocate insertion node ... */
	ins_node = (struct bd_map_node *)
	        bd_allocs(allocs,
                      sizeof(struct
                             bd_map_node),
			          logger);

    if (!(ins_node))
        return 0;

    /* return the insertion status  */
    return bd_map_insert(&ranges,
                         ins_node,
                         &range,
					     (bd_map_cmp_fn)
					         cmp_ranges,
					     &range);
}

/* "neg_range" - sets "*ret" to a
 * map each node of which contains
 * a range between at least one of
 * which a symbol must fall to be
 * the "negative" of "range", that
 * is, thr ranges that will not
 * accept a symbol that range"
 * would accept */
static int neg_range(struct bdlx_range *range,
		             struct bd_map_node **ret,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    *ret = NULL; /*  map is initially empty */

    /* if range lies totally within zero and
     * "MAX_INT", there are two negative ranges:
     * from zero to the lower limit of "range",
     * and from an upper limit of "range" to
     * "MAX_INT"  */
	if (range->lower > 0 &&
                range->upper <
                INT_MAX) {

        if (!(add_range(ret,
                        allocs,
	                    logger,
                        0,
                        range->lower)))
            return 0;

        if (!(add_range(ret,
                        allocs,
	                    logger,
                        range->upper,
                        INT_MAX)))
            return 0;
    }

	/* if, however, the range lower linit is
     * greater than and zero, the hegative
     * range extends from the uppper linit to
     * "MAX_INT" */
    else if (range->lower > 0 &&
             range->upper == INT_MAX)
        if (!(add_range(ret,
                        allocs,
	                    logger,
                        0,
                        range->lower)))
            return 0;

    /* and, if the range upper limit is less
     * than "MAX_INT", the negative range
     * extends from zero to the upper limit
     * to "MAX_INT" */
    else if (range->lower == 0 && range->upper <
                      INT_MAX)
        if (!(add_range(ret,
                        allocs,
	                    logger,
                        range->upper,
                         INT_MAX)))
            return 0;

	return 1; /* if we get to this
               * point, return ducc-
               * essfully */
}

/* "apply_neg" - implements ("applies")
 * the the "negate" pudhes zab NFA that
 * the NFA on top of stack would not acc-
 * ept) operator using the opaque value
 * given in "opaque" and evaluator code
 * list given in "cdes", respectively,
 * and error logger given in "allocs"
 * and "logger", respectively.  This ro-
 * utine serves as the regular expres-
 * sion evaluation stack's "apply" call-
 * back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_neg(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdlx_nfa *nfa, *neg;   /* NFA in stack
                                   * top and its
                                   * negative */
    struct bd_map_node *it, *it2, /* transition */
                       *ins_node, /* iterators, */
                       *negs;     /* insertion node */
                                  /* and negative
                                   * ranges */
    struct bdlx_range *range;     /* current range */
    struct bdlx_nfa_trans *trans; /*  current
                                   * transition */





    /* get NFA from evaluator's
     * stack top and initialise
     * negative NFA to empty */
    nfa = (struct bdlx_nfa *)
              cdes->evltr->
              rt_top->val->val;
    neg->trns = NULL;
    neg->num = 0;


    /* iterate through "nfa"'s
     * transitions */
    for (it = bd_map_first(nfa->trns);
         it;
		 it = bd_map_next(it)) {
        
	    /* get current transition
         * as its key */
        trans = (struct bdlx_nfa_trans *)
               it->key;

        /* get the ranges of the
         * transition */
        if (!(neg_range(range,
                        &negs,
                        allocs,
                        logger)))
            return 0;

        /* iterate through the neg-
         * ative ranges */
        for (it2 = bd_map_first(negs);
	         it2;
			 it2  = bd_map_next(it2)) {

            /* get the node key
             * (a range) */
            range = ( struct bdlx_range *)
	                   it2->key;

            /* set the transition's symbol
             * range from this negative
             * range */
            trans->mn = range->lower;
            trans->mx = range->upper;

            /* allocate the insert-
             * ion node ...*/
            ins_node = (struct bd_map_node *)
                    bd_alloc(allocs,
                             sizeof(struct
                                   bd_map_node),
		                     logger);
            if (!(ins_node))
                return 0;

            /* and, if the allocation was succ-
             * essful, insert this negative
             * transition into the negative
             * negative NFA's transition map
             * and indicate an added transit-
             * ion*/
            if (!(bd_map_insert(&neg->trns,
                                ins_node,
	                            trans,
	                            (bd_map_cmp_fn)
                                    cmp_trans,
                                trans)))
                return 0;
            neg->num++;
        }
	}
    return 1; /* return succ-
               * essfully if we
               * get to this
               * point */
}

/* "apply_ast" - implements ("applies")
 * the the "asterisk" (repeat zero or mo-
 * re times) operator using the opaque
 * value given in "opaque" and evaluator
 * code list  given in "cdes", respecti-
 * vely, and error logger given in "allo-
 * cs" and "logger", respectively.  This
 * routine serves as the regular expres-
 * sion evaluation stack's "apply" call-
 * back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_ast(void *opaque,
                     struct bdev_cdes *cdes,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdlx_nfa *frag =       /* NFA on */
          (struct bdlx_nfa *)     /* top of */
           cdes->evltr->
               rt_top->opaque; /* evaluation
                               /* stack */
    struct bdlx_nfa *nfa =     /* repetition */
		      frag;               /* NFA */

    /* add transitions to both by-pass
     * and repeat "nfa" */
    if (!(add_trans(nfa,
                    allocs,
                    logger,
                    bdlx_sym_type_empty,
                    0,
                    0,
                    0,
                    nfa->num,
                    0,
                    NULL,
                    NULL)))
        return 0;

	if (!(add_trans(nfa,
                    allocs,
                    logger,
                    bdlx_sym_type_empty,
                    0,
                    0,
                    nfa->num,
                    0,
                    0,
                    NULL,
                    NULL)))
	    return 0;

    /* set the top of the
     * run-time stack to
     * the built NFA */
    cdes->evltr->rt_top->
        opaque = (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* "apply_ast" - implements ("applies")
 * the the "question" (repeat zero or one
 * re times) operator using the opaque
 * value given in "opaque" and evaluator
 * code list  given in "cdes", respecti-
 * vely, and error logger given in "allo-
 * cs" and "logger", respectively.  This
 * routine serves as the regular expres-
 * sion evaluation stack's "apply" call-
 * back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_qstn(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlx_nfa *frag =    /* NFA on */
          (struct bdlx_nfa *)  /* top of */
           cdes->evltr->       /* evaluation */
               rt_top->opaque; /* stack */
   struct bdlx_nfa *nfa =      /* NFA fragment */
		  frag;                /* to repeat */

    /* add a forward transition
     * around the fragment to make
     * it optional */
    if (!(add_trans(nfa,
                    allocs,
                    logger,
                    bdlx_sym_type_empty,
                    0,
                    0,
                    0,
                    nfa->num,
                    0,
                    NULL,
                    NULL)))
    	return 0;

    /* set the top of the
     * run-time stack to
     * the that NFA */
    cdes->evltr->rt_top->opaque =
           (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* "apply_ast" - implements ("applies")
 * the the "plus" (repeat moe than one
 * time) operator using the opaque value
 * given in "opaque" and evaluator code
 * list given in "cdes", respectively,
 * and error logger given in "allocs"
 * and "logger", respectively.  This
 * routine serves as the regular expres-
 * sion evaluation stack's "apply" call-
 * back for this operator.  Returns zero
 * on error, non-zero otherwise */
static int apply_plus(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlx_nfa *frag =   /* NFA on */
          (struct bdlx_nfa *) /* top of */
           cdes->evltr->      /* evaluation */
              rt_top->opaque; /* stack */
    struct bdlx_nfa *nfa =    /* repetition */
		      frag;           /* NFA */

    /* repeat "nfa" once,
     * ensuring first iteration
     * can't be by-passed  */
    if (!(cat_nfas(nfa,
                   frag,
                   &nfa,
                   allocs,
                   logger)))
        return 0;

    /* add an empty back
     * transition, which
     * constitutes the
     * loop */
    if (!(add_trans(nfa,
                    allocs,
                    logger,
                    bdlx_sym_type_empty,
                    0,
                    0,
                    nfa->num,
                    0,
                    0,
                    NULL,
                    NULL)))
        return 0;

    /* set the top of the
     * run-time stack to
     * the that NFA */
    cdes->evltr->
           rt_top->opaque =
		   (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* "apply_from" - implements ("applies")
 * the the "repeat from" (repeat a cert-
 * ain minimum number of times) operator
 * using the opaque value given in "opa-
 * que" and evaluator code list given in
 * "cdes", respectively, and error logg-
 * er given in "allocs" and "logger",
 * respectively.  This routine serves
 * as the regular expression evaluation
 * stack's "apply" call-back for this
 * operator.  Returns zero on error,
 * non-zero otherwise */
static int apply_from(void *opaque,
                      struct bdev_cdes *cdes,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlx_nfa *frag =   /* NFA on */
          (struct bdlx_nfa *) /* top of */
           cdes->evltr->      /* evaluation */
           rt_top->opaque;    /* stack */
                              /* repetition */
    struct bdlx_nfa *nfa =    /* NFA */
		      frag;           /* repetition */
    int *from_reps = (int *)  /* count  */
          cdes->evltr->       /* in one below */
          rt_top->prcd->      /* stack top*/
          opaque, i;          /* and iterator */


	/* repeat "nfa" the minimum
     * number of times */
    for (i = 0; i < *from_reps; i++)
        if (!(cat_nfas(nfa,
                       frag,
        		       &nfa,
        		       allocs,
        		       logger)))
            return 0;

    /* ... and add a backwards
     * empty transition */
    if (!(add_trans(nfa,
                    allocs,
                    logger,
                    bdlx_sym_type_empty,
                    0,
                    0,
                    frag->num,
			        0,
                    0,
                    NULL,
                    NULL)))
        return 0;

    /* set the top of the
     * run-time stack to
     * that NFA */
    opaque = (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* "apply_rep_to" - implements ("applies") the
 * the "rep. from" (repeat a certain maximum
 * number of times) operator in the expression
 * evaluator "evltr", using the memory allocator,
 * and error logger, and opaque parameter given
 * in "allocs", "logger" and "opaque", respecti-
 * vely.  This routine ser- ves as the regular
 * expression evaluation stack's "apply" call-
 * back for this operator.  Returns zero on er-
 * ror, non-zero otherwise */
static int apply_rep_to(struct bdev_evltr *evltr,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bdlx_nfa *frag =       /* NFA on */
          (struct bdlx_nfa *)     /* top of */
           evltr->rt_top->opaque; /* evaluation
                                  /* stack */
    struct bdlx_nfa *nfa =        /* repetition */
		      frag;               /* NFA */
    int *to_reps = (int *)        /* repetition */
          evltr->rt_top->prcd->   /* count  */
          opaque, i;              /* in one below */
                                  /* stack top
                                   * and iterator*/

	for (i = 0; i < *to_reps; i++) {

        /* repeat the fragment "to_reps"
         * number of times ... */
        if (!(cat_nfas(nfa,
                       frag,
                       &nfa,
                       allocs,
                       logger)))
            return 0;
	
        /* by-pass the fragment
         * with ab empty transi-
         * tion */
        if (!(add_trans(frag,
                        allocs,
                        logger,
                        bdlx_sym_type_empty,
                        0,
                        0,
                        frag->num * i,
                        frag->num * (i + 1),
                        0,
                        NULL,
                        NULL)))
            return 0;
    }

    /* set the top of the
     * run-time stack to
     * that NFA */
    evltr->rt_top->opaque =
		    (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* "apply_rep_range" - implements ("applies") the
 * the "rep. from" (repeat a certain range
 * number of times) operator in the expression
 * evaluator "evltr", using the memory allocator,
 * error logger and opaque value given in "all-
 * ocs", "logger" and "opaque", respectively.
 * This routine serves as the regular expression
 * evaluation stack's "apply" call-back for this
 * operator */
static int apply_rep_range(struct bdev_evltr *evltr,
                           void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdlx_nfa *frag =       /* NFA on */
          (struct bdlx_nfa *)     /* top of */
           evltr->rt_top->opaque; /* evaluation
                                  /* stack */
    struct bdlx_nfa *nfa =        /* repetition */
		      frag;               /* NFA */
    int *from_reps = (int *)      /* minimum rep- */
         evltr->rt_top->prcd->	  /* itition */
                    opaque,       /* count */
                                  /* in one below */
                                  /* stack top */
         *to_reps = (int *)       /* maximum rep- */
         evltr->rt_top->          /* etition */
         prcd->prcd->opaque,      /* count */
         i;                       /* in two below */
                                  /* stack top and
                                   * itator*/

    /* add the fragment "from_reps"
     * times, making those repetit-
     * ions mandatory */
    for (i = 0; i < *from_reps; i++)
        if (!(cat_nfas(nfa,
                       frag,
                       &nfa,
                       allocs,
                       logger)))
            return 0;
	
    /* add an empty transition around
     * the next "from_reps" to "to_reps"
     * repetitions to make them opt-
     * ional */
	for (i = *from_reps;
         i < *to_reps;
         i++)
        if (!(add_trans(nfa,
        		        allocs,
        		        logger,
                        bdlx_sym_type_empty,
                        0,
                        0,
                        nfa->num = frag->num * i - 1,
                        nfa->num - frag->num * i,
                        0,
                        NULL,
                        NULL)))
            return 0;

    /* set the top of the
     * run-time stack to
     * that NFA */
    evltr->rt_top->opaque =
		    (void *) nfa;

    return 1; /* return success
               * if we get to this
               * point */
}

/* Regular expression grammar production
 * recognition routines */

/* "reduce_re_list" - perform the action
 * associated with the recognition of a
 * regular expression "list", which is a
 * sequence of one or more regular expr-
 * essions, using the reduce items in
 * "items", the number of which is given
 * in"num", the regular expression parser
 * in "parser", and the memory allocator
 * and error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_re_list(struct bdev_cdes *cdes,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* push "sequence" operatorr */
    return bdev_push_op(cdes,
                        bdlx_re_op_seq,
                        NULL,
                        allocs,
                        logger);
}

/* "reduce_rep" - perform the action
 * associated with the recognition of a
 * regular expression "repetition",
 * which is refers to a sequence of
 * one or more regular expressions,
 * using the reduce items in "items",
 * the number of which is given in
 * "num", the regular expression parser
 * in "parser" and the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
static int reduce_re_rep(struct bdev_cdes *cdes,
                         void *lhs,
                         union bdgm_reduce_item *items,
                         int num,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* get get evaluator from parser's
     * run-time pointer */
	struct bdev_evltr *evltr =
           (struct bdev_evltr *)
			cdes->evltr;

    /* get the NFAs from the fragment and
     * repetition count from the right-hand-
     * side items first non-terminal and
     * third numeric token, respectively */
    struct bdlx_nfa * frag =
    (struct bdlx_nfa *) items[0].nt;
    int *rep = (int *)
            ((struct bdlx_re_tok *)
             (items[2].tok))->u.num;

    /* emit code to push fragment
     * to repeat */
    if (!(bdev_push_val(cdes->evltr,
                        bdlx_re_stk_type_nfa,
                        frag,
                        allocs,
                        logger)))
        return 0;

    /* emit code to push count */
    if (!(bdev_push_val(cdes->evltr,
                        bdlx_re_stk_type_int,
                        rep,
                        allocs,
                        logger)))
        return 0;

    /* push "rep" operator with
     * those two previously
     * pushed values as paramet-
     * ers */
    return bdev_push(evltr,
                     bdlx_re_op_rep,
                     NULL,
                     allocs,
                     logger);
}

/* "dig_start" - lexical transition call-
 * back yp start a digit sequence, using
 * the token, digit, opaque parameter, memory
 * allocator smd error logger given in
 * "tok", "dig", "opaque", "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int dig_start(struct bdlx_tok *tok,
                     int dig,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
	struct bdlx_re_tok *re_tok =
	         (struct bdlx_re_tok *) tok;

	re_tok->u.num = (dig - '0');

	return 1;
}

/* "dig_add" - lexical transition call-
 * back yp add to a digit sequence, using
 * the token, digit, opaque parameter,
 * memory allocator smd error logger given
 * in "tok", "dig", "opaque", "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int dig_add(struct bdlx_tok *tok,
                   void *opaque,
                   int dig,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
	struct bdlx_re_tok *re_tok =
	         (struct bdlx_re_tok *) tok;

	re_tok->u.num += (dig - '0');
	return 1;
}

/* "buf_start" - lexical transition call-
 * back yp add to start cillecting chara-
 * cters to a buffer, using the token,
 * initial code-point, opaque parameter,
 * memory allocator smd error logger given
 * in "tok", "cp", "opaque", "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int buf_start(struct bdlx_tok *tok,
                     void *opaque,
                     int cp,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdlx_re_tok *re_tok =
	         (struct bdlx_re_tok *) tok;

    /* initialise token's buffer */
    re_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));

    /* and return success if inialis-
     * ation succeeded */
	return  re_tok->u.buf != NULL;
}

/* "buf_add" - lexical transition call
 * back to add a character to a buffer,
 * using the token, added code-point,
 * opaque parameter, memory allocator
 * and error logger given in "tok",
 * "cp", "opaque", "allocs" and "logger",
 * rspectively.  Returns zero on error,
 * non-zero otherwise */
static int buf_add(struct bdlx_tok *tok,
                   void *opaque,
                   int cp,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdlx_re_tok *re_tok =
           (struct bdlx_re_tok *) tok;

    /* add "cp" to token's buffer */
    return bdbf_add(re_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
    return 1;

}

/* "add_to_buffer" - lexical analyser
 * transition call-back to add the code-
 * point "cp" to the buffer field of the
 * regular expression token in "tok".
 * Returns zero on an error, non-zero
 * otherwise */
static int add_to_buffer(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdlx_re_tok *re_tok =
              (struct bdlx_re_tok *) tok;

     /* add "cp" to "tok"'s buffer */
     return bdbf_add(re_tok->u.buf,
                     allocs,
                     logger,
                     (void *) cp);
}

/* "start_buffer" - lexical analyser
 * transition call-back to initialise
 * the code the buffer field of the
 * regular expression token in "tok".
 * Returns zero on an error, non-zero
 * otherwise */
static int start_buffer(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdlx_re_tok *re_tok =
              (struct bdlx_re_tok *) tok;

    /* initialise "tok"'s buffer */
    if (!(re_tok->u.buf = bdbf_init(allocs,
                                    logger,
                                    sizeof(char))))
    	return 0;

	/* add "cp" to the buffer */
     return bdbf_add(re_tok->u.buf,
                     allocs,
                     logger,
                     (void *) cp);
}

/* "end_def_ref" - lexical analyser
 * transition call-back to end coll-
 * ection of the text of a definition
 * name by "flattening" "tok"'s buffer
 * field and storing it in its defi-
 * nition field.  Returns zero on an
 * error, non-zero otherwise */
static int end_def_ref(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
	struct bdlx_re_tok *re_tok =
              (struct bdlx_re_tok *) tok;

    /* end (by "flattening")
     * "tok"'s buffer */
    if (!(re_tok->u.defn = (char *)
                 bdbf_flttn(re_tok->u.buf,
                            allocs,
                            logger)))
        return 0;

    /* ... and NULL-terminate the
     * created definition name */
    re_tok->u.defn[
           bdbf_len(re_tok->u.buf)
           ] = 0;

	/* add "cp" to the buffer */
     return bdbf_add(re_tok->u.buf,
                     allocs,
                     logger,
                     (void *) cp);
}

/* "dfn_end" - lexical transition call
 * back yp add to emd a definition name,
 * using the token, added code-point,
 * (unused in this case) opaque param-
 * eter, memory allocator and error
 * logger given in "tok", "cp",
 * "opaque", "allucs" and "logger",
 * rspectively.  Returns zero on error,
 * non-zero otherwise */
static int dfn_end(struct bdlx_tok *tok,
                   void *opaque,
                   int cp,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdlx_re_tok *re_tok =
	         (struct bdlx_re_tok *) tok;

	/* set token's definition field to
	 * "flattening of token's buffer */
    if (!(re_tok->u.defn = (char *) bdbf_flttn(
                                re_tok->u.buf,
                                allocs,
                                logger)))
        return 0;

    /* ... and NULL-terminate the
     * created definition name */
    re_tok->u.defn[
           bdbf_len(re_tok->u.buf)
           ] = 0;


    /* return success if flattening
     * (now contained in buffer field)
     * succeeded (is non-zero)  */
    return 1;
}

/* "reduce_dgts_on" - perform the action
 * associated with the opening brace of
 * a repitition sequence, meaning that
 * the repetition number be interpreted as
 * a number rather than an ASCii digit
 * sequence, on the "reduce" items in
 * "items", the number of which is given
 * in "num", the output evaluator in "ev-
 * ltr", and the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_dgts_on(struct bdev_cdes *cdes,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* get the regular exoress-
     * ion token in in the first
     * right-hand-side item */
    struct bdlx_re_tok *re_tok =
         (struct bdlx_re_tok *)
         items[0].tok;

    /* set the token's digit
     * interpretation flag */
    re_tok->dgts = 1;
    return 1;

}

/* "reduce_dgts_off" - perform the action
 * associated with the closing brace
 * of a repetition sequence, meaning that
 * the repetition number not be interpreted
 * as a number rather than an ASCII digit
 * sequence, on the "reduce" items in
 * "items", the number of which is given
 * in "num", the output evaluator in "ev-
 * ltr", and the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_dgts_off(struct bdev_cdes *cdes,
                           void *lhs,
                           union bdgm_reduce_item *items,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* get the regular exoress-
     * ion token in in the first
     * right-hand-side item */
    struct bdlx_re_tok *re_tok =
         (struct bdlx_re_tok *)
         items[0].tok;

    /* clear digit interpretation flag */
    re_tok->dgts = 0;
    return 1;
}

/* "reduce_dfns_on" - perform the action
 * associated with the recopening brace
 * of a definition name, reuting that
 * the reoyition alphabetic sequences
 * be interpreted as definitons referen-
 * ces rather than an ASCII sequence, on
 * the "reduce" items in "items", the
 * number of which is given in "num", the
 * output evaluator in "evltr", and the
 * memory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int reduce_dfns_on(struct bdev_cdes *cdes,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* get the regular exoress-
     * ion token in in the first
     * right-hand-side item */
    struct bdlx_re_tok *re_tok =
         (struct bdlx_re_tok *)
         items[0].tok;

    /* set definition capturing
     * flag */
    re_tok->dfns = 1;
    return 1;

}

/* "reduce_dfns_off" - perform the action
 * associated with the closing brace of
 * a definition name, meaning that alp-
 * habetic sequences be interpreted as def-
 * inition references rather than an ASCii
 * sequences, on the "reduce" items in
 * "items", the number of which is given
 * in "num", the output evaluator in "evltr",
 * and the memory allocator and error logger
 * given in "allocs" and "logger", respecti-
 * vely. Returns zero on error, non-zero
 * otherwise */
static int reduce_dfns_off(struct bdev_cdes *cdes,
                           void *lhs,
                           union bdgm_reduce_item *items,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* get the regular exoress-
     * ion token in in the first
     * right-hand-side item */
    struct bdlx_re_tok *re_tok =
         (struct bdlx_re_tok *)
         items[0].tok;

    /* clear definition capturing
     * flag */
    re_tok->dfns = 0;
    return 1;

}

/* "reduce_expr" - perform the action
 * associated with of a sub-expression
 * between oaraentheses, on the "reduce"
 * items in "items", the number of which
 * is given in "num", the regular expres-
 * sion evaluator in "evltr", and the me-
 * mory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int reduce_expr(struct bdev_cdes *cdes,
                       void *lhs,
                       union bdgm_reduce_item *items,
                       int num,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    return 1;
}

/* "reduce_def_ref" - perform the action
 * associated with of a "def ref -> {id}"
 * between oaraentheses, on the "reduce"
 * items in "items", the number of which
 * is given in "num", the regular expres-
 * sion evaluator in "evltr", and the me-
 * mory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int reduce_def_ref(struct bdev_cdes *cdes,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    return 1;
}

/* "reduce_neg_def_ref" - perform the act-
 * ionassociated with of a "def ref ->
 * {<id>}" production, on the "reduce"
 * items in "items", the number of which
 * is given in "num", the regular expres-
 * sion evaluator in "evltr", and the me-
 * mory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int reduce_neg_def_ref(struct bdev_cdes *cdes,
                              void *lhs,
                              union bdgm_reduce_item *items,
                              int num,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    return 1;
}

/* "reduce_bar" - perform the action
 * associated with of a "expression ->
 * expression | expression" production,
 * on the "reduce" items in "items", the
 * number of which is given in "num",
 * the regular expres-sion evaluator in
 * "evltr", and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_bar(struct bdev_cdes *cdes,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    return 1;
}

/* "reduce_range_rep" - perform the act-
 * ion associated with the repetition of
 * an NFA between a minimum and maximum
 * number of times, on the "reduce" it-
 * ems in "items", the number of which
 * is given in "num", the regular expres-
 * sion evaluator in "evltr", and the me-
 * mory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int reduce_range_rep(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_re+from_rep" - perform the act-
 * ion associated with the repetition of
 * an NFA from a min-imum number of tim-
 * es, on the "reduce" items in "items",
 * the number of which is given in "num",
 * the regular expression evaluator in
 * "evltr", and the memory allocator and
 * error logger givenin "allocs" and "lo-
 * gger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_re_from_rep(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_to_rep" - perform the act-
 * ion associated with the repetition of
 * an NFA up to a maximum number of tim-
 * es, on the "reduce" items in "items",
 * the number of which is given in "num",
 * the regular expression evaluator in
 * "evltr", and the memory allocator and
 * error logger givenin "allocs" and "lo-
 * gger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int reduce_to_rep(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_dfn_off" - perform the act-
 * ion associated with clearing the
 * definition collection flag on the
 * "reduce" items in "items", the nu-
 * mber of which is given in "num",
 * the regular expression evaluator
 * in "evltr", and the memory alloc-
 * ator and error logger givenin "a-
 * llocs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_dfn_off(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_dfn_on" - perform the act-
 * ion associated with setting the
 * the definition collection flag on
 * the "reduce" items in "items", the
 * number of which is given in "num",
 * the regular expression evaluator
 * in "evltr", and the memory alloc-
 * ator and error logger givenin "all-
 * ocs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_dfn_on(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_class_list" - perform the act-
 * ion associated with a character class
 * list on the "reduce" items in "items",
 * the number of which is given in "num",
 * the regular expression evaluator in
 * "evltr", and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_class_list(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_neg_class_list" - perform the
 * action associated with the negation
 * of a character class list on the "red-
 * uce" items in "items", the number of
 * which is given in "num", the regular
 * expression evaluator in "evltr" and
 * the memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_neg_class_list(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_clsses" - perform the action
 * associated with the beginning of a
 * character class list on the "reduce"
 * items in "items", the number of whi-
 * ch is given in "num", the regular ex-
 * pression evaluator in "evltr" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_clsses(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_clss" - perform the action
 * associated with a single class of a
 * character class list on the "reduce"
 * items in "items", the number of whi-
 * ch is given in "num", the regular ex-
 * pression evaluator in "evltr" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_clss(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_clss_sym" - perform the act-
 * ion associated with a single class of
 * a character class list consisting of
 * a single symbol on the "reduce" items
 * in "items", the number of which is
 * given in "num", the regular express-
 * ion evaluator in "evltr" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_clss_sym(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_clss_rnge" - perform the act-
 * ion associated with a single class of
 * a character class list consisting of
 * a tamge of symbols on the "reduce"
 * items in "items", the number of which
 * is given in "num", the regular expre-
 * ssion evaluator in "evltr" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_clss_rnge(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_clss_def_ref" - perform the
 * action associated with a single class
 * of a character class list consisting
 * of a definition reference on the "red-
 * uce" items in "items", the number of
 * which is given in "num", the regular
 * expression evaluator in "evltr" and
 * the memory allocator and error logger
 * given in "allocs" and "logger", resp-
 * ectively.  Returns zero on error, non-
 * zero otherwise */
static int reduce_clss_def_ref(
		        struct bdev_cdes *cdes,
		        void *lhs,
		        union bdgm_reduce_item *items,
                int num,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    return 1;
}

/* "init_re" - regular expression parser's
 * initialisation call-back function, using
 * "re" as the regular expression parser,
 * opaque parameter, memory allocator and
 * error logger given in "re", "opaque",
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int init_re(struct bdpr_parser *re,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdev_evltr *evltr =  /* regular */
          (struct bdev_evltr *) /* expression's */
           re->rt;              /* evaluator in
	                             * in parser's
                                 * run-time poi-
                                 * nter */

    /* assign regular expression lexical
     * states from analyser's pool of unu-
     * sed states */
    int dig_start_state  = bdlx_new_state(re->lex),
        dig_add_state    = bdlx_new_state(re->lex),
        defn_start_state = bdlx_new_state(re->lex),
        defn_add_state   = bdlx_new_state(re->lex);

    /* regular expression
     * parser's output codes */
    struct bdev_cdes *cdes;

    if (!(cdes = bdev_cdes_init(evltr,
                                allocs,
                                logger)))
    	return 0;

    /* ... initialise evaluator itself */
    if (!(evltr = bdev_init(bdev_exec_type_intrp,
                            re_prec_table,
                            allocs,
                            logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_rep,
                       apply_rep,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_seq,
                       apply_seq,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_chce,
                       apply_chce,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_neg,
                       NULL,
                       NULL,
                       apply_neg,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_dfn_ref,
                       apply_dfn_ref,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    if (!(bdev_add_cde(evltr,
                       bdlx_re_cde_end,
                       apply_end,
                       NULL,
                       NULL,
                       allocs,
                       logger)))
        return 0;

    /* ... add evaluator's operators */
    if (!(bdev_add_op(evltr,
                      bdlx_re_op_rep,
                      bdlx_re_cde_rep,
                      0,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdlx_re_op_seq,
                      bdlx_re_cde_seq,
                      0,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdlx_re_op_alt,
                      bdlx_re_cde_chce,
                      0,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdlx_re_op_lpr,
                      bdev_res_cde_none,
                      0,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    if (!(bdev_add_op(evltr,
                      bdlx_re_op_rpr,
                      bdev_res_cde_none,
                      0,
                      bdev_assc_l2r,
                      allocs,
                      logger)))
        return 0;

    /* add parser's lexical tokens */
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\*",
                        bdlx_start_state,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_ast)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\?",
                        bdlx_start_state,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_qstn)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\+",
                        bdlx_start_state,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_plus)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\|",
                        bdlx_start_state,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_bar)))
        return 0;

    /*  if we're scanning digit
     * sequences as numbers ... */
    if (((struct bdlx_re_tok *)
    		(re->tok))->dgts) {

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[0-9]",
                            bdlx_start_state,
                            dig_start_state,
                            (bdlx_trans_fn)
                                dig_start,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[0-9]",
                            dig_start_state,
                            dig_add_state,
                            (bdlx_trans_fn)
                                dig_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[0-9]",
                            dig_add_state,
                            dig_add_state,
                            (bdlx_trans_fn)
                                dig_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[^0-9]",
                            dig_add_state,
                            dig_add_state,
                            (bdlx_trans_fn)
                                dig_add,
                            NULL,
                            1,
                            bdlx_re_tok_num)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[^0-9]",
                            dig_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdlx_re_tok_num)))
            return 0;
    }

    /*  if we're scanning for alpha-
     * betic sequences as definiti-
     * ons ... */
    if (((struct bdlx_re_tok *)
            (re->tok))->dfns) {

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[A-Za-z]",
                            bdlx_start_state,
                            defn_start_state,
                            (bdlx_trans_fn)
                                start_buffer,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[A-Za-z]",
                            defn_start_state,
                            defn_add_state,
                            (bdlx_trans_fn)
                                add_to_buffer,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[A-Za-z]",
                            defn_add_state,
                            defn_add_state,
                            (bdlx_trans_fn)
                                add_to_buffer,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[^A-Za-z]",
                            defn_add_state,
                            0,
                            (bdlx_trans_fn)
                                end_def_ref,
                            NULL,
                            1,
                            bdlx_re_tok_defn)))
            return 0;

        if (!(bdlx_add_rule(re->lex,
                            allocs,
                            logger,
                            "[^A-Za-z]",
                            defn_start_state,
                            0,
                            (bdlx_trans_fn)
                                end_def_ref,
                            NULL,
                            1,
                            bdlx_re_tok_defn)))
            return 0;
    }

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\(",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_lpar)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_lsq)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_rsq)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\{",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_lcly)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\}",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_rcly)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\.",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_any)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "\\-",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdlx_re_tok_dsh)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        bdlx_start_state,
                        defn_start_state,
                        (bdlx_trans_fn)
                            start_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        defn_start_state,
                        defn_add_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        defn_add_state,
                        defn_add_state,
                        (bdlx_trans_fn)
                            add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[^a-zA-Z]",
                        defn_add_state,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_defn)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        bdlx_start_state,
                        dig_start_state,
                        (bdlx_trans_fn)
                            dig_start,
                        NULL,
                        0,
                        0)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        dig_start_state,
                        dig_add_state,
                        (bdlx_trans_fn)
                            dig_add,
                        NULL,
                        0,
                        0)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        dig_add_state,
                        dig_add_state,
                        (bdlx_trans_fn)
                            dig_add,
                        NULL,
                        0,
                        0)))
        return 0;
	
    if (!(bdlx_add_rule(re->lex,
                        allocs,
                        logger,
                        "[^0-9]",
                        0,
                        0,
                        0,
                        NULL,
                        1,
                        bdlx_re_tok_num)))
        return 0;
	
    /* ... and add the regular expression
     * parser's grammar productions */
    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_re_list,
                        2,
                        (bdgm_reduce_fn)
                            reduce_re_list,
                        bdlx_re_nt_re_list,
                        bdlx_re_nt_re)))
        return 0;
	
    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_re,
                        3,
                        (bdgm_reduce_fn)
                            reduce_expr,
                        bdlx_re_tok_lpar,
                        bdlx_re_nt_re,
                        bdlx_re_tok_rpar)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_rnge_rep,
                        5,
                        (bdgm_reduce_fn)
                            reduce_range_rep,
                        bdlx_re_nt_dgts_on,
                        bdlx_re_tok_int,
                        bdlx_re_tok_cma,
                        bdlx_re_tok_int,
                        bdlx_re_nt_dgts_off)))
        return 0;
	
    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clss_list,
                        3,
                        (bdgm_reduce_fn)
                            reduce_class_list,
                        bdlx_re_tok_lsq,
                        bdlx_re_nt_clsses,
                        bdlx_re_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clss_list,
                        4,
                        (bdgm_reduce_fn)
                            reduce_neg_class_list,
                        bdlx_re_tok_lsq,
                        bdlx_re_tok_crt,
                        bdlx_re_nt_clsses,
                        bdlx_re_tok_rsq)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clsses,
                        2,
                        (bdgm_reduce_fn)
                            reduce_clsses,
                        bdlx_re_nt_clsses,
                        bdlx_re_nt_clss)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clsses,
                        1,
                        (bdgm_reduce_fn)
                            reduce_clss,
                        bdlx_re_nt_clss)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clss,
                        1,
                        (bdgm_reduce_fn)
                            reduce_clss_sym,
                        bdlx_re_tok_sym)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clss,
                        3,
                        (bdgm_reduce_fn)
                            reduce_clss_rnge,
                        bdlx_re_tok_sym,
                        bdlx_re_tok_dsh,
                        bdlx_re_tok_sym)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_clss,
                        3,
                        (bdgm_reduce_fn)
                            reduce_clss_def_ref,
                        bdlx_re_tok_lcrly,
                        bdlx_re_tok_defn,
                        bdlx_re_tok_rcrly)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_from_rep,
                        4,
                        (bdgm_reduce_fn)
                            reduce_re_rep,
                        bdlx_re_nt_re,
                        bdlx_re_nt_dgts_on,
                        bdlx_re_tok_int,
                        bdlx_re_nt_dgts_off)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_from_rep,
                        5,
                        (bdgm_reduce_fn)
                            reduce_re_from_rep,
                        bdlx_re_nt_re,
                        bdlx_re_nt_dgts_on,
                        bdlx_re_tok_cma,
                        bdlx_re_tok_int,
                        bdlx_re_nt_dgts_off)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_to_rep,
                        5,
                        (bdgm_reduce_fn)
                            reduce_to_rep,
                        bdlx_re_nt_re,
                        bdlx_re_nt_dgts_on,
                        bdlx_re_tok_int,
                        bdlx_re_tok_cma,
                        bdlx_re_nt_dgts_off)))
        return 0;
		
    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_re,
                        3,
                        (bdgm_reduce_fn)
                            reduce_bar,
                        bdlx_re_nt_re,
                        bdlx_re_tok_bar,
                        bdlx_re_nt_re)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_def_ref,
                        3,
                        (bdgm_reduce_fn)
                            reduce_def_ref,
                        bdlx_re_nt_dfn_on,
                        bdlx_re_tok_defn,
                        bdlx_re_nt_dfn_off)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_def_ref,
                        4,
                        (bdgm_reduce_fn)
                            reduce_neg_def_ref,
                        bdlx_re_nt_dfn_on,
                        bdlx_re_tok_crt,
                        bdlx_re_tok_defn,
                        bdlx_re_nt_dfn_off)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_dfn_on,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dfn_on,
                        bdlx_re_tok_lcly)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_dfn_off,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dfn_off,
                        bdlx_re_tok_rcly)))
        return 0;

    if (!(bdgm_prod_add(re->gmr,
                        allocs,
                        logger,
                        evltr,
                        bdlx_re_nt_dgts_on,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dgts_on,
                        bdlx_re_tok_lcly)))
        return 0;

    return bdgm_prod_add(re->gmr,
                         allocs,
                         logger,
                         evltr,
                         bdlx_re_nt_dgts_off,
                         1,
                         (bdgm_reduce_fn)
                             reduce_dgts_off,
                         bdlx_re_tok_rcly);
}

/* "run_dfa" executes the entries in the DFA
 * "dfa", uasing the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively, in the orcess increntling the
 * input pointer "in" by the number if chara-
 * cters read from the input stream and setting
 * "tok" to rekfexct the tuoken foumd in that
 * stream */
static int run_dfa(struct bdlx_dfa *dfa,
                   int **in,
                   struct bdlx_tok *tok,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger) 
{
	int state = 0;                 /* current
	                                * state */
	struct bdlx_dfa_entry *entry;  /* xirrent
	                                * entry */

    do { /* repeat intil ... */
        
        /* update bext state from "next"
         * field of current state and
         * inout symbol */
        state = dfa->states[state].
                     ins[(**in)].next;

        /* ... and get enrty if that new
         * state from current symbol */
        entry = dfa->states[state].
                       ins + (**in);

        /* if entry has a trans-
        ition call-back, call it */
        if (entry->fn)
            entry->fn(tok,
            		  (**in)++,
			          entry->opaque,
                      allocs,
                      logger);
    
    /* until current state is
     * accepting  */
    } while (!(dfa->states[
                  state].acc));
}

/* "bdlx_init" - allocates, using the allocator
 * and error logger given in "allocs" and "logger", 
 * respectively, and, using "tok", sets "*lex"
 * to a lexical analyser, initially containing
 * no rules or efinitions.  Returns zero on
 * error, non-zero otherwise */
int bdlx_init(struct bdlx_tok *tok,
              struct bdlx_lex **lex,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bdbf_buffer *buf;

    /* initialise buffer holding
     * regular expression compil-
     * ation output (an NFA) */
    buf = bdbf_init(allocs,
                    logger,
                    sizeof(int));
	if (!(buf))
        return 0;

    /* allocate aanalyser */
    (*lex) = (struct bdlx_lex *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdlx_lex),
                     logger);
   if (!(*lex))
       return 0;

   /* ... and load the regular expr-
    * ession parser content.  The
    * parser's opaque value, in this
    * case, is the buffer initial-
    * ised  sbove, into wgich will
    * be place the codes of the "vir-
    * tual machine" which, when
    * "executed", whill generate the reg-
    * ular expression's NFA */
    return ((*lex)->re = bdpr_init(allocs,
                                  logger,
                                  (bdpr_init_fn)
                                      init_re))
                                  != NULL;
}

/* "bdlx_tok_init" - using the allocator and error
 * logger given in "allocs" and "logger", respect-
 * ively, sets "rok" to a lexical token of the
 * size given in "sz" and having the initialisat-
 * ion, type, reset, and name call-backs given in
 * "init", "type", "reset", and "name", respecti-
 * vely.  Returns zero on error, the token other-
 * wise */
int bdlx_tok_init(struct bd_allocs *allocs,
                  struct bd_logger *logger,
                  struct bdlx_tok *tok,
                  int sz,
                  bdlx_tok_init_fn init,
	              bdlx_tok_type_fn type,
                  bdlx_tok_reset_fn reset,
                  bdlx_tok_name_fn name)
{
    /* sset "tok" to s mmemory
     * block of the appropriate
     * size */
    tok = (struct bdlx_tok *)
            bd_alloc(allocs,
                     sz,
                     logger);
    if (!(tok))
         return 0;

    /* ... snd set its fields */
    tok->sz    = sz;
    tok->type  = type;
    tok->init  = init;
    tok->reset = reset;
    tok->name  = name;

    return 1; /* return successs
               * when we get to
               * this point */
}

/* "bdlx_add_rule" - using the memory allocator
 * and error logger given in "allocs" and "logger",
 * respectively, add a "rule", which is an associa-
 * tion between the "pattern" of the input
 * regular expression given in the "re" and the
 * "recognitioncall-back" function given in "fn",
 * to call when the pattern is successfully and
 * totally read.  "Type" indicates the token type
 * to accept (otherwise NULL if not accepting),
 * "prec" identifies the rule's preceding state,
 * "next" is the resulting state after the pattern
 * is successfully read and the "opaque" specifies
 * the opaque parameter to pass with the call to
 * "fn" */
int bdlx_add_rule(struct bdlx_lex *lex,  
                  struct bd_allocs *allocs,
                  struct bd_logger *logger,
                  char *re,         /* regular expression */ 
                  int prec,         /* preceding state */
                  int next,         /* resulting state */
                  bdlx_trans_fn fn, /* rule's call-back */
                 void *opaque,      /* callback's opaque
                                     * parameter */
                  int acc,          /* whether rule is
                                     * accepting */
                  int type)         /* type of accept-
                                     * ing token */
{
   struct bdlx_nfa *re_nfa, *fn_nfa; /* NFAs cont-
						* aining recognition function
						* and regular expressions,
                        * respectively */

    /* sdd an empty transition to the rule's
     * pteceeding state */
    if ((!(add_trans(lex->nfa,
                     allocs,
                     logger,
                     bdgm_sym_empty,
                     0,
                     0,
                     lex->nfa->num,
                     prec,
                     0,
                     NULL,
                     NULL))))
   	return 0;
    /* if there id a regular expr-
     * ession string for this rule,
     * compile it to an NFA */
    if (re)
        if (!(compile_re(re,
                         lex->re,
                         re_nfa,
                         allocs,
                         logger)))
            return 0;

    else /* there's no regular
          * expression string,
          * so ... */

        /* set the regular expr-
         * ession NFA to be an
         * empty NFA */
        if (!(init_nfa(0,
                       &re_nfa,
                       allocs,
                       logger)))
            return 0;

        /*... and also add a transition
         * implementing the rule's func-
         * tion (as the NFA's only transi-
         * tion) */
        if (!(init_nfa(0,
                       &fn_nfa,
                       allocs,
                       logger)))
            return 0;
        if ((!(add_trans(fn_nfa,
                         allocs,
                         logger,
        		         bdgm_sym_empty,
        		         0,
        		         0,
        		         0,
        		         0,
        		         1,
        		         fn,
        		         opaque))))
        	return 0;

    /* and concatenate both the
     * function and regular expr-
     * ession NFA onto the analy-
     * aer's NFA */
    if (!(cat_nfas(lex->nfa,
                   re_nfa,
                   &lex->nfa,
                   allocs,
                   logger)))
        return 0;
    if (!(cat_nfas(lex->nfa,
                   fn_nfa,
                   &lex->nfa,
                   allocs,
                   logger)))
        return 0;

    /* then add an empty transition
     * to to the rule's successor
     * state */
    if ((!(add_trans(lex->nfa,
                     allocs,
                     logger,
    		         bdgm_sym_empty,
    		         0,
    		         0,
    		         lex->nfa->num,
    		         next,
    		         0,
    		         NULL,
    		         NULL))))
    	return 0;

}

/* "bdlx_add_def" - using the memory allocator
 * and error logger given in "allocs" and "logger"
 * respectively, add a "definition", which is an
 * association between the regular expression and
 * name given in "re" and "name", respectively,
 * within the analyser "lex" */
int bdlx_add_def(struct bdlx_lex *lex,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger,
                 char *re,  /* regular expression */ 
                 char *name) /* name */
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */
    struct bdlx_nfa *nfa;         /* regular
                                   * express-
                                   * ion's NFA */

    /* compile the regular expre-
     * ssion ninto an NFA */
    if (!(compile_re(re,
                     lex->re,
                     nfa,
                     allocs,
                     logger)))
        return 0;

    /* insert the definition's NFA
     * into the analyser's defini-
     * tions set */
    ins_node = (struct bd_map_node *)
        bd_alloc(allocs,
                 sizeof(struct
		                bd_map_node),
                 logger);
    if (!(ins_node))
        return 0;
    return bd_map_insert(&lex->defs,
                         ins_node,
                         name,
                         (bd_map_cmp_fn)
                             strcmp,
                         name);
}

/* "bdlx_start" - prepares, using the allocator
 * and error logger given in "allocs" and "logger"
 * parameters, respectively, the lexical analyser
 * given in "lex" to start reading input using
 * its constituent rules and definitions */
int bdlx_start(struct bdlx_lex *lex,
               struct bd_allocs *allocs,
               struct bd_logger *logger)
{
    /* the analyser's NFA will have been,
     * by this point, populated with the
     * rules addeed via
     * "bdlx_add_rule" so, in order to run
     * the analyesr's DFA in "bdlx_next",
     * the DFA needs to be created from
     * that NFA */
	return (!(nfa2dfa(lex->nfa,
                      &lex->dfa,
                      allocs,
                      logger)));
}

/* "bdlx_next" - sets "tok" to the next
* token found by performing lexical anal-
* ysis using "lex" on the input stream
 * the start of which is pointed to by "in",
 * in the process updating "in" so as to
 * point just after the location at which'
 * the scanned token was found, using the
 * memory allocator and error logger in
 * "allocs" and "logger", respectively */
int bdlx_next(struct bdlx_lex *lex,
              struct bdlx_tok *tok,
              int **in,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
	/* run the anlyser's DFA against
	 * the input stream which will,
	 * as a side effect, increment the
	 * stream pointer and set "tok" to
	 * its appropriate value */
    return run_dfa(lex->dfa,
                   in,
                   tok,
                   allocs,
                   logger);
}

/* "bdlx_new_state" - returns the next un-
 * allocated fstate of the analyser given
 * in "lex"  */
int bdlx_new_state(struct bdlx_lex *lex)
{
	/* nezxt "unallocated" state
	 * is next above DFA's size */
	return lex->nfa->num++;
}
