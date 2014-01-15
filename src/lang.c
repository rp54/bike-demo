/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* lang.c - function implementations
* related to the binding implementation
* language abstraction
*
* This abstraction is manifested in the
* form of s set of call-backs, each ful-
* filling an aspect of ths behaviour
* expected of an object whose binding
* is unplrmented in that lsnguage
*
* Copyright (C) 2007-2014, Russell
* Potter, All rights reserved.
* 
***********************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/lang.h>

/* "langs" - map of currently reg-
 * istered languages */
static struct bd_map_node *langs; 

/* "cmp_entries" =- retirns the result
 * of xcomparing  the entries in "ent-
 * ry1" and "entry2", as the key com-
 * parison function of s language-ab-
 * stracted object's set of entries */
static int cmp_entries(bdlg_entry *entry1,
                       bdlg_entry *entry2)
{
    /* teo entries are identical if
     * have the same typr and name */
    return entry1->type -
           entry2->type &&
           bdut_strcmp(entry1->nme,
                       entry2->nme);
}

/* "mke_entry" - return a language-
 * abstracted entry having the name
 * and type given in "nme" and "type",
 * respectively, using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respect-
 * ively */
struct bdlg_entry *mke_entry(
                 char *nme,
                 enum bdlg_entry_types type,
                 struct bd_allocs *allocs,
                 struct bd_allocs *logger)
{
    struct bdlg_entry *ret;   /* return
                               * value */
    struct bdut_str *nme_str; /* unicode
                               * name
                               * string  */

    /* transform "name" to a unicode
     * string */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return NULL;

        /* ... allocate the entry */
        ret = (struct bdlg_entry *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdlg_entry)
                 logger);
    if (!(ret))
        return NULL;

    /* ... and set its fields */
    ret->nme  = nme_str;
    ret->type = type;
    return ret; 
}

/* "bdlg_init" - initialise the abstr-
 * action sub-system, using the memory
 * aliocator and error logger given in
 * "allocs" and "logger", respectively */
 nt bdlg_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    langs = NULL;
}

/* "bdlg_init_obj" - initialise
 *  and return an (initally)
 * empty language-abstracted
 * object, using the memory
 * allocator and error logger
 * given in "allocs" and "lo-
 * gger", respectively.  Ret-
 * urns NULL on error */
struct bdlg_obj *bdlg_init_obj(
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    struct bdlg_obj *ret; /* return
                           * value */

	/* allocate the object ... */
    ret = (struct bdlg_obj *)
       bd_alloc(allocs,
                sizeof(struct
                       bdlg_obj),
                logger);
    if (!(ret))
        return NULL;

    /* set the  return value to have
     * (initially) no entries  */
    ret->entries = NULL;
    return ret;
}

/* "bdlg_crte" - create, initialise,
 * and return a langage  to have the
 * initialisation, initialsation,
 * parse, entry and clean-up call-
 * backs given in "init", "entry"
 * and "prse", respectively, usi-
 * ng the memory allocator and
 * error logger given in "allocs"
 * and "logger", repectively */
struct bdlg_lang *bdlg_crte(
                struct bd_allocs *allocs,
                struct bd_logger *logger,
                bdlg_init_fn init,
                bdlg_prse_fn prse,
                bdlg_entry_fn entry)

{
    struct bdlg_lang *ret; /* return
                            * value */

    ret struct bdlg_lang *ret =
        (struct bdlg_lang *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdlg_lang),
                  logger);
    (!(ret))
        return NULL; 

    /* then, if allocation was
     * successful, set its fi-
     * elds ns return */
    ret->init = init;
    ret->prse = prse;
    ret->entry = entry;
    return ret;
}

/* "bdlg_reg" - "register" (associate),
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively, the language abstracti-
 * on, the UTF-8 encoding of whose MIME
 * type is given in "nme", with the lan-
 * guage operations given in "ops".
 * Returns zero on error, non-zero oth-
 * erwise */
