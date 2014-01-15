/*****************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* grammar.c - function implementations relating to
* the implementation of grammar compilers.
*
* Copyright (C) 2013, Russell Potter, All Rights
* Reserved
*
******************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <bikedemo/grammar.h>
#include <bikedemo/lex.h>
#include <bikedemo/parser.h>
#include <bikedemo/map.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "cmp_prods" - compare two productions as the
 * key comparison function of a grammar's prod-
 * uction set */
static int cmp_prods(struct bdgm_prod *prod1,
                     struct bdgm_prod *prod2) 
{
    int i,    /* right-hand-side iterator */
        diff; /* and difference between number
               * of right-hand-side symbols */

    if (diff = (prod1->num - prod2->num))
        return diff; /* identical productions
                      * have same number of
                      * right-hand-side symbols */

    /* cycle through right-hand-side symbols */
    for (i = 0;
         i < prod1->num;
         i++)

    /* ... and identical productions contain,
     * for each right-right-hand-side symbol,
     * the identical symbols */
     if (diff = (prod2->rhs[i] - prod1->rhs[i]))
            return diff;
    return 0;
}

/* "is_term" - returns non-zero if the type of the
 * grammar symbol ginen in the furst parameter of
 * the grammar given in the second parameter is a
 * terminal symbol, zero otherwise */
static int is_term(int type,
                   struct bdgm_grmr *gmr) 
{
    /* returns non-sero only if symbol is
     * actually a terminal */
    return type <= gmr->num_terms;
}

/* "find_prod" - return a comparison between
 * the productions "prod" and "find", as the
 * "find" callback, used in finding a produ-
 * ction */
static int find_prod(void *prod,
                     void *find) 
{
    /* return a comparision between 
     * "prod" and "find" */
    return cmp_prods((struct bdgm_prod *) prod,
                     (struct bdgm_prod *) find);
}

/* "find_lhs" - return a comparison between
 * the left-hand-side symbols of "prod" and
 * "find", as the "find" callback used to
 * find a production using that symbol */
static int find_lhs(void *prod,
                    void *find) 
{
    /* return comparision between 
     * the left-hand-side symbols of
     * "prod" and "find" */
    return ((struct bdgm_prod *) prod)->lhs -
           ((struct bdgm_prod *) find)->lhs;
}

/* "cmp_idxs" - return a comparison between
 * the production indexes in "idx1" and "idx2",
 * Used as the find" call-back for ordering
 * productions by index */
static int cmp_idxs(int *idx1,
                    int *idx2) 
{
    /* return a numeric comparison between 
     * "iindexces */
    return *idx1 - *idx2;
}

/* is_nt" - returns non-zero if symbol type
 * "tyoe" at the grammar given in "gmr" is
 * a non-terminal symbol */
static int is_nt(int type,
                 struct bdgm_grmr *gmr) 
{
    /* symbol is non-terminal if its type is
     * in the grammar's non-terminal range */
return type > gmr->num_terms;
}

/* "cmp_syms" - compare two grammar symbol
 * types, returning zero if they both represent
 * the symbol type,negative if the first is
 * greater, or positive otherwise */
static int cmp_syms(int *sym1,
                    int *sym2)
{
    /* return the difference between
     * the two symbol types */
    return (*sym1 - *sym2);
}

/* "excl_sym" - copies the symbols in "in"
 * from those in "out" except for symbols
 * of type "type", using the memory all-
 * ocator xnd error logger given in "allocs"
 * and "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
static int excl_sym(struct bd_map_node *in,
                    struct bd_map_node **out,
                    int type,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger) 
{
    struct bd_map_node *it,        /* "in" iter- */
                       *ins_node;  /* ator and
                                    * insertion
                                    * node */
    int sym;                      /* current
                                    * symbol */

    *out = NULL; /* out is initially
                  * empty */
    
    /* iterate through "in" ... */
    for (it = bd_map_first(in);
         it;
         it = bd_map_next(it)) {

        sym = * (int *) &it->key;

        /* if the symbol isn't 
         * "type", insert it
         * into "out" */
        if (sym != type) {

            ins_node = (struct bd_map_node *)
                   bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);
            if (!(ins_node))
                return 0;
            if (!(bd_map_insert(out,
                                ins_node,
                                &sym,
                                (bd_map_cmp_fn)
                                      cmp_syms,
                                &sym)))
                return 0; 
        }
    }
    return 1;
}

/* "cmp_items" - compare the two grammar items
 * fuvrn in "item1" and "item2" */ 
/* return non-zero if they both refer to the
 * same rule sand right-hand-side index */
static int cmp_items(struct bdgm_item *item1,
                     struct bdgm_item *item2)
{
    /* return zero only if both items refer to
     * same the same rules, right-hand-side
     * indexes and look-ahead symbols */
    return cmp_prods(item1->prod,
                     item2->prod) &&
           item1->rhs  - item2->rhs  &&
           item1->lah  - item2->lah;
}

/* "union_syms" - using the memory allocator and
 * error ogger given in "allocs" and "logger",
 * respectively, modifies "total_items" to be a
 * union of the symbols in "add_items" and those
 * in "*total_items".  Returns a negative value
 * on error, a positive value if an item was
 * added to "total_syms", and zero otherwise */
static int union_syms(struct bd_map_node **total_syms,
                      struct bd_map_node *add_syms,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)

{
    struct bd_map_node *it,        /* symbol iterator */
                       *ins_node;  /* and insertion node */ 
                                   /* current symbol and */
        int *sym,                  /* whether any symbols */
            added = 0;             /* in "add_syms" were
                                    * added to "total_it-
                                    * ems" */
    
    /* iterate through items in "total_items" ... */
    for (it = bd_map_first(*total_syms);
                    it;
                    it = bd_map_next(it)) {

        /* get iterator's key (a symbol) */
        sym = (int *) it->key;

        /* if this item isn't in
         * "add_items" ... */
        if (!(bd_map_find(add_syms,
                          &sym,
                          (bd_map_cmp_fn)
                             cmp_syms))) {

             /* allocate insertion node */
            ins_node = (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);
            if (!(ins_node))
                return -1;

            /* ... then add to total
             * items set */
            if (!(bd_map_insert(total_syms,
                                ins_node,
                                &sym,
                                (bd_map_cmp_fn)
                                   cmp_syms,
                                &sym)))
                return -1;
        
            /* and record that at least
             * one item was added */
            added = 1;
        }
    }
    return added;
}

/* "contains_sym" - indicates, by returning a non-zero
 * value, whether the set of symbol types in "types"
 * contains a symbol of type "type".  Zero is returned
 * otherwise */
static int contains_sym(int type,
                        struct bd_map_node *syms,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bd_map_node *it; /* symbol iterator */

    /* cycle through all symbol types in the set ... */
    for (it = bd_map_first(syms);
         it;
         it = bd_map_next(it))

        /* indicate success if symbol type is the
         * relevant type */
        if (type == * (int *) &it->key)
            return 1;
            /* otherwise, if none of the symbols is
             * that type, return zero */

    return 0;
}

/* "union_sym" -  sets the union of grammar symbol
 * type "sym" with  the symbol types in "*syms" to
 * "*syms". Returns a negative value on error, pos-
 * itive if "type" is added to "syms", and zero oth-
 * erwise */
static int union_sym(struct bd_map_node **syms,
                     int type,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion node */
    int ret = 0;                  /* status to
                                   * return */

    /* set "ret" to whether "type" is
     * in "syms" ... */
    if (bd_map_find(*syms,
                    &type,
                    (bd_map_cmp_fn)
                        cmp_syms)) {

        /* then add it, indicating the
         * addition by returning non-
         * zero */

        /* first allocate node to
         * insert .. */
        ins_node = (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bd_map_node),
                           logger);
        if (!(ins_node))
            return -1;

        /* then insert the symbol */
        if (!(bd_map_insert(syms,
                            ins_node,
                            &type,
                            (bd_map_cmp_fn)
                                  cmp_syms,
                            &type)))
            return -1;

        /* if we get here, indicate
         * the symbol was added */
        return 1; 
    
    } else /* symbol isn't in
            * "syms" ... */
        
        /* indicate symbol wasn't
         * added */
        return 0;
}

