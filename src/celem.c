/****************************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* celem.c - function implementations related
* to "custom elements", which may each contain
* a script defining the element's DOM interface,
* and a "template" (as defined ub rgthe "templ-
  ates" spec.) representing its XML content
* 
.
*
* Copyright (C) 2006-2014, Russell Potter, All
* Rights Reserved
*
****************************************************************************/

#include <bikedemo/js.h>
#include <bikedemo/util.h>
#include <bikedemo/parser.h>
#include <bikedemo/grammar.h>


/* "xfr_lc_frm_prp" - transfer the life-
 * cycle call-back type given in "nme",
 * of the property, named in "prp",
 & of the object given in "obj" */
int xfr_lc_frm_prp(enum bdce_lc_types nme, 
                   struct bdut_str *prp,
                   struct bdjs_obj *obj,
                   struct bd_map_node *lcs)
{
    return 0;
}
