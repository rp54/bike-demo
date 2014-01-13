
/********************************************************************
*
* BikeDemo - Exercise bike demonstration scenery
*
* dom.c - function implementations relating to
* the W3C's Document Object Model (DOM), which
* populate each type of XML nodes with it's
* constituent methods and attributes, allowing
* them to behave in a a manner consistent with
the DOM specification.
* 
* Copyright (C) 2007-2013, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/lang.h>
#include <bikedemo/evt.h>
#include <bikedemo/xml.h>
#include <bikedemo/dom.h>

struct bd_logger;
struct bd_allocs;

/* "get_val" - returns a generic cast of
 * the appropriate part of the value of
 * the object in "obj" , according to
 * the type of the value.  Returns NULL
 ib on error */
static void *get_val(struct bdlg_obj *obj)
{
    switch (obj->impl.type) {

        case bdlg_val_type_num:
            return (void *) obj->impl.val.num;

        case bdlg_val_type_str:
            return (void *) obj->impl.val.str;

        case bdlg_val_type_num:
            return (void *) obj->impl.val.bln;

        case bdlg_val_type_cmplx:
            return obj->impl.val.opaque;

        default:
            return NULL;
    }
}

/* "set_val" - sets the appropriate
 * of the value of the object in "obj",
 * according to its intended type in
 * "type".  Returns zero on error,
 * non-zero otherwise */
static int set_val(struct bdlg_obj *obj,
                   void *val,
                   enum bdlg_val_types type)
{
    switch (type) {

        case bdlg_val_type_num:
            obj->impl.val.num = (int) val;

        case bdlg_val_type_bln:
            obj->impl.val.bln = (int) val;

        case bdlg_val_type_str:
            obj->impl.val.str = (struct bdut_str *) val;

        case bdlg_val_type_cmplx:
            obj->impl.val.opaque = val;

        default:
            return NULL;
    }
}

/* "set_nde_nde_nme" - implements the "N-
 * ode"-class "nodeName" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_nde_nde_nme(
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

/* "get_xcptn_nme" - implements the "DOME-
 * xception"-class "name" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_xcptn_nme(
                           struct bdlg_lang *lang,
                           struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{
    set_val(ctx, errs[], bdlg_val_rtype_str);

    return ctx;
}

/* "errs" - list of DOM error
 * names abnsd sdescription as
 * used in "DOMError"'s
 * "name" abnd "description"
 attributes */
static struct {
    char *nme;
    char *dsc;
} errs[bddm_err_cdes_num] = {

    "IndexSizeError",             "The index is not in "
                                  "the allowed range",
    NULL,                         NULL,
    "HierarchyRequestError",      "The index could yield "
                                  "an incorrect node tree",
    "WrongDocumentError",         "The object is in the "
                                  "wrong document",
    "InvalidCharacterError",      "The string contains "
                                  "invalid characters",
    NULL,                         NULL,
    "NoModificationAllowedError", "The object can not be "
                                  "modified",
    "NotFoundError",              "The object can not be "
                                  "found here",
    "NotSupportedError",          "The operation is not "
                                  "supported",
     NULL,                        NULL,
    "InvalidStateError",          "The object is in an "
                                  "invalid state",
    "SyntaxError",                "The string did not " 
                                  "match the expected "
                                  "pattern",
    "InvalidModificationError",   "The object can not be "
                                  "modified in this way",
    "NamespaceError",             "The operation is not "
                                  "allowed by Namespaces "
                                  "in XML",
    "InvalidAccessError",         "The object does not "
                                  "support the operation "
                                  "or argument",
     NULL,                        NULL,
     NULL,                        NULL,
    "SecurityError",              "The operation is "
                                  "insecure",
    "NetworkError",               "A network error occurred",
    "AbortError",                 "The operation was aborted",
    "URLMismatchError",           "The given URL does not "
                                  "match another URL",
    "QuotaExceededError",         "The quota has been "
                                  "exceeded",
    "TimeoutError",               "The operation timed out",
    "InvalidNodeTypeError",       "The supplied node is "
                                  "incorrect or has an "
                                  "incorrect ancestor for "
                                  "this operation",
    "DataCloneError",             "The object can not be "
                                  "cloned",
    "EncodingError",              "The encoding operation "
                                  "(either encoded or "
                                  "decoding) failed"
};

/* "set_xcptn" - returns a DOM errror ob-
 * ject the forle;klsds of which have been
 * populated based on the error code given
 * in "cde", using the memory allocator
 * ansd error logger given in "allocs" and
 * "logger", respectively */
struct bdlg_obj *set_xcptn(
                int cde,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    struct bdlg_obj *ret;

    if ((ret = bdlg_init_obj(allocs,
                             logger)))
        return NULL;

	ret->impl.val.opaque = code;

        if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "name",
                         (bdlg_cll_fn)
                             get_xcptn_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "code",
                         (bdlg_cll_fn)
                             get_err_cde,
                         allocs,
                         logger)))
        return NULL;


}

/* "mut_tree" - perform the process-
 * ing required by the DOM spec whenever
 * the node given in "chld", the parent
 * of which is given in "prnt", is mut-
 * ated, the type of which is given in
 * "type".  when "spprss_obsvrs" has a
 * non-zero value mutation observers
 * are called, abnd "xcptn" is set, on
 * exit, to anty throerwn exception.
 * Returns zero on error, non-zero oth-
 * erwise */
int mut_tree(struct bdxl_node *chld,
             struct bdxl_node *prnt,
             enum bddm_mut_types type,
             struct bdlg_obj *xcptn,
             int spprss_obsvrs)
{
    switch (type) {

        case bddm_mut_type_ins:
            break;

        case bddm_mut_type_pre_ins:
            break;

        case bddm_mut_type_appnd:
            break;

        case bddm_mut_type_rmve:
            break;
    }
}
/* "get_nde_nde_nme" - implements the "N-
 * ode"-class "nodeName" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_nde_nde_nme(
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

/* "set_nde_nde_nme" - implements the "N-
 * ode"-class "nodeName" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_nde_nde_nme(
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

/* "get_nde_nde_type" - implements the "N-
 * ode"-class "nodeType" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_nde_nde_type(
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

/* "get_nde_val" - implements the "Node"-class "n-
 * odeValue"  attribute getter, using the "conte-
 * xt" object, array of arguments, number of arg-
 * uments, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_val(
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

/* "do_set_nde_val" - implements the "Node"-class
 * "nodeValue" attribute setter, using the "this"
 * object, array of arguments, number of arguments,
 * memory allocator, error logger and object to
 * set, on exit, to any thrown exception "lang",
 * "ctx" , "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *do_set_nde_val(
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

/* "set_nde_nde_type" - implements the "Node"-
 * class "nodeType" attribute setter, using the
 * "this" object, array of arguments, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *set_nde_nde_type(
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

/* "set_nde_nde_val" - implements the "Node"-
 * class "nodeValue" attribute setter, using the
 * "this" object, array of arguments, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *set_nde_nde_val(
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

/* "get_nde_nde_val" - implements the "Node"-
 * class "nodeValue" attribute getter, using the
 * "this" object, array of arguments, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_nde_val(
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

/* "get_nde_prnt" - implements the "Node"-class
 * "parentNode" attribute getter, using the "this"
 * object, array of arguments, number of arguments,
 * memory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang", "c-
 * tx", "args", "num", "allocs"m "logger" and "xc-
 * ptn", respectively */
