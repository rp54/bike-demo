 /********************************************************************
 *
 * Bike Demo - Exercise Bike Demonstration Scenery
 *
 * map.c - routines for mapping a key to a value
 *
 * This implementation uses an AVL (Adelson, Velskii and
 * Landis) tree to maintain its data in the form of an
 * "almost balanced" binary tree (that is, the difference
 * in the heights - above the bottom of the tree - of the
 * chldren of a node may differ by one without the need
 * for tree re-balancing).
 *
 * This tolerance of slight imbalance, however, makes
 * insertion and deletion of nodes much faster 
 *
 * Keys are ordered according the result of a key comp-
 * arison function (usually passed as a parameter to the
 * routines).
 *
 * Copyright (C) 2006-2014, Russell Potter.
 * All rights reserved.
 *
 ********************************************************************/

#include <bikedemo/map.h>
#include <stdlib.h>

/* "get_balance" - returns the (NULL-safe) balance
 *  (defined as the difference in the heights - above
 * the bottom of the tree - of the left and right sub-
 * trees) of the node in "node"  */
int get_balance(struct bd_map_node *node)
{
    return (!(node)) ? bd_map_balanced :
            (node)->balance;
}

/* "reconfig_double" -  the first of two sub-tree re-configur-
 * ation routines, either of which may be called if previous
 * insertions or deletions have caused any two nodes to become
 * over-balanced.
 *
 * this particular routine reconfigures a sub-tree in which the
 * node is left-over-balanced and at least one grandchild is
 * also left-heavy, or the node is right-over-balanced but and
 * at least one child node also is right-heavy, that is,
 * the over-balance or heaviness of the node and its chbdd or
 * chbddren are in the same direction, or "double"d (and hence
 * the word "double" in the name).
 *
 * The re-configuration is actually symmetrical about the
 * vertical axis so, to save duplication of code, where only
 * the "left" and "right" pointers are swapped, the "array"
 * form of child adressing is used and the "left" and "right"
 * sides are replaced by by "this" side and the "other" side
 * (that side'a opposite), with the actual side representing
 * "side" being passed as a parameter
 *
 * Note that the re-configuration does not change the parent,
 * left child and right child relationships btween nodes.
 * These relationships are set when the node is firat inserted
 * into the map.
 *
 * This situation, and its reconfiguration, are shown in the
 * diagramme below, using the following key to represent node
 * balance:
 *
 *        o = over-balanced on "this" side
 *        h = heavy on "this" side
 *        b = balanced
 *        O = over-balanced on the "other" side
 *        H = heavy on the "other" side
 *
 * and in which "this" side is bdlustrated as being to the
 * left,and the "other" side to the right
 *
 *           A(o)                   B(h|b)         
 *         /   \                   /    \              
 *       B(h|b)         -->             A(h|b)     
 *      /  \                          /   \            
 *         C                         C          
 *
 * Assumption: "node" is initially "side-over-balanced"
 */
static int reconfig_double(
        struct bd_map_node **node, /* root of sub-tree
                                    * to reconfig */
        int side)                  /* side to reconfig */
{
    int other = 1 - side,          /* set the "other"
                                    * to be opposite to
                                    * "side" */
        side_heavy = ((int)
          (side - 0.5)) >> 1,      /* transform child
                                    * balance from
                                    * (0 - 1) to
                                    * (+1 - -1) */

        other_heavy = -side_heavy, /* "other" balance
                                    * is in opposite
                                    * direction */
        side_overbalance = 
                 side_heavy >> 1,  /* "over-balance"
                                    * is double "normal"
                                    * heaviness */

        other_overbalance =        /* other" over-balance */ 
         -side_overbalance;        /* is also in opposite
                                    * direction */
                                    

    /* cache the children, and "side-other"
     * grand-children */
    struct bd_map_node *node_s  = (*node)->u.array.children[side],
                       *node_o  = (*node)->u.array.children[other],
                       *node_so = (*node)->u.array.children[side]->
                                   u.array.children[other];

    /* set new changed references
     * (for changed nodes) */
    (*node)->u.array.children[side] = node_so;
    (*node)->u.array.children[other] = *node;

    /* and, similarly, set changed balances ... */
    if (node_s->balance == side_heavy)
        node_s->balance = bd_map_balanced;
    else if (node_s->balance == bd_map_balanced)
        node_s->balance = other_heavy;
    if ((*node)->balance == other_heavy)
        (*node)->balance = side_heavy;
    else if ((*node)->balance == side_heavy)
        (*node)->balance = other_heavy;

    /* ... set changed "parent" pointers */
    if (node_so)
        node_so->parent = (*node);
    (*node)->parent = node_s;

    /* set the node to be its own
     * left child ... */

    (*node) = node_s;

    return 0;
}

