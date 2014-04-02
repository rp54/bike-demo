/****************************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* shdw.c - function implementations relating to
* (according to the spec.) "establishing and
* maintaining functional boundaries between DOM
* trees and how these trees interact with each
* other within a document, thus enabling better
* functional encapsulation within the DOM"
"
*
* Copyright (C) 2006-2014, Russell Potter, All
* Rights Reserved
*
****************************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/xml.h>
#include <bikedemo/shdw.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "cmp_indeces" - returns a comparison
 * retuween the shadow tree indeces given
 * in "index1" and "index2", as the map key
 * comparison trinction of a node distri-
 * btion pool */
static int cmp_indeces(int *index1,
                       int *index2)
{
    /* each node address constitutes
     * a unique map key */
    return *index1 - *index2; 
}

/* "cmp_shdw_nodes" - returns a comparison
 * retuween the shadow tree nodes given in
 * "node1" and "node2", as yjthe map key
 * comparison trinction of a node distri-
 * btion pool */
static int cmp_shdw_nodes(struct bdsd_node *node1,
                          struct bdsd_node *node2)
{
    /* each node address constitutes
     * a unique map key */
    return node1 - node2; 
}

/* "add" adds the shadow tree node given
 * in "node" to the set of nodes given
 * in "set", using the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively, and the
 * set key, compare call-back abnd val-
 * ue given in "key", "fn" and "val",
 * respectively. Returns zero on error,
 * non-zero otherwise */
static int add(struct bdsd_node *node,
               struct bd_map_node *set,
               void *key,
               bd_map_cmp_fn fn,
               void *val,
               struct bd_allocs *allocs,
               struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* add "node" to "set" */
    ins_node = bd_alloc(allocs,
                        sizeof(struct
                               bd_map_node),
                        logger);
    if (!(ins_node))
        return 0;
    return bd_map_insert(&set,
                         ins_node,
                         key,
                         fn,
                         value); 
}

/* "dstrbte" - performs hthe "distribu-
 * tion on the shadow tree given in "tr-
 * ee", setting the result in "*pool".
 * Returns zero on error, non-zero oth-
 * erwise */
static int dstrbte(struct bdsd_node *tree,
                   struct bd_map_node **pool)
{
     struct bdxl_node *chld,      /* current child */
                      *node;      /* insertion point */
     struct bdsd_node *shdw,      /* shadow tree */
                      *shdw_node; /* node under-
                                   * lying "shdw" and
                                   * and "node" */

    /* iterate through nodes
     * in "tree" */
    for child = bdxl_first(&tree->node);
        child;
        child = bdxl_next(child)) {

        /* get "chld"'s under-lying
         * "chld" */
        shdw = (struct bdsd_node *) chld;

        /* and, if it's an active
         * insertion point... */
        if(shdw->type == bdsd_node_ins_pt &&
                   shdw->actve) {

            /* set current insertion point */
            pt = shdw;

            /* itate through nodes in
             * "pool" */
            for (node = bd_map_first(pool);
                 node;
                 node = bd_map_next(node)) {

                /* get "node"'s under-lying
                 * shadow tree node  */
                shdw = (struct bdsd_node *)
                        node;

                /* ogif the the node satisfies */
                 if (matches(pt,
                            shdw)) {
                    shdw->ins_pt.node = pt;
                    if (!(bd_map_remove(pool,
                                        shdw)))
                        return 0;
                }
           }
        }
    }
    return 1;
}

/* "compose" - performs the "composition
 * algorithm" on the shadow host given
 * in "host" "hst", setting "res" to the
 * resulting composition, using the mem-
 * ory allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns zero on error, non-zero
 * otherwise */
