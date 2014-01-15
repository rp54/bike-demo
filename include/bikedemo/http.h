/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* http.h - function proto-types data
* structures for sending HTTP requests
* and parsing HTTP responses.
*
* Copyright (C) 2007-2014, Russell Potter.
* All rights reserved.
* 
***********************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __BD_HTTP_H
#define __BD_HTTP_H

struct bdht_rqst;
struct bdht_rsp;

#include <bikedemo/iri.h>
#include <bikedemo/xml.h>
#include <bikedemo/util.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/map.h>
#include <bikedemo/iri.h>

/* "bdht_chnkd_nts" - the non-terminals
 * of the parser grammmar no decode the
 * HTTP "chunked" transfer ebncoding
 * (see the spec. for details on this
 * encoding) */
enum bdht_chnkd_nts {

    bdht_chnkd_nt_chnk =
                 bdgm_sym_nres + 1,
    bdht_chnkd_nt_lst_chnk,
    bdht_chnkd_nt_trlr,
    bdht_chnkd_nt_chnk_extnsn,
    bdht_chnkd_nt_chnk_sz,
    bdht_chnkd_nt_ext_nme,
    bdht_chnkd_nt_ext_val,
    bdht_chnkd_num_toks,
};


/* bdht_vrbs" enumeration - the possible
 * types of HTTP request verbs, which
 * include "PUT", "GET" and "POST" */
enum bdht_vrbs {

    bdht_vrb_get,     /* "GET" verb */
    bdht_vrb_head,    /* "HEAD" verb */
    bdht_vrb_post,    /* "POST" verb */
    bdht_vrb_options, /* "OPTIONS" verb */
    bdht_vrb_put,     /* "PUT" verb */
    bdht_vrb_delete,  /* "DELETE" verb */
    bdht_vrb_trace,   /* "TRACE" verb */
    bdht_vrb_connect  /* "CONNECT" verb */
};

/* bdht_toks" enumeration - the possible
 * types of  lexical tokens of the HTTP
 * message grammar */
enum bdht_toks {

    bdht_tok_crlf, /* carriage return/line
                    * feed sequence */
    bdht_tok_cln,  /* colon (":") */
    bdht_tok_sp,   /* white-space */
    bdht_tok_lpr,  /* left parenthesis */
    bdht_tok_rpr,  /* right parenthesis */
    bdht_tok_lws,  /* linear white-space
                    * (carriage return/
                    * line-feed followed
                    * by white-space) */
    bdht_tok_end,  /* two conseCutive
                    * CRLFs marking end
                    *  id of headers */
    bdht_toks_num, /* two conseCutive
                    * CRLFs marking end
                    *  id of headers */
};

 /* possible lexical tokensof HTTP "chunked
  * encoding ". */
enum bdht_chnkd_toks {

    bdht_tok_chnkd_hex,  /* hexa-dec-
                          * imal digit
                          *  digit */
    bdht_tok_chnkd_octt, /* opaque octet */
    bdht_tok_chnkd_crlf, /* rarriage return/
                          * line-keed  sequ-
                          * ence */
    bdht_tok_chnkd_str   /* quoted string */
};

/* "bdht_hdr_type_fn" typedef - proto- type
 * of the function used to implement the
 * behaviour modification the presence in
 * the header may cause to its response.
 * Zero is returned on failure, non-zero
 * otherwise.
 *
 * The first parameter contains the respo-
 * nse, ghthe second an opaque value and
 * the third the content of the header it-
 * self */
typedef int (* bdht_hdr_type_fn)(
                        struct bdht_msg *,
                        void *,
                        struct bdut_bytes *);

/* "bdht_dcde_fn" typedef - proto-type
 * of the function vcalled to implement
 * the decoding of the content-encoding
 * of an HTTP request or response(as
 * specifed in its "Content-Encoding"
 * header)  from a stream of octets,
 * as given in the first parameter, to
 * a stream of code-points, whuich the .
 * routine returns (or NULL on error)  */
 ypedef struct bdut_str *(* bdht_dcde_fn)(
                       struct bdut_bytes *);