/* "cmp_cores" - returns the result of a comparison
 * of ythe item cores (that is, without a look-ahead
 * symbol)in "core1" and "core2" */
static int cmp_cores(struct bdgm_item *core1,
                     struct bdgm_item *core2)
{
    /* compare only the rule snd right-hand-
     * side indeces */
    return core1->rhs  - core2->rhs &&
           core1->prod - core2->prod;
}

/* "cmp_core_sets" - returns the result of a comp-
 * arison between the cores of the iteh sets in
 * "set1" and "set2" */
static int cmp_core_sets(struct bd_map_node *set1,
                         struct bd_map_node *set2)
{
    struct bd_map_node *node1 = bd_map_first(set1), 
                       *node2 = bd_map_first(set2);
                             /* iterators of "est1"
                              * and "set2" */
    struct bdgm_item *core1,  /* current cores for */
                     *core2;  /* "set1" and "set2" */
                    

    /* get keys of "set1" and "set2" */
    core1 = (struct bdgm_item *)
                 node1->key;
    core2 = (struct bdgm_item *)
                 node2->key;
    

    /* continur intil either iterator is exhausted */
    while (!(!core1 && !core2)) {

        /* ... rof rule right-hand-side indeces or
         * rules are different, return the differ-
         * ences */
        if (cmp_cores(core1, core2))
            return cmp_cores(core1, core2);

        /* fetch bext nodes in sets */
        node1 = bd_map_next(node1);
        node2 = bd_map_next(node2);
    }

    /* return non-zero (non-identi-
     * cal) unless both iterators
     * are simultansously exhausted */
    return (!(!core1 && !core2));
}

/* "grp_cores" - modify "grp" so that only
 * the item cores not already present in
 * "grp" are added to it, using the memory
 * allocator and error logger given in
 * "allocs" and "logger", respectively:
 * This routine ensures "grps" contains item
 * cores with no duplicates.  Returns a
 * negative value on error, positive if any
 * items were added to "*grps" and zero
 * otherwise */
static int grp_cores(struct bd_map_node **grps,
                     struct bd_map_node *set,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bd_map_node *it,       /* iterator */
                       *ins_node; /* and insertion
                                   * node */
    struct bdgm_item *core;       /* current core */
    int added = 0;                /* whether an
                                   * addition was
                                   * made to "grps"  */

    /* iterate over the item
     * cores in "set" */
    for (it = bd_map_first(set);
         it;
         it = bd_map_next(it)) {

        /* get the node's key (an item) */
        core = (struct bdgm_item *)
                         it->key;

        /* if item isn't found in
         * the item groups ... */
        if (!(bd_map_find(*grps,
                          core,
                          (bd_map_find_fn)
                              cmp_cores))) {
 

            /* allocate the insertion
             * node ... */
            ins_node = (struct bd_map_node *)
                        bd_alloc(allocs,
                                 sizeof(struct
                                        bd_map_node),
                                 logger);
            if (!(ins_node))
                 return -1;

            /* insert a new, empty look-
             * ahead symbol map into
             * "grps" */
            if (!(bd_map_insert(grps,
                                ins_node,
                                core,
                                (bd_map_find_fn)
                                    cmp_cores,
                                core)))
                return -1;

            /* and, at this point indicate
             * we successfully added a symbol */
            added = 1;
        }
    }
    return added;
}

/* "build_lah_map" - build, and set "*ret" to, a
 * map containing all the look-ahead symbols found
 * in the map of items of "all" matching the item
 * core in "core", using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns zero on error, non-zero
 * otherwise */
static int build_lah_map(struct bd_map_node *all,
                         struct bd_map_node **ret,
                         struct bdgm_item *item,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    struct bdgm_item *curr;         /* current item */
    struct bd_map_node *it,         /* item core
                                     * iterator */
                       *lah_map,    /* built look-ahead
                                     * map for "item" */
                       *ins_node;   /* insertion
                                     * node */
                                         
    /* iterate through the item
     * cores in "all" */
    for (it = bd_map_first(all);
         it;
         it = bd_map_next(it)) {

        /* get its key (an item) */
        curr = (struct bdgm_item *)
                 it->key;

        /* find the item's look-ahead
         * map in 'grps" */
        lah_map = bd_map_find(all,
                             curr,
                             (bd_map_find_fn)
                                 cmp_cores);
        if (lah_map) {
            /* and if the current item matches
             * "core" ... */
            if (!(cmp_cores((struct bdgm_item *)
                            it->key,
                            item))) {

                /* allocate insertion node */
                ins_node = (struct bd_map_node *)
                      bd_alloc(allocs,
                               sizeof(struct
                                      bd_map_node),
                               logger);
                if (!(ins_node))
                    return -1;

                /* and insert into look-ahead
                 * symbol map */
                if (!(bd_map_insert(ret,
                                    ins_node,
                                    &curr->lah,
                                    (bd_map_cmp_fn)
                                           cmp_syms,
                                    &curr->lah)))
                    return -1;
            }
        }
    }
    return 1;
}

/* "merge" - sets "ret" to an item set cons-
 * isting of a union of the cores of "set1"
 * and "set2",(whose cores are assumed to
 * be identical), but with the look-ahead sym-
 * bols in each of their constituent items
 * having been first grouped on identical cor-
 * es.  The look-ahead symbols of each group
 * are then then un-grouped (added individ-
 * ually) to "ret", using the memory alloc-
 * ator error logger given in "allocs" and
 * "logger", respectively.
 *
 * This merge is used to reduce the number of
 * states initially generated by the LALR
 * table generation algorithm.  Returns zero
 * on error, non-zero otherwise */
static int merge(struct bd_map_node *all,
                 struct bd_map_node *set1,
                 struct bd_map_node *set2,
                 struct bd_map_node **ret,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bd_map_node *it, *j,   /* group iterator, current */ 
                       *lah_map,  /* group, symbol iterator, */
                       *ins_node; /* look-ahead symbols
                                   * having current item 
                                   * core */
    struct bdgm_item *item;       /* current item */
    
    /* first build a map containing node for
     * each unique core (that is, such that
     * there are no duplicates) found in the
     * union of "set1" and "set2", keyed to
     * the item core and valued to a map oF
     * the look-ahead symbols found in that
     * union */

    /* first build the item core groups from
     * "set1" and "set2" */
    if (grp_cores(&all,
                  set1,
                  allocs,
                  logger) < 0)
        return 0; 

    if (grp_cores(&all,
                  set2,
                  allocs,
                  logger) < 0)
        return 0; 

    /* ... then iterate over the groups,
     * building the look-ahead map for
     * each item */
    for (it = bd_map_first(all);
         it;
         it = bd_map_next(it)) {

        /* get the map's key (an
         * item) */
        j = (struct bd_map_node *)
                     it->key;

        item = (struct bdgm_item *)
                     bd_alloc(allocs,
                              sizeof(struct
                                     bdgm_item),
                              logger);
        if (!(item))
            return 0;


        /* snd build the look-ahead
         * map for that item ... */
        if (!(build_lah_map(all,
                            &lah_map,
                            item,
                            allocs,
                            logger)))
            return 0;
       
        /* allocate the insertion 
         * node */
        ins_node = (struct bd_map_node *)
               bd_alloc(allocs,
                        sizeof(struct
                               bd_map_node),
                        logger);
        if (!(ins_node))
            return 0;

        /* ... and insert the look-
         * ahead map into the item
         * node */
        if (!(bd_map_insert(&all,
                            ins_node,
                            item,
                            (bd_map_cmp_fn)
                                cmp_cores,
                            lah_map)))
            return 0;
    }

    /* lastly, for wach item core
     * symbol in each item core
     * group ... */
    for (it = bd_map_first(all);
         it;
         it = bd_map_next(it)) {

        /* get the node's key (an item)
         * and value (a map of the look-
         * ahead symbols having that core */
        item = (struct bdgm_item *)
                  it->key;
        lah_map = (struct bd_map_node *)
                  it->value;

        /* and iterate the roup the
         * look-ahead symbols having
         * the current core */
        for (j = bd_map_first(lah_map);
             j;
             j = bd_map_next(j)) {

            /* get the item's look-ahead
             * symbol */
            item->lah = * (int *) &j->key; 

            /* allocate insertion node */
            ins_node = (struct bd_map_node *)
                        bd_alloc(allocs,
                                 sizeof(struct
                                        bd_map_node),
                                 logger);
            if (!(ins_node))
                return 0;
        }
        
        /* and add the item to the result
         * set */
        if (!(bd_map_insert(ret,
                            ins_node,
                            item,
                            (bd_map_cmp_fn)
                                cmp_items,
                            item)))
            return 0;
    }

    /* if we get to this point
     * return success */
    return 1; 
}