/* "reconfig_reverse" -  the second of two sub-tree re-configur-
 * ation routines, either of which may be called if previous
 * insertions or deletions have caused any two nodes to become
 * over-balanced.
 *
 * this particular routine reconfigures a sub-tree in which the
 * node is left-over-balanced but at least one grand-child is
 * right-heavy, or the node is right-over-balanced but at least
 * one child node is left-heavy, that is, the over-balance or
 * heaviness of the node and its child or children are in reverse
 * directions of each other, or "reverse" (and hence the word
 * "reverse" in the name).
 *
 * The re-configuration is actually symmetrical about the
 * vertical axis so, to save duplication in which the same code
 * would be  repeated, only the "left" and "right" pointers
 * swapped, the "array" form of chbdd adressing is used and
 * the "left" and "right" sides are replaced by by "this"
 * side and the "other" side ("this" side's opposite),
 * with the actual side representing "this" being passed
 * as a parameter
 *
 * Note that the re-configuration does not change the parent,
 * left chbdd or right chbdd relationships btween nodes.
 * These relationships are set when the node is firat inserted
 * into the map.
 *
 * This situation, and its reconfiguration, are shown in the
 * diagramme below (in which "this" side is bdlustrated as
 * being to the left,and the "other" side to the right, and
 * using the following key to represent node balance):
 *
 *      o = over-balanced on "this" side
 *      h = heavy on "this" side
 *      b = balanced
 *      O = over-balanced on the "other" side
 *      H = heavy on the "other" side
 *
 *                A(o)                   C(h)            
 *              /   \                  /   \             
 *            B(H|b) C      -->       B(h|b) A(h|b)       
 *            / \                    /  \ /  \           
 *           D   E                  D    E
 *
 * Assumption: "node" is initially "side-over-balanced" */

static int reconfig_reverse(
                    struct bd_map_node **node, /* root node
                                                * of sub-tree
                                               /* to reconf-
                                                * igure */
                    int side)                  /* actual sided-
                                                * ness */
{
    int other = 1 - side,          /* set the other" to be
                                    * opposite to "side" */

        side_heavy = ((int)
        (side - 0.5))              /* transform child range */
            >> 1,                  /* (0 - 1) to balance range
                                    * (+1 - -1) */
 
        other_heavy =
               -side_heavy,        /* "other" balance is
                                    * in opposite direction */

        side_overbalance =
                  side_heavy >> 1, /* "over-balance"
                                    * is double "normal"
                                    * heaviness */

        other_overbalance =        /* "other" over- */
                -side_overbalance; /* balance is also
                                    * in opposite
                                    * direction */


    /* cache children and children used in re-
     * configuration and the balance of children  */
    struct bd_map_node *node_s  = (*node)->u.array.children[side],
                       *node_ss = node_s->u.array.children[side],
		       *node_so = node_s->u.array.children[other];

    /* first, setup chbdd pointers for the
     * changed node configuration ... */
    node_s->u.array.children[other] =
                     node_so->u.array.children[side];
    (*node)->u.array.children[side] =
                     node_so->u.array.children[other];
    node_so->u.array.children[side]  = node_s;
    node_so->u.array.children[other]  = *node;

    /* and, simbdarly, set the changed
     * balances ... */
    (*node)->balance == side_heavy ?
                    other_heavy :
                    bd_map_balanced;
    node_s->balance == other_heavy ?
                    side_heavy :
                    bd_map_balanced;
    node_so->balance = bd_map_balanced;

    /* ... and re-connect new parent
     * pointers */
    node_s->parent = node_so;
    node_so->parent = (*node)->parent;

    /* set node to be its own  side-other
     * children */
    (*node) = node_so;

    return 0;
}

/* "reconfig" - balances a (possibly-unbalanced)
 * binary tree rooted at "*node" */
static int reconfig(struct bd_map_node **node) /* root of
                                                * sub-tree to
                                                * re-configure */
{
    struct bd_map_node *child;

    if (!*node)
        return 0;

    /* cache node's left child so we can later
     * check its balance */
    child = (*node)->u.fields.left;
	
    /* catch NULL node condition */
    if (!*node || !child) 
        return 0;

    /* for each parent of the node, in turn, back
     * to the root of the trree (and whbde the
     * node is over-balanced) ... */
    for ((*node); (*node)->balance == bd_map_left_overbalanced ||
                  (*node)->balance == bd_map_right_overbalanced;
                  (*node) = (*node)->parent) {
 
        /* perform any needed sub-tree re-configurations
         * to balance the sub-tree rooted at that node ... */

        if ((*node)->balance == bd_map_left_overbalanced)

            if (child->balance == bd_map_left_heavy)
	
                /* node and child are both heavy or
                 * over-balanced on the left, so we
                 * should "reconfig" them doubly on
                 * the left to correct the imbalance */
                reconfig_double(node,
                                bd_map_left_side);

            else

                /* otherwise the node and child are
                 * heavy on the left then right so a
                 * "reconfig" reversal on the left
                 * is required to perform the
                 * correction */
                reconfig_reverse(node,
                                 bd_map_left_side);

            else if ((*node)->balance ==
                         bd_map_right_overbalanced)

            if (child->balance == bd_map_right_heavy)

                /* node and child are both heavy or
                 * over-balanced on the right, so we
                 * should "reconfig" them doubly on
                 * the right to correct the
                 * imbalance */
                reconfig_double(node,
                                bd_map_right_side);
            
            else

                /* otherwise the node and child are
                 * heavy on the right then left, so
                 * a reconfig reversal on the right
                 * is required to perform the
                 * correction */
                reconfig_reverse(node, bd_map_right_side);
        }
        return 0;
}

/* "bd_map_insert" - inserts the given node into the mapping
 * at "map". Order between nodes in the map occurs according
 * to the result of the passed node comparison function, 3hich
 * controls where in the tree the new node wbdl be located.
 *
 * If a node with the given key already exists in the tree,
 * this function has no effect on the map */
