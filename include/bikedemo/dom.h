/********************************************************************
*
* BikeDemo - Exercise bike demonstration scenery
*
* dom.h - data types and function proto-types rel-
* ated to the W3C's Document Object Model (DOM), a
* set of APIs to, amongst others, read from and wr-
* ite to and from files and modify XML documents,
* as well as load snd save them to and from input
* and output streams.
*
* Copyright (C) 2007-2012, Russell Potter
* All rights reserved
*
*********************************************************************/

#ifndef __BD_DOM_H
#define __BD_DOM_H

#ifdef __cplusplus
extern ("C") {
#endif

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/lang.h>
#include <bikedemo/evt.h>
#include <bikedemo/xml.h>

struct bdxl_elem;
struct bdxl_doc;
struct bdxl_cdta;
struct bdxl_attr;
struct bdxl_txt;
struct bdxl_impl;
struct bdxl_pi;
struct bdxl_rnge;
struct bdxl_it;
struct bdxl_wlkr;
struct bdxl_cnfg;
struct bdxl_opts;
enum bdxl_dflt_types;

  /* "bddm_fltr_acpts" enumeration - the
 * possible results of ainlyfiltering a
 * node, (Aas isused bt gthe DOM Filter's
 * "acceptNode" method) namely: accept-
 * ance, rejection or skipping the node */
enum bddm_fltr_acpts {

    bddm_fltr_acpt_accpt, /* accept node */
    bddm_fltr_acpt_rjct,  /* reject node */
    bddm_fltr_acpt_skip   /* skip node */
};

/* "bddm_whats" enumeration - a
 * mask specifying the possible
 * types of types, as used by
 * the DOM filter "qwhat to show"
 nmask */
enum bddm_whats {

    bddm_what_all  =     -1,    /* all nodes */
    bddm_what_elem =     0x1,   /* elements */
    bddm_what_attr =     0x2,   /* attributes */
    bddm_what_txt =      0x4,   /* text nodes */
    bddm_what_cdta =     0x8,   /* CDATA
                                 * sections */
    bddm_what_ent =      0x10,   /* entity */
    bddm_what_ent_ref =  0x20,   /* entity ref. */
    bddm_what_pi =       0x40,  /* processing
                                 * instructions */
    bddm_what_cmt =      0x80,  /* comment */
    bddm_what_doc =      0x100, /* documents */
    bddm_what_doc_type = 0x200, /* document
                                 * types */
    bddm_what_doc_frg =  0x400, /* document
                                 * fragments */
    bddm_what_ntn =      0x800  /* notations */
};

/* "bddm_mut_types" - the posible types
 * of node tree mutation, such as "in-
 * swrtion", "pre-insertion", "removal"
 * and "appending", used by various DOM
 * algorithms */
enum bddm_mut_types {

    bddm_mut_type_ins,     /* insertion */
    bddm_mut_type_pre_ins, /* pre-insert-
                            * ion */
    bddm_mut_type_rmve,    /* removal */
    bddm_mut_type_appnd    /* append */
};

/* "bddm_nde_types" enumeration - the po-
 * ssible types of DOM nodes, including
 * elements, text, documents and comme-
 * nts */
enum bddm_nde_types {

    bddm_nde_elm,         /* element */
    bddm_nde_txt     = 3, /* text */
    bddm_nde_pi      = 7, /* processing
                           * instruction */
    bddm_nde_cmt,         /* comment */
    bddm_nde_doc,         /* document */
    bddm_nde_doctype,     /* document
                           * type */
    bddm_nde_docfrg       /* document
                           * fragment */
};

/* "bddm_err_cdes" enumeration - the po-
 * ssible DOM error codes */
enum bddm_err_cdes {

    bddm_err_cde_idx_sz,         /* index size */
    bddm_err_cde_hrchy_rqst = 3, /* hierarchy requ-
                                  * est */
    bddm_err_cde_wrng_doc,       /* wrong document */
    bddm_err_cde_invld_chr,      /* invalid chara-
                                  * cter */
    bddm_err_cde_no_mdfctn = 7,  /* no modification
                                  * allowed */
    bddm_err_cde_fnd,            /* not found */
    bddm_err_cde_spprtd,         /* not supported  */
    bddm_err_cde_state = 11,     /* invalid state  */
    bddm_err_cde_syntx,          /* syntax error */
    bddm_err_cde_ns,             /* name-space */
    bddm_err_cde_accss,          /* invalid access */
    bddm_err_cde_type = 17,      /* type mismatch */
    bddm_err_cde_scrty,          /* security */
    bddm_err_cde_ntwrk,          /* network */
    bddm_err_cde_abrt,           /* abort */
    bddm_err_cde_url,            /* URL mismatch */
    bddm_err_cde_qta,            /* quota exceeded */
    bddm_err_cde_tmt,            /* timeout */
    bddm_err_cde_clne,           /* data clone */
    bddm_err_cdes_num            /* number of
                                  * errors */
};

/* "bddm_doc_pstns" enumeration - flags repr-
 * esenting the possible relative document
 * posutions, as used by "compareDocument-
 * Position" */
enum bddm_doc_pstns {

    bddm_doc_pstn_dscnnctd,     /* disconnected */
    bddm_doc_pstn_prcdng   = 2, /* preceding */
    bddm_doc_pstn_fllwng   = 4, /* following */
    bddm_doc_pstn_cntnd    = 8, /* contained */
    bddm_doc_pstn_cntnd_by = 16 /* contained by */
};

/* "bddm_fltr_fn" typedef = prototype
 * of the function called by a DOM Fil-
 * ter to ro sdetermine  (by returning
 * an "accept" value)  the Node given
 * in the first parameter should be
 * accepted for processing */
typedef enum bddm_accs ( *bddm_fltr_fn)(
                 struct bdxl_node *);

/* "bddm_add_elem_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns NULL
 * on error */
struct bdlg_obj *bddm_add_elem_prps(
                       struct bdxl_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bddm_add_doc_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Document given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bddm_add_doc_prps(
                      struct bdxl_doc *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_nde_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Node given in the first parameter, us-
 * ing the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bddm_add_nde_prps(
                      struct bdxl_node *,
                      struct bd_allocs *,
                      struct bd_logger *);


/* "bddm_add_evt_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Event given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bddm_add_evt_prps(
                      struct bdet_evt *,
                      struct bd_allocs *,
                      struct bd_logger *);


/* "bddm_add_evt_trgt_prps" - returns a
 * language abstracted object populated
 * with the methods and attributes of
 * the DOM Event Target given in the fi-
 * rst parameter, using the memory allo-
 * cator and error logger given in the
 * second and third parameters, respect-
 * ively.  Returns NULL on error */
struct bdlg_obj *bddm_add_evt_trgt_prps(
                      struct bdet_trgt *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_cdta_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * CDATA Section given in the first par-
 * ameter, using the memory allocator a-
 * nd error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_cdta_prps(
                      struct bdxl_cdta *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_attr_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Attribute given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_attr_prps(
                      struct bdxl_attr *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_txt_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Text Node given in the first parame-
 * ter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_txt_prps(
                      struct bdxl_txt *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_impl_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Text Implementation given in the first
 * parameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_impl_prps(
                      struct bdxl_impl *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_pi_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Processing Instruction given in the
 * first parameter, using the memory all-
 * ocator and error logger given in the
 * second and third parameters, respect-
 * ively.  Returns NULL on error */
struct bdlg_obj *bddm_add_pi_prps(
                      struct bdxl_pi *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_rnge_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Range given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bddm_add_rnge_prps(
                      struct bdxl_rnge *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_it_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Node Iterator given in the first par-
 * ameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns
 * NULL on error */
struct bdlg_obj *bddm_add_it_prps(
                      struct bdxl_it *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_wlkr_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Tree Walker given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_wlkr_prps(
                      struct bdxl_wlkr *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_fltr_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Node Filter given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_fltr_prps(
                      struct bdxl_fltr *,
                      struct bd_allocs *,
                      struct bd_logger *);

/* "bddm_add_doc_type_prps" - returns a
 * language abstracted object populated
 * with the methods and attributes of the
 * DOM Doocument Type given in the first
 * parameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_doc_type_prps(
                      struct bdxl_doc_type *,
                      struct bd_allocs *,
                      struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
