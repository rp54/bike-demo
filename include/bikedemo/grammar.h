/*****************************************************************
*
* BikeDemo - Exercise bike demonstration
* scenery
*
* gramar.h - data structures and function
* proto-types for the grammar parser capa-
* ble of processing grammatical constru-
* cts
*
* There are two important structures in
* of interest in this source file: "bdgm_-
* prodset", which represents a set of
* production rules of a grammar, and
* "bdgm_table", which is an LR parsing
* table, built from the production set.
*
* This implementation generates a parser
* table known as a Look-Ahead Left-Right
* (LALR) table, which is able to correct-
* ly handle shift-reduce conflicts.
* 
* The SLR algorithm is taken from "Princi-
* ples of Compiler Design", A V Aho & G D
* Ullman, Addison-Wesley, 1977 (a.k.a
* "The Dragon Book") pp 219-223
*
* Copyright (C) 2006-2014, Russell Potter,
* All Rights Reserved
*
******************************************************************/

#ifndef __BD_GRAMMAR_H
#define __BD_GRAMMAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/allocs.h> 
#include <bikedemo/log.h>
#include <bikedemo/lex.h>
#include <bikedemo/map.h>
    
/* "bdgm_log_syn" - index of log
 * table syntax error message */
#define bdgm_log_syn 0

/* "bdgm_rsvd_toks" - possible "rese-
 * rved" symbols in a grammar, namely,
 * the "empty", "rend-of-stream" and
 * start symols */
enum bdgm_rsvd_syms {

    bdgm_sym_empty, /* empty symbol */
    bdgm_sym_eos,   /* start symbol */
    bdgm_sym_start, /* end-of-stream
                       symbol */
    bdgm_sym_nres   /* number of
                     * reserved sym-
                     * bols */
};

/* "bdgm_actn_types" enumeration - the
 * possible types of the actions located
 * at each entry of an LALR "action"
 * table.
 *
 * These actions are:
 *
 *        "reduce": A complete production
 *                  has been recognised on
 *                  the input and therefore
 *                  needs to be reduced, in
 *                  which case the number
 *                  of symbols in the ght-
 *                  hand-side of the produ-
 *                  ction arepopped offf
 *                  the symbol stack,and the
 *                  left-hand-side symbol
 *                  is pushed onto the stack.
 *
 *       "shift":   The next input token is
 *                  read, then pushed onto
 *                  the symbol stack.
 * 
 *      "accept":   A complete grammar has
 *                  been recognised, in resp-
 *                  onse to which parsing
 *                  ceases.
 *
 *      "error":    The parsing table has ent-
 *                  ered an error state, in
 *                  which case an error reco-
 *                  very routine is called */
enum bdgm_actn_types {

    bdgm_actn_reduce,
    bdgm_actn_shift,
    bdgm_actn_accept,
    bdgm_actn_error
};

/* "bdgm_reduce_item" union - a single item
 * in the array of ite passed as a parameter in
 * a call to the production's "reduction" call-
 * back and consists of an opaque pointer (in
 * the case in which the item's corresponding
 * symbol is a non-terminal) or a lexical token
 * (in case it is a terminal) */
 union bdgm_reduce_item {

    void *nt;             /* non-terminal */
    struct bdlx_tok *tok; /* terminal */
};

/* "bdgm_stack_item" structure - a single item
 * in an LALR table stack, and consists of a
 * pointer to the previous and neext items in
 * the stack (so the item may be iterated in
 * both forward and reverse directions), a state
 * (which may be later re-exposed by later rule
 * reductions), a grammar symbol type, the curr-
 * ent symbol type corresponding to this item
 * and a scanned lexical token and pointers to
 * the next and previous items in th stack */
 struct bdgm_stack_item {

    struct bdgm_stack_item *prev, /* previous */
                           *next; /* and next
                                   * items in
                                   * stack */
    int state, type;              /* item sym-
                                   * bol type
                                   * and next
                                   * shift
                                   * state */
    struct bdlx_tok *tok;         /* scanned
                                   * lexical
                                   * token */
};