/* "merge_all" - modify the set of item sets
 * given in "*all" so as to merge identical
 * cores, the resulting number of which is
 * then set in "*num", using the memory all-
 * ocator and error logger given in "allocs"
 * and "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int merge_all(struct bd_map_node **all,
                     int *num,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    int add;                      /* whether to continue
                                   * merging sets */
    struct bd_map_node *merged,   /* result of merging */
                       *ins_node, /* the two current */
                       *node1,    /* sets, insertion node, */
                       *node2,    /* item set nodes and */
                       *set1,     /* the iterators of */
                       *set2;     /* those sets */
      
    do { /* continue to merge items ... */

        add = 0; /* but initially assume not
                  * keep adding  */

        /* cycle through two (different)
         * items in sets */
        for (node1 = bd_map_first(*all);
             node1;
             node1 = bd_map_next(node1))

            for (node2 = bd_map_first(*all);
                 node2;
                 node2 = bd_map_next(node2)) {

                /* get the keys of the nodes
                (item sets) */
                set1 = (struct bd_map_node *)
                          node1->key;
                set2 = (struct bd_map_node *)
                          node2->key;

                /* if the items are diffe-
                 * ren, but with identical
                 * cores ... */
                if (node1 != node2 &&
                       (!(cmp_core_sets(set1,
                                     set2)))) {

                    /* we need to add, so
                     * indicate this */
                    add = 1;

                    /* set "merged" to merger
                     * of "i" and "j"  */
                    if (!(merge(*all,
                                set1,
                                set2,
                                &merged,
                                allocs,
                                logger)))
                        return 0;

                    /* ... allocate the
                     * insertion node */
                    ins_node = (struct bd_map_node *)
                          bd_alloc(allocs,
                                   sizeof(struct
                                          bd_map_node),
                                   logger);
                    if (!(ins_node))
                        return 0; 

                    /* ... and replace "node1" and "node2"
                     * with the merged set by removing
                     * "node1" and "node2", then inserting
                     * "merged" into "items" in their place */
                    if (!(bd_map_remove(all, node1)))
                        return 0; 
                    if (!(bd_map_remove(all, node2)))
                        return 0;
                    if (!(bd_map_insert(all,
                                        ins_node,
                                        merged,
                                        (bd_map_cmp_fn)
                                            cmp_core_sets,
                                        merged)))
                         return 0;

                    /* then decrement the number of
                     * items in "items" to account
                     * for the merge */
                   (*num)--;
                }
            }
    } while (add); /* continue until we can't merge
                   * any longer */

    /* if we get to this point, we return succe-
     * ssfully */
    return 1;
}

/* "union_item" -  if the grammar item "item" is not in 
 * the set of items "items", inserts it into the set
 * and returns non-zero, otherwse returns zero.  A neg-
 * ative value is returned on error */

static int union_item(struct bdgm_item *item,
                      struct bd_map_node **items,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* if "item" isn't already in "items" ... */
    if (!(bd_map_find(*items,
                      item,
                      (bd_map_cmp_fn)
                           cmp_items))) {

        ins_node = (struct bd_map_node *)
                  bd_alloc(allocs,
                           sizeof(struct
                               bd_map_node),
                           logger);
        if (!(ins_node))
            return -1;

        /* ... then perform insertion */
        if (!(bd_map_insert(items,
                            ins_node,
                            item,
                            (bd_map_cmp_fn)
                                 cmp_items,
                            item)))
            return -1; 

        return 1; /* indicate an item
                   * was added */
    
    } else
        return 0; /* otherwise, indi-
                   * cate an item
                   * wasn't added */
}

/* declare proto-type of "first" so as
 * to resolve mutual dependency with
 * "first1" */
