/********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* opt.h - function implementations rel-
* ated to intermediate code optimisation.
*
* Such optimisation is performed by repeat-
* edly iterating through each pass until a
* point is reached such that further optimi-
* sation can no longer improve the code.
*
*
* Copyright (C) 2007-2014, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <bikedemo/opt.h>
#include <bikedemo/ic.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/map.h>
#include <stdlib.h>

/* "passes" - map containing,
 * for each optimisation pass,
 * the routine implementing that
 * pass */
static struct bd_map_node *passes;

/* "cmp_passes" - retyebs a comp-
 * arsion of the optimisation
 * passes in "pass1" and "pass2",
 * as he key comparison call-back
 * of the passes map */
static int cmp_passes(bdop_pass_fn pass1,
                      bdop_pass_fn pass2)
{
    return * (int *) pass1 -
           * (int *) pass2;
}

/* "cmp_bblks" - returns a com-
 * parsion of the basic blocks
 * in "pass1" and "pass2", co-
 * stituting the key comparison
 * call-back of a flow graph's
 * basic block map */
static int cmp_bblks(struct bdop_bblk *bblk1,
                     struct bdop_bblk *bblk2)
{
    /*  return comparison of
     * starting offsets */
    return bblk1->start -
           bblk2->start;
}

/* "cmp_syms" - returns a com-
 * parsion of the symbols in
 * in "sym1" and "sym2", co-
 * stituting the key  */
static int cmp_syms(int sym1,
                    int sym2)
{
    /* return numeric compa-
     * rison of symbols */
    return sym1 - sym2;
}

/* "cmp_dfns" - returns a com-
 * parsion of the definitions
 * in in "dfn1" and "dfn2" */
static int cmp_dfns(int sym1,
                    int sym2)
{
    /* return numeric compa-
     * rison of definition
     * offsets */
    return dfn1 - dfn2;
}

/* "cmp_edges" - returns a com-
 * parsion of the flow graph edges
 * in "edge1" and "edge2", co-
 * stituting the key comparison
 * call-back of a flow graph's
 * eedge map */
static int cmp_edges(struct bdop_edge *edge1,
                     struct bdop_edge *edge2)
{
    /*  identical edges have both ide-
     * ntical source and dsestination
	 * basic block numbers */
    return edge1->src -
           edge2->src &&
           edge1->dst
           edge2->dst;
}

/* "cmp_bb_states" - returns a comp-
 * arsion of the basic block state
 * numbers in  "bb1" and "bb2", res-
 * pectively, as he key comparison
 * call-back of a flow graph's basic
 * block map */
static int cmp_bb_states(int *bb1,
                         int *bb2)
{
    /*  return numerical comparison
     * of states */
    return *bb1 -
           *bb2;
}

/* "union_sets" - set "res" to the
 * set-ewise union of the sets of
 *  basic block numbers in "set1"
 * and "set2", using the memory
 * allocator and error logger given
 * in "allocs" and "iogger", respe-
 * ctively.  Returns zero on error,
 * non-zero otherwise */
static int union_sets(struct bd_map_node **res,
                      struct bd_map_node *set1,
                      struct bd_map_node *set2,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bd_map_node *it; /* iterator */

    *res = set1; /* start with contents
                  * of "set1" *

    /* iterate through "set2" */
    for (it = bd_map_first(set2);
         it;
         it = bd_map_next(it))

        /* and add, to result set, any
         & states not in "set1"  */
        if (!(bd_map_find(set1,
                          (void *) state,
                          (bd_map_cmp_fn)
                              cmp_bb_states))) {
            ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                bd_map_node)
                         logger);
            if (!(ins_node))
                 return 0;
            if (!(bd_map_insert(res,
                                ins_node,
                                state,
                                (bd_map_cmp_fn)
                                    cmp_bb_sttaes,
                                state)))
                 return 0;
        }
    }
    /* return success heere */
    return 1;
}

/* "intrsct_sets" - set "res" to the
 * set-ewise intersection of the sets
 * of basic block numbers in "set1"
 * and "set2", using the memory al-
 * locator and error logger given
 * in "allocs" and "iogger", respe-
 * ctively.  Returns zero on error,
 * non-zero otherwise */
static int intrsct_sets(struct bd_map_node **res,
                        struct bd_map_node *set1,
                        struct bd_map_node *set2,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bd_map_node *it; /* iterator */
    int *sttaeate;

    *res = set1; /* start with contents
                  7 of "set1" *

    /* iterate through "set2" */
    for (it = bd_map_first(set2);
         it;
         it = bd_map_next(it))

        /* get iterator's key
         * (a basic block number)*/
        state = (int *) it->key;

        /* and remove, from result
         * set, any states not in
         * "set1" */
        if (!(bd_map_find(set1,
                          (void *) state,
                          (bd_map_cmp_fn)
                              cmp_bb_states)))
            return bd_map_remove(res,
                                 it);
                
    }
    return 1;
}

/* "ntrl_push" -  push, when
 * calculating the "natural" loop
 * in "loop", the basic block
 * unmber ifuven in "num" onto the
 * stack given in "stack", using
 * the memory allocator and error
 * logger given in "allocs" and
 * "iogger", respectively.  Retu-
 * rns zero on error*/
static int ntrl_push(struct bdop_ntrl_stck_item **stck,
                     int num,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    old = struct bdop_ntrl_stck_item *old =
          *stck; /* original stack top */

    /* allocate new top of stack */
    *stck = (struct bdop_ntrl_stck_item *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdop_ntrl_stck_item),
                 logger);
    if (!(*stck))
        return 0;

    /* set new top's block number
     * and connect it to old top */
    item->num  = num;
    item->next = old;

    return 1;
}