/* "bdgm_reduce_fn" function - the prototype
 * ofmthe function called when a grammar pro-
 * duction rule is "reduced" (that is, all
 * its right-hand-side symbols have been re-
 * cognised).  The first parameter is a par-
 * sing output value, the second parameter
 * points to a generic cast of the value to
 * be produced by the production (which sh-
 * ould be some transformation of the right-
 * hand-side items so as to to carry out the
 * effect of the production's action), the
 * third points to the start of "reduce it-
 * ems" corresponding to each grammar symbol
 * in the right-hand-side of the production,
 * the fourth parameter is the number of
 * those parameters and the fifth and sixth
 * para- meters are the memory allocator and
 * error logger with which to allocate memory
 * and log errors, respectively */
typedef int (* bdgm_reduce_fn)(
                       void *,
                       void *,
                       union bdgm_reduce_item *,
                       int,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdgm_item" structure - represents a combin-
 * ation of a grammar production, the index into
 * the right-hand-side of that production, and
 * a look-ahead symbol type , used in the gene-
 * ration of an LALR parsing table from a set
 * of grammar production rules */
struct bdgm_item {

    struct bdgm_prod *prod; /* production */
    int rhs,                /* index, into right-
                             * hand-side symbols,
                             * of rule */
        lah;                /* look-ahead
                             * symbol */
};

/* "bdgm_action" structure - data used to specify
 * the action of an LALR parsing table, which cons-
 * ists of the the type of action, the index of the
 * production to reduce (for a "reduce" action)
 * and the stste to which to shift (for a "shift"
 * action) */
struct bdgm_action {

    enum bdgm_actn_types type;  /* type of action */

    union {
        struct bdgm_prod *prod; /* production to
                                 * reduce */
        int state;              /* state to which to
                                 * shift */
    } u;                   
};

/* "bdgm_state" structure - a single state of an
 * LALR table, produced during each interation of
 * the SLR table generation algorithm.  Each state
 * contains the "action" entries, specifying "what
 * to do" when each symbol is encoutered, for each
 * terminal symbol of the state and the "goto"
 * entries specifying the state to which to trans-
 * ition for each terminal and non-terminal symbol */
struct bdgm_state {

    struct bdgm_action *actions; /* entries for
                                  * "actions"
                                  * table for this
                                  * state */
    int *gotos;                  /* entries for
                                  * "goto"for this
                                  * state */
};

/* "bdgm_table" structure - the LALR parser table
 * generated from a set of production rules by the
 * LALR table genera- tion algorithm.  The struct-
 * ure consists of a list of the LALR states gener-
 * ated by the algorithm along with a stack of
 * table states, a stack of rule reduction items
 * (used by reduction call-backs), the stacks'
 * height and the current state in the parsing
 * process */
struct bdgm_table {

    int hgt, num, curr;            /* height of stack,
                                   /* number of */
                                   /* states and
                                    * current state */
    struct bdgm_stack_item *stack; /* state stack */
    struct bdgm_state *states;     /* the list of
                                    * states consti-
                                    * tuting the
                                    * table */
};

/* "bdgm_prod" structure - represents a single
 * "production" of a grammar, consisting of a
 * single non-terminal grammar symbol forming
 * the left-hand-side of the production and a
 * list of terminal and non-terminal right-hand-
 * side symbols to which that symbol "expands"
 * (that is, which it constitutes) */
struct bdgm_prod {

    int lhs, *rhs, num;    /* symbol forming
                            * left-hand-side
                            * of production,
                            * the array of
                            * symbols forming
                            * right-hand-side
                            * of rule and the
                            * number of right-
                            * hand-side symbols */
    void *opaque;          /* opaque data poi-
                            * nter to be passed
                            * with "reduce" call */
    bdgm_reduce_fn reduce; /* function to call
                            * when rule
                            * is reduced */
};

/* "bdgm_grmr" structure - represents a set of
 * production rules.  The set is prefixed by an
 * integer count of the number of terminals and
 * non-terinals in the grammar, while the cont-
 * ent of the structure is actually a map of
 * the production rules themselves */
struct bdgm_grmr {

    int num_terms, num_nts;    /* number of term-
                                * inal and non-
                                * terminal symbols,
                                * respectively, in
                                * the production
                                * set */
    struct bd_map_node *prods; /* map of productio-
                                *  ns in the gram-
                                * mar */
};

/** function proto-types **/

/* "bdgm_prod_add" - using the opaque value given
 * in the fourth parameter, adds the production
 * represented by the left-hand- side symbol type
 * and production's "reduction call-back" given
 * in the fifth and seventh parameters, respect-
 * ively, and the list of right-hand-sidesymbol
 * types given in the eighth and folowing para-
 * meters, to the grammar given the first param-
 * eter.  Returns zero on error, non- zero oth-
 * erwise */
int bdgm_prod_add(struct bdgm_grmr *,
                  struct bd_allocs *,
                  struct bd_logger *,
                  void *,
                  int,
                  int,
                  bdgm_reduce_fn,
                  ...);

/* "bdgm_cleanup" - reclaims resources used by the
 * grammar given in the first parameter, usually
 * called just before the grammar itself is recl-
 * aimed, using the memory allocator amd error
 * logger given in the second and and third para-
 * meters, respectively.  Returns zero on error,
 * non-zero otherwise */
int bdgm_cleanup(struct bdgm_grmr *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdgm_init" - allocates, initialise and ret-
 * urns, using the memory allocator and error
 * logger given in the third and fourth param-
 * eters, respectively,  a grammar to have the
 * number of tokens and number of symbols giv-
 * en in the first and second parameters, res-
 * pectively.  Returns NULL on error */
struct bdgm_grmr *bdgm_init(int,
                            int,
                            struct bd_allocs *,
                            struct bd_logger *);

/* "bdgm_table_init" - allocates and returns,
 * using the mmory allocator and error logger
 * given in the second and third parameters,
 * respectively, returns a an LR table built
 * from the grammar given in the first param-
 * eter.  A logger to which allocation and
 * other errors will be logged is given in
 * the third parameterReturns BNULL on error */
struct bdgm_table *bdgm_table_init(
                        struct bdgm_grmr *,
                        struct bd_allocs *,
                        struct bd_logger *);

/* "bdgm_lr_table_build" - generates the ent-
 * ries in the LALR table given in the third
 * parameter from the grammar given in first
 * parameter such having th rstart symbol gi-
 * ven in the second parameter such that
 * "recognised" ("reduced") production rules
 * in the grammar are cause the generation
 * of some form of output.  Returns zero on
 * error, non-zero otherwise  */
int bdgm_table_build(struct bdgm_grmr *,
                     int,
                     struct bdgm_table *,
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdgm_table_run" - using the memory alloc-
 * ator and error logger given in the dixth
 * and seventh parameters, respectively, proc-
 * esses the entries in the LALR grammar
 * table given in the fifth parameter, gen-
 * erated from the grammar in the fourth
 * parameter corresponding to the input of
 * the lexical token given in the third
 * parameter, genterating the output spec-
 * ified in the second parameter.  Returns
 * zero on error, non-zero otherwise  */
int bdgm_table_run(void *,
                   void *,
                   struct bdlx_tok *,
                   struct bdgm_grmr *,
                   struct bdgm_table *,
                   struct bd_allocs *,
                   struct bd_logger *);

/* "bdgm_table_cleanup" - reclaims resources
 * used by the LR table given in the first
 * parameter, usually called just before
 * the table itself is reclaimed.  Returns
 * zero on error, non-zero otherwise  */
int bdgm_table_cleanup(struct bdgm_table *);

#ifdef __cplusplus
}
#endif

#endif