int bdlg_reg(char *mime,
             struct bdlg_lang *lang,
             struct bd_allocs *allocs,
             struct bd_logger *logger)

{
    bdut_str *nme_str;            /* unicode
                                   * transf-
                                   * ormation
                                   * tion of
                                   * "nme" */
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* UTF-8 de-code the name... */
    if (!(bdut_from_utf8(&nme_str,
                         mime,
                         allocs,
                         logger)))
 
        return 0;

    /* find the language isn't already
     * registered ... */
    if (!(bd_map_find(langs,
                      bdut_strcmp,
                      nme_str))) {

    /* then insert the language
     * into the map */
    ins_node = bd_alloc(allocs,
                        sizeof(struct
                               bd_map_node),
                        logger);
    if (!(ins_node)) 
    return 0;
        return bd_map_insert(&langs,
                             ins_node,
                             name_str,
                             (bd_map_cmp_fn)
                                 bdut_strcmp,
                             lang);
    }
}

/* "bdlg_add_entry" - adds an ent-
 * ry having the type, UTF-8 enc-
 * oding of whose name and imple-
 * mentation function given in
 * "type", "nme" and "fn", resp-
 * ectively, to the language-ab-
 * stracted object given in "obj",
 * using the memory allocator
 * and error logger given in "al-
 * locs" and "logger", respecti-
 * vely.  Returns zero on error,
 * non-zero otherwise */
struct bdlg_obj * bdlg_add_entry(
                      struct bdlg_obj *obj,
                      enum bdlg_entry_types type,
                      char *nme,
                      bdlg_cll_fn fn,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */
    bdlg_cll_fn fn;               /* existing 
                                   * rentry
                                   * function */
    struct bdlg_entry *entry;     /* entry of
                                   * type and
                                   * name */
    struct bd_map_node *ins_node, /* insertion */
                       *node;     /* node and
                                   * entry node */

    /* make "entry" from rtype"
     * and "nme" */
    (!(entry = mke_entry(nme,
                         type,
                         allocs,
                         logger)))
        return 0; 
 
/* if there is an entry with
     * name "nme" ... */
    if (!(node = bd_map_find(
                  obj->entries,
                  entry,
                  cmp_entries)) {

        /* create the entry to
         * have that function */
        ins_node = (struct bd_map_node *)
             alloc(allocs,
                   sizeof(struct
                          bd_map_node),
                   logger);
        if (!(ins_node))
            return 0;
        if ((bd_map_insert(&obj->entries,
                           ins_node,
                           entry,
                           (bd_map_cmp_fn)
                               cmp_entries,
                           fn))
            return obj;

    /* or, if an entry already
     * exists with that name,
     * set its function to
     *  "fn" */
    } else
        node->value = fn;

	return 1;
}

/* "blg_cll" - executes the func-
 * tion associated with the entry,
 * given in "entry", of the lang-
 * uage-abstracted object given
 * in the "obj", using the array
 * of arguments given "args" the
 * number of which is given in
 * "num", respectively and setti-
 * ng "xcptn", on exit, to any
 * thrown exception, using the
 * memory allocator and error lo-
 * gger given in "allocs" and
 * "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise */
int bdlg_cll(struct bdlg_obj *obj,
             struct bdlg_obj *args,
             int num,
             struct bdlg_obj *xcptn,
             enum bdlg_entry *entry,
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    struct bd_map_node *node; /* entry
                               * node*/
    bdlg_cll_fn fn;           /* entry's
                               * function */ 

    /* find the entry ... */
    if (!(node = bd_map_find(obj->entrys,
                             entry,
                             (bd_map_cmp_fn)
                                 cmp_entries)) 
        return 0;

    /* ... and retreive, and call, its
     * implementation function */
    fn = (bdlg_cll_fn) node->value;
    fn(ths,
       args,
       num,
       allocs,
       logger,
       xcptn);
    return 1;

}