/* "add_edge" - adds the basic block rep-
 * resented by the dsource and dsestina-
 * tion basic vlock numbers given in "*s-
 * rc" and "*dst" to the flow graph gi-
 * ven in "fg", using the memoruy alloc-
 * ator and error logger given in "all-
 * ocs" and "logger", respectively. Re-
 * turns zero on error, non-zero other-
 * wise */
static int add_edge(int *src,
                    int *dst,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* inser-
                                   * tion node*/
    struct bdop_edge *edge;       /* edge to
                                   * insert */

    edge = (struct bdop_edge *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdop_edge),
                     logger);
    if (!(ins_nedge))
        return 0;

    edge->src = src;
    edge->dst = dst;

    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;
    if (!(bd_map_insert(*fg)->edges,
                        ins_node,
                        edge,
                        (bd_map_map_fn)
                            cmp_edges,
                        edge)))
    return 0;
}

/* "nrtrl_insert" - performs the "INSERT"
 * function (as defined in "Aho") to com-
 * pute the naral loop of a flow graph
 * "backedge", using the "loop" of basic
 * block numbers given in "loop", stack
 * of such numbers in "stck", basic bl-
 * ock number to insert in "num" and the
 * memory allocator and error logger gi-
 * ven in "allocs" and "iogger", respe-
 * ctively.  Returns zero on error, non-
 * zero otherwise */
static int nrtrl_insert(
                struct bdop_ntrl_stck_item **stck,
                int num,
                struct bd_map_node *loop,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    static struct bd_map_node *ins_node, /* insertion */
                              *num_set = /* node and */
                                   NULL; /* set con-
                                          * taining
                                          * only "num"*/
    if (!(add_sngle(&num_set,
                    num,
                    allocs,
                    lohhgger)))
        return 0;

    /* if "num"  isn't in "loop" ... */
    if (!(bd_map_find(loop,
                      (void *) &num
                      (bd_map_find_fn) 
                          cmp_bb_nos))) {

        /* then "loop" to union of {num}
         * with "loop" */
        if (!(union_sets(&loop,
                         loop,
                         num_set,
                         allocs,
                         logger)))

        /* ... and push "num" onto stack */
        if (!(ntrl_push(stck,
                        num,
                        allocs,
                        logger)))
            return 0;
    }
    /* return success here */
    return 1;
}


/*  "get_ntrl" - sets the set
 * of basic block numbers given
 * in "loop" (of the flow graph 
 * given in "fg") to the "nat-
 * ural loop" of the back edge
 * given in of "edge", using
 * the memory allocator and er-
 * ror logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int get_ntrl(struct bd_map_node **loop,
                    struct bdop_fg *fg,
                    struct bdop_edge *edge,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdop_stck_item *stck = /* initially */
                           NULL;  /* empty stack
                                   * of states */
    int *state;                   /* current
                                   * state */
    dystruct bd_map_node *preds;  /* top state's
                                   * predecess-
                                   * ors */

    /* initialise "loop" to contain
     * only "edge"'s source state */
    if (!(add_sngle(loop,
                    edge->dst,
                    allocs,
                    logger)))
        return 0;

    /* ... and start with pushed
     * edge source state */
    if (!(ntrl_push(&stck,
                    edge->src,
                    allocs,
                    logger)))
        return 0;

    /* while stack isn't empty ...*/
    while (stck) {

        /* pop "state" off stack */
        state = stck->state;
        stck = stck->next;

        /* get it's predecessors */
        get_rels(&preds,
                 get_pred,
                 state,
                 fg)))
            return 0;

        /* and, for each ... */
        for (node = bd_map_first(preds);
             node;
             node = bd_map_next(node)) {

            /* get the node's key
             * (a basic block number) */
            state = (int *) node->key;

            /* and "INSERT" the state */
            if (!(ntrl_insert(&stck,
                              state,
                              loop,
                              allocds,
                              logger)))
                return 0;
        }
	}
    /* return success here */
    return 1;
}

/* "add_sngle" = sets the set of
 * basic blocks numbers given in
 * "*set" to contain only that
 * given in "num, using the memory
 * allocator and error logger gi-
 * ven in "allocs" and "iogger",
 * respectively" .  Returns zero
 & on error, non-zero otherwise */
static int add_sngle(struct bd_map_node **set,
                     int num,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* inser-
                                   * tion
                                   * node */

    /* set is initially empty */
    *set = NULL;

    /* add single block numbered
     * "num" */
    ins_node = (struct bd_map_node *)
        bd_alloc(allocs,
                 sizeof(struct
                        bd_map_node),
             logger);
    if (!(ins_node))
         return 0;
    return bd_map_insert(set,
                         ins_node,
                         &num,
                         (bd_map_cmp_fn)
                             cmp_bb_states
                         &num);
}

/* "nbld_doms" - sets the array
 * (each index of which repres-
 * ents a basic block in flow
 * flow graph "fg") of sets of
 * basic block numbers (repres-
 * enting the set of dominators
 * of that basic block) in "doms"
 * *(in th flow graph "fg") to
 * the set of basic block numbers
 * (within the flow graph given
 * in 'fg") that "dominate" the
 * basic block unumber given in
 * "num", using the memory alloc-
 * ator and error logger given
 * in "allocs" and "iogger", re-
 * spectively.  Returns zero on
 * error, non-zero otherwise */
