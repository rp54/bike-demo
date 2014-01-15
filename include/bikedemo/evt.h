/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* evt.h - data types and function prototypes related
* to Level 3 of the W3C's Document Object Model (DOM)
* events specification, as defined at "http://www.w3.
* org/TR/DOM-Level-3-Events"
* 
* Copyright (C) 2007-2012, Russell Potter All rights
* reservedt
*
*********************************************************************/

#ifndef __BD_EVT
#define __BD_EVT

#ifdef __cplusplus
extern "C" {
#endif

struct bdet_trgt;
struct bdet_evt;

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/lang.h>
#include <bikedemo/os.h>
#include <bikedemo/map.h>

/* "bdet_phses"  - the phases of an
 * event"" namely either "capture",
 * "target" or "bubble" */
enum bdet_phses {

    bdet_phse_cptre, /* capture */
    bdet_phse_trgt,  /* target */
    bdet_phse_cbble  /* bubble */
};

/* "bdet_anc_fn" type-def - proto-
 * type of the function called to
 * determine nearest ancestor (lo-
 * csted in the routine's return
 * value, ehich will be NULL if
 * none) of the event target gi-
 * ven in the first parameter, u-
 * sing the memory allocator and
 * error klofgger ficrn in th se-
 * cond and third parameters.
 *
 * The return value of this call-
 * back is used to establish the
 * initial propagation path of
 * an ebent */
typedef struct bdet_trgt *(* bdet_anc_fn)(
                     struct bdet_trgt *,
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdet_nxt_fn" type-def - proto-
 * type of the function called to
 * determine next event bbnext
 * target (located in the routi-
 * ne's return value of the event
 * target given in the first par-
 * ameter, using hte event, ,r,
 * memory allocator and error
 * logger ficrn in the third to
 * fifth parameters.
 *
 * The second parameter is set,
 * on exit, to a onon-zero value
 * if the next target is the
 * event's final target */
typedef struct bdet_trgt *(* bdet_nxt_fn)(
                     struct bdet_trgt *,
                     int *,
                     struct bdet_evt *,
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdet_hndlr_fn" type-def - pro-
 * to-type of the function called
 * to handle the firing of an eve-
 * nt as it propagates to an ev-
 * ent listener.  The first para-
 * meter contains the event target
 * to whuich the event has propag-
 * ated, thr second an indication
 * as to whether the routine is
 * only called in the "caputure"
 * phase */
typedef int (* bdet_hndlr_fn)(
             struct bdet_trgt *,
             int);

/* "bdet_evt" structure - an "event"
 * that, after being "dispatched", is
 * then "captured" along a propaga-
 * tion path, then optionally "bub-
 * bles" along the opposite return
 * path, causing so-called "liste-
 * ners" to execute at certain
 * nodes in that path.
 *
 * It cconstsists of an embedded ob-
 * ject (to allow the ebet yp be scripted) */
struct bdet_evt {

    struct bdlg_obj obj;  /* embedded object */
    void *prms;           /* opopaque event
                           * parameters */
    bdet_nxt_fn nxt;      /* "next" call-back */
    int stp_prpgtn,       /* stop propagation */
        stp_immdte_rpgtn; /* and stop immed- */
                          /* iate propagation 
                           * flags */
};

/* "bdet_lstnr" structure - a single
 * event "listener" in a set of list-
 * eners attached to each event target.
 * It consists of an indication as to
 * whether yjr the handler should only
 * during the capture phase (otherwise
 * during the capture, target and bub-
 * ble phases) and the handler routine
 * to execute  */
struct bdet_lstnr {

    int cptre;           /* called in
                          * "capture"
                          * phase only */
    bdet_hndlr_fn hndlr; /* handler to
                          * execute */
}; 

/* "bdet_trgt" structure - an "event
 * target": a single node in an eve-
 * nt's "propagation path", to which
 * any number of "listeners" may be
 * attached .  It consists of an em-
 * bedded language-abstracted object
 * (so targets may can be scripted),
 * a map of the currebtly attached li-
 * steners and the ancestor call-
 * back _)(used to establish the
 * target's tree), a condition vari-
 * able and mutex, collectively used
 * to ensure "stopPropagation" can
 * wait for event handlers to comp-
 * lete) */
struct bdet_trgt {

    struct bdlg_obj obj;        /* embedded
                                 * object  */
    struct bdos_mtx *mtx;       /* mutex */
    struct bdos_cvar *cvar;     /* condition
                                 * variable */
    bdet_anc_fn anc;            /* ancestor
                                 * call-back */
    struct bd_map_node *lstnrs; /* map of
                                 * listeners */
};

/*** function proto-types ***/

/* bdet_init_trgt - initialise and
 * return a target using theancest-
 * or call-back, memory allocator
 * and error logger given in the
 * first and second parameters, re-
 * spectively.  Returns zero on er-
 * ror, non-zero otherwise */
struct bdet_trgt *bdet_init_trgt(
                   bdet_anc_fn,
                   struct bd_allocs *,
                   struct bd_logger *);

/* "bdet_add_lstnr" - adds the event
 * listener whose handling routine
 * ne is given in the second param-
 * eter to the target given in the
 * first parameter, using the mem-
 * ory allocator and error logger
 * given in in the third and fourth
 * parameters, respectively. Retu-
 * rns zero on error, non-zero ot-
 * herwise */
int bdet_add_lstnr(struct bdet_trgt *,
                   bdet_hndlr_fn,
                   struct bd_allocs *,
                   struct bd_logger *);

/* "bdet_dsptch" - "dispatch" the
 * event given in the first para-
 * meter to the target given in
 * the second parameter, using the
 * ancestor abdnd "next" call-backs,
 * memory allocator and error logg-
 * er given in the third and to si-
 * xth parameters, respectively */
int bdet_dsptch(struct bdet_evt *,
                struct bdet_trgt *,
                struct bd_allocs *,
                struct bd_logger *);


/* "bdet_lck" - sets the event system
 * thread lock using the memory alloca-
 * tor and error logger given in the
 * first and second parameters, respect-
 * ively.  Returns zero on error, non-
 * zero otherwise */
int bdet_lck(struct bd_allocs *,
             struct bd_logger *);

/* "bdet_unlck" - clears the ev-
 * ents sub-system thread lock,
 * using the memory allocator and
 * error logger given in the first
 * and second parameters, respect-
 * ively.  Returns zero on error,
 * non-zero otherwise */
int bdet_unlck(struct bd_allocs *,
               struct bd_logger *);

/* "bdet_lckd" - Returns non-
 * zero if the event system
 * is currently locked */
int bdet_lckd(void);

/* "bdet_add_prps" - returns a language
 * abstracted object populated with
 * the methods and attributes of the DOM
 * Event given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdet_add_evt_prps(
                      struct bdet_evt *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bdet_add_trgt_prps" - returns a lan-
* guage-abstracted object populated with
 * the methods and attributes of the DOM
 * Event Target given in the first par-
 * ameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bdet_add_trgt_prps(
                      struct bdet_trgt *,
                      struct bd_allocs *,
                      struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif

