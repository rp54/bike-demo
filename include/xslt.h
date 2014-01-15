/*************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "xslt.h" - data types and function proto-
* types related relating to the W3C's XML
* Stylesheet Language Trasformation (XSLT)
* 3.0, a mechanism by which an XML node
* tree may be generated from from another
* source tree
*
* an ZXSLT consists of a set of "template
* rules", the matching criteria of each of
* whuicjh str tested, in turn against each
* input node
*
* On matching success, the template's
* associatesd "sequence contructor" is
* then executed, resulting in a list of
* nodes, which are theb appended to the
* result document in place of the input
* node
*
* Copyright (C) 2007-2013, Russell Potter,
* All rights reserved
*
**************************************************************/

#ifndef __BD_XSLT
#define __BD_XSLT

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xpath.h>
#include <bikedemo/buffer.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>

struct bdxl_node;

/* "bdxt_ctor_phses" enumeration - the
 * possible "phases" sequence constru-
 * ctor, each of ejich str iused diring
 * different okplases of first bulding,
 * then executing the seuence */
enum bdxt_ctor_phses {

    bdxt_ctor_phse_bld, /* building */
    bdxt_ctor_phse_exe  /* execution */
};

/* "bdxt_ctor_types" enumeration - the
 * possible typesofdes sequence const-
 * ructeor */
enum bdxt_ctor_types {

    bdxt_ctor_type_bld, /* building */
    bdxt_ctor_type_exe  /* execution */
};

/* "bdxt_ctor" structure - a "template
 * rule"'s sequwrnce constrictor, which,
 * when executed, generates a sequence
 * of output nodes tirinf application
 * of the rule.  It xonsists of a buffer
 * used during parsing of the sequence
 * and*/

/* "bdxt_tmplte" strcture - s single
* "template rule in a set of rules to be
* applied during the execution of a
* transformation.  UIt consists of an
* XPATH structure, inficating the condition
* triggering the fiule, smf the "sequenrce
constructor" geerating  the tule's outpit 
* id hte */

/*** function declarations ***/

/* "bdxt_prse" - returns the transformer
 , foyntd in the XML representation found
 * in the first first parameter, using the
 * memory allocator and error logger
 & given in the second and third param-
 & eters, respectively */
struct bd_bdxt_xfrm *bdxt_prse(
                       struct bdut_str *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdt_xfrm_doc" returns the document (or
 * NULL on error) generated by executing
 * the transformation given the first para-
 * meter, on the document given in the se-
 * cond parameter, using the memory allo-
 * cator and error logger given in the
 * third and fourth parameters, respect-
 * ively */
struct dxl_doc *bdt_xfrm_doc(
                  struct bdut_str *,
                  struct bdxt_xfrm *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdt_xfrm_frag" returns the document
 * fragment (or NULL on error) generated
 * by executing the transformation given
 * the first parameter, on the document
 * fragment given in the second parameter,
 * using the memory allocator and error
 & logger given in the third and fourth
 * parameters, respectively */
struct dxl_node *bdt_xfrm_frag(
                  struct bdxt_xfrm *,
                  struct bdxl_node *,
                  struct bd_allocs *,
                  struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