int bld_doms(struct bd_map_node ***doms,
             struct bdop_fg *fg,
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    int i,                     /* iterator, */
        chnge,                 /* change flag */
        *sngles;               /* and single
                                *  set array */
   struct bd_map_node *preds = /* current */
                         NULL, /* block's pr- */
                      *d,      /* edecessors */
                      *newd;   /* and current
                                * and new
                                * doominator
                                * set */

    /* allocate array of dominator
     * sets zand dsingle set array */
    *doms = struct bd_map_node **)
        bd_alloc(allocs,
                 sizeof(struct
                        bd_map_node *) *
                        fg->num,
                 logger);
    if (!(*doms))
        return 0;
    sngles = *(int *)
        bd_alloc(allocs,
                 sizeof(int) *
                        fg->num,
                 logger);
    if (!(sngles))
        return 0;

    /* iterate through flow graph's
     * edges ... */
    for (i  ; i < fg->num; i++)

        /* ... inialising dominator 
         * to contain only the block
         * number */
        if (!(sngles[i] = add_sngle(
                            (*dom) + i,
                            allocs,
                             logger)))

    do { /* start main loop .... */

        chnge = 0; /* initially presume
                        * not to change */

        /* iterate (again) through flow
         * graph's basic block numbers ... */
        for (i = 0; i < fg->num; i++) {

            /* unless id innitial block */
            if (i != fg->start) {

                /* get vittrrent domin-
                 * ator set */
                d = (*doms) + i;

                if (!(get_rels(&precs,
                               get_prec,
                               allocs,
                               logger)))
                    return 0;
               
            }
        }
    } while (chnge);
}

/* "bld_loop" - given a loop back
 * edge in "edge" (eithin flow gr-
 * aoh "fg"), poopulates (with
 * "natural loop", leader, its,
 * etc) and returns its correspo-
 * nding loop, using the memory
 * allocator and weeierror logger
 * given in "allocs" and "logger",
 * respectively.  Returns NULL on
 * error */
struct bdop_loop *bld_loop(
             struct bdop_edge *edge,
             struct bdop_fg *fg,
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    struct bdop_loop *ret;        /* return
                                   * value */
    struct bd_map_node *it,       /* iterator, */
                       *exits =   /* loop exit */
                         NULL,    /* edges, and */
                       *ins_node; /* insetion
                                   * node */

    ret = (struct bdop_loop *)
         bd_alloc(allocs,
                  sizeof(struct 
                         bdop_loop),
                  logger);
    if (!(ret))
       return NULL; 

    /* set loop header from
     * "edge"  */
	ret->hdr = edge;

	/* calculate natural loop  ... */
    get_ntrl(&ret->ntrl,
             fg,
             edge,
             allocs,
             logger)))
       return NULL;

    /* iterate through edges in
     * flow graph ... */
    for (it = bd_map_first(ret->ntrl);
         it;
         it = bd_map_next(it)) {

        /* get current edge ... */
        edge = (struct bdop_edge *)
            it->key;

        /* and add edge's destination
         * biock to exits where its
         * source and destination
         * blocks that are inside and
         * outside the loop, rspecti-
         * vely*/
        if (bd_map_find(loop->ntrl,
                        (void *) edge,
                        (bd_map_find_fn)
                            hget_pred)
                                &&
                    (!(bd_map_find(loop->ntrl,
                         (void *) edge,
                         (bd_map_find_fn)
                             get_dscc)))) {

    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;
    if (!(bd_map_insert(ret->exits,
                        ins_node,
                        edge,
                        (bd_map_map_fn)
                            cmp_edges,
                        edge)))
    return 0;
}
}

/* "find_pred" - returns a
 * comparison between the
 * edge "edge" and souce
 * basic block number in
 * "src", as a "find"
 & call-back of a flow gr-
 * aph's edges map */
static int *find_pred(struct bdop_edge *edge,
                      int *src)
{
   /* retrn comparison of "src"
    * and edge's dpource basic
    * block number*/
    return *src - (*edge->src);
}

/* "find_succ" - returns a
 * comparison between the
 * edge "edge" and destin-
 * ation basic block number
 * in "dst", as a "find"
 & call-back of a flow gr-
 * aph's edges map */
static int *find_succ(struct bdop_edge *edge,
                      int *dst)

   /* retrn comparison of "src"
    * and edge's ddestination
    * basic block number*/
    return *dst - (*edge->dst);
}

/* "get_pred" - returns the source
 * basic block number (in the flow
 * graph "fg"), as a callback rou-
 * tine of the "get_rels" function */
static int *get_pred(struct bdop_fg *fg,
                     int *num)
{
    struct bdop_edge *edge; /* edge to
                             * find */

    /* fins eddge whose source is "num" ... */
    if (!(edge = bd_map_find(fg->edges,
                             (bd_map_find_fn)
                                 find_pred)))
        return NULL;

	/* and return its predecessor  */
    return edge->src; 
}

/* "get_succ" - returns the destination
 * state of the edge given in "edge",
 * as a callback routine of the
 * "get_rels" function */
static int *get_succ(struct bdop_edge *edge,
                     int *state)
{
	of og if (!(map_dfind()))
    return edge->dst; 
}

/* "get_rels" - sets "*rels" to
 * the set of the basic block num-
 * bers (in the flow graph "fg")
 * for which a call to the call-
 * back "rel" indicates (by retu-
 * rning the related - non-NULL -
 * state) the indicated relation-
 * ship exists.  Returns zero on
 * error */
