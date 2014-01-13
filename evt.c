/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* evt.c - function proimplememenations related
* to Level 3 of the W3C's Document Object Model
* (DOM) events specification, as defined at
* "http://www.w3.org/TR/domcore#events" 
*
* Copyright (C) 2007-2013, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <bikedemo/evt.h>
#include <bikedemo/js.h>
#include <bikedemo/util.h>
#include <bikedemo/os.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "lck_lvl" - indication as to
 * whether the event sub-system
 * is currently locked */
static int lck_lvl;

/* "mtx" - the event sub-
 * system lthread lock */
static struct bdos_mtx *mtx;

/* "cll_trgt_add_lstnr" - implements the
 * "REventTarget"-class "addEventListener"
 * method, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_trgt_add_lstnr(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_type" - implements the "Event"-
 * class "type" attribute getter, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *get_evt_type(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_trgt" - implements the "Event"-
 * class "target" attribute getter, using
 * the language abstraction, "context" ob-
 * ject, array of arguments, number of ar-
 * guments, memory allocator, error logge-
 * r and object to set, on exit, to any
 * thrown exception in "lang", "ctx", "ar-
 * gs", "num", "allocs", "logger" and "xc-
 * ptn", respectively */
static struct bdlg_obj *get_evt_trgt(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_phse" - implements the "Event"-
 * class "phase" attribute getter, using
 * the language abstraction, "context" ob-
 * ject, array of arguments, number of ar-
 * guments, memory allocator, error logge-
 * r and object to set, on exit, to any
 * thrown exception in "lang", "ctx", "ar-
 * gs", "num", "allocs", "logger" and "xc-
 * ptn", respectively */
static struct bdlg_obj *get_evt_phse(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_curr_trgt" - implements the
 * "Event"-class "target" attribute gett-
 * er, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *get_evt_curr_trgt(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_evt_stp_prpgtn" - implements the
 * "Event"-class "stopPropagation" method,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, numb-
 * er of arguments, memory allocator, er-
 * ror logger and object to set, on exit,
 * to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "lo-
 * gger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *cll_evt_stp_prpgtn(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_evt_stp_immdte_prpgtn" - implem-
 * ents the "Event"-class "stopImmediate-
 * Propagation" method, using the langu-
 * age abstraction, "context" object, ar-
 * ray of arguments, number of arguments,
 * memory allocator, error logger and o-
 * bject to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcpt-
 * n", respectively */
static struct bdlg_obj *cll_evt_stp_immdte_prpgtn(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_cnclble" - implements the
 * "Event"-class "cancelable"  attribute
 * getter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *get_evt_cnclble(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_bbbls" - implements the "Event"-
 * class "bubbles"  attribute getter, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of
 * arguments, memory allocator, error lo-
 * gger and object to set, on exit, to a-
 * ny thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "log-ger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_evt_bbbls(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "call_evt_stp_prpgtn" - implements
 * the "Event"-class "stopPropagation" 
 * method, using the language abstracti-
 * on, "context" object, array of argu-
 * ments, number of arguments, memory
 * allocator, error logger and object to
 * set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respectiv-
 * ely */
static struct bdlg_obj *call_evt_stp_prpgtn(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_evt_prvnt_dflt" - implements the
 * "Event"-class "preventDefault"  method,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, numb-
 * er of arguments, memory allocator, er-
 * ror logger and object to set, on exit,
 * to any thrown exception in "lang", "c-
 * tx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_evt_prvnt_dflt(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_dflt_prvntd" - implements
 * the "Event"-class "defaultPrevented"
 * attribute getter, using the language
 * abstraction, "context" object, array
 * of arguments, number of arguments, m-
 * emory allocator, error logger and ob-
 * ject to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "log-ger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *get_evt_dflt_prvntd(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_is_trstd" - implements the
 * "Event"-class "isTrusted" attribute
 * getter, using the language abstract-
 * ion, "context" object, array of arg-
 * uments, number of arguments, memory
 * allocator, error logger and object
 * to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "n-
 * um", "allocs", "log-ger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *get_evt_is_trstd(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_evt_tmstmp" - implements the
 * "Event"-class "timeStamp" attribute
 * getter, using the language abstract-
 * ion, "context" object, array of arg-
 * uments, number of arguments, memory
 * allocator, error logger and object
 * to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "n-
 * um", "allocs", "log-ger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *get_evt_tmstmp(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_evt_ctor" - implements the "Ev-
 * ent"-class constructor, using the l-
 * anguage abstraction, "con- text" ob-
 * ject, array of arguments, number of
 * arguments, memory alloca- tor, err-
 * or logger and object to set, on ex-
 * it, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs",
 * "log-ger" and "xcptn", respectively */
static struct bdlg_obj *cll_evt_ctor(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_evt_init_evt" - implements the
 * "Event"-class "timeStamp" method, u-
 * sing the language abstraction, "con-
 * text" object, array of arguments,
 * number of arguments, memory alloca-
 * tor, error logger and object to set,
 * on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "all-
 * ocs", "log-ger" and "xcptn", resp-
 * ectively */
static struct bdlg_obj *cll_evt_init_evt(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_trgt_rm_lstnr" - implements the
 * "EventTarget"-class "removeEventLis-
 * tener" method, using the language abs-
 * traction, "context" object, array of
 * arguments, number of arguments, memory
 * allocator, error logger and object to
 * set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *cll_trgt_rm_lstnr(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_trgt_add_lstnr" - implements the
 * "EventTarget"-class "addEventListener"
 * method, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_trgt_add_lstnr(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "cll_trgt_dsptch" - implements the
 * "EventTarget"-class "dispatchEvent"
 * method, using the language abstract-
 * ion, "context" object, array of arg-
 * uments, number of arguments, memory
 * allocator, error logger and object to
 * set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *cll_trgt_dsptch(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}



/*  "cmp_hndlrs" - returns the
 * result of a comparison bet-
 * ween the event handler call-
 & backs in "hndlr1" and "hndlr2",
 * as a key comparison funct-
 * ion of a target's map of
 * listeners */
int cmp_hndlrs(bdet_hndlr_fn hndlr1,
               bdet_hndlr_fn hndlr2)
{
    /* listeners are identical
     * if have the same handler 
     * routine */
    return (* (int *) &hndlr1) -
           (* (int *) &hndlr2);
}


/* "bdet_add_lstnr" - adds the event
 * handling routine given in "hndlr"
 * to the target given in "trgt",
 * using the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise */
int bdet_add_lstnr(struct bdet_trgt *trgt,
                   bdet_hndlr_fn hndlr,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* allocate insertion node ... */
    ins_node =  (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;

    /* ... and attach "lstnr" to "trgt"
     * by inserting it inyo "trgt"'s map
     * of listeners */
    return bd_map_insert(&trgt->lstnrs,
                         ins_node,
                         &hndlr,
                         (bd_map_cmp_fn)
                             cmp_hndlrs,
                         &hndlr);
}

/* "bdet_lck" - sets the event system
 * thread lock using the memory alloca-
 * tor and error logger given in "allo-
 * cs" and "logger", respectively.
 * Returns zero on error, non-zero oth-
 * erwise */
int bdet_lck(struct bd_allocs *allocs,
		     struct bd_logger *logger)
{
    /* lock the event
     * mutex and update
     * the lock status */
    mtx->ops->lck(mtx,
                  allocs,
                  logger);
    lck_lvl++;

    return 1;
}

/* "bdet_unlck" - clears the ev-
 * ents sub-system thread lock,
 * using the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
int bdet_unlck(struct bd_allocs *allocs,
		       struct bd_logger *logger)
{
    /* lock the event
     * mutex and update
     * the lock status */
    mtx->ops->unlck(mtx,
                    allocs,
                    logger);
    lck_lvl--;

    return 1;
}

/* "bdet_lckd" - Returns non-
 * zero if the event system
 * is currently locked */
int bdet_lckd(void)
{
    /* lock level will
     * be positive if
     * unlocked */
	return lck_lvl > 0;
}

/* "bdet_add_evt_prps" - returns a lang-
*  uage-abstracted object populated with
 * the methods and attributes of the DOM
 * Event given in "evt", using the memo-
 * ry allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns NULL on error */
struct bdlg_obj *bdet_add_evt_prps(
                      struct bdet_evt *evt,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlg_obj *ret;    /* return value */

    /* initialise with tanguage-
     * abstracted object's entries */
    if (!(ret = bdlg_init_obj(allocs,
                              logger)))
        return NULL;

    /* and add DOM event target entries */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_ctor,
                         NULL,
                         (bdlg_cll_fn)
                             cll_evt_ctor,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "type",
                         (bdlg_cll_fn)
                             get_evt_type,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "type",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "tarqet",
                         (bdlg_cll_fn)
                             get_evt_trgt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "tarqet",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "currentTarqet",
                         (bdlg_cll_fn)
                             get_evt_curr_trgt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "currentTarqet",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "currentPhase",
                         (bdlg_cll_fn)
                             get_evt_phse,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "currentPhase",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "bubbles",
                         (bdlg_cll_fn)
                             get_evt_bbbls,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "bubbles",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "cancelable",
                         (bdlg_cll_fn)
                             get_evt_cnclble,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "cancelable",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "preventDefault",
                         (bdlg_cll_fn)
                             cll_evt_prvnt_dflt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "defaultPrevented",
                         (bdlg_cll_fn)
                             get_evt_dflt_prvntd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "defaultPrevented",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "isTrusted",
                         (bdlg_cll_fn)
                             get_evt_is_trstd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "isTrusted",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "timeStamp",
                         (bdlg_cll_fn)
                             get_evt_tmstmp,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "timeStamp",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "stopPropagation",
                         (bdlg_cll_fn)
                             call_evt_stp_prpgtn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "stopImmediatePropagation",
                         (bdlg_cll_fn)
                             cll_evt_stp_immdte_prpgtn,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                         "mominitEvent",
                         (bdlg_cll_fn)
                             cll_evt_init_evt,
                         allocs,
                         logger);
}

/* "bdet_add_trgt_prps" - returns a lan-
* guage-abstracted object populated with
 * the methods and attributes of the DOM
 * Event Target given in the "trgt", us-
 * ing the memory allocator and error
 * logger given in the "allocs" and "lo-
 * gger" parameters, respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdet_add_trgt_prps(
                      struct bdet_trgt *trgt,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlg_obj *ret;    /* return value */

    /* initialise with tanguage-
     * abstracted object's entries */
    if (!(ret = bdlg_init_obj(allocs,
                               logger)))
        return NULL;

    /* and add DOM event target entries */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "addEventListner",
                         (bdlg_cll_fn)
                             cll_trgt_add_lstnr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "removeEventListner",
                         (bdlg_cll_fn)
                             cll_trgt_rm_lstnr,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "dispatchEvent",
                          (bdlg_cll_fn)
                             cll_trgt_dsptch,
                         allocs,
                         logger);
}