/* "bdht_xfr_dcde_fn" typedef - proto-
 * type of the function vcalled to imp-
 * lement the decoding of the transfer-
 * encoding of an HTTP request or res-
 * ponse(as specifed in its "Transfer-
 * Encoding" header)  fof a message,
 * transforming the input content, as
 * given in the first parameter, to
 * the input content, whuich the . rou-
 * tine returns (or NULL on error)  */
 ypedef struct bdut_bytes *(* bdht_xfr_dcde_fn)(
                       struct bdut_bytes *);

/* "bdht_tok" structure - the lex-
 * ical token for parsing an octet
 * stream to from an HTTP response,
 * consisting of a generic token
 * (so that the token may be used
 * generically), its type, the
 * currently scanned version number,
 * a buffer into which various parts
 * of the request is collected  and
 * the input's stream's current
 * code-point */
struct bdht_tok {

    struct bdlx_tok tok;         /* generic token */
    enum bdht_toks type;         /* token type */

    union {
        int vrsn;                /* version number */
        struct bdbf_buffer *buf; /* buffer to
                                  * collect parts
                                  * of IRI */
        int cp;                  /* current code
                                  * point */
    } u;
};

/* "bdht_chnkd_tok" structure - the
 * lexical token for parsing the
 * content of an an HTTP "chunked"
 * encoding, consisting of a generic
 * token (so that the token may be
 * used generically), its type, a
 * uffer into which various parts
 * of the request is collected and
 * the input's stream's current
 * code-point  */
struct bdht_chnkd_tok {

    struct bdlx_tok tok;         /* generic token */
    enum bdht_toks type;         /* token type */

    union {
        struct bdbf_buffer *buf; /* buffer to
                                  * collect parts
                                  * of IRI */
        int cp;                  /* current code
                                  * point */
    } u;
};

/* "bdht_rqst" structure - an in-memory
 * representation of an HTTP request,
 * consisting of the message of which
 * the trequest is a part, the "verb"
 * of the request and an indication as
 * to whether the request carries a
 * body */
struct bdht_rqst {

    struct bdht_msg msg; /* meessage */
                         /* headers */
    enum bdht_vrbs vrb;  /* request
                          * verb */
    int hs_bdy;          /* whether has
                          * request
                          * body */
};

/* "bdht_rsp" structure - an in-memory
 * trepresebtation of an HTTP response,
 * which consisting of the message of
 * which the response is a part, a des-
 * cription of the response, the resp-
 * onse status, the status code and an
 * indication as to whether the resp-
 * onse has a body  */
struct bdht_rsp {

    struct bdht_msg *msg; /* meessage */
                          /* headers */
    int cde;              /* return sta-
                           * tus code */
    struct bdut_str *dsc; /* status */
    int hs_bdy;           /* whether has
                           * body */
};

/* "bdht_prxy" structure - the proxy
* parameters of a rewquest , consist-
* ing of a oioxt jhost and the port
* on which the proxy host listens */
struct bdht_prxy {

    char *hst;          /* proxy host */
    unsigned short prt; /* listening
                         * port */
};

/* "bdht_prms" structure - the param-
 * eters constituting the opaque poi-
 * nter in the HTTP scheme's resol-
 * ution function, consisting of a
 * map of request headers, and the
 * optional proxy (NULL if not used) */
struct bdht_prms {

    struct bd_map_node *hdrs; /* map of */
                              /* headers */
\    struct bdht_prxy *prxy;   /* proxy host
                               * and port*/
};

/*** Function prototypes ***/

/* "bdht_init" - returns the HTTP req-
 * uest parser whose lexical anal-
 * yser and grammar table hsve been
 * populated with content for parsing
 * HTTP request.  Returns zero on
 * error, on-zero otherwise */