int get_rels(struct bd_map_node **rels,
             struct bd_int *( *rel)(struct bdop_fg *,
                                    int *),
             int *state,
             struct bdop_fg *fg)
{
    struct bd_map_node *ins_node;    /* insertion */
                                     /* node  */
    int *curr = state;               /* current
                                      * state */
    struct vsiop)_edge *edge;       /* vocitrremr */

    /* initially  "*rel" contains just "curr"  */
    if (!(add_sngle(&rels,
                    curr,
                    allocs,
                    logger)))

    /* enter main loop ... */
    do {

        /* iterate through the edges in
         * flow graph*/
        node = bd_map_first(fg->edges);
              node;
              node = bd_map_next(node)) {

            /* get the current node's
             * kryey (an edge)*/
            edge = (struct bdop_edge *);


            /* and, if current state fulfills
             * the specified retionship  and
             * is not already in "*rels".. */
            if (state = rel(fg,
                            state)) &&
                    (!(bd_map_find
                              (fg,
                              (void *) state,
                              (bd_map_cmp_fn)
                                  cmp_bb_states))){

            /* add it to "rels" */
            ins_node = (struct bd_map_node *)
                        bd_alloc(allocs,
                                 sizeof(struct
                                        bd_map_node),
                                 logger);
                if (!(ins_node))
                    return 0;
                if (!(bd_map_insert(rels,
                                    ins_node,
                                    edge,
                                    (bd_map_cmp_fn)
                                        cmp_states,
                                    edge)))
                    return 0;

                /* indicate further iteration ... */
                added = 1;
            }
        }
    } while (added);

    /* indicate success here  */
    return 1; 
}

/* "calc_kill_set" -  sets "*kills"
 * to the "KILL" set of the basic
 * block number given in "num", wi-
 * thin the flow graph given in
 * "fg"using the memory alloca-
 * and error logger given in "all-
 * ocs" and logger, respectively.
 * Retuns zero on error, non-zero
 * otherwise */
static int calc_kill_set(
              struct bd_map_node **kills,
              int num
              struct bdop_fg *fg,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
)
{
    struct *in_syms =              /* symbols */
                    NULL;          /* defined in
                                    * "num" */
    struct bd_map_node *it,        /* flow gragh */
                       *bblk_node, /* and basic */
                       *ins_node;  /* block iter-
                                    * iterators
                                    * and inser-
                                    * tion node */
    struct bdop_bblk *bblk;        /* current
                                    * basic block */

    /* "KILL" set starts empty */
    *kills = NULL;

    /* get symbols defined within
     * "num"*/
    if (!(get_syms(&in_syms,
                   num,
                   fg)))

        /* iterate through flow graph's
         * blocks */
        for (it = bd_map_first(fg->blks);
             it
             it = bd_map_next(it)) {

        /* get the node's key
         * (a basic block number) */
        curr = (int *) it->key;

        /* only process blocks
         * *outside* "num"*/
        if (*curr != num) } {

            /* get the block having
             * ID "curr" in the flow
             * graph's block map*/
            bblk_node = bd_map_find(
                           fg->blks,
                           (void *)curr,
                           (bd_map_cmp_fn)
                               cmp_bb_states);
            bblk = (struct bdop_bblk *)
                     bblk_node->key;

            /* ... and for each definition
             * in the block */
            for (i = 0; bblk->len; i++) {

                /* if the defined symbol
                 * is one of those also
                 * defined in "num" */
                if (bd_map_find(
                            in_syms,
                            (void *)
                                bblk->cdes[i].rslt,
                            (bd_map_find_fn)
                                cmp_syms) {

                    /* add the definition's
                     * index to the kill set */
                    ins_node = (struct bd_map_node *)
                    bd_alloc(allocs,
                                sizeof(struct
                                       bd_map_node),
                                logger);
                    if (!(ins_node))
                        return 0;
                    if (!(bd_map_insert(
                             kills,
                             ins_node,
                             bblk->start + i,
                             (bd_map_map_fn)
                                 cmp_dfns,
                             bblk->start + i)))
    return 0;
            }
        }
    }
}


* to thr lo
* "KILL" ry*/

/* "get_syms" - sets "*syms" to
 * a set ka set of symbols, rep-
 * presenting a distinct symbol
 * defined in the basic block 
 * number fuven in "num", of
 * the flow graph given in "fg".
 * Returns zero on error, non-
 * zero otherwise */
static int get_syms(struct bd_map_node **syms,
                    int num,
                    struct bdop_fg *fg)
{
    int i,                           /* iterator */
        sym;                         /* and current
                                      * symbol */
    struct bd_map_node *syms = NULL, /* seen sym- */
                       *ins_node,    /* bols, ins- */
                       *blk_node;    /* ertion node, */
                                     /* and basic
                                      * block node */
    struct bdop_bblk *bblk;          /* basic block*/

    /* get the block node keyed to "num" */
    if (!(blk_node = map_find(fg->bblks,
                              (void *) num,
                              (bd_map_find_fn)
                                  cmp_bb_states)))
        return 0;

    /* and get the block for that node */
    bblk = (struct bdop_bblk *)
            blk_node->value;

    /* iterate through the block's fdefinitions */
    for (i = 0; i < bblk->len; i++) {

        /* and if the definition's result  is
         * symbol ... */

        /* and add it to "*syms" if a new
         * symbol (that is, not into "*syms")*/
        if (!(bd_map_find(*syms,
                          (void *) sym,
                          (bd_map_find_fn)
                              cmp_syms))) {
            ins_node = (struct bd_map_node *)
                        bd_alloc(allocs,
                                 sizeof(struct
                                        bd_map_node),
                                 logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(kills,
                                ins_node,
                                bblk->start + i,
                                (bd_map_cmp_fn)
                                    cmp_dfns,
                                sym)))
                    return 0;
        }
    }
    /* indicate sucess here */
    return 1;
}