static int first(int *,
                 int,
                 struct bd_map_node **,
                 struct bdgm_grmr *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "first1" - sets "*ret" to a map representing
 * the set of the terminal symbols that
 * immediately follow the symbol type "type" in
 * the grammar "gmr", using the memory allocator
 * and error logger given in "allocs" and
 * "logger", respectively.
 *
 * Based on the algorithm given in
 * "http://www.jambe.co.nz/UNI/FirstAnd-
 * FollowSets.html".
 *
 * Returns zero on error, non-zero
 * otherwise */
static int first1(int type,
                  struct bd_map_node **ret,
                  struct bdgm_grmr *gmr,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bd_map_node *it,       /* iterator, */
                       *ins_node, /* insertion */
                       *add_syms; /* node and sym-
                                   * bols to add */
    int added, res;               /* whether symbols */
                                  /* were added this
                                   * iteration and
                                   * result of symb-
                                   * ols' union
                                   * with result set */
    struct bdgm_prod *prod,       /* current produ- */
                     *rule;       /* ction and rule */

    /* if "type" is a terminal ...*/
    if (is_term(type,
                gmr)) {

        /* add just this symbol
         * to the resulting set */

        /* so start by clearing the set,
         * then inserting just "type" */
        *ret = NULL;
        ins_node = (struct bd_map_node *)
                   bd_alloc(allocs,
                            sizeof(struct
                                   bd_map_node),
                            logger);
        if (!(ins_node))
            return 0;
        if (!(bd_map_insert(ret,
                            ins_node,
                            &type,
                            (bd_map_cmp_fn)
                                cmp_syms,
                            &type)))
            return 0;

    } else { /* "type" is a non-
              * terminal*/

        /* initially we don't add
         * symbols by default */
        added = 0;

        /* iterate through
         * "gmr"'s production
         * set */
        for (it = gmr->prods;
             it;
             it = bd_map_next(it)) {

            /* get the iterator's
             * value (a production) ... */
            rule = (struct bdgm_prod *)
                  it->key;

            prod = (struct bdgm_prod *)
                   bd_alloc(allocs,
                            sizeof(struct
                                   bdgm_prod),
                            logger);
            if (!(prod))
                return 0;

            /* ... and, if the rule's
             * left-hand-side symbol is
             * this symbol ... */
            if (prod->lhs == type)

                /* if this ia an "empty"
                 * rule (that is, cont-
                 * aining no right-hand-
                 * side symbols) ...*/
                if (!(prod->num)) {

                    /* add the empty symbol
                     * to the result set */
                    res = union_sym(ret,
                                    bdgm_sym_empty,
                                    allocs,
                                    logger);
                    if (res < 0)
                        return 0;

                    else if (res > 0)
                        added = 1;

                } else { /* so is a non-empty
                        * rule */

                    /* set "add_syms" to the
                     * "first set" of the set
                     * of right- hand-side
                     * symbols */
                    if (!(first(prod->rhs,
                                prod->num,
                                &add_syms,
                                gmr,
                                allocs,
                                logger)))
                        return 0;

                    /* ... and keep adding
                     * symbols if any of
                     * these symbols
                     * aren't already in
                     * the resulting set
                     * (indicated by s pos-
                     * itive return value) */
                    res = union_syms(ret,
                                     add_syms,
                                     allocs,
                                     logger);
                    if (res > 0)
                        added = 1;

                    /* otherwise, signal
                     * that the unioning
                     * failed ...  */
                    else if (res < 0)
                        return 0;
                }
        }
    }
    /*  if there's no error, indicate a
     *  successful return  */
    return added >= 0;
}

/* "first" - sets "*ret" to a map representing
 * the set of the terminal symbols that imme-
 ( diately follow the array of symbol types
 * the beginning of which is pointed to by "type"
 * snd the number of which is given in "num",
 * in the grammar "gmr", using the memory allo-
 * cator and error logger given in "allocs"
 * and "logger", respectively.
 *
 * Based on the algorithm given in
 * "http://www.jambe.co.nz/UNI/FirstAnd-
 * FollowSets.html".
 *
 * Retuens ezro on error, non-zero
 * otherwise */
static int first(int *types,
                 int num,
                 struct bd_map_node **ret,
                 struct bdgm_grmr *gmr,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    int res;                   /* result of
                                * symbol
                                * containm-
                                * ent query */
    struct bd_map_node *frsts, /* set of first */
                       *rest;  /* symbols and
                                * "first set"
                                * of remaining
                                * symbols */


    /* dryset "frsts" to the "first set" of
     * the first symbol in "types" .... */
    if (!(first1(*types,
                 &frsts,
                 gmr,
                 allocs,
                 logger)))
        return 0;

    /*  ... and, if that set desn't contain
     * the "empty" symbol */
    if (!(res = contains_sym(bdgm_sym_empty,
                             frsts,
                             allocs,
                             logger))) {
        if (res < 0)
            return 0;

        /* set "ret" to the "first set"
         * of the first symbol in "types" */
        if (!(first1(*types,
                     ret,
                     gmr,
                     allocs,
                     logger)))
            return 0;        

    } else { /* otherwise, "*types" *does*
              * contain the "empty" symbol */

        /* so remove it */
        if (!(excl_sym(frsts,
                       &frsts,
                       bdgm_sym_empty,
                       allocs,
                       logger)))
            return 0;

        /* and, if there is only one symbol
         * in "types" */
         if (num == 1)

            /* set "rest" to the "first
             * set" of the first symbol
             * in "types" */
            if (!(first1(*types,
                         &rest,
                         gmr,
                         allocs,
                         logger)))
                return 0;

            else /* or, otherwisem,  "types" has
                  * are multiple symbols */

                /* so set "rest" to the fitst set
                 * of the symols in "types" after
                 * the first */
                if (!(first(types + 1,
                            num - 1,
                            &rest,
                            gmr,
                            allocs,
                            logger)))
                    return 0;

         /* and union thise two sets */
        if (!(union_syms(&rest,
                         frsts,
                         allocs,
                         logger)))
            return 0;    
    }

    /* return success if we
     * get here */
    return 1; 
}

/* "follow" - Sets "ret" to the "follow set" of non-
 * terminal symbol type "type" in grammar "gmr"(where
 * a "follow set" is defined as the set of terminal
 * symbols that can appear immediately to the right
 * of "type").  Returns zero on error, non-zero other-
 * wise,
 *
 * Based on the algorithm given in
 * "http://www.jambe.co.nz/UNI/FirstAnd-
 * FollowSets.html".

 *
 * Returns zero on error, non-zero
 * otherwise */
static int follow(int type,
                  struct bd_map_node **ret,
                  struct bdgm_grmr *gmr,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bd_map_node *it,     /* production */
                       *frsts,  /* iterator, */
                       *frsts1, /* "first" set, */
                       *fllws;  /* first set */
                                /* excluding */
                                /* empty symbol
                                 * and follow set
                                 * of rule's left-
                                 * hand symbol */
    struct bdgm_prod *prod,     /* production */
                     *rule;     /* and rule */
    int item,                   /* right-hand  */
        added;                  /* symbol index */
                                /* and whether sy- */
                                /* mbols were ad- 
                                 * ded */

    /* initially presume not to add
     * symbols */
    added = 0;

    do { /* continue to add symbols to
          * the result set ... */

        /* iterate through the productions
         * in the grammar */
        for (it = bd_map_first(gmr->prods);
             it;
             it = bd_map_next(it)) {

            /* get iterator value
             * (a production) */
            rule = (struct bdgm_prod *) 
                      it->key;

            /* if the rule's left-hand symbol
             * is the "start" symbol, add the
             * "end of stream" symbol */
            if (rule->lhs == bdgm_sym_start)
                added = union_sym(ret,
                                 bdgm_sym_eos,
                                 allocs,
                                 logger);
            if (added)
                    return 0;

            prod = (struct bdgm_prod *)
                bd_alloc(allocs,
                         sizeof(struct
                                bdgm_prod),
                         logger);
            if (!(prod))
                return 0;

            prod->rhs = (int *)
                bd_alloc(allocs,
                         sizeof(int) * prod->num,
                         logger);
            if (!(prod->rhs))
                return 0;

            /* iterate through the rule's
             * right-hand-side symbols ... */
            for (item = 0;
                 item < prod->num;
                 item++)

                /* ... and, if the current
                 * symbol is of this type ... */
                if (prod->rhs[item] == type) {

                    /* est "frsts" to the
                     * "first set" of those
                     * right-hand symbols
                     * to the right id the
                     * current symbol */
                    if (!(first(prod->rhs + item,
                                prod->num - item,
                                &frsts,
                                gmr,
                                allocs,
                                logger)))
                        return 0;

                    /* ... set "frsts1" to
                     * be the same as the
                     * above set, but excl-
                     * uding empty symbol */
                    if (!(excl_sym(frsts,
                                   &frsts1,
                                   bdgm_sym_empty,
                                   allocs,
                                   logger)))
                        return 0;

                    /* ... and set "fllws"
                     * to the "follow set"
                     * of the rule's left-
                     * hand symbol */
                    if (!(follow(rule->lhs,
                                 &fllws,
                                 gmr,
                                 allocs,
                                 logger)))
                        return 0;

                    /* if the current
                     * symbol isn't
                     * the left-most,
                     * sff the non-empty
                     * symnold from the
                     * right-hand aymb-
                     * ols' "first set" */
                    if (item < prod->num)
                        added = union_syms(ret,
                                           frsts1,
                                           allocs,
                                           logger);


                    /* or, if th r urej is
                     * rhe right-most, add
                     * all (including empty)
                     * symbols from the above-
                     * mentioned "first set" */
                    else
                        added = union_syms(ret,
                                           fllws,
                                           allocs,
                                           logger);
                }

                /* if the the set includes
                 * the empty symbol and this
                 * isn't the right-most
                 * symbol, then add the left-
                 * side symbol's "follow" set */
                if (contains_sym(bdgm_sym_empty,
                                 frsts,
                                 allocs,
                                 logger) && item <
                                       rule->num)
                    added = union_syms(ret,
                                       fllws,
                                       allocs,
                                       logger);
            } 
        }
    while (added > 0); /* ... until there's
                        * an error or no
                        * more symbols have
                        * been added this
                        * iteration */

    /* if there's no error,
     * ("added" will be neg-
     * ative on error  - as
     * returned by "union-
     * syms"), return succ-
     * ess */
    return added >= 0;
}

/* "closure" - Sets "*clsre" to the "closure" of the set
 * of grammar items in "items" of the production rule
 * set "gmr", which is the set of items which starts hav-
 * ing the items in "items" and adding items not already
 * in the closure that have a left-hand-side non-terminal,
 * where there an existing item possessing a right-hand-
 * side position just to the left of that symbol.  Retu-
 * rns zero on error, non-zero otherwise */
static int closure(struct bd_map_node *items,
                   struct bd_map_node **clsure,
                   struct bdgm_grmr *gmr,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bd_map_node *ret = items, /* the closure items to */
                       *gmr_it,      /* return, grammar, */
                       *item_it,     /* and item and first set */
                       *fst_it,      /* iterators */
                       *prod_node,   /* node of item's*/
                       *add_node,    /* production, node of  */    
                       *firsts;      /* "goto" set to add and 
                                     /* first terminals set */
    struct bdgm_prod *gmr_prod,      /* remainder of rule */
                     *item_prod;     /* item productions */
        int rest_len,                /* length of remainder */
            *rest,                   /* (after item's right- */
            lah_fst,                 /* hand-side index, */
            res;                     /* look-ahead of
                                      * current first set
                                      * and "union_syms"
                                      * result */
    struct bdgm_item *item,          /* current item */
                     *add_item;      /* in "items" and */
                                     /* "goto" items to add */
                                     /* to total items */
    int add = 1, sym;                /* whether to continue */
                                     /* to add symbols and
                                      * current symbol type */

    /* repeat until no more items can be added ... */
    do {

        /* initially assume to keep adding
         * symbols  */
        add = 0;

        /* cycle "it" through the items in
         * "items" */
        for (item_it = bd_map_first(items);
            item_it;
            item_it = bd_map_next(item_it)) {

            /* cast "item" to the correct type */
            item = (struct bdgm_item *)
                      item_it->key;

            /* find production of "item" */
            prod_node = bd_map_find(gmr->prods,
                                    &item->prod,
                                    (bd_map_find_fn)
                                        cmp_prods);
            if (!(prod_node))
                return 0;

            /* ... and get the node's key (a prod-
             * uction rule) */
            item_prod = (struct bdgm_prod *)
                      prod_node->key;

            /* get the item's symbol */
            sym = item_prod->rhs[item->rhs];

        /* ... get a pointer to the "rest" of
         * the right-hand-side (that is, the
         * symbols after the iten'a right-
         * hand-side index),  and the number
         * of these symbols */
        rest = item_prod->rhs + item->rhs + 1;
        rest_len = (*item_prod->rhs) - item_prod->num;

        /* if the symbol is non-terminal
         and the right-hand-side index
         * is within proper range ... */
        if (is_nt(sym, gmr) && item->rhs <
                           item_prod->num)

            /* iterate through the grammar's
             * productions */
            for (gmr_it = bd_map_first(gmr->prods);
                 gmr_it;
                 gmr_it = bd_map_next(gmr_it)) {

                /* get the node's value (a prod-
                 * uction rule */
                gmr_prod = (struct bdgm_prod *)
                    gmr_it->key;

                /* and look for this symbol as
                 * left-hand-side */
                if (sym == gmr_prod->lhs) {

                     rest[rest_len] = item->lah;

                     /* set "firsts" to the
                      * "first set" of the
                      * rest of the current
                      * production'f right-
                      * side symbols */
                     if (!(first(rest,
                                 rest_len + 1,
                                 &firsts,
                                 gmr,
                                 allocs,
                                 logger)))
                         return 0;

                     /* and iterate through
                      *  the symbols in that
                      *  set ... */
                     for (fst_it = bd_map_first(firsts);
                          fst_it;
                          fst_it = bd_map_next(fst_it)) {               
                                

                        /* get ghe current symbol as
                         * the node's key */
                        lah_fst = * (int *) fst_it->key;

                        /* ... allocate insertion node .... */
                         add_node = (struct bd_map_node *)
                               bd_alloc(allocs,
                                        sizeof(struct
                                               bd_map_node),
                                        logger);
                       if (!(add_node))
                           return 0; 

                       /* get the node's key (an item) */
                       add_item = (struct bdgm_item *)
                                       add_node->key;

                       /* and initialise the item to ref-
                        * er to this rule and whose right-
                        * hand-side index points to the
                        * start of the rule */
                       add_item->rhs  = 0;
                       add_item->lah  = lah_fst;

                       /* union this item
                        * with the return set */
                       res = union_item(add_item,
                                        clsure,
                                        allocs,
                                        logger);
                       if (res < 0)
                           return 0;

                         /* if items aren't
                          * already present,
                          * then signal to
                          * keep adding  */
                         if (res > 0)
                             add = 1;
                         
                    }
                }
            }
        }
    }

    while (add); /* ... until no more
                  * symbols havebeen
                  * added this iterat-
                  * ion */

    /* return successfully
     * if there is no error,
     * when we get to this
     * point  */
    return add >= 0;
}

/* "goto_fn" - sets "gotos" to the a modification
 * of the  closure of the items in  "items" such
 * that, if the right-hand-side position of an
 * item just precedes a terminal grammar symbol
 * of type "type", that item's right-hand-side
 * position will be incremented to after the posi-
 * tion of that symbol (thereby simulating a
 * "shift" on that symbol type).  Returns zero
 * on error, non-zero otherwise */ 
static int goto_fn(int type,
                   struct bd_map_node *items,
                   struct bd_map_node **gotos,
                   struct bdgm_grmr *gmr,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdgm_prod *prod;         /* current item's
                                     * production */
    struct bdgm_item *item;         /* current item */
                                    /* current rule preceding
                                     * the item's right-hand-
                                     * side index */
    int add = 1, s;                 /* an indication of
                                     * whether to keep add-
                                     * ing symbols and the
                                     * symbol to which the
                                     * current item refers */
    struct bd_map_node *it,         /* item iterator and */
                       *node;       /* insertion node */


    /* the "goto" set starts
     * from the input set */
    *gotos = items;                 

    /* repeat until no more items can be added */
    while (add) {

        add = 0; /* start each iteration
                  * assuming not to add */

        /* cycle "it" through each item
         * in "items" */
        for (it = bd_map_first(items);
             it;
             it = bd_map_next(it)) {

            /* get key of "it" (an item) */
            item = (struct bdgm_item *)
                    it->key;

            /* get the production of the rule */
            node = bd_map_find(gmr->prods,
                               &item->prod,
                               (bd_map_find_fn)
                                   find_prod);
            if (!(node))
                return 0;
           
            /* get the hey of "node" (a
             * production) */
            prod = (struct bdgm_prod *)
                        node->key; 

            /* set "s" to "item"'s symbol in
             * production */
            s = prod->rhs[item->rhs];

            /* if the current symbol is
             * this symbol (and the item's
             * right-hand-side index isn't
             * at the end of the rule),
             * increment the item's right-
             * hand-side position (to past
             * the symbol) and add the item
             * to the return set */
            if (!(cmp_syms(&s, &type)) &&
                     item->rhs >= 0 &&
                     item->rhs < prod->num)
                item->rhs++;            
        }
    }

    /* set "*gotos" to the "closure*
     * of "items" and return the st-
     * atus of the call */
    return closure(items,
                   gotos,
                   gmr,
                   allocs,
                   logger);
}

/* "build_item_sets" - sets "*sets" to the set
 * of grammar item sets , the number of which
 * is set in "num", from the grammar in "gmr",
 * which represents the non-deterministic fi-
 * nite automaton capable of recognising str-
 * ings generated by this grammar, using the
 * memory allocator, error logger, tsytart sy-
 * mbol and recursive flag given in "allocs",
 * "logger", "start" and "rcrse", respective-
 * ly.  Returns zero error, non-zero otherw-
 * ise */
static int build_item_sets(struct bdgm_grmr *gmr,
                           int *num,
                           struct bd_map_node **sets,
                           int start,
                           int rcrse,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    int num_sets = 0;                /* number of item sets
                                      *  in return set */
    struct bd_map_node *total_items, /* total set of items */
                       *goto_items,  /* "goto items" for */
                       *node = NULL, /* insertion node, */
                       *start_items, /* start set, */
                       *it,          /* item set iterator, */
                       *ins_node,    /* insertion node, */
                       *lah_node,    /* look-ahead sym- */
                       *lahs;        /* bol node and
                                      * map */
    struct bdgm_item *start_item,    /* start item */
                     *init_item;     /* and initial item */
    struct bdgm_prod *start_prod,    /* start production */
                     *prod;          /* and current prod- */
    int sym,                         /* uction */
        add = 1,                     /* symbol, whether */
        *lah;                        /* er to keep adding
                                      * sets ajnd current
                                      * klook-ahead symbol */

    /* find production of having start left-
     * hand-side symbol */
    start_prod = (struct bdgm_prod *)
                 bd_map_find(gmr->prods,
                             (void *) &start,
                             find_lhs);

    init_item = (struct bdgm_item *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdgm_item),
                   logger);
    if (!(init_item))
        return 0; 

    /* and set up core having that symbol */
    init_item->prod = start_prod;
    init_item->rhs  = 0;

    /* mnon-recursively build sets of items
     * required by aquisition of look-ahead
     * symbols for start symbol */
    if (rcrse) 
        build_item_sets(gmr,
                        &num_sets,
                        &total_items,
                        start,
                        0,
                        allocs,
                        logger);

    /* iterate through productions
     * in grammar */
    for (it = bd_map_first(gmr->prods);
         it;
         it = bd_map_next(it)) {

        /* get current production from
         * node's key */
        prod = (struct bdgm_prod *)
                   it->key;

        /* if production's left-hand-side
         * symbol is start symbol */
        if (prod->lhs == start) {

            /* build look-ahead symbols
             * for that symbol   */
            build_lah_map(total_items,
                          &lahs,
                          init_item,
                          allocs,
                          logger);

            /* cycle through each of those
             * look-ahead symbols */
            for (lah_node = bd_map_first(lahs);
                 lah_node;
                 lah_node = bd_map_next(lah_node)) {

                /* get the symbol from the node's key*/
                lah = (int *) lah_node->key;

                start_item = (struct bdgm_item *)
                     bd_alloc(allocs,
                             sizeof(struct
                                    bdgm_item),
                              logger);
                if (!(start_item))
                    return 0;

                /* build an item from that production
                 * and look-ahead symbol */
                start_item->prod = start_prod;
                start_item->rhs = 0;
                start_item->lah = *lah;

                /* and add that item to the dystart set */
                ins_node = (struct bd_map_node *)
                     bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger);
                if (!(ins_node))
                    return 0;
                if (!(bd_map_insert(&start_items,
                                    ins_node,
                                    start_item,
                                    (bd_map_cmp_fn)
                                        cmp_items,
                                    start_item)))
                    return 0;
            }

            /* start the total items as the
             * closure of the start item */
            closure(start_items,
                    &start_items,
                    gmr,
                    allocs,
                    logger);

            while (add) { /* keep iterating while we've
                           * added sets */

                add = 0;  /* start assuming not to add
                   * a set */

                /* cycle through each type of terminal
                 * symbol */
                for (sym = 0; sym < gmr->num_terms;
                               sym++) {

                    /* cycle through each existing
                     * item set */
                    for (it = bd_map_first(total_items);
                         it;
                         it = bd_map_next(it)) {

                        /* the "goto_items" for
                         * the state starts
                         * as the "goto set" of
                         * the * current symbol
                         * type for the current
                         * item set */
                        if (!(goto_fn(sym,
                                      it,
                                      &goto_items,
                                      gmr,
                                      allocs,
                                      logger)))
                            return 0;

                        /* if any part of this
                         * "goto set" is
                         * outside the accumu-
                         * lated total items ... */
                        if (!(union_syms(&total_items,
                                         goto_items,
                                         allocs,
                                         logger))) {

                            /* then indicate to co
                             * continue adding sets */
                            add = 1;

                            (*num)++; /* and that we
                                       * have another
                                       * state */


                            /* allocate insertion
                             * node */
                            node =
                            (struct bd_map_node *)
                            bd_alloc(allocs,
                                     sizeof(struct
                                            bd_map_node),
                                     logger);

                            /* ... and insert
                             * the "goto" items
                             * into the total
                             * items as a const-
                             * ituent set */
                            if (!(bd_map_insert(
                                       sets,
                                       node,
                                       goto_items,
                                       (bd_map_cmp_fn)
                                           cmp_items,
                                       goto_items)))
                                return 0;
                        }

                        /* now that all item sets
                         * are built, we reduce
                         * their number ny merging
                         * those that have identi-
                         * cal cores */
                        if (!(merge_all(sets,
                                        num,
                                        allocs,
                                        logger)))
                            return 0;
                    }
                }
            }
        }
    }
    /* return success
     * when we get here */
    return 1;
}