static struct bdlg_obj *get_nde_prnt(
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

/* "get_nde_chldrn" - implements the "Node"-class
 * "childNodes" attribute getter, using the "this"
 * object, array of arguments, number of arguments,
 * memory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang", "c-
 * tx", "args", "num", "allocs", "logger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *get_nde_chldrn(
                           struct bdlg_lang *lang,struct bdlg_obj *ctx,
                           struct bdlg_obj *args,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger,
                           struct bdlg_obj **xcptn)
{ 
    return NULL;
}

/* "get_nde_frst_chld" - implements the "Node"-cl-
 * ass "firstChild" attribute getter, using the "c-
 * ontext" object, array of arguments, number of a-
 * rguments, memory allocator, error logger and ob-
 * ject to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_frst_chld(
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

/* "get_nde_lst_chld" - implements the "Node"-cl-
 * ass "lastChild" attribute getter, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory al-
 * locator, error logger and object to set, on
 * exit, to any thrown exception in "lang", "c-
 * tx", "args", "num", "allocs", "logger" and "x-
 * cptn", respectively */
static struct bdlg_obj *get_nde_lst_chld(
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

/* "get_nde_nxt_sblng" - implements the "Node"-
 * class "nextSibling" attribute getter, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments, m-
 * emory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "lo-
 * gger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_nxt_sblng(
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

/* "get_nde_prv_sblng" - implements the "Nod-
 * e"-class "previousSibling" attribute gett-
 * er, using the language abstraction, "cont-
 * ext" object, array of arguments, number of
 * arguments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respective-
 * ly */
static struct bdlg_obj *get_nde_prv_sblng(
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

/* "get_nde_ns_uri" - implements the "Node"-cl-
 *  ass "namespaceURI" attribute setter, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments, me-
 * mory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "lo-
 * gger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_ns_uri(
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

/* "get_nde_bse_uri" - implements the "Node"-
 * class "baseURI" attribute setter, using the
 * language abstraction, "context" object, ar-
 * ray of arguments, number of arguments, mem-
 * ory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdjs_obj *get_nde_bse_uri(
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

/* "get_nde_prfx" - implements the "Node"-cl-
 * ass "prefix" attribute getter, using the
 * language abstraction, "context" object, a-
 * rray of arguments, number of arguments,
 * memory allocator, error logger and object
 * to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_prfx(
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

/* "set_nde_prfx" - implements the "Node"-class
 * "prefix" attribute setter, using the langua-
 * ge abstraction, "context" object, array of a-
 * rguments, number of arguments, memory alloc-
 * ator, error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx" "a-
 * rgs", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *set_nde_prfx(
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

/* "get_nde_lcl_nme" - implements the "Node"-
 * class "prefix" attribute setter, using the
 * language abstraction, "context" object, a-
 * rray of arguments, number of arguments, m-
 * emory allocator, error logger and object
 * to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_nde_lcl_nme(
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

/* "get_nde_txt_cnt" - implements the "Node"-cl-
 * ass "textContent" attribute setter, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory all-
 * ocator, error logger and object to set, on ex-
 * it, to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_nde_txt_cnt(
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

/* "get_nde_attrs" - implements the "Node"-
 * class "attributes" attributes setter, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of a-
 * rguments, memory allocator, error logger
 * and object to set, on exit, to any thro-
 * wn exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_nde_attrs(
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

/* "get_nde_ownr_doc" - implements the "Node"-
 * class "ownerDoument" attribute setter, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "lo- gger" and "xcptn", respe-
  ctively */
static struct bdlg_obj *get_nde_ownr_doc(
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

/* "cll_nde_insrt_bfre" - implements the "Node"-
 * class "insertBefore" method, using the "this"
 * object, array of arguments, number of argume-
 * nts, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_insrt_bfre(
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

/* "cll_nde_cmp_doc_pstn" - implements the "Node"-
 * class "compareDocumentPosition" method, using
 * the language abstraction, "context" object, a-
 * rray of arguments, number of arguments, memory
 * allocator, error logger and object to set, on
 * exit, to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_nde_cmp_doc_pstn(
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

/* "cll_nde_rplce_chld" - implements the "Node"-
 * class "replaceChild" method, using the "this"
 * object, array of arguments, number of argume-
 * nts, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exception
 * in "ths", "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_nde_rplce_chld(
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

/* "cll_nde_rmve_chld" - implements the "Node"-
 * class"removeChild" method, using the "tconte-
 * xt" object, array of arguments, number of ar-
 * guments, memory allocator, error logger and
 * object to set, on exit, to any thrown eexce-
 * ption in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_rmve_chld(
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

/* "cll_nde_appnd_chld" - implements the "Node"-
 * class "appendChild" method, using the "this"
 * object, array of arguments, number of argum-
 * ents, memory allocator, error logger and ob-
 * ject to set, on exit, to any thrown eexcept-
 * ion in "lang", "ctx", "args", "num", "allo-
 * cs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_appnd_chld(
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

/* "cll_nde_hs_chld_ndes" - implements the "No-
 * de"-class "hasChildNodes" method, using the
 * language abstraction, "context" object, ar-
 * ray of arguments, number of arguments, mem-
 * ory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "l-
ogger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_hs_chld_ndes(
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

/* "cll_nde_lkp_ns_uri" - implements the "N-
 * ode"-class "lookupNamespaceURI" method,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respectiv-
 * ely */
static struct bdlg_obj *cll_nde_lkp_ns_uri(
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

/* "cll_nde_clne_nde" - implements the "Node"-cl-
 * ass "cloneNode" method, using the language abst-
 * raction, "context" object, array of arguments,
 * number of arguments, memory allocator, error l-
 * ogger and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num", "al-
 * locs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_clne_nde(
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

/* "set_nde_txt_cnt" - implements the "Node"-cla-
 * ss "textContent" attribute setter, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory
 * allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_nde_txt_cnt(
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

/* "cll_nde_nrmlse" - implements the "Node"-cla-
 * ss "normalise" method using the language abstr-
 * action, "context" object, array of arguments,
 * number of arguments, memory allocator, error
 * logger and object to set, on exit, to any th-
 * rown exception in "lang", "ctx", "args", "nu-
 * m", "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_nde_nrmlse(
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

/* "get_nde_prnt_nde" - implements the "Node"-cla-
 * ss "parentNode" attribute getter, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory all-
 * ocator, error logger and object to set, on
 * exit, to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_nde_prnt_nde(
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

/* "get_nde_prnt_elem" - implements the "Node"-cla-
 * ss "parentElement" attribute setter, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory all-
 * ocator, error logger and object to set, on
 * exit, to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_nde_prnt_elem(
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

/* "cll_nde_is_eql_nde" - implements the "Node"-
 * class "isSameNode" method, using the "context"
 * object, array of arguments, number of argumen-
 * ts, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exception
 * in "lang", "ctx", "ctx", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_nde_is_eql_nde(
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

/* "cll_nde_cntns" - implements the "Node"- class
 * "contains" method, using the "context" object,
 * array of arguments, number of arguments, mem-
 * ory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "ctx", "num", "allocs","logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_nde_cntns(
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

/* "cll_rm_chld" - implements the "Node"- class
 * "removeChild" method, using the "context" object,
 * array of arguments, number of arguments, mem-
 * ory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "ctx", "num", "allocs","logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_nde_rm_chld(
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

/* "cll_nmlse" - implements the "Node"- class "n-
 * ormalise" method, using the "context" object,
 * array of arguments, number of arguments, mem-
 * ory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "ctx", "num", "allocs","logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_nrmlse(
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

/* "cll_nde_lkp_prfx" - implements the "Node"-
 * class "lookupPrefix" method, using the "th-
 * is" object, array of arguments, number of
 * arguments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *cll_nde_lkp_prfx(
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

/* "ccll_nde_is_dflt_ns" - implements the "Node"-
 * class "isDefaultNamespace" method, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory al-
 * locator, error logger and object to set, on
 * exit, to any thrown eexception in "lang", "c-
 * tx", "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_nde_is_dflt_ns(
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

/* "fget_doc_type" - implements the "Document"-cl-
 * ass "doctype" attribute getter, using the "th-
 * is" object, array of arguments, number of argu-
 * ments, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs", "l-
 * ogger" and "xcptn", respectively */
static struct bdlg_obj *get_doc_type(
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

/* "get_doc_impl" - implements the "Document"-
 * class "implementation" aribute getter, us-
 * ing the language abstraction, "context" o-
 * bject, array of arguments, number of argu-
 * ments, memory allocator, error logger and
 * object to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num", "a-
 * llocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *get_doc_impl(
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

/* "get_doc_doctype" - implements the "Docu-
 * ment"-class "doctype" aribute getter, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and object
 * to set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "lo-
 * gger" and "xcptn", respectively */
static struct bdlg_obj *get_doc_doctype(
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

/* "get_doc_doc_elem" - implements the "Docu-
 * ment"-class "documentElement" attribute g-
 * etter, using the language abstraction, "co-
 * ntext" object, array of arguments, number
 * of arguments, memory allocator, error log-
 * ger and object to set, on exit, to any th-
 * rown exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn", r-
 * espectively */
static struct bdlg_obj *get_doc_doc_elem(
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

/* "get_doc_cnt_type" - implements the "Docu-
 * ment"-class "contentType" attribute getter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_doc_cnt_type(
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

/* "cll_doc_crte_elem" - implements the "Docume-
 * nt"-class "createElement" method, using the
 * language abstraction, "context" object, arr-
 * ay of arguments, number of arguments, memo-
 * ry allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_elem(
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

/* "cll_doc_crte_doc_frg" - implements the "Doc-
 * ument"-class "createDocumentFragment" method,
 * using the language abstraction, "context" ob-
 * ject, array of arguments, number of arguments,
 * memory allocator, error logger and object to
 * set, on exit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_doc_frg(
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

/* "cll_doc_crte_txt_nde" - implements the "Doc-
 * ument"-class "createTextNode" method, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments, me-
 * mory allocator, error logger and object to
 * set, on exit, to any thrown exception in "l-
 * ang", "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_txt_nde(
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

/* "cll_doc_crte_wlkr" - implements the "Doc-
 * ument"-class "createTreeWalker" method, us-
 * ing the language abstraction, "context" ob-
 * ject, array of arguments, number of argume-
 * nts, memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_wlkr(
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

/* "cll_doc_crte_cmt" - implements the "Document-
 * nt"-class "createComment" method, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory al-
 * locator, error logger and object to set, on
 * exit, to any thrown eexception in "lang", "c-
 * tx", "args", "num", "allocs", "logger" and "x-
 * cptn", respectively */
static struct bdlg_obj *cll_doc_crte_cmt(
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

/* "get_doc_cmpt_mde" - implements the "Document-
 * nt"-class "compatMode" method, using the lang-
 * uage abstraction, "context" object, array of
 * arguments, number of arguments, memory alloc-
 * ator, error logger and object to set, on exit,
 * to any thrown eexception in "lang", "ctx", "a-
 * rgs", "num", "allocs", "logger" and "xcptn",
 respectively */
static struct bdlg_obj *get_doc_cmpt_mde(
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

/* "cll_doc_crte_cdta" - implements the "Document-
 * nt"-class "createCDATASection" method, using
 * the language abstraction, "context" object, a-
 * rray of arguments, number of arguments, memory
 * allocator, error logger and object to set, on
 * exit, to any thrown exception in "lang", "ctx",
 * "args", num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_doc_crte_cdta(
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

/* "cll_doc_crte_pi" - implements the "Document-
 * nt"-class "createProcessingInstruction" me-
 * thod, using the language abstraction, "cont-
 * ext" object, array of arguments, number of
 * arguments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and"xcptn", respective-
 * ly */
static struct bdlg_obj *cll_doc_crte_pi(
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

/* "cll_doc_crte_rnge" - implements the "Docu-
 * ment"-class "createRange" method, using the
 * language abstraction, "context" object, a-
 * rray of arguments, number of arguments, m-
 * emory allocator, error logger and object
 * to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and"xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_rnge(
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

/* "cll_doc_crte_evt" - implements the "Docu-
 * ment"-class "createEvent" method, using the
 * language abstraction, "context" object, a-
 * rray of arguments, number of arguments, m-
 * emory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "l-
 * ogger" and"xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_evt(
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

/* "cll_doc_crte_nde_it" - implements the "Do-
 * cument"-class "createNodeIterator" method,
 * using the language abstraction, "context"
 * object, array of arguments, number of arg-
 * uments, memory allocator, error logger and
 * object to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and"xcptn", respectiv-
 * ely */
static struct bdlg_obj *cll_doc_crte_nde_it(
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

/* "cll_doc_crte_attr" - implements the "Doc-
 * ument"-class "createAttribute" method, us-
 * ing the language abstraction, "context" ob-
 * ject, array of arguments, number of argum-
 * ents, memory allocator, error logger and o-
 * bject to set, on exit, to any thrown exce-
 * ption in "lang", "ctx", "args", "num", "a-
 * llocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_attr(
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

/* "cll_doc_get_elms_by_tg_nme" - implements the
 * "Document"-class "getElementsByTagName" meth-
 * od, using the language abstraction, "context"
 * object, array of arguments, array, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_get_elems_by_tg_nme(
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

/* "cll_doc_get_elms_by_clss_nme" - implements the
 * "Document"-class "getElementsByClassName" meth-
 * od, using the language abstraction, "context"
 * object, array of arguments, array, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_get_elems_by_clss_nme(
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

/* "cll_doc_prpnd" - implements the"Document"-
 * class "prepend" method, using the language-
 * abstraction, "contex" * object, array of ar-
 * guments, array, number of arguments, memory
 * allocator, error logger and object to set,
 * on exit, to any thrown except- ion in "la-
 ( ng", "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_prpnd(
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

/* "cll_doc_appnd" - implements the"Document"-
 * class "append" method, using the language-
 * abstraction, "contex" * object, array of ar-
 * guments, array, number of arguments, memory
 * allocator, error logger and object to set,
 * on exit, to any thrown except- ion in "la-
 ( ng", "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_appnd(
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

/* "cll_doc_crte_elem_ns" - implements the "Docum-
 * ent"-class "createElementNS" method, using the
 * language abstraction, "context" object, array
 * of arguments, number of ar- guments, memory al-
 * locator, error logger and object to set, on ex-
 * it, to any thrown eexception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_doc_crte_elem_ns(
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

/* "cll_doc_crte_attr_ns" - implements the "Docume-
 * nt"-class "createAttributeNS" method, using the
 * language abstraction, "context" object, array of
 * arguments, number of arguments, memory allocator,
 * error logger and object to set, on exit, to any
 * thrown exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_crte_attr_ns(
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

/* "cll_doc_get_elms_by_tg_nme_ns" - implements
 * the "Document"-class "getElementsByTagNameNS"
 * method, using the language abstraction, "con-
 * text" object, array of arguments, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown excep-
 * tion in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xptn", respectively */
static struct bdlg_obj *cll_doc_get_elems_by_tg_nme_ns(
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

/* "cll_doc_get_elm_by_id"- implements the "Docu-
 * m-ent"-class getElementByID" method, using the
 * language abstraction, "context" object, array
 * of arguments, number of arguments, memory all-
 * ocator, error logger and object to set, on ex-
 * it, to any thrown eexception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_doc_get_elem_by_id(
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

/* "cll_doc_imprt_nde" - implements the "Document"-
 * class "importNode" method, using the "this" obj-
 * ect, array of arguments, number of arguments, me-
 * mory allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang", "c-
 * tx" "args", "num", "allocs", "logger" and "xc-
 * ptn", respectively */
static struct bdlg_obj *cll_doc_imprt_nde(
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

/* "get_doc_in_enc" - implements the Documen-
 * t"-class "inputEncoding" attributgetter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "loger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_doc_in_enc(
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

/* "get_doc_chrst" - implements the Documen-
 * t"-class "characterSet" attributgetter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "loger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_doc_chrst(
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

/* "get_doc_xml_enc" - implements the Docu-
 * ment"-class "xmlEncoding" attributgetter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_doc_xml_enc(
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

/* "get_doc_stndlne" - mplements the Docu-
 * ment"-class "xmlStandalone" attribute
 * getter, using the language abstraction,
 * "context" object, set of arguments, nu-
 * mber of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *get_doc_stndlne(
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

/* "set_doc_stndlne" - implements the Docu-
 * ment"-class "xmlStandalone" attribute
 * setter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "l- ogger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_doc_stndlne(
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

/* "get_doc_doc_uri" - implements the Doc-
 * ument"-class "documentURI" attribute g-
 * etter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *get_doc_doc_uri(
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

/* "cll_doc_adpt_nde" - implements the Document"-
 * class "adoptNode" method, using the "this" ob-
 * ject, array of arguments, number of arguments,
 * memory allocator, error logger and object to
 * set, on exit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_doc_adpt_nde(
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

/* "cll_doc_nmlse" - implements the Document"-
 * class "normalize" method, using the "this"
 * object array of arguments, number of argum-
 * ents, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *cll_doc_nmlse_doc(
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

/* "set_doc_vrsn" - implements the Document"-
 * class "xmlVersion" attribute setter, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and object
 * to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xc ptn",  respectively */
static struct bdlg_obj *set_doc_vrsn(
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

/* "get_doc_vrsn" - implements the Document"-
 * class "xmlVersion" attribute getter, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thro-
 * wn exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_doc_vrsn(
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

/* "get_strct_errs" - implements the Document"-
 * class "strlctErrorChecking" attribute gett-
 * er, using the language abstraction, "cont-
 * ext" object, array of arguments, number of
 * arguments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_strct_errs(
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

/* "get_doctype_nme" - implements the "Document-
 * Type"-class "name" method, using the "this"
 * object, array of arguments, number of argu-
 * ments, memory allocator, error log-ger and
 * object to set, on exit, to any thrown eexc-
 * epti- on in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respectiv-
 * ely */
static struct bdlg_obj *get_doctype_nme(
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

/* "get_doctype_ents" - implements the "Document-
 * Type"-class "entities" method, using the "th-
 * is" object, array of arguments, number of arg-
 * uments, memory allocator, error logger and ob-
 * ject to set, on exit, to any thrown eexcepti-
 * on in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_doctype_ents(
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

/* "get_doctype_ntns" - implements the "Document-
 * Type"-class "notations" method, using the "th-
 * is" object, array of arguments, number of argu-
 * ments, memory allocator, error logger and ob-
 * ject to set, on exit, to any thrown eexcepti-
 * on in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_doctype_ntns(
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

/* "get_doctype_pub" - implements the "Document-
 * Type"-class "pubicId" method, using the "th-
 * is" object, array of arguments, number of arg-
 * uments, memory allocator, error logger and
 * object to set, on exit, to any thrown eexcep-
 * tion in "lang", "ctx", "args", "num", "allo-
 * cs", "logger" and "xcptn", respectively */
static struct bdlg_obj *get_doctype_pub(
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

/* "get_doctype_sys" - implements the "Document-
 * Type"-class "systemId" method, using the "this"
 * object, array of arguments, number of arguments,
 * memory allocator, error logger and object to
 * set, on exit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doctype_sys(
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

/* "get_doctype_intrnlsbst" - implements the "Doc-
 * umentType"- class "internalSubset" method, usi-
 * ng the language abstraction, "context" object,
 * array of arguments, number of arguments, memo-
 * ry allocator, error logger and object to set,
 * on exit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doctype_intrnl_sbst(
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

/* "get_elem_tg_nme" - implements the "Eleme-
 * nt"-class "tagName" attribute,getter usin-
 * g the language abstraction, "context" obj-
 * ect, array of arguments, number of argume-
 * nts, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_tg_nme(
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

/* "get_elem_prfx" - implements the "Eleme-
 * nt"-class "prefix" attribute,getter usin-
 * g the language abstraction, "context" obj-
 * ect, array of arguments, number of argume-
 * nts, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_prfx(
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

/* "get_elem_lcl_nme" - implements the "Ele-
 * ment"-class "localName" attribute getter
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_lcl_nme(
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

/* "get_elem_clss_nme" - implements the "Ele-
 * ment"-class "className" attribute getter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_clss_nme(
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

/* "get_elem_clss_lst" - implements the "Ele-
 * ment"-class "classList" attribute getter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_clss_lst(
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

/* "get_elem_chldrn" - implements the "Ele-
 * ment"-class "children" attribute getter,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_chldrn(
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

/* "get_elem_frst_elem_chld" - implements the
 * "Element"-class "firstElementChild" attrib-
 * ute getter, using the language abstraction,
 * "context" object, array of arguments, num-
 * ber of arguments, memory allocator, error
 * logger and object to set, on exit, to any
 * thrown exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn", res-
 * pectively */
static struct bdlg_obj *get_elem_frst_elem_chld(
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

/* "get_elem_ns_uri" - implements the "Ele-
 * ment"-class "namespaceURI" attribute getter
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_elem_ns_uri(
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

/* "get_elem_id" - implements the "Element"-
 * class "id" attribute getter, using the lan-
 * guage abstraction, "context" object, arr-
 * ay of arguments, number of arguments, mem-
 * ory allocator, error logger and object to
 * set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_elem_id(
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

/* "get_elem_frst_elem_sblng" - implements
 * the Element"-class "firstElementSibling"
 * attribute getter, using the language abs-
 * traction, "context" object, array of ar-
 * guments, number of arguments, memory al-
 * locator, error logger and object to set,
 * on exit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs", "l-
 * ogger" and "xcptn", respectively */
static struct bdlg_obj *get_elem_frst_elem_sblng(
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

/* "get_elem_elem_cnt" - implements the "E-
 * lement"-class "elementCount" attribute
 * getter, using the language abstraction,
 * "context" object, array of ar- guments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_elem_elem_cnt(
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

/* "get_elem_lst_elem_sblng" - implements
 * the Element"-class "lastElementSibling"
 * attribute getter, using the language abs-
 * traction, "context" object, array of ar-
 * guments, number of arguments, memory al-
 * locator, error logger and object to set,
 * on exit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs", "l-
 * ogger" and "xcptn", respectively */
static struct bdlg_obj *get_elem_lst_elem_sblng(
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

/* "cll_elem_get_attr" - implements the "Element"-
 * class "getAttribute" method, using the "this"
 * object, array of arguments, number of argumen-
 * ts, memory allocator, error logger and object
 * to set, on exit, to any thrown exception in
 * "lang", "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_get_attr(
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

/* "cll_elem_get_attr_ns" - implements the "Ele-
 * ment"-class "getAttributeNS" method, using t-
 * he language abstraction, "context" object, a-
 * rray of arguments, number of arguments, mem-
 * ory allocator, error logger and object to set,
 * on exit, to any thrown eexception in "lang",
 * "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_get_attr_ns(
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

/* "cll_elem_rmve_attr_ns" - implements the
 * "Element"-class "removeAttributeNS" method,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respectiv-
 * ely */
static struct bdlg_obj *cll_elem_rmve_attr_ns(
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

/* "set_elem_prfx" - implements the "Element"-
 * class "prefix" method, using the "context"
 * object, array of arguments, number of arg-
 * uments, memory allocator, error logger and
 * object to set, on exit, to any thrown exce-
 * ption in "lang", "ctx", "args", "num", "al-
 * locs", "logger" and "xcptn", respectively */
static struct bdlg_obj *set_elem_prfx(
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

/* "cll_elem_get_attr_nde" - implements the
 * "Element"- class "getAttributeNode" meth-
 * od, using the language abstraction, "con-
 * text" object, array of arguments, number
 * of arguments, memory allocator, error lo-
 * gger and object to set, on exit, to any
 * thrown exception in "lang", "ctx" "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_get_attr_nde(
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

/* "cll_elem_rm_attr_ns" - implements the "Ele-
 * ment"- class "removeAttribute" method, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments, me-
 * mory allocator, error logger and object to
 * set, on exit, to any thrown exception in "l-
 * ang", "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_rm_attr_ns(
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

/* "cll_elem_set_attr_ns" - implements the "El-
 * ement"- class "setAttributeNS" method, us-
 * ing the language abstraction, "context" obj-
 * ect, array of arguments, number of arguments,
 * memory allocator, error logger and object to
 * set, on exit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_elem_set_attr_ns(
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

/* "cll_elem_rm_attr" - implements the "Eleme-
 * nt"- class "removeAttribute" method, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments, m-
 * emory allocator, error logger and object to
 * set, on exit, to any thrown exception in "l-
 * ang", "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_rm_attr(
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

/* "cll_elem_hs_attr" - implements the "El-
 * ement"- class "hasAttribute" method, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of a-
 * rguments, memory allocator, error logger
 * and object to set, on exit, to any thr-
 * own exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_hs_attr(
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

/* "cll_elem_prpnd" - implements the "Ele-
 * ment"- class "prepend" method, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *cll_elem_prpnd(
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

/* "cll_elem_appnd" - implements the "Ele-
 * ment"- class "append" method, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *cll_elem_appnd(
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

/* "cll_elem_bfre" - implements the "Ele-
 * ment"- class "before" method, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *cll_elem_bfre(
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

/* "cll_elem_aftr" - implements the "Ele-
 * ment"- class "after" method, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *cll_elem_aftr(
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

/* "cll_elem_rmve" - implements the "Ele-
 * ment"- class "remove" method, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obj-
 * ect to set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respe-
 * ctively */
static struct bdlg_obj *cll_elem_rmve(
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

/* "cll_elem_set_attr" - implements the "El-
 * ement"- class "setAttribute" method, us-
 * ing the language abstraction, "context"
 * object, array of arguments, number of a-
 * rguments, memory allocator, error logger
 * and object to set, on exit, to any thr-
 * own exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_set_attr(
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

/* "cll_elem_hs_attr_ns" - implements the
 * "Element"- class "hasAttribute" method,
 * using the language abstraction, "conte-
 * xt" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_hs_attr_ns(
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

/* "cll_elem_hset_id_attr" - implements the
 * "Element"- class "setIdAttribute" method,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_hset_id_attr(
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

/* "cll_elem_get_elems_by_tg_nme" - implements
 * the "Element"- class "getElementsByTagName"
 * method, using the language abstraction, "co-
 * ntext" object, array of arguments, number
 * of arguments, memory allocator, error log-
 * ger and object to set, on exit, to any th-
 * rown exception in "lang", "ctx" "args", "n-
 * um", "allocs", "logger" and "xcptn", resp-
 * ectively */
static struct bdlg_obj *cll_elem_get_elems_by_tg_nme(
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

/* "cll_elem_get_elems_by_clss_nme" - implemen-
 * ts the "Element"- class "getElementsByClass-
 * Name" method, using the language abstracti-
 * on, "context" object, array of arguments,
 * number of arguments, memory allocator, err-
 * or logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx" "ar-
 * gs", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_get_elems_by_clss_nme(
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

/* "cll_elem_get_elems_by_tg_nme_ns" - impleme-
 * nts the "Element"- class "getElementsByTag-
 * NameNS" method, using the language abstrac-
 * tion, "context" object, array of arguments,
 * number of arguments, memory allocator, err-
 * or logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx" "ar-
 * gs", "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_elem_get_elems_by_tg_nme_ns(
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

/* "cll_elem_rmve_attr" - implements the "Element"-
 * class "removeAttribute" method, using the "t-
 * his" object, array of arguments, number of arg-
 * uments, memory allocator, error logger and ob-
 * ject to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs", "l-
 * ogger" and "xcptn", respectively */
static struct bdlg_obj *cll_elem_rmve_attr(
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

/* "cll_txt_splt_txt" - implements the "Text"-
 * class "splitText" method, using the "this"
 & * object, array of arguments, number of arg-
 * uments, memory allocator, error logger and
 * object to set, on exit, to any thrown eexcep-
 * tion in "lang", "ctx", "args", "num", "all-
 * ocs", "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_txt_splt_txt(
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

/* "get_txt_elem_cnt_ws" - implements the "Text"-
 * class "elementContentWhitespace" attribute
 * getter, using the language abstraction, "con-
 * text" object, array of arguments, number of
 * arguments, memory allocator, error logger and
 * object to set, on exit, to any thrown except-
 * ion in "lang", "ctx", "args", "num", "allo-
 * cs", "logger" and "xcptn", respectively */
static struct bdlg_obj *get_txt_elem_cnt_ws(
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

/* "get_txt_whle_txt" - implements the "Text"-
 * class "wholeText" attribute getter, using
 * the language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and object
 * to set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_txt_whle_txt(
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

/* "cll_txt_rplce_whle_txt" - implements the
 * "Text"-class "replaceWholeText" method,
 * using the language abstraction, "context"
 * object, array of arguments, number of ar-
 * guments, memory allocator, error logger
 * and object to set, on exit, to any thrown
 * exception in "lang", "ctx" "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_txt_rplce_whle_txt(
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

/* "get_cdta_len" - implements the "Chara-
 * cterData"-class "length" attribute get-
 * ter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_cdta_len(
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

/* "get_cdta_dta" - implements the "Chara-
 * cterData"-class "data" attribute get-
 * ter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_cdta_dta(
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

/* "set_cdta_dta" - implements the "Chara-
 * cterData"-class "data" attribute setter,
 * using the language abstraction, "conte-
 * xt" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *set_cdta_dta(
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

/* "cll_cdta_appnd_dta" - implements the
 * "CharacterData"-class "appendData" m-
 * ethod, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "logg-
 * er" and "xcptn", respectively */
static struct bdlg_obj *cll_cdta_appnd_dta(
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

/* "cll_cdta_sbstr_dta" - implements the
 * "CharacterData"-class "substringData"
 * method, using the language abstractio-
 * n, "context" object, array of argumen-
 * ts, number of arguments, memory allo-
 * cator, error logger and object to set,
 * on exit, to any thrown exception in
 ( "lang", "ctx", "args", "num", "allocs",
 ( "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_cdta_sbstr_dta(
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

/* "cll_cdta_insrt_dta" - implements the
 * "CharacterData"-class "insertData" m-
 * ethod, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on ex-
 * it, to any thrown eexception in "lang",
 * "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_cdta_insrt_dta(
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

/* "cll_cdta_del_dta" - implements the
 * "CharacterData"-class "deleteData" m-
 * ethod, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on e-
 * xit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_cdta_del_dta(
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

/* "cll_cdta_del_dta" - implements the
 * "CharacterData"-class "replaceData" m-
 * ethod, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on e-
 * xit, to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "log-
 * ger" and "xcptn", respectively */
static struct bdlg_obj *cll_cdta_rplce_dta(
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

/* "get_pi_dta" - implements the "Proce-
 * ssingInstruction"-class "data" attrib-
 * ute getter, using the language abstr-
 * action, "context" object, array of a-
 * rguments, number of arguments, memory
 * allocator, error logger and object to
 * set, on exit, to any thrown exception
 * in "lang", "ctx", "args", "num", "al-
 * locs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_pi_dta(
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

/* "get_pi_trgt" - implements the "Pro-
 * cessingInstruction"-class "target" at-
 * tribute getter, using the "context"
 * object, array of arguments, number of
 * arguments, memory allocator, error lo-
 * gger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_pi_trgt(
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

/* "get_attr_nme" - implements the "Att-
 * r"-class "name" attribute getter, us-
 * ing the language abstraction, "conte-
 * xt" object, array of arguments, num-
 ( ber of arguments, memory allocator,
 * error logger and object to set, on e-
 * xit, to any thrown exception in "lan-
 * g", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *get_attr_nme(
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

/* "get_attr_val" - implements the "Attr-
 * ibute-class "value" attribute getter,
 * using the language abstraction, "con-
 * text" object, array of arguments, nu-
 * mber of arguments, memory allocator,
 * error logger and object to set, on e-
 * xit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs",
 * "lgger" and "xcptn", respectively */
static struct bdlg_obj *get_attr_val(
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

/* "get_attr_prfx" - implements the "Attr"-
 * class "prefix" attribute getter, using
 * the language abstraction, "context" obj-
 * ect, array of arguments, number of arg-
 * uments, memory allocator, error logger
 * and object to set, on exit, to any thr-
 * own exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *get_attr_prfx(
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

/* "get_attr_spcfd" - implements the "Attr"-
 * class "specified" attribute getter, using
 * the language abstraction, "context" obj-
 * ect, array of arguments, number of argum-
 * ents, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_attr_spcfd(
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

/* "get_attr_ownr" - implements the "Attr"-
 * class "owner" attribute getter, using
 * the language abstraction, "context" obj-
 * ect, array of arguments, number of argum-
 * ents, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_attr_ownr(
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

/* "get_attr_schma" - implements the "Attr"-
 * class "schema" attribute getter, using
 * the language abstraction, "context" obj-
 * ect, array of arguments, number of argum-
 * ents, memory allocator, error logger and
 * object to set, on exit, to any thrown ex-
 * ception in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_attr_schma(
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

/* "get_attr_is_id" - implements the "Attr"-
 * class "isID" attribute getter, using the
 * language abstraction, "context" object,
 * array of arguments, number of arguments,
 * memory allocator, error logger and obje-
 * ct to set, on exit, to any thrown exce-
 * ption in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *get_attr_is_id(
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

/* "get_attr_ns_uri" - implements the
 * "Attr-class "localName" attribute
 * getter, using the language abstrac-
 * tion, "context" object, array of
 * arguments, number of arguments, me-
 * mory allocator, error logger and ob-
 * ject to set, on ex- it, to any thro-
 * wn exception in "lang", "ctx", "ar-
 * gs", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_attr_ns_uri(
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

/* "cll_impl_hs_ftre" - implements the
 * "Implementation"-class "hasFeature"
 * method, using the language abstrac-
 * tion, "context" object, array of a-
 * rguments, number of arguments, mem-
 * ory allocator, error logger and ob-
 * ject to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *cll_impl_hs_ftre(
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

/* "cll_impl_get_ftre" - implements the
 * "Implementation"-class "getFeature"
 * method, using the language abstrac-
 * tion, "context" object, array of a-
 * rguments, number of arguments, mem-
 * ory allocator, error logger and ob-
 * ject to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *cll_impl_get_ftre(
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

/* "cll_impl_crte_doctype" - implements
 * the "Implementation"-class "createD-
 * ocumentType" method, using the "this"
 * object, array of arguments, number
 * of arguments, memory allocator, err-
 * or logger and object to set, on exit,
 * to any thrown exception in "lang", "c-
 * tx" "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_impl_crte_doctype(
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

/* "cll_impl_crte_doc" - implements the
 * "Implementation"-class "createDocum-
 * ent" method, using the language abst-
 * raction, "context" object, array of
 * arguments, number of arguments, mem-
 * ory allocator, error logger and obj-
 * ect to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "log- ger" and "xc-
 * ptn", respectively */
static struct bdlg_obj *cll_impl_crte_doc(
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

/* "cll_impl_crte_html_doc" - implemen-
 * ts the "Implementation"-class "crea-
 * teHTMLDocument" method, using the
 * language abstraction, "context" obj-
 * ect, array of arguments, number of
 * arguments, memory allocator, error
 * logger and object to set, on exit,
 * to any thrown exception in "lang",
 * "ctx", "args", "num", "allocs", "lo-
 * gger" and "xcptn", respectively */
static struct bdlg_obj *cll_impl_crte_html_doc(
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

/* "cll_strlst_itm" - implements the "S-
 * tringList"-class "item index method,
 * using the "this"  object, array of
 * arguments, number of arguments, me-
 * mory allocator, error logger and ob-
 * ject to set, on exit, to any thrown
 * exception in "lang", "ctx", "args",
 * "num", "allocs", "logger" and "xcp-
 * tn", respectively */
static struct bdlg_obj *cll_strlst_itm(
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

/* "cll_strlst_cntns" - implements
 * the "StringList"-class "contains"
 * method, using the language abst-
 * raction, "context" object, array
 * of arguments, number of arguments,
 * memory allocator, error logger
 * and object to set, on exit, to
 * any thrown exception in "lang",
 * "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectiv-
 * ely */
static struct bdlg_obj *cll_strlst_cntns(
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

/* "cll_nmelst_get_nme" - implements
 * the "NameList"-class "getName" m-
 * ethod, using the language abstra-
 * ction, "context" object, array of
 * arguments, number of arguments, m-
 * emory allocator, error logger and
 * object to set, on exit, to any th-
 * rown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_nmelst_get_nme(
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

/* "cll_nmelst_get_ns_uri" - implem-
 * ents the "NameList"-class "getN-
 * amespaceURI" method, using the
 * language abstraction, "context"
 * object, array of arguments, num-
 * ber of arguments, memory alloc-
 * ator, error logger and object to
 * set, on exit, to any thrown exc-
 * eption in "lang", "ctx", "args",
 * "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_nmelst_get_ns_uri(
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

/* "cll_nmelst_cntns" - implements
 * the "NameList"-class "contains"
 * method, using the language abs-
 * traction, "context" object, ar-
 * ray of arguments, number of ar-
 * guments, memory allocator, err-
 * or logger and object to set, on
 * exit, to any thrown exception
 * in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_nmelst_cntns(
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

/* "cll_nmelst_cntns_ns" - im-
 * plements the "NameList"-cl-
 * ass "containsNS" method, u-
 * sing the language abstrac-
 * tion, "context" object, arr-
 * ay of arguments, number of
 * arguments, memory allocator,
 * error logger and object to
 * set, on exit, to any thrown
 * exception in "lang", "ctx",
 * "args", "num", "allocs", "l-
 * ogger" and "xcptn", respec-
 * tively */
static struct bdlg_obj *cll_nmelst_cntns_ns(
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

/* "get_nmelst_len" - mplements
 * the "NameList"-class "length"
 * attrinute getter, using the
 * language abstraction, "cont-
 * ext" object, array of argum-
 * ents, number of arguments,
 * memory allocator, error log-
 * ger and object to set, on e-
 * xit, to any thrown exception
 * in "lang", "ctx", "args", "n-
 * um", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_nmelst_len(
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

/* "get_evt_trgt" - implements the
 * "Event"-class "target" attribu-
 * te getter, using the "xcontext"
 * object, array of arguments, num-
 * ber of arguments, memory alloc-
 * ator, error logger and object
 * to set, on exit, to any thrown
 * exception in "lang", "ctx", "a-
 * rgs", "num", "allocs", logger"
 ( and "xcptn", respectively */
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

/* "get_evt_curr_trgt" - implements
 * the "Event"-class "currentTarget"
 * attribute getter, using the "co-
 * ntext" object, array of argumen-
 * ts, number of arguments, memory
 * allocator, error logger and obj-
 * ect to set, on exit, to any thr-
 * own exception in "ctx", "args",
 * "num", "allocs", "logger" and
 * "xcptn", respectively */
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

/* "get_evt_phse" - implements the
 * "Event"-class "currentPhase" att-
 * ribute getter, using the langua-
 * ge-abstraction, "context" obje-
 * ct, array of arguments, number
 * of arguments, memory allocator,
 * error logger and object to set,
 * on exit, to any thrown excepti-
 * on in "lang", "ctx", "args",
 * "num", "allocs", "logger" and
 * "xcptn", respectively */
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

/* "get_evt_bbls" - implements the
 * "Event"-class "bubbles" attrib-
 * ute getter, using the "fcontext"
 * object, array of arguments, num-
 * ber of arguments, memory allo-
 * cator, error logger and object
 * to set, on exit, to any thrown
 * exception in "lang", "ctx", "a-
 * rgs", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *get_evt_bbls(
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

/* "get_evt_cnclble" - implements
 * the "Event"-class "cancelable"
 * attribute getter, using the "t-
 * his" object, array of arguments,
 * number of arguments, memory all-
 * ocator, error logger and object
 * to set, on exit, to any thrown
 * exception in "lang", "ctx", "a-
 * rgs", "num", "allocs", "logger"
 * and "xcptn", respectively */
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

/* "get_evt_dflt_prvntd" - implem-
 * ents the "Event"-class "refaul-
 * tPrevented" attribute getter,
 * using the language abstraction,
 & "context" object, array of arg-
 * uments, number of arguments,
 * memory allocator, error logger
 * and object to set, on exit, to
 * any thrown exception in "lang",
 * "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respecti-
 * vely */
static struct bdlg_obj *get_dflt_prvntd(
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

/* "get_evt_is_trstd" - implem-
 * ents the "Event"-class "isTr-
 * usted" attribute getter, us-
 * ing the language abstraction,
 * "context" object, array of
 * arguments, number of arg-um-
 * ents, memory allocator, error
 * logger and object to set, on
 * exit, to any thrown exception
 * in "lang", "ctx", "args",
 * "num", "allocs", "logger"
 * and "xcptn", respectively */
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

/* "get_evt_tme_stmp" - imple-
 * ments the "Event"- class
 * "timeStamp" attribute get-
 * ter, using the "this" obj-
 * ect, array of arguments,
 * number of arguments, me-
 * mory allocator, error lo-
 * gger and object to set,
 * on exit, to any thrown e-
 * xception in "lang", "ctx",
 * "args", "num", "allocs",
 * "logger" and "xcptn", re-
 * spectively */
static struct bdlg_obj *get_evt_tme_stmp(
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

/* "cll_evt_stp_prpgtn" - imple-
 * ments the "Event"- class "st-
 * opPropagation" method, using
 * the language abstraction, "c-
 * ontext" object, array of ar-
 * guments, number of arguments,
 * memory allocator, error log-
 * ger and object to set, on ex-
 * it, to any thrown exception
 * in "lang", "ctx", "args", "n-
 & um", "allocs", "logger" and
 * "xcptn", respectively */
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

/* "cll_evt_prvnt_dflt" - impl-
 * ements the "Event"- class
 * "preventDefauult" method, u-
 * sing the language abstract-
 * ion, "context" object, array
 * of arguments, number of arg-
 * uments, memory allocator, er-
 * ror logger and object to set,
 * on exit, to any thrown excep-
 * tion in "lang", "ctx", "args",
 * "num", "allocs", "logger" a-
 * nd "xcptn", respectively */
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

/* "cll_evt_evt_init" - impleme-
 * nts the "Event"- class "event-
 * Init" method, using the "thi-
 * s" object, array of arguments,
 * number of arguments, memory a-
 * llocator, error logger and ob-
 * ject to set, on exit, to any
 * thrown exception in "lang",
 * "ctx" "args", "num", "allocs",
 * "logger" and "xcptn", respec-
 * tively */
static struct bdlg_obj *cll_evt_evt_init(
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

/* "cll_evt_trgt_add_evt_lstnr" -
 * implements the "EventTarget"-
 * class "addEventListner" meth-
 * od, using the language abstr-
 * action, "context" object, ar-
  ray of arguments, number of
 * arguments, memory allocator,
 * error logger and object to
 * set, on exit, to any thrown
 * exception in "lang", "ctx",
 * "args", "num", "allocs", "l-
 * ogger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_evt_trgt_add_evt_lstnr(
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

/* "cll_evt_trgt_rmve_evt_lstnr" -
 * implements the "EventTarget"-
 * class "removeEventListner" meth-
 * od, using the language abstra-
 * ction, "context" object, array
 * of arguments, number of argum-
 * ents, memory allocator, error
 * logger and object to set, on
 * exit, to any thrown exception
 * in "lang", "ctx", "args", "nu-
 * m", "allocs", "logger" and "x-
 * cptn", respectively */
static struct bdlg_obj *cll_evt_trgt_rmve_evt_lstnr(
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

/* "cll_evt_trgt_dsptch_evt" - imp-
 * lements the "eventTarget"-class
 * "dispatchEvent" method, using
 * the language abstraction, "con-
 * text" object, array of argume-
 * nts, number of arguments, mem-
 * ory allocator, error logger and
 * object to set, on exit, to any
 * thrown exception in "lang", "ct-
 * x", "args", "num", "allocs",
 * "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_evt_trgt_dsptch_evt(
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

/* "cll_doc_type_bfre" - implemen-
 * ts the "DocumentType"-class "be-
 * fore" method, using the language
 * abstraction, "context" object,
 * array of arguments, number of
 * arguments, memory allocator, e-
 * rror logger and object to set,
 * on exit, to any thrown exception
 * in "lang", "ctx", "args", "num",
 * "allocs", "logger" and "xcptn",
 * respectively */
static struct bdlg_obj *cll_doc_type_bfre(
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

/* "cll_doc_type_aftr" - implemen-
 * ts the "DocumentType"-class "af-
 * fter" method, using the language
 * abstraction, language abstract-
 * ion, "context" object, array of
 * arguments, number of arguments,
 * memory allocator, error logger
 * and objec to set, on exit, to
 * any thrown exception in "lang",
 * "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_doc_type_aftr(
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

/* "cll_doc_type_rplce" - implemen-
 * ts the "DocumentType"-class "re-
 * plce" method, using the language
 * abstraction, language abstract-
 * ion, "context" object, array of
 * arguments, number of arguments,
 * memory allocator, error logger
 * and object to set, on exit, to
 * any thrown exception in "lang",
 * "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respect-
 * ively */
static struct bdlg_obj *cll_doc_type_rplce(
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

/* "cll_doc_type_rmve" - implemen-
 * ts the "DocumentType"-class "re-
 * move" method, using the language
 * abstraction, "context" object,
 * array of arguments, number of
 * arguments, memory allocator, er-
 * ror logger and object to set,
 * on exit, to any thrown excepti-
 * on in "lang", "ctx", "args",
 * "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_doc_type_rmve(
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

/* "get_doc_type_nme" - imple-
 * ments the "DocumentType"-
 * class "name" attribute ge-
 * tter, using the language
 * abstraction, "context" ob-
 * ject, array of arguments,
 * number of arguments, memo-
 * ry allocator, error logger
 * and object to set, on exit,
 * to any thrown exception in
 * "lang", "ctx", "args", "n-
 * um", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_type_nme(
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

/* "get_doc_type_pub" - imple-
 * ments the "DocumentType"-class
 * "publicId" attribute getter,
 * using the language abstrac-
 * tion, "context" object, array
 * of arguments, number of argum-
 * ents, memory allocator, error
 * logger and object to set, on
 * exit, to any thrown exception
 * in "lang", "ctx", "args", "n-
 * um", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_type_pub(
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

/* "get_doc_type_sys" - imple-
 * ments the "DocumentType"-class
 * "systemId" attribute getter,
 * using the language abstrac-
 * tion, "context" object, array
 * of arguments, number of argum-
 * ents, memory allocator, error
 * logger and object to set, on
 * exit, to any thrown exception
 * in "lang", "ctx", "args", "n-
 * um", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_type_sys(
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

/* "bddm_add_nde_prps" - returns a lang-
 * uage abstracted object populated with
 * the methods and attributes of the DOM
 * Node given in "nde", using the memory
 * allocator and error logger given in
 * "allocs" and "logger", respectively.
 * Returns NULL on error */
struct bdlg_obj *bddm_add_nde_prps(
                       struct bdxl_node *nde,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdlg_obj *ret;         /* return value */
    struct bdxl_node *nde = NULL; /* super-class
                                   *  object */

    /* start "ret" with the entries
     * for an "eventTarget"-classed
     * object */

    if (!(ret = bdet_add_trgt_prps(&nde->trgt,
                                   allocs,
                                   logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "nodeName",
                         (bdlg_cll_fn)
                             get_nde_nde_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "nodeName",
                         (bdlg_cll_fn)
                             set_nde_nde_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "nodeValue",
                         (bdlg_cll_fn)
                             get_nde_nde_val,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "nodeValue",
                         (bdlg_cll_fn)
                             set_nde_nde_val,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "baseURI",
                         (bdlg_cll_fn)
                             get_nde_bse_uri,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "baseURI",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "nodeType",
                         (bdlg_cll_fn)
                             get_nde_nde_type,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "nodeValue",
                         (bdlg_cll_fn)
                             get_nde_nde_val,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "nodeValue",
                         (bdlg_cll_fn)
                             set_nde_nde_val,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "nodeValue",
                         (bdlg_cll_fn)
                             get_nde_txt_cnt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "nodeValue",
                         (bdlg_cll_fn)
                             set_nde_txt_cnt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "parentNode",
                         (bdlg_cll_fn)
                             get_nde_prnt_nde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "parentNode",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "parentElement",
                         (bdlg_cll_fn)
                             get_nde_prnt_elem,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "parentElement",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "ownerDocument",
                         (bdlg_cll_fn)
                             get_nde_ownr_doc,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "ownerDocument",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "childNodes",
                         (bdlg_cll_fn)
                             get_nde_chldrn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "childNodes",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "firstChild",
                         (bdlg_cll_fn)
                             get_nde_frst_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "firstChild",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "firstChild",
                         (bdlg_cll_fn)
                             get_nde_frst_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "firstChild",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "lastChild",
                         (bdlg_cll_fn)
                             get_nde_lst_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "lastChild",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "nextSibling",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "previousSibling",
                         (bdlg_cll_fn)
                             get_nde_prv_sblng,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "previousSibling",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "hasChildNodes",
                         (bdlg_cll_fn)
                             cll_nde_hs_chld_ndes,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "compareDocumentPosition",
                         (bdlg_cll_fn)
                             cll_nde_cmp_doc_pstn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "contains",
                         (bdlg_cll_fn)
                             cll_nde_cntns,
                         allocs,
                         logger)))
        return NULL;


    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "insertBefore",
                         (bdlg_cll_fn)
                             cll_nde_insrt_bfre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "removeChild",
                         (bdlg_cll_fn)
                             cll_nde_rm_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "appendChild",
                         (bdlg_cll_fn)
                             cll_nde_appnd_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "replaceChild",
                         (bdlg_cll_fn)
                             cll_nde_rplce_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "normalize",
                         (bdlg_cll_fn)
                             cll_nde_nrmlse,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "textContent",
                         (bdlg_cll_fn)
                             get_nde_txt_cnt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "textContent",
                         (bdlg_cll_fn)
                             set_nde_txt_cnt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "lookupPrefix",
                         (bdlg_cll_fn)
                             cll_nde_lkp_prfx,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "lookupNamespaceURI",
                         (bdlg_cll_fn)
                             cll_nde_lkp_prfx,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "isDefaultNamepace",
                         (bdlg_cll_fn)
                             cll_nde_is_dflt_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "lookupNamepaceURI",
                         (bdlg_cll_fn)
                             cll_nde_lkp_ns_uri,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "insertBefore",
                         (bdlg_cll_fn)
                             cll_nde_insrt_bfre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "appendChild",
                         (bdlg_cll_fn)
                             cll_nde_appnd_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "replaceChild",
                         (bdlg_cll_fn)
                             cll_nde_rplce_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "removeChild",
                         (bdlg_cll_fn)
                             cll_nde_rmve_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "normalize",
                         (bdlg_cll_fn)
                             cll_nde_nrmlse,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "isEqualNode",
                         (bdlg_cll_fn)
                             cll_nde_is_eql_nde,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "cloneNode",
                          (bdlg_cll_fn)
                              cll_nde_clne_nde,
                          allocs,
                          logger);
}

/* "bddm_add_doc_prps" populates the lan-
 * guage-abstracted object in "doc" with
 * the methods and attributes of a DOM
 * Document, using the memory allocator
 * and error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
struct bdlg_obj *bddm_add_doc_prps(
                      struct bdxl_doc *doc,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdlg_obj *ret;         /* return
                                   * value */
    struct bdxl_node *nde = NULL;  /* Node super-
                                   * class object */
	void *opaque  = doc->nde.usr; /* opaque
                                   * value */

    /* start "doc" with the entries
     * for a "Node"-classed object */
    if (!(ret = bddm_add_nde_prps(nde,
                                  allocs,
                                  logger)))
        return NULL;

    /* then add entries for a "Document"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "implementation",
                         (bdlg_cll_fn)
                             get_doc_impl,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "implementation",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "characterSet",
                         (bdlg_cll_fn)
                             get_doc_chrst,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "characterSet",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "doctype",
                         (bdlg_cll_fn)
                             get_doc_doctype,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "docType",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "documentElement",
                         (bdlg_cll_fn)
                             get_doc_doc_elem,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "documentElement",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "documentURI",
                         (bdlg_cll_fn)
                             get_doc_doc_uri,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "documentURI",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "compatMode",
                         (bdlg_cll_fn)
                             get_doc_cmpt_mde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "compatMode",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "characterSet",
                         (bdlg_cll_fn)
                             get_doc_chrst,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "characterSet",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "contentType",
                         (bdlg_cll_fn)
                             get_doc_cnt_type,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "contentType",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createElement",
                         (bdlg_cll_fn)
                             cll_doc_crte_elem,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createElementNS",
                         (bdlg_cll_fn)
                             cll_doc_crte_elem_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createDocumentFragment",
                         (bdlg_cll_fn)
                             cll_doc_crte_doc_frg,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "ceateTextNode",
                         (bdlg_cll_fn)
                             cll_doc_crte_txt_nde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "ceateComment",
                         (bdlg_cll_fn)
                             cll_doc_crte_cmt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createDocumentFragment",
                         (bdlg_cll_fn)
                             cll_doc_crte_doc_frg,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createTextNode",
                         (bdlg_cll_fn)
                             cll_doc_crte_txt_nde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createComment",
                         (bdlg_cll_fn)
                             cll_doc_crte_cmt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createProcessingInstruction",
                         (bdlg_cll_fn)
                             cll_doc_crte_pi,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "importNode",
                         (bdlg_cll_fn)
                             cll_doc_imprt_nde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "adoptNode",
                         (bdlg_cll_fn)
                             cll_doc_adpt_nde,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createRange",
                         (bdlg_cll_fn)
                             cll_doc_crte_rnge,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createEvent",
                         (bdlg_cll_fn)
                             cll_doc_crte_evt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createNodeIterator",
                         (bdlg_cll_fn)
                             cll_doc_crte_nde_it,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "createTreeWalker",
                         (bdlg_cll_fn)
                             cll_doc_crte_wlkr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByTagName",
                         (bdlg_cll_fn)
                             cll_doc_get_elems_by_tg_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByTagNameNS",
                         (bdlg_cll_fn)
                             cll_doc_get_elems_by_tg_nme_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByClassName",
                         (bdlg_cll_fn)
                             cll_doc_get_elems_by_clss_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "prepend",
                         (bdlg_cll_fn)
                             cll_doc_prpnd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "append",
                         (bdlg_cll_fn)
                             cll_doc_appnd,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "getElementByID",
                          (bdlg_cll_fn)
                              cll_doc_get_elem_by_id,
                          allocs,
                          logger);
}

/* "bddm_add_elem_prps" populates the lan-
 * guage-abstracted object in "elem" with
 * the methods and attributes of a DOM
 * Element, using the memory allocator
 * and error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
struct bdlg_obj *bddm_add_elem_prps(
                           struct bdxl_elem *elem,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdxl_node *nde = NULL;  /* node super-
                                   * class object */
    struct bdlg_obj *ret;         /* return object */

    /* start "elem" with the entries
     * for a "Node"-classed object */
    if (!(ret = bddm_add_nde_prps(nde,
                                  allocs,
                                  logger)))
        return NULL;

    /* then add entries for an "Element"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "tagName",
                         (bdlg_cll_fn)
                             get_elem_tg_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "tagName",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "prefix",
                         (bdlg_cll_fn)
                             get_elem_prfx,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "prefix",
                         (bdlg_cll_fn)
                             set_elem_prfx,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "localName",
                         (bdlg_cll_fn)
                             get_elem_lcl_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "localName",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "namespaceURI",
                         (bdlg_cll_fn)
                            get_elem_ns_uri,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "namespaceURI",
                         (bdlg_cll_fn)
                            NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "id",
                         (bdlg_cll_fn)
                             get_elem_id,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "id",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "className",
                         (bdlg_cll_fn)
                             get_elem_clss_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "className",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "classList",
                         (bdlg_cll_fn)
                             get_elem_clss_lst,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "classList",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getAttribute",
                         (bdlg_cll_fn)
                             cll_elem_get_attr,
                         opaque,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getAttributeNS",
                         (bdlg_cll_fn)
                             cll_elem_get_attr_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "setAttribute",
                         (bdlg_cll_fn)
                             cll_elem_set_attr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "setAttributeNS",
                         (bdlg_cll_fn)
                             cll_elem_set_attr_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByTagName",
                         (bdlg_cll_fn)
                             cll_elem_get_elems_by_tg_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByClassName",
                         (bdlg_cll_fn)
                             cll_elem_get_elems_by_clss_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getElementsByTagNameNS",
                         (bdlg_cll_fn)
                             cll_elem_get_elems_by_tg_nme_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "hasAttribute",
                         (bdlg_cll_fn)
                             cll_elem_hs_attr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "removeAttribute",
                         (bdlg_cll_fn)
                             cll_elem_rmve_attr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "removeAttributeNS",
                         (bdlg_cll_fn)
                             cll_elem_rmve_attr_ns,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "children",
                         (bdlg_cll_fn)
                             get_elem_chldrn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "children",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "firstElementChild",
                         (bdlg_cll_fn)
                             get_elem_frst_elem_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "firstElementChild",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "lastElementChild",
                         (bdlg_cll_fn)
                             get_elem_frst_elem_chld,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "lastElementChild",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "firstElementSibling",
                         (bdlg_cll_fn)
                             get_elem_frst_elem_sblng,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "firstElementSibling",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "lastElementSibling",
                         (bdlg_cll_fn)
                             get_elem_lst_elem_sblng,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "lastElementSibling",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "childElementCount",
                         (bdlg_cll_fn)
                             get_elem_elem_cnt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "childElementCount",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "prepend",
                         (bdlg_cll_fn)
                             cll_elem_prpnd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "append",
                         (bdlg_cll_fn)
                             cll_elem_appnd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "before",
                         (bdlg_cll_fn)
                             cll_elem_bfre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "after",
                         (bdlg_cll_fn)
                             cll_elem_aftr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "remove",
                         (bdlg_cll_fn)
                             cll_elem_rmve,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "hasAttributeNS",
                          (bdlg_cll_fn)
                              cll_elem_hs_attr_ns,
                          allocs,
                          logger);
        

}

/* "bddm_add_attr_prps" populates the lan-
 * guage-abstracted object in "attr" with
 * the methods and attributes of the DOM
 * Attribute given in "attr", using the
 * memory allocator and error logger gi-
 * ven in "allocs" and "logger", respec-
 * tively.  Returns zero on error, non-
 * zero otherwise */
struct bdlg_obj *bddm_add_attr_prps(
                          struct bdxl_attr *attr,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bdlg_obj *ret;        /* return object */
    struct bdxl_node *nde = NULL; /* super-class
                                  * object */


    /* start "attr" with the entries
     * for a "Node"-classed object */
    if (!(ret = bddm_add_nde_prps(nde,
                                  allocs,
                                  logger)))
        return NULL;

    /* then add entries for an "Attr"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "name",
                         (bdlg_cll_fn)
                             get_attr_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "name",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "specified",
                         (bdlg_cll_fn)
                             get_attr_spcfd,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "specified",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "ownerElement",
                         (bdlg_cll_fn)
                             get_attr_ownr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "ownerElement",
                         (bdlg_cll_fn)
                             NULL,
                         opaque,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "schemaTypeInfo",
                         (bdlg_cll_fn)
                             get_attr_schma,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "schemaTypeInfo",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "isId",
                         (bdlg_cll_fn)
                             get_attr_is_id,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "isId",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger);
}

/* "bddm_add_attr_prps" populates the lan-
 * guage-abstracted object in "doc_type"
 * with the methods and attributes of the
 * DOM Docment Type given in "attr", us-
 * ing the memory allocator and error lo-
 * gger given in "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise */
struct bdlg_obj *bddm_add_attr_prps(
                          struct bdxl_doc_type *doc_type,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bdlg_obj *ret;        /* return object */
    struct bdxl_node *nde = NULL; /* super-class
                                  * object */


    /* start "attr" with the entries
     * for a "Node"-classed object */
    if (!(ret = bddm_add_nde_prps(nde,
                                  allocs,
                                  logger)))
        return NULL;

    /* then add entries for a "DocmentType"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "name",
                         (bdlg_cll_fn)
                             get_doc_type_nme,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "name",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "publicId",
                         (bdlg_cll_fn)
                             get_doc_type_pub,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "publicId",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "systemId",
                         (bdlg_cll_fn)
                             get_doc_type_sys,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "systemId",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "before",
                         (bdlg_cll_fn)
                             cll_doc_type_bfre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "after",
                         (bdlg_cll_fn)
                             cll_doc_type_aftr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "replace",
                         (bdlg_cll_fn)
                             cll_doc_type_rplce,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "remove",
                          (bdlg_cll_fn)
                              cll_doc_type_rmve,
                          allocs,
                          logger);
}

/* "bddm_add_cdta_prps" populates the
 * language-abstracted object in "cdta"
 * with the methods and attributes of
 * DOM CharacterData, using the memory
 * allocator and error logger gi- ven
 * in "allocs" and "logger", respecti-
 * vely.  Returns zero on error, non-
 * zero otherwise */
struct bdlg_obj *bddm_add_cdta_prps(
                           struct bdxl_cdta *cdta,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdlg_obj *ret;           /* return value */
    struct bdxl_node *nde = NULL;    /* Node super-
                                     * class bject */

    /* start "ret" with the entries
     * for a "Node"-classed object */

    if (!(ret = bddm_add_nde_prps(nde,
                                  allocs,
                                  logger)))
        return NULL;

    /* then add entries for a "CharacterData"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "length",
                         (bdlg_cll_fn)
                             get_cdta_len,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "length",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "data",
                         (bdlg_cll_fn)
                             get_cdta_dta,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "data",
                         (bdlg_cll_fn)
                             set_cdta_dta,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "substringData",
                         (bdlg_cll_fn)
                             cll_cdta_sbstr_dta,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "appendData",
                         (bdlg_cll_fn)
                             cll_cdta_appnd_dta,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "insertData",
                         (bdlg_cll_fn)
                             cll_cdta_insrt_dta,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "replaceData",
                         (bdlg_cll_fn)
                             cll_cdta_rplce_dta,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "deleteData",
                          (bdlg_cll_fn)
                              cll_cdta_del_dta,
                          allocs,
                          logger);
}

/* "bddm_add_pi_prps" populates the lan-
 * guage-abstracted object in "pi" with
 * the methods poand attributes of DOM
 * Processing Instruction, using the me-
 * mory allocator and error logger given
 * in "allocs" and "logger", respective-
 * ly.  Returns zero on error, non-zero
 * otherwise */
struct bdlg_obj *bddm_add_pi_prps(
                          struct bdxl_pi *pi,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bdlg_obj *ret;    /* eturn value */

    /* start "pi" with the entries
     * for a processing instructrion-
     * classed object */
    if (!(ret = bddm_add_cdta_prps(&pi->cdta,
                                   allocs,
                                   logger)))
        return NULL;

    /* then add entries for a
     * "ProcessingInstruction"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "target",
                         (bdlg_cll_fn)
                               get_pi_trgt,
                           allocs,
                           logger)))
        return NULL; 

    return bdlg_add_entry(ret,
                          bdlg_entry_attr_set,
                          "target",
                          (bdlg_cll_fn)
                                NULL,
                           allocs,
                           logger);
}

/* "bddm_add_impl_prps" populates the
 * implementation in "impl" with the
 * methods and attributes of a DOM
 * Implementation, using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero on error,
 * non-zero otherwise */
struct bdlg_obj *bddm_add_impl_prps(
                           struct bdxl_impl *impl,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdlg_obj *ret;  /* return value */


    /* no super-class , so start with an
     * empty langage-abstracted object */
    if (!(ret = bdlg_init_obj(allocs,
                              logger)))
        return NULL;

    /* ... and add DOM Implementation
     * properties*/
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "getFeature",
                         (bdlg_cll_fn)
                             cll_impl_get_ftre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "hasFeature",
                         (bdlg_cll_fn)
                             cll_impl_hs_ftre,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                        "createDocumentType",
                         (bdlg_cll_fn)
                             cll_impl_crte_doctype,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                        "createDocument",
                         (bdlg_cll_fn)
                             cll_impl_crte_doc,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_mthd_cll,
                          "createHTMLDocument",
                          (bdlg_cll_fn)
                              cll_impl_crte_html_doc,
                          allocs,
                          logger);
}


/* "bddm_add_txt_prps" populates the lan-
 * guage-abstracted object in "txt" with
 * the methods and attributes of a DOM
 * Text node, using the memory allocat-
 * or and error logger given in "allocs"
 * and "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
struct bdlg_obj *bddm_add_txt_prps(
                        struct bdxl_txt *txt,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bdlg_obj *ret;  /* return value */

    /* start "txt" with the entries
     * for a CDATA section-classed
     * object */
    if (!(ret = bddm_add_cdta_prps(&txt->cdta,
                                   allocs,
                                   logger)))
        return NULL;

    /* then add entries for a "Text"-
     * classed object */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_mthd_cll,
                         "splitText",
                         (bdlg_cll_fn)
                             cll_txt_splt_txt,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "wholeText",
                         (bdlg_cll_fn)
                             get_txt_whle_txt,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_attr_set,
                          "wholeText",
                          (bdlg_cll_fn)
                              NULL,
                          allocs,
                          logger);
}