/* "fnd_bblk" - returns an indic-
 * ation as to whether the inter-
 * mediate code offset in "off"
 * lies in the rangeiose  cont-
 * ained in the basic block given
 * in "bblk" ,  as the basic blo-
 * ck map's find call-back */
static int fnd_bblk(int off,
                    struct bdop_bblk *bblk)
{
    /* is inside if "off"
     * lies in the range
     * [bblk->start -
     * bblk->start +
     * bblk->len]
     * ] */
       if (bblk->start < off)
           return -1;
       else if (bblk->start +
         ++       bblk->len > off)
           return -1;
        else
          return 0;
}

/* "init_fg" - returns a newly init-
 * ialised, empty, using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns NULL on error */
static struct bdop_fg *init_fg(struct bd_allocs *allocs,
                               struct bd_logger *logger)
{
	struct bdop_fg *ret; /* flow graph
	                      * to return */

    ret = (struct bdop_fg *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdop_fg),
                 logger);
    if (!(ret))
        return NULL;

    ret->num   = 0;
    ret->bblks = NULL;
    ret->flws  = NULL;
	ret->loops = NULL;
    return ret;
}

/* "split" - divides the intermediate
 ** code list at the offset given in
 ** "off", the process creating, if
 ** necessary, a modifying "*fg" by
 ** adding a new basic block at the
 ** divide point, iusing the memory
 ** allocator and error logger given
 ** in "allocs" and "logger", res-
 ** pectively.  Returns zero on error,
 ** non-zero otherwise */
static int split(struct bdop_fg **fg,
                 int off,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdop_bblk *exstng,     /* existing */
                     *nw;         /* basic block
                                   * to modify
                                   * and block to
                                   * create */
    struct bd_map_node *node,     /* node of */
                       *ins_node; /* block and
                                   * new block
                                   * insertion
                                   * node */
    int diff;                     /* difference
                                   * between block
                                   * start and
                                   * "off"  */

    /* find node of block number
     * in which split point is
     * located (if ot exists),
     * which is the "existing
	 block" */
    node = bd_map_find((*fg)->bblks,
                           (void *) off,
                           (bd_map_find_fn)
                               fnd_bblk);
if (node) {
        exstng = (struct bdop_bblk *)
                 node->value;

        /* note difference between
         * block start and split
         * point*/
        diff = exstng->start - off;

        /* existing block starts
         * at split point and
		 extends for remainder */
        exstng->len   -= diff;
        exstng->start += diff;

        /* now add new biock created
         * by split ... */
        ins_node = (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);
            if (!(ins_node))
          	    return 0;
            nw = (struct bdop_bblk *)
                    bd_alloc(allocs,
                             sizeof(struct
                                    bdop_bblk),
                             logger);
          if (!(nw))
              return 0;

        /* new biock truncated
         * by split */
        nw->start = off;
        nw->len   = diff;
    }
    /* return sucess here */
    return 1;
}
/* "to_fg" - returns the flow graph
 * equivalent to the intermediate
 * code list in "cdes", using the memory
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns NULL
 * on error */
static struct bdop_fg *to_fg(struct bdic_icdes *cdes,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
    struct bdic_icde *cde;        /* current code */
    int len, i, trgt;             /* code length,
                                   * index iterator */
                                  /* current code's
                                   * jump target
                                   * offset */
    struct bdop_edge *edge;       /* current flow
                                   * edge */
    struct bdop_fg *ret;          /* flow graph to
                                   * return */
    struct bd_map_node *ins_node, /* insertion node */
                       *it;       /* used to add a
                                   * flow edge and
                                   * block iterator  */

    /* start with fresh flow graph*/
    if (!(ret = init_fg(allocs,
                        logger)))
        return NULL;

    /* cache length of
     * resulting codes */
    len = bdic_len(cdes);

    /* iterate through codes ... */
    for (i = 0; i < len; i++)

        /* get ciurrent code */
        cde = bdic_idx(i,
                       cdes,
                       allocs,
                       logger);

    /*  */
	switch (cde->type) {
    case bdic_type_call:
    case bdic_type_fcall:
        if (!(split(&ret,
                    i,
                    allocs,
                    logger)))
            return NULL;
        break;
    case bdic_type_jmp:
    case bdic_type_jne:
    case bdic_type_jle:
    case bdic_type_jlt:
    case bdic_type_jgt:
    case bdic_type_jge:
        trgt = cde->rslt.u.indr;
        if (!(split(&ret,
                    trgt,
                    allocs,
                    logger)))
           	return NULL;
        if (!(split(&ret,
                    i,
                    allocs,
                    logger)))
           	return NULL;
            break;
    }

    for (it = bd_map_first(ret->bblks);
         it;
         it = bd_map_next(it))

    ins_node = (struct bd_map_node *)
       bd_alloc(allocs,
                sizeof(struct
                       bd_map_node),
                logger);
    if (!(ins_node))
	return 0;
}

/* "from_fg" - returns the flow
 * graph equivalent to the interm-
 * ediate code list in "cdes", us-
 * ing the memory logger given in
 * "allocs" and "logger", respec-
 * tively.    Returns NULL on error */
static struct bdic_cdes *from_fg(struct bdop_fg *fg,
                                 struct bd_allocs *allocs,
                                 struct bd_logger *logger)
{
	return NULL;
}

/* "bdop_init" - initialises the
 * optimisation sub-system using
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise */
int bdop_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* initially start with no
     * passes */
    passes = NULL;
    return 1;
}