/* "init_state" - intialise and return , using
 * the memory allocator and error logger given in
 * "allocs" and "logger", respectively, the "act-
 * ion" and "goto" entries of a state of the LALR
 * parsing table "table", generated from the gram-
 * mar "gmr", initially setting its "goto" states
 * to -l and actions to "error" */
static struct bdgm_state *init_state(struct bdgm_table *table,
                                     struct bdgm_grmr *gmr,
                                     struct bd_allocs *allocs,
                                     struct bd_logger *logger)
{
    int i;                  /* terminal symbol iterator */
    struct bdgm_state *ret; /* table state to return */


   /* allocate the state itself */
    ret = (struct bdgm_state *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bdgm_state),
                           logger);
    if (!(ret))
        return NULL;


   /* ... and, if successful, allocate
    * its "actions" array, the umber of 
    * which is the number of terminals */
   ret->actions = (struct bdgm_action *)
                  bd_alloc(allocs,
                           sizeof(struct
                                  bdgm_action) *
                                  gmr->num_terms,
                           logger);
    if (!(ret->actions))
        return NULL;
        
   /* allocate "goto" array, with its length
    * being the number of non-terminals */
   ret->gotos = (int *) bd_alloc(allocs,
                                 sizeof(int) *
                                 gmr->num_nts,
                        logger);
    if (!(ret->gotos))
        return NULL;

    /* initialise the the state's "action"
     * entries to "error" (the default) */
    for (i = 0; i < gmr->num_terms; i++)
        ret->actions[i].type = bdgm_actn_error;

    /* initialise the the row's "goto"
     * entries to -1 (representing a
     * "blank" state) */
    for (i = 0; i < gmr->num_nts; i++)
        ret->gotos[i] = -1;

    /* return stccess when we get here */
    return ret;
}

