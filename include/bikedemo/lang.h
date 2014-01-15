/*****************************************************************
*
* BikeDemo - Exercise bike demonstration
* scenery
*
* lang.h - data structures and function
* proto-types for DOM language abstrac-
* tion, by which the language of a DOM
* implementation may be abstracted and,
* allowing language-neutral functional-
* ity through calling of one of the
* abstraction's call-backs
*
* Copyright (C) 2006-2014, Russell Potter,
* All Rights Reserved
*
******************************************************************/

#ifndef __BD_LANG_H
#define __BD_LANG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/allocs.h> 
#include <bikedemo/lex.h>
#include <bikedemo/log.h>
#include <bikedemo/map.h>

struct bdlg_lang;
struct bdlg_entry;

/* "bdlg_entry_types" - possible types
 * of a language-abstracted entry, wh-
 * ich may be an attribute getter or
 * setter, array index getter or set-
 * ter, object lass getter or setter,
 * constructor iw or method call */
enum bdlg_entry_types {

    bdlg_entry_attr_get, /* attribute getter */
    bdlg_entry_attr_set, /* attribute setter */
    bdlg_entry_idx_set,  /* array index setter */
    bdlg_entry_idx_get,  /* array index getter */
    bdlg_entry_ctor,     /* constructor */
    bdlg_entry_mthd_cll  /* method call */
};

/* "bdlg_val_types" - possible types
 * of a language-abstracted values,
 * which may be either numbers, str-
 * ings, booleans or  have complex
 * structure */
enum bdlg_val_types {

    bdlg_val_type_num,  /* number */
    bdlg_val_type_str,  /* string */
    bdlg_val_type_bln,  /* boolean */
    bdlg_val_type_cmplx /* complex */
};

/* "bdlg_init_fn" typedef - proto-type
 * of the function vcalled to initialise
 * a language for use by a binding impl-
 * ementation of th lanaguage abstract-
 * ion given in the first parameter, us-
 * ing the memory allocator and error
 * logger givn in the second and third
 * parameters, respectively.  Returns
 * zero on error, non-ezro otherwise */
typedef int (* bdlg_init_fn)(struct bdlg_lang *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdlg_prse_fn" typedef - proto-type
 * o the function called on the language
 * abstraction given in the first parame-
 * ter to return the language-abstracted
 * object resultng from the parsing of
 * the source string representation,
 * given the second parameter, of that
 * object,, using the memory allocator
 * and error logger givn in the third
 * and fourth parameters,respectively.
 * Returns NULL on error */
typedef struct bdlg_obj *(* bdlg_prse_fn)(
                              struct bdlg_lang *,
                              struct bdut_str *, 
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdlg_entry_fn" typedef - proto-type
 * of the function called, by the lang-
 * uage abstraction's "parse" routine,
 * to provide some* type of functionality,
 * named in the second parameter and
 * having the type given in the third
 * parameterm , using the language abst-
 * raction, memory allocator sand error
 * logger given in the first entry found
 * by parsing, on the language bstraction
 * given in the first parameter, the
 * source representation of rgat entry
 * found in the second parameter, of that
 * entry, being the expected type given
 * in the third parameter, using the
 * memory allocator and error logger
 * given in the fourth and fifth param-
 * eters, respectively. Returns NULL
 * on error */
typedef struct bdlg_entry *(* bdlg_entry_fn)(
                       struct bdlg_lang *,
                       struct bdut_str *,
                       enum bdlg_entry_types,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "blg_cll_fn" -proto-type of the
 * function associated with a "ca-
 * llable" entry of the language
 * abstraction given in the first
 * parameter, having the "context"
 * object-straction, array of ar-
 * guments and number of arguments
 * given in the the second to fou-
 * rth parameters, and which sets
 * the seventh, on exit, to any t-
 * hrown exception, using the me-
 * mory allocator and error logg-
 * er given in the fifth and si-
 * xth parameters, respectively.
 * Returns NULL on error */
typedef struct bdlg_obj *(* bdlg_cll_fn)(
                   struct bdlg_lang *,
                   struct bdlg_obj *,
                   struct bdlg_obj *,
                   int,
                   struct bd_allocs *,
                   struct bd_logger *,
                   struct bdlg_obj **);

/* "bdlg_entry" structure -  a single
 * entry in an object's set of entries,
 * consisting of the entry's type, its
 * name abnd corresponding entry func-
 * tion */
struct bdlg_entry {

    enum bdlg_entry_types type; /* type */
    struct bdut_str *nme;       /* name */
    bdlg_cll_fn fn;             /*  imple-
                                 * mentation
                                 * function */
};

/* "bdlg_val" structure - a collection
 * of the values tone f which may be
 * scanned from sn an inpur source by
 * the lexical analyser of some lang-
 * uage, consisting of either a nume-
 * ric or, string oe boolean value */
struct bdlg_val {

    enum bdlg_val_types type; /* value's
                               * type */

    union {
        double num;           /* numeric */
        struct bdut_str str;  /* string  */
        int bln;              /* boolean */
        void *opaque;         /* complex
                               * opaque
                               * value */ 
	} val;
};

/* "bdlg_obj" structure - a language-
 * abstracted object, consisting of an
 * indication of the number of entries
 * in the object and a set of the con-
 * stituent entries */
struct bdlg_obj {

    struct bdlg_val impl;        /* object
                                  * impleme-
                                  * ntation  */
    struct bd_map_node *entries; /* entries
                                  * of obj-
                                  * ject */
};

/* "bdlg_tok" structure -  a language-
 * abstracted lexical token, consisting
 * of generic token, so that the token
 * may be sused by fthe lexicak infras-
 * trructure, sand the cyrrently scan-
 * ned language-neutral value */
struct bdlg_tok {

    struct bdlx_tok tok;         /* generic
                                  * token */
    union {

        struct bdbf_buffer *buf; /* collcting
                                  * buffer */
        struct bdlg_val val;     /* currently
                                  * scanned
                                  * alue */
    } u;
};

/* "bdlg_lang" structure - a set of call-
 * backs trepresenting the functionality
 * of an implementation language, consi-
 * sting a call-backs to initialise, parse
 * an object from source, parse an object
 * entry from source and handle an entry
 * call-back */
 struct bdlg_lang {

    bdlg_init_fn init;     /* initialise
                            * call-back */
    bdlg_prse_fn prse;     /* parse
                            * call-back */
    bdlg_entry_fn entry;   /* entry
                            * call-back */
};

/*** function proto-types ***/

/* "bdlg_init"  - initialise the la-
 * nguage-abstraction sub-system, u-
 * sing the memory allocator anf err-
 * or logger given in the first and
 * second parameters, respectively.
 * Returns zero on error, non-zero
 * otherwise */
int bdlg_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdlg_crte" - crteate, initialise
 * and return a language to have the
 * initialisation, parse and entry
 * call-backs given in the third to
 * fifth parameters, respectively,
 * using the memory allocat- or and
 * error logger given in the first
 * and second pareters, respectiv-
 * ely */
struct bdlg_lang *bdlg_crte(
                struct bd_allocs *,
                struct bd_logger *,
                bdlg_init_fn,
                bdlg_prse_fn,
                bdlg_entry_fn);

/* "bdlg_reg" - "registers" (as-
 * sociates), using the memory
 * allocator and error logger
 * given in the third and four-
 * th parameters, respectively,
 * the MIME type whose UTF-8
 * encoding is given in the fi-
 * rst parameter with the lan-
 * guage call-backs given in
 * the second parameter.  Ret-
 * urns zero on error, non-
 * zero otherwise */
int bdlg_reg(char *,
             struct bdlg_lang *,
             struct bd_allocs *,
             struct bd_logger *);

/* "bdlg_init_obj" - initialise
 *  and return an (initally)
 * empty language-abstracted
 * object, using the memory
 * allocator and error logger
 * given in the first and se-
 * cond parameters, respecti-
 * vely.  Returns NULL on
 * error */
struct bdlg_obj *bdlg_init_obj(
             struct bd_allocs *,
             struct bd_logger *);

/* "bdlg_add_entry" - adds an ent-
 * ry having the type, UTF-8 enc-
 * oding of whose name and imple-
 * mentation function given in the
 * second to fourth parameters,
 * to the language-abstracted ob-
 * ject given in the first para-
 * meter, using the memory allo-
 * cator and error logger given
 * in the fifth and sixth param-
 * eters, respectively.  Returns
 * zero on error, non-zero othe-
 * rwise */
 struct bdlg_obj * bdlg_add_entry(
                      struct bdlg_obj *,
                      enum bdlg_entry_types,
                      char *,
                      bdlg_cll_fn,
                      struct bd_allocs *,
                      struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