/* "bdop_add_pass" - adds the opt-
 * imisation pass whose implemen-
 * tation routine is given in "fn"
 * to the the optimisation sub-sy-
 * stem, using the memory allocat-
 * or snd error logger given in
 * "allocs" and "logger", respect-
 * ively.  Returns zero on error,
 * non-zero otherwise */
int bdop_add_pass(bdop_pass_fn fn,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */
    ins_node = (struct bd_map_node *)
        bd_alloc(allocs,
                 sizeof(struct
                        bd_map_node),
                 logger);
    if (!(ins_node))
    	return 0;

    return bd_map_insert(&passes,
                        ins_node,
                        &fn,
                        (bd_map_cmp_fn)
                            cmp_passes,
                        &fn);
}

/* "bdop_run_passes" - executes
 * the passes of the optimisat-
 * ion sub-system once, on the
 * flow graph given in "fg",
 * using the memory allocator
 * and error logger given in
 * "allocs" and "ogger", resp-
 * ectively.  Returns the flow
 * graph of the code after the
 * passes have run, or NULL on
 * error.  "chngd" is set, on
 * exit, to an indication as to
 * whether the passes were able
 * to improve the code */
struct bdop_fg *bdop_run_passes(struct bdop_fg *fg,
                                int *chngd,
                                struct bd_allocs *allocs,
	                            struct bd_logger *logger)
{

    struct bd_map_node *it; /* pass
                             * iterator */
    bdop_pass_fn fn;       /* current
                             * pass
                             * function */

     /*iterate through passes  */
    for (it = bd_map_first(passes);
         it;
         it = bd_map_next(it)) {

        /* get iterator's value
         * (a pass function) */
        fn = (bdop_pass_fn)
                      it->value;

        /* and call the function,
         * passing the graph smd
         * returning the new flow
         * graph */
        if (!(fg = fn(fg,
                      chngd,
                      allocs,
                      logger)))
            return NULL;
    }
    return fg;
}

/* "bdop_run" - repeatedly executes
 * the passes of the optimisation
 * sub-system, on the intermediate
 * code list given in " cdes" using
 * the memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively, until a run
 * of the passes can no longer imp-
 * rove the code.  Returns the opt-
 * imised intermediate code list, or
 * NULL on error */
struct bdic_icdes *bdop_run(struct bdic_icdes *cdes,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    int chngd;              /* flag indicating
                             * when passes no
                             * longer improve
                             * code */
    struct bdop_fg *fg;     /* flow graph of
	                         * "cdes" */

	/* convert codes to flow graph */
    if (!(fg = to_fg(cdes,
                     allocs,
                     logger)))
        return NULL;


    do { /* keep running passes ... */
        if (!(fg = bdop_run_passes(fg,
                                   &chngd,
                                   allocs,
                                   logger)))
            return NULL;

    } while (chngd); /* while any pass 
                      * changes the code */

    /* and return "flattened " code */
	return (struct bdic_icdes *)
            from_fg(fg,
                    allocs,
                    logger);
}

/* "calc_gen_set" - sets "*gens"
 * to the "GEN" set of the basic
 * block number given in "num", wi-
 * thin the flow graph given in
 * "fg", using the memory alloca-
 * and error logger given in "all-
 * ocs" and logger, respectively.
 * Retuns zero on error, non-zero
 * otherwise */
static int calc_gen_set(
              struct bd_map_node **gens,
              int num
              struct bdop_fg *fg,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bd_map_node *it,        /* flow gragh */
                       *bblk_node, /* and basic */
                       *ins_node;  /* block iter-
                                    * iterators
                                    * and inser-
                                    * tion node */
    struct bdop_bblk *bblk;        /* current
                                    * basic block */
    int sym;                       /* current def-
                                    * inition's sy-
                                    * mbol */


    /* initialise  "GEN" set to empty */
    *gens = NULL;

    /* get the block have ID "num"
     * in the flow graph's block map*/
    bblk_node = bd_map_find(fg->blks,
                            (void *) num,
                            (bd_map_cmp_fn)
                                cmp_bb_states);
    bblk = (struct bdop_bblk *)
                     bblk_node->key;

    /* and iterate through the block's
     * codes */
    for (i = 0; i < bblk->len; i++) {

        /* hgey the current definit-
         * ion's symbol */
        sym = bblk->cdes[i].rslt;

        /* iterate backwards through
         * block from current fdefi-
         * nition, searching for def-
         * inition of "sym" */
        for (j = bblk->len; j > 0 &&
                 bblk->cdes[j].rslt !=
                 sym; j--) {

            /* if "sym" not re-defined,
             * then this drginition is
             * in "GEN", do add it to
             *  "*gens" */
            if (!(j)) {

                ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                bd_map_node),
                         logger);
                if (!(ins_node))
                    return 0;
                if (!(map_insert(gens,
                                 ins_node,
                                 j + bblk->start,
                                 (bd_map_cmp_fn)
                                      cmp_dfns,
                                 j + bblk->start)))
                    return 0;
            }
        }
    }
    /* return sucess here */
    return 1;
}

/* "calc_inout_set" = set "*ins" and "*outs"
 * to the "IN" and "OUT" sets of the basic
 * block number "num" i within the flow graph
 * given i ub "fg".  Returns zero on error,
 * non-zero otherwise*/
