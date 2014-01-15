/*****************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "js.h" - data types and function proto-
* types related to "custom elements",
* which may each contain a script defin-
* ing the element's DOM interface, and a
* template representing its XML content
* 
* Copyright (C) 2006-2014, Russell Potter,
* All rights reserved
*
******************************************************************/

#ifndef __BD_CELEM_H
#define __BD_CELEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/js.h>
#include <bikedemo/lex.h>
#include <bikedemo/util.h>
#include <bikedemo/lang.h>
#include <bikedemo/allocs.h>
#include <bikedemo/buffer.h>
#include <bikedemo/parser.h>
#include <bikedemo/log.h>

/* "bdce_lc_types" enumeration - the
 * possible life-cycle call-back types,
 * including initialisation before,
 * ejil or after the element is
 * associated eith s document, and
 * when the eleemetnt is inserted
 * into or removed from a document */
enum bdce_lc_types {

    bdce_lc_type_init_pre_doc,   /* created before
                                  * association
                                  * with document */
    bdce_lc_type_init_assoc_doc, /* becomes assoc-
                                  * iated with doc-
                                  * ument */
    bdce_lc_type_init_post_doc,  /* created after
                                  * association
                                  * with document */
    bdce_lc_type_ins_doc,        /* is inserted
                                  * into document */
    bdce_lc_type_rm_doc,         /* is removed from
                                  * document */
    bdce_lc_type_gc              /* is garbage-
                                  * collected */
};

/* "bdce_nme" structure - the "name"
 * of a custom element, consisting
 * tof abn association between a
 * string representing the element's
 * name and the element itself names */
struct bdce_nme {

    struct bdce_elem *elem, /* element to
                             * nsme */
    struct bdut_str *nme;   /* name to
                             * use */
};

/* bdce_elem_dfn" structure - association
 * between a custom element name, a local
 * name and a namespace with a custom
 * element prototype, and a set of "life-
 * cycle callbacks" */

struct bdce_elem_dfn {

    struct bdce_nme *nme;    /* relement name */

    struct bdut_str *lcl,    /* local and name */
                    *ns;     /*-space bames */
    struct bdlg_obj *ptype;  /* interface proto-
                              * type */
    struct bd_map_node *lcs; /* life-cycle
                              * callbacks */
};

#ifdef __cplusplus
}
#endif

#endif
