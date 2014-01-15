 /********************************************************************
 *
 * BikeDemo - exercise bike demonstration scenery 
 *
 * map.h - type declarations and function proto-types
 * for mapping a key of any type to a value, also of
 * any type.
 *
 * This implementation usees an AVL (Addison, Veleski
 * and Landis) tree, in which, after the tree moves
 * to an "unbalanced" state (where a "balanced" tree
 * gives the tree its desirable O(log N) search per-
 * formance) due to a series of insertions or dele-
 * tions on the tree, "near" balance is returned to
 * the tree.
 *
 * "Near" balance, here, is a condition in which the
 * left and right sub-trees of a node may differ in
 * height by one without triggering re-configuration)
 *
 * These re-configurations between nodes of nodes in
 * the tree bring the node's balance back to at most
 * one
 *
 * this maintenance of "near" (rather than "exact")
 * balance allows for much faster insertions and del-
 * etions
 *
 * Copyright (C) 2006-2012, Russell Potter, All rights
 * reserved
 *
 ********************************************************************/

#ifndef __BD_MAP_H
#define __BD_MAP_H

#ifdef __cplusplus
extern "C" {
#endif


/* "bd_map_cmp_fn" call-back function - sets the
 * order between nodes in the tree.  When two nodes
 * are compared, the function is called using two
 * two opaque values (one from each of the nodes be-
 * ing compared).  If the return value of the fun-
 * ction is less than zero, the first node should
 * precede second in the map, while if the return
 * value is greater than zero the second should
 * precede the first. A return value of zero ind-
 * icates the nodes are equal */
typedef int (* bd_map_cmp_fn)(void *,  /* first item */
                              void *); /* second item */

/* "bd_find_fn" callback function - returns the re-
 * sult of a comparison between a datum and the key
 * of a map node. used when finding the datum in a
 * map as a part o the "bd_map_find" function.  Each
 * node is compared to the datum and rthe result of
 * the comparison dictates whether the node's left
 * or right child will then be compared.  If the node
 * is less than the datum, a negative result will
 * occur.  If the node is greateer than the datum,
 * a positive result will be returned. A zero will
 * be returned if the two are equal */
typedef int (* bd_map_find_fn)(void *,  /* datum */
                               void *); /* node key */

/* "bd_map_side" enumeration - an indication of the side
 * of a node on which a child is located */
enum bd_map_side {

    bd_map_left_side,
    bd_map_right_side

};

/* "bd_map_balance" enumeration - possible types of
 * balance in a node */
enum bd_map_balance {

    bd_map_left_overbalanced  = -2,
                        /* will require
                         * re-balance */
    bd_map_left_heavy         = -1,
    bd_map_balanced           = 0,
    bd_map_right_heavy        = 1,
    bd_map_right_overbalanced = 2
                        /* will require
                         * re-balance */

};

/* - = "bd_map_node" structure - a single node in a map,*/
struct bd_map_node {

    void *key, *value;           /* key and value
                                  * of node */
    enum bd_map_balance balance; /* difference between
                                  * left and right sub-
                                  * tree heights */
    union { /* union containing the node's child pointers in either
             * array or field notation, so the children may be
             * addressed either by index or name, depending on
             * the context */
    
        struct {
            struct bd_map_node *children[2]; /* array
                                              * notation */
        } array;
        struct {
            struct bd_map_node *left, *right; /* field
                                               * notation */
        } fields;
    
    } u;

    struct bd_map_node *parent; /* the next node "up"
                                 * in the tree */
};

/***  function proto-types ***/

/* "bd_map_insert" - inserts the node given in the
 * second parameter, having the key and value given
 * map given in the third and fifth parameters, re-
 * spectively, into the map given in the second
 * parameter which is ordered according to the res-
 * ult of calls to the key comparison function giv-
 * en in the fourth parameter.  Returns zero on er-
 * ror, non-zero otherwise */
int bd_map_insert(struct bd_map_node **,
                  struct bd_map_node *,
                  void *,
                  bd_map_cmp_fn,
                  void *);

/* "bd_map_remove" -  removes the node given in the
 * second parameter from the map given in the first
 * parameter.  Returns zero on error, non-zero oth-
 * erwise */
int bd_map_remove(struct bd_map_node **,
                  struct bd_map_node *);

/* "bd_map_find" - returns the node containing the
 * value given in the second parameter, as found
 * in the map given in the first parameter.  Retu-
 * trns NULL if not found */
struct bd_map_node *bd_map_find(struct bd_map_node *, 
                                void *,
                                bd_map_find_fn);

/* "bd_map_first" - returns the first node of the
 * map given in the first parameter, or NULL if
 * map is empty */
struct bd_map_node *bd_map_first(struct bd_map_node *); 

/* "bd_map_last" - returns the last node of the
 * map given in the first parameter, or NULL if
 * map is empty*/
struct bd_map_node *bd_map_last(struct bd_map_node *);

/* "bd_map_next" - returns the node after of the node
 * given in the first parameter, or NULL if node is
 * is the last in its map */
struct bd_map_node *bd_map_next(struct bd_map_node *); 

/* "bd_map_prev" - returns the node previous to the
 * node given in the first parameter, or NULL if node
 * is the first in its map */
struct bd_map_node *bd_map_prev(struct bd_map_node *);

#ifdef __cplusplus
}
#endif

#endif