int bdht_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdht_prse_rsp" -  returns the HTTP response,
 * reprsented as IRI data, in the bytes given
 * in the first parameter, using the memory all-
 * ocator and error logger given in the second
 * and third parameters, respectively.  Returns
 * NULL on error.  Constitutes the "generic parse"
 * routine of the "HTTP" IRI scheme */
struct bdir_dta *bdht_prse_rsp(struct bdut_bytes *,
                               struct bd_allocs *,
                               struct bd_logger *);

/* "bdht_snd_rqst" - sends an HTTP request,
 * using the proxy opaque parameter, proxy
 * host and proxy port given in the seco-
 * nd to fourth parameters, respectively,
 * by writing to the file descriptor giv-
 * en in the fifth parameter, the "gener-
 * ically-formattted" IRI given in the fi-
 * rst parameter, using the memory alloca-
 * tor and error logger given in the sixth
 * ifth and seventh parameters, respectiv-
 * ely.  non-zero otherwise.  Constitutes
 * the "generic send" routine of the "HTTP"
 * IRI scheme */
typedef int bdht_snd_rqst(struct bdir_gnrc_iri *,
                          void *,
                          char *,
                          int,
                          int,
                          struct bd_allocs *,
                          struct bd_logger *);

/* "bdht_add_hdr_type" add the header
 * type whose UTF-8-encoded name, its
 * associated implementation routine
 * are and an opaque value passed with
 * the routine call are given in the
 * first to third parameters, respe-
 * ctively, to the set of known HTTP
 * HTTP response header types */
int add_hdr_type(char *,
                 void *,
                 bdht_hdr_type_fn);

/* "bdht_crte_rqst" - creates, initi-
 * alises and returns, using the me-
 * mory allocator and error logger
 * given in the first and second par-
 * ameters, respectively, an HTTP
 * request containing no headers
 * and no content (be are added la-
 * ter, but before the request is
 * sent.  Returns NULL on error */
struct bdht_rqst *bdht_crte_rqst(
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdht_get_rqst_hdr"  returns the
 * value of the header of the request
 * given in the first parameter the
 * UTF-8 encoded name  of which is
 * given in the second parameter, us-
 * ing the memory allocator and error
 * logger given in the third and fou-
 * rth parameters, respectively. Ret-
 * urns NULL on error */
char *bdht_get_rqst_hdr(struct bdht_rqst *,
                        char *,
                        struct bd_allocs *,
                        struct bd_logger *);

/* "bdht_add_rqst_hdr"  adds the hea-
* der the UTF-8-encoded name and val-
* ue of which is given in the second
* and third parameters, respectively,
 * to the request given in the first
 * parameter, using the memory alloc-
 * ator and error logger given in the
 * fourth and fifth par- ameters, re-
 * spectively. Returns zero on error */
int bdht_add_rqst_hdr(struct bdht_rqst *,
                     char *,
                     char *,
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdht_get" - returns the HTTP resp-
 * onse,to a request sent to the host
 * and path identified in the IRI giv-
 * en in the first parameter, using the
 * read and write call-backs, proxy
 * host name, memory allocator and
 * error logger given the second to
 * sixth parameters, respectively, and
 * using the request headers given in
 * the eighth and following parameters
 * (in the form of name/value param-
 * eter pairs), the the number of which
 * is given in the seventh parameter */
struct bdht_rsp *bdht_get(struct bdir_gnrc_iri *,
                          int,
                          int,
                          char *,
                          struct bd_allocs *,
                          struct bd_logger *,
                          int,
                          ...);

/* "bdht_rslve_doc" - returns the XML
 * document found by parsing the data
 * buffer given in the first parameter
 * into an XML document (or, in the
 * case if a response containing an
 * HTML document, that document's XML
 * equivalent), using the request par-,
 * meters, memory allocator and XML
 * error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdxl_doc *bdht_rslve_doc(struct bdir_dta *,
                                struct bdht_prms *,
                                struct bd_allocs *,
                                struct bdxl_logger *);

#ifdef __cplusplus
}
#endif

#endif