/* "set_state_entries" - sets the "action" and
 * "goto" entries for the row in the parsing
 * table table state "tate" corresponding to
 * the item set items" (being psrt of the set
 * of item sets "all") generated from the gra-
 * mmar "gmr".  Returns zero on error, non-
 * zero otherwise*/
static int set_state_entries(struct bdgm_table *table,
                             struct bdgm_state *state,
                             struct bd_map_node *items,
                             struct bd_map_node *all,
                             struct bdgm_grmr *gmr,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
    int i, j;                     /* various iterators */
    struct bd_map_node *it,       /* current item node, */
                       *node,     /* iterator, */
                       *set,      /* current item set */
                       *fllw_set, /* follow set of  */ 
                       *fs_node;  /* item's left-hand- */
                                  /* side symbol, 
                                   * and follow set  */
                                  /* node */
    int fs;                       /* follow set symbol */
    struct bdgm_action *actions,  /* "state"'s action */ 
                       *action;   /* table and current  */
    int sym;                      /* action  */
                                  /* current item's
                                   * symbol */
    struct bdgm_item *item;       /* current item and
                                   * current item's */ 
    struct bdgm_prod *prod;       /* production */

    /* for aech terminal in the grammar ... */
    for (i = 0; i < gmr->num_terms; i++) {


        /* get the tstate's action table and
         * current action */
        actions = state->actions;
        action = actions + i;

        /* ...and eac iten in item in the state ... */
        for (it = bd_map_first(items);
                               it;
                               it = bd_map_next(it)) {

            /* cast state iterator to the
             * correct type (an item) */
            item = (struct bdgm_item *)
                    it->key;

            /* get production for item */
            node = bd_map_find(gmr->prods,
                               &item->prod,
                               (bd_map_find_fn)
                                    find_prod);
            if (!(node))
                return 0;

            /* get production of node 
            /* (node's key) */
            prod = (struct bdgm_prod *)
                     node->key;

            /* get symbol in production */
            sym = prod->rhs[item->rhs];

            /*  if the right-hand-side of item
             * is not at end of production ... */
            if (item->rhs < prod->num) {

                j = 0; /* initialise set
                        * iterator */

                /* ... if the current item
                 * is in the "goto set" of
                 * ""items", then the action will
                 * be a shift to the curent state
                 * ("j", which also pushes that
                 * state onto the parser's stack)

                /* cycle through this state's items */
                for (set = bd_map_first(all);
                     set;
                     set = bd_map_next(set)) {

                    /* if the current symbol
                     * is in the "goto set"
                     * of the "j"th item ... */
                    if (bd_map_find(set,
                                    item,
                                    (bd_map_cmp_fn)
                                        cmp_items)) {
                        /* set the current
                         * action to "shift" to
                         * state "j" */
                        action->type = bdgm_actn_shift;
                        action->u.state = j;
                    }
                }

                j++; /* move ot next set */

                /* if the current item's right-hand-
                 * side index is at the end of the
                 * right-hand-side and the item's
                 * right-hand-side  ... */
            } else if (item->rhs == prod->num) {

                /* get the follow set for the item's
                 * left-hand-side symbol's follow
                 * set */
                if (!(follow(prod->lhs,
                             &fllw_set,
                             gmr,
                             allocs,
                             logger)))
                    return 0;

                /* ... and iterate through its
                 * symnols */
                for (fs_node = bd_map_first(fllw_set);
                     fs_node;
                     fs_node = bd_map_next(fs_node))

                    /* get its key */
                    fs = * (int *) &fs_node->key;

                   /*  ... then set the action
                    * to "reduce" symbols in
                    * item left-hand-side sym-
                    * bol's follow set */
                   actions[fs - gmr->num_terms].
                               type = bdgm_actn_reduce;
                   actions[fs - gmr->num_terms].u.
                               prod = item->prod;
               }

               /* if this is the start" rule .. */
               if (sym == bdgm_sym_start)

                   /* then set the saction for the
                    * current state on the "end of
                    * fbde" symbol to "accept" */
                   state->actions[bdpr_tok_eos].type =
                                       bdgm_actn_accept;

               /* now we process the goto entries:.... */ 

               /* start by cycling "i" through
                * the non-terminals in the
                * grammar's production rule set ... */
               for (i = 0; i < gmr->num_terms; i++)

                   /* then cycling "j" through the
                    * states of the table ... */
                   for (it = bd_map_first(all);
                                   it;
                                   it = bd_map_next(it)) {
 
                       /* "nt_goto_items" is the "goto
                        * set" of the current non-
                        * terminal symbol type */
                       struct bd_map_node *
                                    nt_goto_items;

                       if (!(goto_fn(sym,
                                     it,
                                     &nt_goto_items,
                                     gmr,
                                     allocs,
                                     logger)))
                           return 0;

                       /* ensure the non-terminal
                        * symbol type is distinct
                        * from reserved lexical
                        * symbol types */
                       sym = j + (gmr->num_terms -
                                   bdgm_sym_nres);

                       /* cycle through each item in
                        * the "gOto set" of the
                        * current item */
                       for (it = bd_map_first(
                                nt_goto_items);
                                it;
                                it = bd_map_next(it)) {
                        
                           /* cast item to the
                            * correct type */
                           item = (struct bdgm_item *)
                                        it->key;
                    
                           /* if the "goto set" of the
                            * current symbol type
                            * contains the current
                            * ("j"th) item, then set
                            * the "goto" entry to state
                            * "j" */
                           if (bd_map_find(nt_goto_items,
                                           item,
                                           (bd_map_find_fn)
                                               cmp_items))

                                state->gotos[i] = j;
                       }
                       return 0;
                   }      
            }
    }
    return 0; 
}

/* "build_state" - returns, and builds the
 * "goto" and "action" entries for, the par-
 * sing table "table" corresponding the set
 * of items in "items" (being part overall
 * set of items "all"), generated from the
 * production rule set "gmr" */
static struct bdgm_state *build_state(
                       struct bd_map_node *items,
                       struct bd_map_node *all,
                       struct bdgm_table *table,
                       struct bdgm_grmr *gmr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    int i = 0;               /* state index */
    struct bd_map_node *it;  /* item set iter-
                              * ator */
    struct bdgm_state *ret;  /* return state */

    /* cycle through the items sets in "all" */
    for (it = bd_map_first(all);
                it;
                it = bd_map_next(it)) {
    
        /* initialise that state */
        ret = init_state(table,
                         gmr,
                         allocs,
                         logger);

        /* build the table state entries
         * from the current item set */
        set_state_entries(table,
                          ret,
                          it,
                          all,
                          gmr,
                          allocs,
                          logger);
    }
    return ret;
}

/* "bdgm_prod_add" - using the opaque value
 * given in "opaque", adds the production
 * represented by the left-hand-side symbol
 * type and production's "reduction call-b-
 * ack" given in "lhs" and "reduce", respe-
 * ctively, and the list of right-hand-side
 * symbol types givrn in th r eighth and fo-
 * llowing parameters, to the grammar given
 * in "gmr".  Returns zero on error, non-
 * zero otherwise*/
int bdgm_prod_add(struct bdgm_grmr *gmr,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger,
                  void *opaque,
                  int lhs,
                  int num_rhs,
                  bdgm_reduce_fn reduce,
                  ...)
{
    struct bdgm_prod *prod;       /* production rule to
                                   * initialise */
    int i, lhs_sym;               /* variable argument
                                   * iterator and left-
                                   * hand-side symbol */
    va_list args;                 /* variable argument
                                   * list (representng
                                   * right-hand-side
                                   * symbols) */
    struct bd_map_node *node,     /* production's node */
                       *ins_node; /* in production set
                                   * and insertion node */

    /* allocate the production */
    prod = (struct bdgm_prod *)
                   bd_alloc(allocs,
                            sizeof(struct
                                   bdgm_prod),
                            logger);

    /* quit if the allocation was  unsuccessful ... */
    if (!(prod))
        return 0;

    prod->lhs = lhs_sym = 0; /* set left-hand-
                              * side symbol */
    prod->num = num_rhs;     /* set number of
                              * right-hand-side
                              * symbols */
    
    /* allocate right-hand-side symbols */
    prod->rhs = (int *)
              bd_alloc(allocs,
                      num_rhs * sizeof(int),
                      logger);

    /* initialise right-hand-side
     * symbols teration */
    va_start(args,
             reduce);

    /* and, for each symbol on the
     * right-hand-side ... */
    for (i = 0; i < num_rhs; i++)

        /* set the right-hand-side
         * symbol's type from the
         * variable argument list */
        prod->rhs[i] = va_arg(args,
                              int);

    /* then cleanup the variable
     * argument processing */
    va_end(args);

    /* if pofduction isn't already
     * in production set ... */
    if (!(node = bd_map_find(gmr->prods,
                             prod,
                             (bd_map_find_fn)
                                 cmp_prods))) {

        /* ... insert it */
        ins_node = (struct bd_map_node *)
                bd_alloc(allocs,
                         sizeof(struct
                                bd_map_node),
                      logger);
        if (!(ins_node))
            return 0;
        if (!(bd_map_insert(&gmr->prods,
                            ins_node,
                            (void *) prod,
                            (bd_map_cmp_fn)
                                cmp_prods,
                            (void *) reduce)))
            return 0;

    /* otherwise, if this production already
     * exists in the set, set it's value
     * to "redyuce" */
    } else
        node->value = (void *) reduce;


    /* indicate success when
     * get here */
    return 1;
}

/* "bdgm_cleanup" - reclaims resources used
 * by the grammar given in "gmr", usually called
 * just before the grammar itself is recla-
 * imed, using the memory alocator "allocs"  */
int bdgm_cleanup(struct bdgm_grmr *gmr,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bd_map_node *node; /* node iterator */

    /* while grammar is non-empty ... */
    while (gmr->prods) {

        node = bd_map_first(gmr->prods);

        /* free that production's right-
         * hand-side symbol list memory
         * pointer */
        bd_map_remove(&gmr->prods,
                      gmr->prods);
    }
    return 0;
}

/* "bdgm_table_init" - allocates, using the memory allocator
 * and error logger given in "allocs" amd "logger", respecti-
 * vely, returns an LR table.  A logger to which allocation
 * and other errors will be logged is given in the
 * "logger" */
struct bdgm_table *bdgm_table_init(struct bdgm_grmr *gmr,
                                   struct bd_allocs *allocs,
                                   struct bd_logger *logger)
{
    struct bdgm_table *ret = NULL; /* parsing table
                                    * to return */

    /* build table from grammar */
    bdgm_table_build(gmr,
                     bdgm_sym_empty,
                     ret,
                     allocs,
                     logger);

    return ret;
}

/* bdgm_table_build" - generates the entries in the LR
 * table given in "table" from the grammar given in
 * "gmr", using the start state "start", such that when
 * each rule in the grammar is recognised ("reduced"),
 * it causes the generation of output, in the form of
 * output codes */