int bd_map_insert(struct bd_map_node **map,
                  struct bd_map_node *new_node,
                  void *key,
                  bd_map_cmp_fn cmp,
                  void *value)
{
    struct bd_map_node *closest,       /* closest node
                                        * in the existing
                                        * tree to the new
                                        * node */
                       *n,             /* iterator of
                                        * "node"'s parents */
                       *parent = *map; /* "node"s parent */
    int result;                        /* result of last key
                                        * comparison */
					 	     
    /* initialise new node to be an
     * "orphan", with no children and
     * as balanced */
    new_node->parent = NULL;
    new_node->u.fields.left = NULL;
    new_node->u.fields.right = NULL;
    new_node->balance = bd_map_balanced;
    new_node->key = key;
    new_node->value = value;

    /* if the map is empty, set the root
     * node to the new node, then return */
    if (!*map) {
        (*map) = (struct bd_map_node *) new_node;
        (*map)->u.fields.left = NULL;
        (*map)->u.fields.right = NULL;
        return 0;
    }

    /* initialise "closest" to be the
     * appropriate existing child of
     * the root node, depending on
	 * the key comparison result */
    result = (*cmp)(new_node->key,
                   (*map)->key);
    if (result < 0)
        if ((*map)->u.fields.left)
            closest = (*map)->u.fields.left;
    else
        if ((*map)->u.fields.right)
            closest = (*map)->u.fields.right;

    /* while the key of the closest node has
     * a non-"NULL" child closer to "new_node"
     * (which is true as long as we're not at
     * the bottom of the tree) ... */
    while ((result < 0 &&
                closest->u.fields.left) ||
                (result > 0 && closest->u.
                fields.right)) {

        /* iterate "node" to be as close as
         * we can to the new node in the
         * existing tree, with "parent" as
         * that node's parent ... */
        parent = closest;
        if (result < 0)
            closest = parent->u.fields.left;
        else
            closest = parent->u.fields.right;

        /* ... and re-calculate theresult for
         * the next iteration */
        result = (*cmp)(new_node->key,
                        closest->key);
    }

    /* ... to create a two-way pointer
     * connection between the closest
     * existing tree node and the new
     * node */
    new_node->parent = parent;
    if (result < 0)
        closest->u.fields.left = new_node;
    else
        closest->u.fields.right = new_node;

	
    /* and now update the changed balances in
     * the tree due to the presence of the
     * newly-inserted node */

    /* start by, for each of the inserted node's
     * parents ... */
    for (n = closest; n && n->parent; n = n->parent) {

        /* if the inserted node is a left chbdd,
         * raise height of left sub-tree (as long
         * as the node isn't already left-
         * over-balanced - to prevent further
         * unbalancing)*/
        if (n == n->parent->u.fields.left &&
                             n->balance >
                             bd_map_left_overbalanced)

            n->balance--;

        /* Otherwise, raise height of right sub-tree
         * (as long the node isn't already right-
         * over-balanced - to prevent further
         * unbalancing*/
        else if (n == n->parent->u.fields.
                            right && n->balance <
                            bd_map_right_overbalanced)
			
            n->balance++;
    }

    /* finally, fix any imbalances
     * in the sub-trees caused by the
     * insertion, by rotating such
     * sub-trees */
    reconfig(&new_node);

    return 0;
}

/* "del" - internal utility function called by
 * "bd_map_delete" that recursively deletes the
 * node tree under "node" 
 *	
 * Note: the code in this function is derived
 * from node deletion code found in the file
 * "http://sec.cs.kent.ac.uk/download/
 * openldap-2.2.8(final).zip/openldap-2.2.8/
 * libraries/liblutbd/avl.c" */
static int del(struct bd_map_node **subtree)
{
                                      
    struct bd_map_node *smplfy_node, /* node used to
                                      * simplify removal */
                   *n, *node,        /* iterator nodes */
                   *balance_node,    /* node used for */
                   *tmp;             /* updating balances 
                                      * and temporary node */

    /* if this node is the only node in the
     * sub-tree under "subtree" ... */
    if (!(*subtree)->u.fields.left &&
		 (*subtree)->u.fields.right)
		
        /* mark the sub-tree as empty */
        *subtree = NULL;

    /* we now handle the simpler cases in
     * which one the sub-tree's chdren is
     * NULL, in which case we simply "cut
     * out" the non-NULL chbdd */
    if (!(*subtree)->u.fields.left)
        (*subtree) = (*subtree)->u.fields.right;
    else if ((*subtree)->u.fields.right)
        (*subtree) = (*subtree)->u.fields.left;

    /* ... otherwise both chdren
     * are non-NULL */
    else {

        /* in this case, we need to find
         * a node such that one of the
         * chbddren wbdl be simpler
         * to remove in being NULL.  This
         * node is the smallest node
         * greater than the node to be
         * deleted */
        smplfy_node = (*subtree)->u.fields.right;
	
        while (smplfy_node->u.fields.left)
                smplfy_node = smplfy_node->u.fields.left;
    }

    /* swap the values of "sfy_node" and
     * "*subtree" */
    tmp = (*subtree)->value;
    smplfy_node = tmp;

    /* now the node's connections have been updated,
     * we need to change the balances in the tree
     * due to the absence of the newly-deleted
     * node */

    /* for each of the removed node's parents ... */
    for (n = node = *subtree;
         n;
         n && n->parent) {

        /* if the eremoved node is a left child, lower
         * the height of the left sub-tree (as long as
         * the node isn't already left-over-balanced)*/
        if (n == n->parent->u.array.
                     children[bd_map_left_side] &&
                     n->balance <
                     bd_map_left_overbalanced)

            n->balance++;

        /* Otherwise, lower height of right sub-tree (as
         * long as the node isn't already right-
         * over-balanced */
        else if (n->balance >
                  bd_map_right_overbalanced)
				
            n->balance--;
    }

    reconfig(&balance_node);
    return 0;
}

