/********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* opt.h - data types and function proto-
* types related to intermediate code opt-
* imisation.
*
* Such optimisation is performed by repeat-
* edly iterating through each pass until a
* point is reached such that further optimi-
* sation can no longer improve the code.
*
*
* Copyright (C) 2007-2012, Russell Potter
* All rights reserved
*
*********************************************************************/

#ifndef __BD_OPT
#define __BD_OPT

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/ic.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/map.h>
#include <stdlib.h>

/* "bdop_pass_fn" typedef - proto-type of
 * the function called to perform a single
 * intermediate code optimisation pass.
 *
 * the first parameter contains the code
 * to optimise, in the form of a flow graph,
 * prior to the execution of the pass, the
 * second is set, on exit, to an indication
 * as to whether the pass changed the code,
 * the third and fourth contains the memory
 * allocator and error logger to use in
 * implementing the pass.  returns the flow
 * graph resulting from execution of the
 * pass, or NULL on error */
typedef struct bdop_fg *(* bdop_pass_fn)(
                       struct bdop_fg **,
                       int *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdop_fg" structure - a "flow graph"
 * of "basic blocks" (as defined in "Aho" ),
 * consisting of the initial basic block
 * number, number of basic blocks in the
 * graph, a set of edges between them
 * representing execution flows and the
 * set of detected loops */
struct bdop_fg {

	int start, num;            /* initial
	                            * block number
	                            * and number
	                            * of blocks in
	                            * graph */
    struct bd_map_node *bblks, /* set of basic */
                       *edges, /* blocks, edges */
                       *loops; /* and loops */
};

/* "bdop_edge" structure - an 
 * edge, in a flow graph, betw-
 * een basic blocks, consisting
 * of  its source and destinat-
 * ion block numbers */
struct bdop_edge {

    int src,    /* source and */
        dst;    /* destination */
                /* block numbers */
};

/* "bdop_rchdfs" structure - the
 * set of "reaching definitions"
 * (as defined in "Aho") for a
 * basic block, consisting of the
 * "IN", block's "OUT", "GEN",
 * "KILL", "DEFN" and "USE" sets  */
struct bdop_rchdfs {

    struct bd_map_node *ins,   /* "IN", */
                       *outs,  /* "OUT" */
                       *gens,  /* "GEN", */
                       *kills, /* "KILL" */
                       *dfns,  /* "DEFN" and */
                       *uses;  /* "USE" sets */
                       
                       
};

/* "bdop_ud" structure - an array
 * of sets, each representing the
 * definitions defining the sym-
 * bol in each of the operands of
 * an intermediate code, populat-
 * ed after use-definition chains
 * have been applied ro the code */
struct bdop_ud {

    struct bd_map_node *dfns[3]; /* ud-chains
                                  * for each of
                                  * the (max 3)
                                  * operands */
}

/* "bdop_loop" structure - a "loop"
 * in a flow graph, as defined in
 * "aho"), consisting of set of
 * vbasic block numbers in the lo-
 * op's "natural" loop, edge of
 * the loop's entry ("header") and
 * the set of exit edges  */
struct bdop_loop {

    struct bd_map_node *ntrl,  /*  of basic */
                       *exits; /* block numbers
                                * in "natural
                                * loop" and exit
                                * edges */
    struct bdop_edge *hdr;     /* header edge */
};

/* "bdop_dag_item" structure - a sin-
 * gle node in a Directed Acyclic Gr-
 * aph (or DAG), which is used to im-
 * plement optimisations such as com-
 * mon sub-expression elimination,
 * consisting of a set of oprands, a
 * parent node, an  indication as to
 * whether the node is a leaf or inner
 * node, a set of attached identif-
 * iers and either the node's value
 * at current point in DAG construc-
 * tion(for leaf nodes) or an oper-
 * ator symbol (for inner nodes) */
struct bdop_dag_item {

    struct bd_dag_item *prnt;      /* parent */
                       *oprnds[2]; /* and oper-
                                    * ands */
    int is_lf;                     /* whether le-
                                    * af or inner
                                    * node*/
    struct bd_map_node *attchd;    /* attached
                                    * identif-
                                    * iers */
    union {
        int op,                    /* operator */
        struct bdic_val *val;      /* or current
                                    * value */
    } u;
};

/* "bdop_dag" structure - the set of
 * DAG items constituting a DAG, along
 * with a set values, one for each
 * symbol) representing the cvalue
 * of "NODE" function for that symbol */
struct bdop_dag {

    struct bd_map_node *items, /* items */
                       *nodes; /* and "NODE"
                                * values */
};

/* "bdop_bblk" structure - a "basic block""
 * (as defined in "Aho") repre- senting a
 * segment of code without jumps, consisting
 * of the intermediate codes in the block,
 * the number of codes in the block, its DAG,
 * computed ud-chains and reaching definit-
 * ions */
struct bdop_bblk {

    struct bdic_icde *icdes;    /* codes
                                 * in block */
	int len;                    /* number of
	                             * codes */
    struct bdop_ud *uds;        /* block's ud-
                                 * chains */
    struct bdic_dag_item *dag;  /* block DAG */
    struct bdic_rchdfs *rchdfs; /* reaching
                                 * definitions */
};

/*** function proto-types ***/

/* ""bdop_init - initialises the
 * optimisation sub-system using
 * the memory sllocator snd error
 * logger guvb in thr furat and
 * second parameters, respectiv-
 * ely.  Returbs zero on error,
 * non-zero otherwise */
int mgop_init(struct bd_allocs *,
		      struct bd_logger *);

/* "bdop_add_pass" - adds the opt-
 * imisation pass whose implemen-
 * tation routine is given in the
 * first parameter to the the
 * optimisation sub-system, using
 * the memory allocator snd error
 * logger given in the second and
 * third parameters, respectively.
 * ely.  returns zero on error,
 * non-zero otherwise */
int bdop_add_pass(bdop_pass_fn,
                  struct bd_allocs *,
	              struct bd_logger *);

/* "bdop_run_passes" - executes
 * the passes optimisation sub-
 * system whose once, using the
 * memory allocator snd error
 * logger given in the third and
 * fourth parameters, respectively.
 * ely.  The second parameter is
 * set, on exit, to an indication
 * as to whether the flow graph
 * was changed by one of the pass-
 * es. Returns the flow graph af-
 * ter the passes have run, or
 * NULL on error */
struct bdop_fg *bdop_run_passes(struct bdop_fg *,
                                int *,
                                struct bd_allocs *,
	                            struct bd_logger *);

/* "bdop_run" - repeatedly executes
 * the passes of the optimisation
 * sub- system, on the intermediate
 * code list given in the first para-
 * meter, using the memory allocator
 * and error logger given in the
 * second and third parameters, re-
 * spectively, until a run of the
 * passes can no longer improve the
 * code.  Returns the flow code list
 * after optimisation has completed,
 * or NULL on error */
struct bdic_icdes *bdop_run(struct bdic_icdes *,
                            struct bd_allocs *,
	                        struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif

