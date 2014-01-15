/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* http.c - function definitions
* related to HTTP requests and resp-
* onses
*
* Copyright (C) 2007-2013, Russell Potter.
* All rights reserved.
* 
***********************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/xml.h>
#include <bikedemo/iri.h>
#include <bikedemo/http.h>
#include <bikedemo/html.h>
#include <bikedemo/lex.h>
#include <bikedemo/parser.h>
#include <bikedemo/buffer.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>
#include <memory.h>

/* "hdr_types" the map of known header
 * types that may be be received by an
 * HTTP request's response, keyed to the
 * type's name and valued to its implen-
 * tation call-back */
static struct bd_map_node *hdr_types;

/* "cmp_hdrs" return a ompa-
 * riso between the HTTP he-
 * aders in "hdr1" and "hdr2",
 * as the nap ket comparison
 * function of a response's
 * of the NULL terminated
 * code-point arrays given
 * in "cps1" and "cps2" */
static int cmp_hdrs(struct bdut_bytes hdr1,
                    struct bdut_bytes hdr2)
{
    int len_diff;
    /* return byte-by-byte
     * comparison of names*/

    if (len_diff = (hdr1.len - hdr2.len))
        return len_diff;
	else
    return strncmp(hdr1.bytes,
	               hdr2.bytes,
	               hdr1.len);
}

/* global parsers for de-serial-
 * ising HTTP responses */
static struct bdpr_parser *prsr;

/* "tok_name" - returns the index of the
 * messsage table entry describing the token
 * in "tok", enere "exact"  whether the des-
 * cription should be specific, rather than
 * general  */
static int tok_name(struct bdlx_tok *tok,
                    int exact,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)

{
    return 0; /* do nothing */
}

/* "init_tok" - perform once only
 * initialisation of the HTTP token
 * "tok", using the memory allocator
 * smd error logger given in "allocs"
 * and "logger" */
static int init_tok(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdht_tok *ht_tok  =
                (struct bdht_tok *) tok;

    /* initialise HTTP token's buffer */
    return (ht_tok->u.buf = bdbf_init(
                            allocs,
                            logger,
                             sizeof(int)))
                             != NULL;
}

/* "init_tok_type" - retuen he type
 * of the HTTP token given in "tok",
 * using the memory allocator * and
 * error logger given in "allocs" and
 * "logger", respectively */
static int tok_type(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdht_tok *ht_tok  =
                (struct bdht_tok *) tok;

    /* return "type" field */
	return ht_tok->type;
}

/* "ereset_tok" - perform processing on
 * "token tok" rassicuated uth reseting
 * eqyuews vbefore each csll to "next"  */
static int reset_tok(struct bdlx_tok *tok,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdht_tok *ht_tok  =
                (struct bdht_tok *) tok;

    /* reset (by  initialising) the HTTP
     * token's buffer */
    return ht_tok->u.buf = bdbf_init(
                            allocs,
                            logger,
                            sizeof(int)))
                            != NULL;
}

/* "reduce_start" - perform the action
 * associated with the recognition of
 * the (start) "hesers body" production
 * the output JSON HTTP response in "rsp",
 * generic cast of the left-hand-side sym-
 * bol in "lhs" to contain the result of
 * the reduction, the "reduce" items in
 * "items", the number of which is given
 * in "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_rsp_start(struct bdht_rsp *rsp,
                            void *lhs,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    rsp = (struct bdht_rsp *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdht_rsp),
                     logger);
    if (!(rsp))
        return 0;

    rsp->hdrs = (struct bd_map_node *)
                 items[0].nt;
    rsp->bdy  = (struct bdut_bytes *)
                 (items[1].nt);

    lhs = (struct bdht_rsp *) rsp;
    return 1;
}

/* "reduce_add_hdr" - perform the action
 * associated with the recognition of
 * a production to add a header the out-
 * put HTTP response in "rsp", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items in
 * "items", the number of which is given
 * in "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_add_hdr(void *out,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bd_map_node *ins_node, /* insertion */
                       *hdrs;     /* node and
                                   * header map */
    struct bdut_bytes *name =     /* header name */
            (struct bdut_bytes *)
            items[0].nt;
    struct bdut_bytes *val =
            (struct bdut_bytes *)
            items[1].nt;

    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;

    if (!(bd_map_insert(&hdrs,
                        ins_node,
                        (void *) name,
                        (bd_map_cmp_fn)
                            cmp_hdrs,
                       (void *) val)))
        return 0;

    lhs = (struct bd_map_node *) hdrs;
    return 1;
}

/* "reduce_add_hdr" - perform the action
 * associated with the recognition of
 * a production to add a header the out-
 * put HTTP response in "rsp", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items in
 * "items", the number of which is given
 * in "num" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_add_hdr(void *out,
                          void *lhs,
                          union bdgm_reduce_item *items,
                          int num,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bd_map_node *ins_node, /* insertion */
                       *hdrs;     /* node and map
                                   * to which to
                                   * add header */
    struct bdut_bytes *name =     /* header name */
            (struct bdut_bytes *)
            items[0].nt;
    struct bdut_bytes *val =
            (struct bdut_bytes *)
            items[1].nt;

    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;

    if (!(bd_map_insert(&hdrs,
                        ins_node,
                        (void *) name,
                        (bd_map_cmp_fn)
                            cmp_hdrs,
                       (void *) val)))
        return 0;

    lhs = (struct bd_map_node *) map;
    return 1;
}

/* "reduce_start_hdrs" - perform the
 * action associated with the recogni-
 * tion of a production to start the
 * headers of an output HTTP request
 * or response, a generic cast of wh-
 * ich is given in "out" left-hand-
 * side symbol in "lhs" to contain
 * the result of the reduction, the
 * "reduce" items in "items", the
 * number of which is given in "num"
 * and the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_start_hdrs(void *out,
                             void *lhs,
                             union bdgm_reduce_item *items,
                             int num,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)
{
    struct bdut_bdhr_hdr *hdr =     /* initial header  */
            (struct bdht_hdr *)
            items[0].nt;
    struct bdut_bytes *val =
            (struct bdut_bytes *)
            items[1].nt;

    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
    if (!(ins_node))
        return 0;

    if (!(bd_map_insert(&map,
                        ins_node,
                        (void *) name,
                        (bd_map_cmp_fn)
                            cmp_hdrs,
                        (void *) val)))
        return 0;

    lhs = (struct bd_map_node *) map;
    return 1;
}

/* "ld_prsr" = load the lexical analyser
 * and grammar table of the HTTP response
 * parser "prsr" with isxith the lexical
 * rules and and grammar productions to
 * de-serialise an HTTP request.  Returns
 * zero on error, non-zero otherwise */
 static int ld_prsr(struct bdpr_parser *prsr,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdht_rsp *rsp = NULL;
    struct bdht_rqst *rqst = bdht_crte_rqst(allocs,
                                            logger);

    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdht_tok),
                        (bdlx_tok_init_fn) init_tok,
                        (bdlx_tok_type_fn) tok_type,
                        (bdlx_tok_reset_fn) reset_tok,
                        (bdlx_tok_name_fn) tok_name)))
        return 0;

    /* add basic HTTP definitions, as
     * stated in the HTTP spec. */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "chr",
                       "[\\0-\\127]")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "uppr",
                       "[A-Z]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "lwr",
                       "[a-z]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "alpha",
                       "[A-Za-z]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "dig",
                       "[0-9]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "ctl",
                       "\\0-\\31"
                       "\\127]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "cr",
                       "\\13")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "lf",
                       "\\10")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "sp",
                       "\\32")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "crlf",
                       "{cr}{lf}")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "text",
                       "{lws}|"
                       "{^ctl}")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "hex",
                       "[a-zA-Z0-9]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "sep",
                       "[\\(\\)<>@"
                       "],:;\\\\\""
                       "\\[\\]\\?="
                       "]")))
         return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "token",
                       "}{chr}|"
                       "{^ctl}|"
                       "{^sep}")))
         return 0;

     /* ... add the HTTP lexical
      * rules  */
    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{crlf}",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdht_tok_nl)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "(",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdht_tok_lpr)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ")",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdht_tok_rpr)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ":",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdht_tok_cln)))
        return 0;

    /* load parser's grammar pro-
     * ductions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rsp,
                        bdgm_sym_start,
                        2,
                        (bdgm_reduce_fn)
                            reduce_start,
                        bdht_nt_hdrs,
                        bdht_nt_bdy)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rsp,
                        bdht_nt_hdrs,
                        2,
                        (bdgm_reduce_fn)
                            reduce_add_hdr,
                        bdht_nt_hdr,
                        bdht_nt_hdrs)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         rsp,
                         bdht_nt_hdrs,
                         1,
                         (bdgm_reduce_fn)
                             reduce_start_hdrs,
                         bdht_nt_hdr);

}

/* "bdht_prse_rqst" -  returns the HTTP request,
 * reprsented as IRI data, in the bytes given
 * in "bytes", using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error.  Con-
 * stitutes the "generic parse" routine of the
 * "HTTP" IRI scheme for parsing
 * responses, as used by HTTP servers */
struct bdir_dta *bdht_prse_rsp(
                    struct bdut_bytes *bytes,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdir_dta *ret; /* return
                           * value */
    struct bdht_rsp *rsp; /* parsed
                           * response */

    /* parse response from
     * "bytes" */
    if (!(bdpr_run(allocs,
                   logger,
                   qsp_prsr,
                   (void *)
                       rsp,
                   bytes)))
        return NULL;

    /* and set the returned data's
     * type and content from the re-
     * sponse's "Content-Encoding"
     * header and response content,
     * respectively, and return that
     * data */
    ret->type = bdht_get_rsp_hdr(
                   rsp,
                   "Content-Encoding",
                   allocs,
                   logger)
    ret->dta = rsp->cnt;

    return ret;
}

/* "bdht_crte_rqst" -  creates, initiali-
 * ses and returns an HTTP request cont-
 * aining no headers and empty content
 * (which are added later, but before the
 * request is sent), using the memory al-
 * locator and error logger given in "al-
 * locs" and "logger", respectively.  Re-
 * turns NULL on error */
struct bdht_rqst *bdht_crte_rqst(
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdht_rqst *ret; /* return
                            * request */

    /* alloczte the request */
    ret = (struct bdht_rqst *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdht_rqst),
                   logger);
    if (!(ret))
        return NULL;

    /* return rewuest with
     * initially no headers */
    ret->hdrs = (struct bd_map_node *)
            NULL;
    ret->bdy.len  = 0;
    return ret;
}

/* "bdht_crte_rsp" -  creates, initiali-
 * ses and returns an HTTP response cont-
 * aining no headers and empty content
 * (which are added later, but before the
 * requesponse is sent), using the memo-
 * ry allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely.  Returns NULL on error */
struct bdht_rsp *bdht_crte_rsp(
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdht_rsp *ret; /* return
                           * response */

    /* alloczte the response */
    ret = (struct bdht_rsp *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdht_rsp),
                   logger);
    if (!(ret))
        return NULL;

    /* return response with
     * initially no headers
     * or body */
    ret->hdrs = (struct bd_map_node *)
            NULL;
	ret->hs_bdy = 0;
    return ret;
}

/* "bdht_add_hdr" -  adds a header with the
 * UTF-8-transformed name and value given in
 * "name" and "val", respectively, to the
 * request given in "rqst", using the opaque
 * value, memory allocator and error logger
 * given in "opaque", "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherise */
int bdht_add_hdr(struct bdht_rqst *rqst,
                 void *opaque,
                 char *name,
                 char *val,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdut_str name_str,     /* header */
                    val_str;      /* name and
                                   * value code-
                                   * point str-
                                   * ings */
    struct bdut_bytes name_bytes, /* header */
                      val_bytes;  /* name and
                                   * value byte
                                   * strings */
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* initialise and UTF-8-decode
     * name and value */
    name_bytes.bytes = name;
    name_bytes.len   = (int) strlen(name);
    val_bytes.bytes  = val;
    val_bytes.len    = (int) strlen(val);

    if (!(bdut_from_utf8(&name_str,
                         name,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(&val_str,
                         val,
                         allocs,
                         logger)))
        return 0;
    /* insert header into "rqst"'s
     * headers map, keyed and valued to
     * "name", and "value", respectively */
    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                           bd_map_node),
                     logger);
    if (!(ins_node))
    	return 0;
    return bd_map_insert(&rqst->hdrs,
                         ins_node,
                         &name_str,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         &val_str);
}

/* "bdht_init" - returns the HTTP
 * parser whose lexical analyser and
 * grammar table have been populated
 * with content necessary for
 * for parsing HTTP responses, uaing
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
int bdht_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* load request and response
     * parser initialisation
     * functions */
    if (!(rqst_prsr = bdpr_init(NULL,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_rqst_prsr)))
        return 0;

    if (!(rsp_prsr = bdpr_init(NULL,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_rsp_prsr)))
        return 0;

    /* ... and register the "http"
     * scheme's IRI resolution
     * routine */
    return bdir_add_schme("http",
                          bdir_fmt_gnrc,
                          NULL,
                          80,
                          (bdir_gnrc_snd_fn)
                              bdht_wr_rqst,
                          (bdir_gnrc_prse_fn)
                              bdht_prse_rsp,
                          allocs,
                          logger);
}

/* "bdht_rslve_doc" - returns the XML
 * document found by parsing the HTTP
 * bresponse given in "dta" into an
 * XML document (or, in the case if a
 * response containing an HTML docu-
 * ment, that document's XML equival-
 * ent), using the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Retu-
 * rns NULL on error */
struct bdxl_doc *bdht_rslve_doc(
                 struct bdir_dta *dta,
                 struct bdht_prms *prms,
                 struct bd_allocs *allocs,
                 struct bdxl_logger *logger)
{
    struct bdut_bytes *type = /* type of */
          dta->type;          /* returned
                               * data */

    /* NULL-terminate "type" for
     * use by "strlen" and friends */
	type->bytes[type->len] = 0;

    /* response is an XML document
     * if last three letters
     * of MIME type name is "xml"
     * parse it directly into an
     * XML document */
    if (!(strncmp(type->bytes +
                     (strlen(type->bytes) - 3),
                  "xml",
                  3)))
        return bdxl_prse_doc(allocs,
                             &logger->logger,
                             *type,
                             bdxl_ws_dflt_prsrve);

    /* otherwise, if is an HTML
     * document, return an XML
     * equivalent to the HTML
     * document in the response */
    else if (!(strcmp(type->bytes,
                      "text/html")))
        return bdhm_prse(type,
                         allocs,
                         &logger->logger);

    /* and return an error if
     * response is neither of
     * these types */
    else
        return NULL;
}