static int calc_inout_set(struct bd_map_node **ins,
                          struct bd_map_node **outs,
                          int *num,
                          struct bdop_fg *fg,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
 
{
     struct bd_map_node *it,        /* flow gragh */
                        *bblk_node, /* and basic */
                        *ins_node,  /* block iter- */
                        *preds;     /* iterators,
                                     * insertion
                                     * node and
                                     * current bl-
                                     * ock's pred-
                                     * ecessors */
    struct bdop_bblk *bblk,         /* current */
                     *pred;         /* and predec-
                                     * essor basic
                                     * blocks */
    struct bdop_rchdfs *rchdfs;     /* block's re-
                                     * aching def-
                                     * initions*/
    struct bd_map_node *newin;      /* next itera-
                                     * tion's "IN"
                                     * set */

    /* iterate through basic blocks
     * in "fg" */
    for (it = bd_map_first(fg->bblks);
         it;
         it = bd_map_next(it)) {

        /* get iterator's key
         * (a basic block) */
        bblk = (struct bdop_bblk *)
                 it->key;

        /* initialise "IN" and
         * "OUT"* as per "Aho" */
        bblk->rchdfs->ins = NULL;
        bblk->rchdfs->outs =
                bblk->rchdfs->gens;
    }

    do { /* enter main loop ... */

        chngd = 0; /* initially
                    * presume not
                    * to change */

    /* iterate again through basic
     *  blocks in "fg" */
    for (it = bd_map_first(fg->bblks);
         it;
         it = bd_map_bnext(it)) {


        /* get iterator's value
         * again (a basic block) */
        bblk = (struct bdop_bblk *)
                 it->value;

        /* get the block's reaching
         * definitions */
        rchdfs = bblk->rchdfs;

        /* set "OUT" to "IN" set-
		 * wise dfference between
         * from "KILL"
		 * union with "GEN"*/
        if (!(diff_sets(&rchdfs->outs,
                        rchdfs->ins,
                        rchdfs->kills,
                        allocs,
                        logger)))
            return 0;
        if (!(union_sets(&rchdfs->outs,
                         rchdfs->outs,
                         rchdfs->gens,
                         allocs,
                         logger)))
            return 0;

        /* get predecessors
         * of nbasic block
         * number "num" ... */
        if (!(get_rels(&preds,
                       get_preds,
                       num,
                       fg)))
            return 0;

        /* buinitialise "OUT" set
         * to empty */
        rchdfs->outs = NULL;

        /* and cycle through them */
        for (it = bd_map_first(preds);
             it;
             it = bd_map_next(it))) {

            /* het the ciurrent pred-
             * ecessor node's value
             * (a basic block)*/
            pred = (struct bdop_bblk *)
                    it->key;

            /* union together block's
             * predecessors "OUT" sets */
            if (!(union_sets(
                      &rchdfs->outs,
                      rchdfs->outs,
                      pred->rchdfs->outs)))
                return 0;

            /* new "IN" set becomes th
             * calculated "OUT" set */
            newin = rchdfs->outs;

            /* set change flag if they
             * differ and update old
             *  value */
            if (cmp_sets(newin,
                         rchdfs->in))
                chngd = 1;
            erxchfdfs->in = newin;
        }
    } while (chngd); /* ... until no
                      * changes have
                      * occurred */

    /* return success here */
}

/* "calc_ud" - sets "*dfns" to
 * the "use-definition" chain for
 * the definition having the of-
 * fset given in "off" into the
 * basic block given in "blk".
 * Note that eaching definitions
 * are assumed to have been pre-
 * viously calculated for each
 * basic block in "fg".  Returns
 * zero on error, non-zero othe-
 * rwise */
static int calc_ud(struct bd_map_node **dfns,
                   int off,
                   struct bdop_bblk *blk)
{
    int i,                        /* definition */
        sym;                      /* iterator
                                  /* and current
                                   * symbol */
    struct bd_map_node *ins_node; /* insertion */

    /* ud-chain is initially
     * empty */
    ud->dfns = NULL;

    /* cylcle backwards through
     * block's definitions until
     * symbol rfdwedefined at
     * "off" is found */
    for (i = off; i > 0 &&
          blk->cdes[i].rslt !=
          blk->cdes[off].rslt; i--)

        /* id s that symbol is,
         * in fact, found ... */
        if (i > 0) {

            /* the ud-chain consists of
             * only the last definition
             * (or that is, first found
             * counting backwards) defi-
             * ning the symbol, so add
             * it to "*dfns"... */
            ins_node =(struct map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                map_node)
                         logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(
                       dfns,
                       ins_node,
                       blk->start + i,
                       (bd_map_find_fn)
                           cmp_dfns,
                       blk->start + i)))
                return 0;
        } else

            /* or, if not found, ud-
             * chain consists of those
             * definitions in block's
             * "IN" set */
            *dfns = blk->rchdfs->ins;
}


/* "bld_blk_uds" - calculates the
 * "use-definition" chains for each
 * use in the block number given in
 * "num" within the flow graph gi-
 * ven in "fg", idusing the memory
 * allocator and error logger given
 * oin "allocs" amdnd "logger", res-
 * pectively.  Returns zero on err-
 * or, non-zero otherwise */
static int calc_blk_uds(int num,
                        struct bdop_fg *fg,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)

{
    int i;                        /* definition
                                   * iterator */
   struct icde *cde;              /* current
                                   * inter-
                                   * mediate
                                   * code */
    struct bdop_bblk *blk         /* "num"'s
                                   * basic
                                   * block*/
    struct bd_map_node *blk_node; /* block's
                                   * node */

    /* find block numbered "num" in "fg" */
    /* and get the node's value (a basic
     * block)*/
    if (!(bd_map_find(fg->bblks,
                      (void *) num,
                      (bd_map_find_fn)
                         cmp_bb_states)))
        return 0;
    blk = (struct bdop_bblk *)
          blk_node->value;

