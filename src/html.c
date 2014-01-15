/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* html.c - function implementationa related to
* the Hyper-Text Markup Language (HTML), a legacy,
* SGML-like format commonly * used prior to the
* advent of XML.
*
* This implementation parses an input HTML docu-
* ment and returns an XML document having equiva-
* lent content
* 
* Copyright (C) 2007-2013, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

/* "prsr" - the GHTML document
 * parser */
static struct bdpr_parser *prsr;

/* "name_tok" - returns the index of the
 * messsage table entry describing the
 * HTML token "tok", enere "exact"  ind-
 * icates whether the description should
 * be specific, rather than general  */
static int tok_name(struct bdlx_tok *tok,
                    int exact,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)

{
    return 0; /* do nothing */
}

/* "init_tok" - perform once only
 * initialisation of the HTML token
 * "tok", using the memory allocator
 * smd error logger given in "allocs"
 * and "logger" */
static int init_tok(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdhm_tok *hm_tok  =
                (struct bdhm_tok *) tok;

    /* initialise HTTP token's buffer */
    return (hm_tok->u.buf = bdbf_init(
                            allocs,
                            logger,
                             sizeof(int)))
                             != NULL;
}

/* "tok_type" - retuen he type f the
 * HTTP token given in "tok", using
 * the memory allocator * and error
 * logger given in "allocs" and "log-
 * ger", respectively */
static int tok_type(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdhm_tok *hm_tok  =
                (struct bdhm_tok *) tok;

    /* return "type" field */
	return hm_tok->type;
}

/* "ereset_tok" - perform processing on
 * the HTML "token "tok" rassicuated wi-
 * th reseting before each call to "next"  */
static int reset_tok(struct bdlx_tok *tok,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdhm_tok *hm_tok  =
                (struct bdhm_tok *) tok;

    /* reset (by  initialising) the HTTP
     * token's buffer */
    return (hm_tok->u.buf = bdbf_init(
                            allocs,
                            logger,
                            sizeof(int)))
                            != NULL;
}

/* "ld_prsr" = load the lexical analyser
 * and grammar table of the HTML document
 * parser "prsr" with the lexical rules
 * and and grammar productions to parse
 * an HTML document.  Returns zero on er-
 * ror, non-zero otherwise */
 static int ld_prsr(struct bdpr_parser *prsr,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdxl_doc *doc; /* parser output
                           * document */

    doc = bdxl_doc_init(allocs,
                        logger);

    /* initialse parser's token
     * with its call-backs */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdhm_tok),
                        (bdlx_tok_init_fn) init_tok,
                        (bdlx_tok_type_fn) tok_type,
                        (bdlx_tok_reset_fn) reset_tok,
                        (bdlx_tok_name_fn) tok_name)))
        return 0;

    /* add basic HTML definitions, as
     * stated in the HTML parsing rules */
    return bdlx_add_def(prsr->lex,
                        allocs,
                        logger,
                        "nme_chr",
                        "[a-zA-Z1-9]");
}

/* "bdhm_parse" - parses the byte
 * stream given in "input", return-
 * ing the HTML document (after being
 * converted to its equivalent XML)
 * found therein,  using the memory
 * allocator and error klogger given
 * in "allocs" and "logger",respect-
 * ively. Returns NULL on error */
struct bdxl_doc *bdhm_prse(
                struct bdut_bytes *input,
                struct bd_allocs *allocs,
                struct bd_logger *logger)

{
    struct bdut_str *str = NULL; /* unicode 
                                  * version
                     NULL;        * of "input" */
    struct bdxl_doc *ret;        /* return value */

    /* NULL-terminate input */
    input->bytes[input->len] = 0;

    /* transform input to to unicode */
    bdut_from_utf8(str,
                   input->bytes,
                   allocs,
                   logger);

    /* run the HTML parser on "input",
     * which should generate the docu-
     * ment in its opaque value */

    ret = NULL; /* wards off "unu-
                 * sed variable"
                 * warnings */

    bdpr_run(allocs,
             logger,
             prsr,
             (void *)
                 ret,
             str);

    /* .. and return the generated 
     * document */
    return ret;
}