int bdgm_table_build(struct bdgm_grmr *gmr,
                     int start,
                     struct bdgm_table *table,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bd_map_node *items = NULL, /* set of */
                       *it;           /* items set */
                                      /* for "gmr"
                                       * and item
                                       * iterat-
                                       * or */
    int i = 0;                        /* state index
                                       * iterator */

    /* build the sets of "items" ... */
    if (!(build_item_sets(gmr,
                          &table->num,
                          &items,
                          start,
                          1,
                          allocs,
                          logger)))
        return 0;
    
    /* cycle "it" through the items in
     * "items" */
    for (it = bd_map_first(items);
                 it;
                 it = bd_map_next(it)) {

        /* initalise each of the state */
        table->states[i] = *init_state(table,
                                       gmr,
                                       allocs,
                                       logger);

        /* ... and set the entries in the state
         * corresponding to the current item
         * set */
        set_state_entries(table,
                          table->states + i,
                          items,
                          it,
                          gmr,
                          allocs,
                          logger);
       i++;
    }
    return 0;
}

/* "push" push the lexical token "tok" (used
 * by reduce callbacks) and parser state
 * "state" onto the top of the stack of
 * parser table "table", using the memory
 * allocator and error logger given in
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero oth-
 * erwise */
static int push(int state,
                struct bdlx_tok *tok,
                struct bdgm_table *table,
                struct bd_allocs *allocs,
                struct bd_logger *logger)

{
    struct bdgm_stack_item *old, /* previous */
                           *nw;  /* and new
                                  * stack items */

    old = table->stack;
                                   
    table->hgt++; /* increase stack height */
    
    /* set the token in the reduce
     * stack top */
    
    /* allocate a new ststack */
    nw = (struct bdgm_stack_item *)
        bd_alloc(allocs,
                  sizeof(struct
                         bdgm_stack_item),
                  logger);
    if (!(nw))
        return 0;

    /* set the token in the
     * reduce stack top */
    nw->tok = tok;

    /* set remaining fields */
    nw->state = state;
    nw->tok = tok;
    nw->prev = old;
    nw->next = NULL;
    old->next = nw;

    /* ... and reset table's
     * stack pointer */
    table->stack = nw;

    /*  */
    return 1;
}

/* "iterate" - using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively, opaque value in "opaque",
 * ans built output calue in "out", execute
 * a single iteration of the entries in the
 * grammar table in "table" generated from
 * the grammar "gmr" on input of the lexical
 * token in "tok".  Returns zero on error,
 * non-zero otherwise */
static int iterate(void *opaque,
                   void *out,
                   struct bdlx_tok *tok,
                   struct bdgm_grmr *gmr,
                   struct bdgm_table *table,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    int i,                            /* iterator, */
        curr = 0,                     /* number, current */
        nme_idx,                      /* state number and */ 
        ttype;                        /*  index and type
                                       * of token */
    struct bd_map_node *prod_node;    /* node of product-
                                       * ion to reduce */
    enum bdgm_actn_types atype;       /* current action
                                       * type */
    struct bdgm_action *action;       /* current action */
    struct bdgm_prod *prod;           /* production to
                                       * to reduce */
    union bdgm_reduce_item *new_rdcs; /* pointer to array
                                       * of reduce items
                                       * to call with
                                       * reduction call-
                                       * back */
    struct bdut_str syn_msg,          /* syntax, token */
                    nme_msg,          /*  name and form- */ 
                    fmtd_msg;         /* atted error
                                       * strings */

    /* initialise token's type */
    ttype = tok->type(tok,
                      allocs,
                      logger);

    /* get current action type "atype"
     * from action type in current
     * action entry of "table" */
    atype = table->states[
              table->curr].actions->type;

    /* ... so start by switching
     * on that action type ... */
    switch (atype) {

        case bdgm_actn_shift:

            /* het next rok type */
            ttype = tok->type(tok,
                              allocs,
                              logger);

                /* get next action from
                 * action table's current
                 * state and read symbol */
            action = table->states[table->curr].
                       actions;

            /* and push currently red
             * token and stack state
             * from value found in "goto"
             * table for current state
             * and last read token
             * onto stack */
            if (!(push(action->u.state,
                       tok,
                       table,
                       allocs,
                       logger)))
                return 0;
            break;

        /* current action was "reduce" */
    case bdgm_actn_reduce:

            /* get current rule */
            prod = table->states[table->curr].
            actions[ttype].u.prod;

            /* look-up the rule index
             * in the grammar */
            prod_node = bd_map_find(gmr->prods,
                                    &prod,
                                    (bd_map_find_fn)
                                         cmp_prods);
            /* get "prod"'s key (a prod-
             * uction) */
            prod = (struct bdgm_prod *)
                       prod_node->key;

            /* pop that number of
             * items off stack ... */
            for (i = 0; i < prod->num; i++)
                table->stack =
                    table->stack->prev;

            /* allocate the array storage ... */
            new_rdcs = (union bdgm_reduce_item *)
                     bd_alloc(allocs,
                              sizeof(union
                                     bdgm_reduce_item) *
                                          prod->num,
                               logger);
            if (!(new_rdcs))
                return 0;
                
            /* ... and set each item's token from
             * each scanned token, where the corr-
             * esponding symbol is a terminal (non-
             * terminals are left untouched - they
             * are modified  inside reduction call-
             * back) */
            for (i = 0; i < prod->num; i++)
                if (is_term(table->stack->type,
                            gmr)) {
                    new_rdcs[i].tok = tok;
                    table->stack =
                          table->stack->next;
                }

            /* call the production's
             * "reduce" callback using
             * our newly-allocated and set
             * educe item array */
            prod->reduce(opaque,
                         out,
                         new_rdcs,
                         prod->num,
                         allocs,
                         logger);

            /* set next state to the
             * "goto" table entry for
             * current state and
             * rule's lhs symbol,
             * then jump to that state */
            table->states[table->curr].gotos[
                       prod->lhs -
                       (gmr->num_terms +
                       gmr->num_nts)];
            break;

        case bdgm_actn_error: /* syntax
                               * error */

            /* get message for
             * syntax error */
            syn_msg = logger->get(
                   logger,
                   logger->opaque,
                   bdgm_log_syn);

            /* get message index for
             * erroneous token'd con-
             * tent name and get the
             * message for that name */
            if (nme_idx =
                   tok->name(tok,
                             1,
                             allocs,
                             logger) < 0)
                return 0;

            nme_msg = logger->get(
                       logger,
                       logger->opaque,
                       nme_idx);

            /* ... format the token
             * name into the syntax
             * messge */
            fmtd_msg = logger->fmt(
                        logger->opaque,
                        syn_msg,
                        nme_msg);

            /* and log that formatted
             * message */
            logger->log(logger->opaque,
                        fmtd_msg);
            break;
    }
    return 1;
}

/* "bdgm_table_run" - using the memory alloc-
 * ator and error logger given in "qallocs"
 * and "logger", respectively, processes the
 * entries in the LALR grammar table given
 * in "table", generated from the grammar in
 * "gmr",  corresponding to the input of the
 * lexical token given in "tok", genterating
 * the output specified in "out".  Returns
 * zero on error, non-zero otherwise */
int bdgm_table_run(void *opaque,
                   void *out,
                   struct bdlx_tok *tok,
                   struct bdgm_grmr *gmr,
                   struct bdgm_table *table,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    void *lhs = NULL;

    /* keep iterating table while
     * not in  accepting sttate
     * (that is, "not finished") */
    while (table->states[table->curr].
          actions[bdgm_sym_eos].type !=
          bdgm_actn_accept)
        if (!(iterate(opaque,
                      out,
                      tok,
                      gmr,
                      table,
                      allocs,
                      logger)))
            return 0;
    return 1;
}

/* "bdgm_init" - allocates, initialises and returns,
 * using the memory allocator and error logger given
 * in "allocs" and "logger", respectively, a grammar
 * having the number of tokens and number of symbols
 * given in "num_toks" and "num_syms", respectively.
 * Returns NULL on error */
struct bdgm_grmr *bdgm_init(int num_toks,
                            int num_syms,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    struct bdgm_grmr *ret; /* return value */

    /* allocate a grammar ... */
    ret = (struct bdgm_grmr *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdgm_grmr),
                 logger);
    if (!(ret))
        return NULL;

    /* bow allocation was successful,
     * set its fields, and initalise
     * its production set to empty */
    ret->num_terms = num_toks;
    ret->num_nts   = ret->num_terms -
                          num_toks;
    ret->prods = NULL;
    return ret;
}