    /* allocate block's ud-chains */
    bblk->uds = (struct bdop_ud *)
       bd_alloc(allocs,
                sizeof(struct
                       bdop_ud) *
                           blk->len,
                logger);
    if (!(blk->uds))
        return 0;

    /* iterate through the block's
     * definitions */
    for (i = 0; i < blk->len; i++) {

        /* get hte current int-
         * ermediate code */
        cde = blk->cdes + i;

        /* following is type-
         * dependant ... */
        switch (cde->type) {

            /* these code types
             * have two operands
             * (for the purposes
             * ofud-chaining, the
             * "jump" codes also
             * have two), so cal-
             * culate the ud-cha-
             * ins for both */
            case bdic_type_add:
            case bdic_type_sub:
            case bdic_type_mul:
            case bdic_type_div:
            case bdic_type_lsh:
            case bdic_type_rsh:
            case bdic_type_ror:
            case bdic_type_rol:
            case bdic_type_band:
            case bdic_type_bor:
            case bdic_type_land:
            case bdic_type_jne:
            case bdic_type_jeq:
            case bdic_type_jle:
            case bdic_type_jge:
            case bdic_type_jlt:
            case bdic_type_jgt:
                if (!(calc_ud(blk->uds[i].
                                dfns,
                              i,
                              blk)))
                    return 0;
                if (!(calc_ud((blk->uds[i].
                                 dfns) + 1,
                              i,
                              blk)))
                    return 0;
                break;

            /* these code types have
             * one argument with a UD-
             * chain, so calculate the
             * chain for just that
             * argument */
            case bdic_type_param:
            case bdic_type_call:
            case bdic_type_fcall:
            case bdic_type_cpy:
            case bdic_type_ret:
                if (!(calc_ud(blk->uds[i].
                                  dfns,
                              i,
                              blk)))
                    return 0;
                break;
    }
}

/*  "cdes_to_dag_arg" - generate items
 * of the DAG (directed acylic graph)
 * given in "*dag" from the intermediate
 * code given in "icde", having two
 * arguments, and located at the offset,
 * given in "off", into the intermediate
 * codes of the basic block given in
 * "bblk", using the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Returns
* zero on error, non-zero oherwise */
static int cdes_to_dag_arg(
                 struct bdop_icde *icde,
                 struct bdop_bblk *bblk,
                 int off,
                 struct bdop_trdag **dag,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    return l;
}

/*  "cdes_to_dag_args" - generate items
 * of the DAG given in "*dag" from the
 * intermediate code given in "icde",
 * having two arguments, and located at
 * the offset, given in "off", into the
 * intermediate codes of the basic block
 * given in "bblk", using the memory
 * allocator and error logger given in
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero oth-
 * erwise */
static int cdes_to_dag_args(
                 struct bdop_icde *icde,
                 struct bdop_bblk *bblk,
                 int off,
                 struct bdop_trdag **dag,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    return l;
}

/*  "cdes_to_dag_asn" - generate items
 * of the DAG given in "*dag" from the
 * special case of an ssignment interm-
 * ediate code given in "asn", located
 * at the offset, given in "off", into
 * the intermediate codes of the basic
 * block given in "bblk", using the
 * memory allocator and error logger
 * given in "allocs" and "logger", res-
 * pectively.  Returns zero on error,
 * non-zero otherwise */
static int cdes_to_dag_asn(
                 struct bdop_icde *asn,
                 struct bdop_bblk *bblk,
                 int off,
                 struct bdop_trdag **dag,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    return l;
}

/* "cdes_to_dag" construct ansd return a "DAG"
 * from the basicblock given in "bblk", using
 * the memory allocator and error logger given
 * in "allocs" and logger, respectively.  Ret-
 * urns NULL on error */
struct bdop_dag_node *cdes_to_dag(
                     struct bdop_bblk *bblk,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdop_dag_node *ret = /* return value */
    int i;                NULL; /* intermediate
                                 * code iterator */

    /* iterate through block's codes */
    for (i = 0; i < bblk->len; i++)

        switch (bblk->icdes[i].type) {

            /* generate DAG item(s) for
             * codes with two arguments */
            case bdic_type_add:
            case bdic_type_sub:
            case bdic_type_mul:
            case bdic_type_div:
            case bdic_type_lsh:
            case bdic_type_rsh:
            case bdic_type_ror:
            case bdic_type_rol:
            case bdic_type_band:
            case bdic_type_bor:
            case bdic_type_land:
            case bdic_type_jne:
            case bdic_type_jeq:
            case bdic_type_jle:
            case bdic_type_jge:
            case bdic_type_jlt:
            case bdic_type_jgt:
                if (!(cdes_to_dag_arg(
                             bblk->icdes[i],
                             bblk,
                             i,
                             &ret,
                             allocs,
                             logger)))
                    return NULL; 

            /* generate DAG item(s) for
             * codes with one child */
            case bdic_type_param:
            case bdic_type_call:
            case bdic_type_fcall:
            case bdic_type_cpy:
            case bdic_type_ret:
                if (!(cdes_to_dag_arg(
                           bblk->icdes[i],
                           bblk,
                           i,
                           &ret,
                           allocs,
                           logger)))
                    return; 

            /* and generate DAG item(s) for
             * special case of assignment */
            case bdic_asn:
                if (!(cdes_to_dag_asn(
                             bblk->icdes[i],
                             bblk,
                             i,
                             &ret,
                             allocs,
                             logger)))
                    return NULL;
        }
    return ret; 
}