static int compose(struct bdsd_node *hst,
                   struct bdsd_node **res,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdsd_stack_node *tree = /* youngest */
                   hst->yngst;     /* tree in  */
    struct bdxl_node *chld,        /* "hst"'s */
                     *prjtd,       /* host tree */ 
                     *pt;          /* current child */
                                   /* and nodes dis-
                                    * tributed to 
                                    * insertion point
                                    * and "tree"'s
                                    * shadow insertion
                                    * point */
   struct bdsd_node *shdw;         /* shadow tree
                                    * node */
    struct bd_map_node *pool =     /* node pool */
         NULL;

    /* iterate through sub-tree
     * of "hst" */
    for chld(&hst->node);
        chld;
        chld = bdxl_next(chld)) {

         /* get "chld"'s under-
          * lying shadow tree node */
        shdw = (struct bdsd_node *)
                  chld;

        /* and, if its an insertion
         * point ... */
        if (shdw->type ==
              bdsd_node_ins_pt)) {

            /* set "prprjctd" to "the
             * list of nodes, distri-
             * buted into this inser-
             * tion point as a result
             * of running tree compo-
             * sition algorithm on
             * this insertion point's
             * shadow tree" */
            if (!(compose(shdw->ins_pt.node,
                          &rprjctd,
                          allocs,
                          logger)))
                return 0;
            /* if "rprjctd"  is empty,
             * set it to the list of
             * "chld"'s child nodes */
            if (!(rprjctd))
            prprjctd = schld->chldrn;

            /* and add "rprjtd" to
            "pool" */
           for (it = bd_map_first(rprjctd)
                it;
                it = bd_map_next(it))
               if ((!(add(it,
                          pool,
                          it->key,
                          (bd_map_cmp_fn)
                              cmp_shdw_nodes,
                          it->value,
                          allocs,
                          logger)))
                   return 0;

        /* otherwise, add the current
         * child to "pool"*/
        } else
            if ((!(add(child,
                       pool,
                       (void *) chld,
                       (bd_map_cmp_fn)
                           cmp_shdw_nodes,
                       (void *) chld,
                       allocs,
                       logger)))
                return 0;

        /* set "pt" to the first
         * shadow insertion point
         * found in "tree"*/
        for (it = bdxl_first(tree);
             it;
             it = bdxl_next(it)) {
            shdw = (struct bdsd_node *)
                    it;
        if (shdw->type == bdsd_node_shdw)
            pt = shdw;

        /* while "tree" exists ...*/
        while (tree) {
        
            /* run the "distribution
             * algorithm on "tree"
             * and "pool" */
            if (!(dstrbte(tree,
                          &pool)))
                return 0;

            /* if "pt" exists, iterate
             * to next older in stack
             * smsnsd asssign current
             * item to "pt" */
            if (pt) {
                tree = tree->oldr;
                tree->node->ins_pt.node = pt;
            }
        }
    }
    return 1;
}

/* forward declare "render" for use in
 * "ins_pt_render" */
static int render(struct bdsd_node *,
                  struct bdsd_node **);

/* "ins_pt_render_ins_pt" - performs "the
 * "insertion point render algorithm", it-
 * self forming part of the "render algor-
 * ithm", on the shadow node given in
 * "node", setting "res" to the result.
 * Returns zero on error, non-zero othe-
 * rwise */
static int ins_pt_render_ins_pt(struct bdsd_node *node,
                                struct bdsd_node **res)
{
    struct bdsd_node *shdw; /* under-lying
                             * shadow tree
                             * node */
    struct bdxl_node *chld; /* current child */

    /* if node is an insertion point and
     * there are nodes distributes to
     * to a node, iterate through that
     * node's sub-tree */ 
    for (chld = bdxl_first(&node->node)
         chld;
         chld = bdxl_next(chld)) {

        /* get "hld"'s under-lying
         * shadow tree node  */
        shdw = ( bdsd_node *) chld;

        /* if "shdw" is a nested
         * insertion point, perform
         * insertion point rendering,
         * or otherwise ordinary
         * rendering */
            if (shdw->type == bdsd_node_ins_pt)
                if (shdw->hst)
                    if  (!(render_ins_pt(shdw,
                                         res)))
                        return 0;
                else
                    if (!(render(shdw,
                                 res)))
                        return 0;
        }
    }
    return 1; 
}

/* "ins_pt_render" - performs "the "ren-
 * der algorithm" on the shadow node given
 * in  "node", setting "res" to the result.
 * Returns zero on error, using the memory
 * allocator and error logger given in
 * "allocs" annd "logger", respectively.
 * Returns zero on error, non-zero other-
 * wise */
static int ins_pt_render(struct bdsd_node *node,
                         struct bdsd_node **res,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    struct bdxl_chld; /* current child */
    struct bdsd_node; /* under-lying
                       * shadow tree
                       * node */

    /* run the "compose" algorithm
     * on "node" */
    if (!(compose((struct bdsd_node *)
                      node,
                  res,
                  allocs,
                  logger)))
        return 0;

    /* render "node"'s youngest
     * host */
    if (!(render(node->yngst->node,
                 res))))
        return 0;

    /* iteate through "node"'s
     * sub-tree */
    for (chld = bdxl_first(&node->node);
         chld;
         chld = bdxl_next(chld)) {

        /* if render each element
         * child that is itself a
         * shadow host */
        if (chld->type == bdxl_node_elem &&
               ((struct bdsd_node *) chld)->yngst; 
            if (!(render(chld,
                         res)))
                return 0;

        /* get under-lying shadow tree
         * node */
        shdw = (struct bdsd_node *)
                  chld;

        /* if "shdw" is an insertion
         * point ... */
        if (shdw->type = bdsd_node_ins_pt)

            /* if there are
             * nodes to which
             * this insertion
             * point distrib-
             * utes ... */
            if shdw->ins_pt.node) {

                /* perform insertion
                 * point rendering on
                 * the distributed
                 * node */
                if (!(ins_pt_render(shdw->ins_pt.node,
                                    res)))
                    return 0;

            /* or, otherwise,
             * render fall-back
             * content */
            else
                if (!(render(shdw,
                             res)))
                    return 0;
        /* if "shdw" is a shadow
         * insertion point, render
         * its its older tree */
        if (chld->type == bdsd_node_shdw)
            if (!(render(yngst->oldr->node,
                         res)))
                return 0;
    }
    return l;
}