/* "set_doc_ttle" - implements the "docu-
 * ment"-class "title" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_doc_ttle(
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

/* "get_doc_rfrr" - implements the "docu-
 * ment"-class "feferrer" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_rfrr(
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

/* "get_doc_dmn" - implements the "docu-
 * ment"-class "domain" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_dmn(
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

/* "get_doc_url" - implements the "docu-
 * ment"-class "URL" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_url(
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

/* "get_doc_bdy" - implements the "docu-
 * ment"-class "body" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_bdy(
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

/* "set_doc_bdy" - implements the "docu-
 * ment"-class "body" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_doc_bdy(
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

/* "get_doc_applts" - implements the "docu-
 * ment"-class "iapplets" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_applts(
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

/* "get_doc_imges" - implements the "docu-
 * ment"-class "images" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_imges(
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

/* "get_doc_lnks" - implements the "docu-
 * ment"-class "links" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_lnks(
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

/* "get_doc_frms" - implements the "docu-
 * ment"-class "forms" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_frms(
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

/* "get_doc_ckie" - implements the "docu-
 * ment"-class "cookie" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_doc_ckie(
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

/* "cll_doc_opn" - implements the "docu-
 * ment"-class "open" method call, using
 * the language abstraction, "context"
 * object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_doc_opn(
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

/* "cll_doc_clse" - implements the "docu-
 * ment"-class "close" method call, using
 * the language abstraction, "context"
 * object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_doc_clse(
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

/* "cll_doc_wrte" - implements the "docu-
 * ment"-class "write" method call, using
 * the language abstraction, "context"
 * object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *cll_doc_wrte(
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

/* "cll_doc_wrtln" - implements the "do-
 * cument"-class "writeLn" method call,
 * using the language abstraction, "con-
 * text" object, array of arguments, nu-
 * mber of arguments, memory allocator,
 * error logger and object to set, on e-
 * xit, to any thrown exception in "la-
 * ng", "ctx", "args", "num", "allocs",
 * "logger" and "xcptn", respectively */
static struct bdlg_obj *cll_doc_wrtln(
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

/* "cll_doc_get_elems_by_nme" - implem-
 * ents the "document"-class "getEleme-
 * ntsByName" method call, using the la-
 * nguage abstraction, "context" object,
 * array of arguments, number of argum-
 * ents, memory allocator, error logger
 * and object to set, on exit, to any
 * thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger"
 * and "xcptn", respectively */
static struct bdlg_obj *cll_doc_get_elems_by_nme(
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

/* "bdhm_set_opts_prps" - returns 
 * a language-abstracted object
 * populated with DOM properties
 * of the HTML OptionsCollection
 * given in "opts", using the me-
 * mory allocator and error log-
 * ger given in "allocs" and
 * "logger", respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_doc_prps(
                       struct bdhm_doc *doc,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdlg_obj *ret;        /* return value */
    struct bdxl_doc *doc = NULL; /* super-class
                                  *  object */

    /* start "ret" with the entries
     * for an "Document"-classed
     * object */
    if (!(ret = bddm_add_doc_prps(&doc->doc,
                                  allocs,
                                  logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "title",
                         (bdlg_cll_fn)
                             get_doc_ttle,
                         allocs,
                         logger);
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "title",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "referrer",
                         (bdlg_cll_fn)
                             get_doc_rfrr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "referrer",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "domain",
                         (bdlg_cll_fn)
                             get_doc_dmn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "domain",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "URL",
                         (bdlg_cll_fn)
                             get_doc_url,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "URL",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "body",
                         (bdlg_cll_fn)
                             get_doc_bdy,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "body",
                         (bdlg_cll_fn)
                             set_doc_bdy,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "images",
                         (bdlg_cll_fn)
                             get_doc_imges,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "images",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "links",
                         (bdlg_cll_fn)
                             get_doc_lnks,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "links",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "anchors",
                         (bdlg_cll_fn)
                             get_doc_anchrs,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "anchors",
                         (bdlg_cll_fn)
                             NULL,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "cookie",
                         (bdlg_cll_fn)
                             get_doc_ckie,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "cookie",
                         (bdlg_cll_fn)
                             set_doc_ckie,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_cll,
                         "open",
                         (bdlg_cll_fn)
                             cll_doc_opn,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_cll,
                         "close",
                         (bdlg_cll_fn)
                             cll_doc_clse,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_cll,
                         "write",
                         (bdlg_cll_fn)
                             cll_doc_wrte,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_cll,
                         "writeLn",
                         (bdlg_cll_fn)
                             cll_doc_wrtln,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_cll,
                          "getElementsByName",
                          (bdlg_cll_fn)
                              cll_doc_get_elems_by_nme,
                          allocs,
                          logger);
}

/* "bdhm_init" - initialise the HTML
 * parsing sub-system, using the mem-
 * ory allocator and error logger
 * fuven in  "allocs" and "logger"
 * ameters.  Returns zero on error,
 nonon-zero otherwise */
int bdhm_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* load the parser with its
     * grammar productione and
     * lexical rules */
    prsr = bdpr_init(NULL,
                     allocs,
                     logger,
                     (bdpr_init_fn)
                         ld_prsr);

    return prsr != NULL;
}
