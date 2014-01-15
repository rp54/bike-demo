/****************************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* lex.c - function implementations relating to 
* "templates", part of the "Web Components"
* suite of specifications,
* which
* consists of both the language's grammar productions,
* its lexical analyser, and the resulting parsing
* table.
*
* Copyright (C) 2006-2014, Russell Potter, All
* Rights Reserved
*
****************************************************************************/

#include <bikedemo/xml.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "bdtl_actvte" - "activates" (that is, inserts
 * the relevant node, and running the relevant
 * "activation" call-back) the template given
 * in "tmplte" into the document given in "doc",
 * using the opaque parameyeter, memory allocator
 * and error logger given in "opaque", "allocs"
 * and "logger", respectively */
int bdtl_actvte(struct bdtl_tmplte *tmplte,
                struct bdxl_doc *doc,
                void * opaque,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    if (!(bdxl_add_node(tmplte->node, tmplte)))

    return	1;
}