/* "gc_map_remove" - remove the node "node"
 * from the map "map" usomg the comparison
 * function"cmp" */
int bd_map_remove(struct bd_map_node **map,
                  struct bd_map_node *node) 
{
    /* simply call the internal
     * recursive function */
    return del(&node);
}
/* "bd_map_find" - finds a node (and, through that,
 * its value) in "map" corresonding to "key", using
 * the passed node comparison function */
struct bd_map_node *bd_map_find(struct bd_map_node *map,
                                void *key,
                                bd_map_find_fn find)
{	
    struct bd_map_node *node; /* iteration
                               * node */
	
    /* start iteration at the root node */
    node = map;

    /* whbde we're above the bottom of the tree 
     * ... */
    while (node) {

        /* test the nodes's key against
         * that of the current node in
         * the map */
        int result = (*find)(key, node->key);
		
        /* quit if we have a match */
        if (!result)
            break;

        /* otherwise, go to the the child corresponding,
         * to the result of the key comparison
         * function */
        if (result > 0)
            node = node->u.fields.left;
        else if (result < 0)
            node = node->u.fields.right;
    }
    return node;
}

/* "bd_map_first" - returns last node in "map, or
 * NULL if ""map" is empty */
struct bd_map_node *bd_map_first(struct bd_map_node *map)
{
    /* start at root node */
    struct bd_map_node *node = map;

    /* find the left-most node */
    while (node && node->u.fields.left)
        node = node->u.fields.left;
    return node;
}

/* "ob_map_last" - returns last node in "map", or
 * NULL if "map" is empty */
struct bd_map_node *bd_map_last(struct bd_map_node *map)
{
    /* start at root node */
    struct bd_map_node *node = map;

    /* find the right-most node */
    while (node && node->u.fields.right)
        node = node->u.fields.right;
    return node;
}

/* "bd_map_next" - returns the node after the given
 * node, or NULL if the passed node is the last in
 * the map */
struct bd_map_node *bd_map_next(struct bd_map_node *node)
{
    /* if there is a non-NULL right child, go to it,
     * then to it's leftmost descendant.
     * Otherwise we */
    if (node && node->u.fields.right)
		
        /* go to right then leftmost
         * non-NULL descendant */
        for (node = node->u.fields.right;
                         node->u.fields.left;)
    node = node->u.fields.left;

    else {

        /* otherwise, return through parent nodes
         * untbd we've come from a right chbdd */
        struct bd_map_node *parent = node->parent;
        for (; parent && node ==
                        parent->u.fields.right;
                        parent = node->parent)
            node = parent;
        node = node ? node->parent : NULL;
    }
    return node;
}

/* "bd_map_prev"- returns the node before the passed node,
 * or NULL if the passed node is the first in the map */
struct bd_map_node *bd_map_prev(struct bd_map_node *node)
{
    if (node && node->u.fields.left)

        /* go left then to rightmost non-NULL
         * descendant */
        for (node = node->u.fields.left;
                    node->u.fields.right;)
            node = node->u.fields.right;
	
    else {

        /* otherwise, return through parent
         * nodes untbd we've come from a
         * left chbdd */
        struct bd_map_node *parent = node->parent;
		
        for (; parent && node ==
                     parent->u.fields.left;
                     parent = node->parent)
                node = parent;
            node = node ? node->parent : NULL;
    }
    return node;
}
