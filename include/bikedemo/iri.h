/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* iri.h - function proto-types data struct-
* ures related to processing of so-called
* "Internationalised Resource Identifiers"
* (IRIs)
*
* Copyright (C) 2007-2012, Russell Potter.
* All rights reserved.
* 
***********************************************************************/

#ifndef __BD_IRI_H
#define __BD_IRI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/util.h>
#include <bikedemo/lex.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

struct bdir_gnrc_iri; 

/* "bdir_fmts" enumeration  - the types
 * of IRI format: either generic" or
 * "custom"  (i.e, sny other, non-gen-
 * eric, format) */
enum bdir_rslve_fmts {

    bdir_fmt_gnrc, /* generic format */
    bdir_fmt_cstm   /* custom format */
};

/* "bdir_toks" enumeration  - the
 * tokens returned by the IRI lexical
 * analyser, which includes a scheme
 * name character, the scheme delimeter
 * (a colon - ":") and a scheme-speci-
 * fic data character (any other code-
 * point) */
enum bdir_toks {

    bdir_tok_schme, /* scheme character */
    bdir_tok_dlm,   /* scheme delimiter
                     * colon (":") */
    bdir_tok_dta,   /* scheme-specific
                    /* character */
};

/* "bdir_nts" enumeration  - the
 * non-terminals of the top-level
 * IRI grammar */
enum bdir_nts {

    bdir_nt_dta, /* scheme-specific
                  * data */
    bdir_nt_iri  /* IRI */
};

/* "bdir_gnrc_toks" enumeration  - the tokens
 * returned by the IRI "generic syntax"
 * lexical analyser, which includes forward
 * slashes, hash signs and question marks */
enum bdir_gnrc_toks {

    bdir_tok_gnrc_alph, /* alphabetic ("[A-Za-z]") */
    bdir_tok_gnrc_ip4,  /* v4 IP char ("[0-9.]") */
    bdir_tok_gnrc_ip6,  /* v6 IP char ("[a-f
                         * A-F0-9.:]") */
    bdir_tok_gnrc_slsh, /* forward slash ("/") */
    bdir_tok_gnrc_dslh, /* double slash ("//") */
    bdir_tok_gnrc_cln,  /* colon (":") */
    bdir_tok_gnrc_hsh,  /* hash ("#") */
    bdir_tok_gnrc_dns,  /* domain name char. */
    bdir_tok_gnrc_qstn, /* question mark ("?") */
    bdir_tok_gnrc_lsq,  /* left square bracket ("[") */
    bdir_tok_gnrc_rsq,  /* right square bracket ("]") */
    bdir_tok_gnrc_at,   /* at sign ("@") */
    bdir_tok_gnrc_seg,  /* path segment */
    bdir_tok_gnrc_qry,  /* query character */
    bdir_tok_gnrc_frg   /* fragment character */
};

/* "bdir_nts" enumeration  - the non-
 * terminals of the "generic syntax"
 * IRI grammar */
enum bdht_ir_gnrc_nts {

    bdir_nt_gnrc_iri,     /* entire generically-
                           * formatted IRI */
    bdir_nt_gnrc_auth,    /* authority (user/
                           * passwd/host/port) */
    bdir_nt_gnrc_hr,      /* hierachial part */
    bdir_nt_gnrc_usr,     /* user info */
    bdir_nt_gnrc_hst,     /* host name */
    bdir_nt_gnrc_hst_dns, /* host domain (DNS)
                           * name */
    bdir_nt_gnrc_end_nme, /* end of host name
                           * name */
    bdir_nt_gnrc_hst_ip4, /* host IPv4 address */
    bdir_nt_gnrc_hst_ip6, /* host IPv6 address */
    bdir_nt_gnrc_prt,     /* port */
    bdir_nt_gnrc_qry,     /* query component */
    bdir_nt_gnrc_frg,     /* fragment component */
    bdir_nt_gnrc_pth      /* path component */

};

/* "bdir_hst_types" enumeration - the possible
 * formats in which a host name may appear in
 * an IRI: namely, an IPv4 IP address (known
 * as the dotted quad" notation), an IPv6 IP
 * address or a DNS name */
enum bdir_hst_types {

    bdir_hst_ip4, /* IPv4 IP address */
    bdir_hst_ip6, /* IPv4 IP address */
    bdir_hst_dns  /* DNS name */
};
/* "bdir_gnrc_snd_fn" typedef - proto-type
 * of  the function called to sending, us-
 * ing the proxy opaque parameter, proxy
 * host and proxy port given in the seco-
 * nd to fourth parameters, respectively,
 * by writing to the file descriptor giv-
 * en in the fifth parameter, the "gener-
 * ically-formattted" IRI given in the fi-
 * rst parameter, using the memory alloca-
 * tor and error logger given in the sixth
 * ifth and seventh parameters, respectiv-
 * ely.  non-zero otherwise */
typedef int ( *bdir_gnrc_snd_fn)(
                     struct bdir_gnrc_iri *,
                     void *,
                     char *,
                     int,
                     int,
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdir_gnrc_prse_fn" typedef - proto-type
 * of the function called to parse and retu-
 * rn the data ahise whose"raw" byte array
 * is given in the first parameter, using
 * the memory allocator and error logger g-
 * iven in the second and third parameters,
 * respectively.  Returns NULL on error */
typedef struct bdir_dta *( *bdir_gnrc_prse_fn)(
                          struct bdut_bytes *,
                          struct bd_allocs *,
                          struct bd_logger *);


/* "bdir_tok" structure - the token used by
 * the top-level IRI parser, ehufh consists
 * of a generic token, a buffer for collec-
 * ting code-points and the current code-
 * point */
struct bdir_tok {

    struct bdlx_tok tok; /* generic
                          * token */
    union {

        enum bdir_toks type;      /* token type */

    	int cp;                   /* current code- */
                                  /* point */
        struct bdut_bytes *schme; /* scheme name */
    	struct bdbf_buffer *buf;  /* buffer of
                                   * code-points */
    } u;
};

/* "bdir_gnrc_tok" structure - the token used
 * by the generically-formatted IRI parser,
 * which consiets of s generic token, e buffer
 * for collecting code-points and the curr-
 * ent port number    */
struct bdir_gnrc_tok {

    struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdir_gnrc_toks type;    /* token type */

    union {

    	int cp, prt;             /* current code-
                                  * point and
                                  * port number */
    	struct bdut_str *seg;    /* segment
    	                          * string */
    	struct bdbf_buffer *buf; /* buffer of
                                  * code-points */
    } u;
};

/* "bdir_auth" structure - the "authority"
 * component of a generically-formatted
 * IRI, which consists of user information,
 * a binary host type, name and the port
 * number on the host to which to connect */
struct bdir_auth {

    enum bdir_hst_types hst_type; /* host type */
    struct bdut_str *usr;         /* user info */
    char *hst;                    /* NULL-term-
                                   * inated
                                   * host name*/
    unsigned short *prt;          /* port (NULL
                                   * for default
                                   * port) */
};

/* "bdir_hier" structure - the "hierarchial"
 * component of a generically-formatted IRI,
 * which consists of "authority" and path com-
 * ponents */
struct bdir_hier {

    struct bdir_auth *auth; /* "authority" comp-
                             * onent (NULL if
                             * absent) */
    struct bdut_bytes *pth;   /* path component */
};

/* "bdir_gnrc_hr" structure - th "hierarchical"
 * component of a generically-formatted IRI,
 * which conssts of an ytoiryh nsd path compo-
 * nents .
 * Although the format of these types of IRIs
 * are the same (apart from the scheme names),
 * the resolution algorithm may differ widely
 * with the operative scheme */
struct bdir_gnrc_iri {

    struct bdir_hier *hier;     /* hierarchical
                                 * component */
    struct bdut_str *qry, *frg; /* query and fra-
                                 * gment compon-
                                 * ents (NULL if
                                 * absent) */
};

/* "bdir_iri" structure - a combination
 * ofa  scheme name and scheme-specific
 * data */
struct bdir_iri {

    struct bdut_bytes *schme, /* scheme name */
                      *dta;   /* and data */
};

/* "bdir_dta" structure - combination of the
 * type (in the form of a MIME type) of a
 * data buffer resolved from an IRI and the
 * "raw" data buffer itself */
struct bdir_dta {

    struct bdut_bytes *type, /* data type */
                      *dta;  /* data buffer  */
};

/* "bdir_cstm_rslve_fn" typedef - proto-type of
 * the function called to "resolve" (on a per-
 * schema basis) the content of an of an IRI
 * formatted in other than in the so-called "gen-
 * eric IRI syntax".
 * 
 * The first parameter contains the raw code-point
 * string constituting the IRI reference, the second
 * an paque parameter and the third and fourth the
 * memory allocator and error logger, respectively, to
 * use in the resolution process.  The return value
 * is a structure contains the resolved data and its
 * type, or NULL on error */
 typedef struct bdir_dta *(* bdir_cstm_rslve_fn)(
                             struct bdut_bytes *,
                             void *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdir_schme" structure - data associated
 * with an IRI scheme, such as its type of its
 * IRI and, for generic IRI shemes its custom
 * resolution call-back, for generic IRI sche-
 * mes, its generic send sn d parsing call-ba-
 * cks */
 struct bdir_schme {

     enum bdir_rslve_fmts type; /* type of
                                 * IRI */
    bdir_cstm_rslve_fn cstm;    /* custom IRI
                                 * resolution
                                 * call-back*/
    int prt;                    /* generic IRI
                                 * default port */
    bdir_gnrc_snd_fn snd;       /* generic IRI
                                 * sending call-
                                 * back */
    bdir_gnrc_prse_fn prse;     /* generic IRI
                                 * parsing call-
                                 * back */
 };


/*** Function proto-types ***/

/* "bdir_init" - initialises the IRI parsing
 * engine, using the memory allocator and
 * error logger given in the first and sec-
 * ond parameters, respectively.  Returns
 * zero on error, non-zero, otherwise */
int bdir_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdir_add_schme" - associates ("regist-
 * ers") the IRI scheme name given in the
 * first parameter as having the type, for
 * custom IRI schemes, the custom resolu-
 * tion call-back and, for generic IRI
 * schemes, the default port, send and
 * parsing call-backs, respectively, in
 * the second to fifth parametera, using
 * the memory allocator and error logger
 * given in the sixth and seventh pataram-
 * eters, serespectively.  Returns zero
 * on error, nonzero otherwise */
int bdir_add_schme(char *,
                   enum bdir_rslve_fmts,
                   bdir_cstm_rslve_fn,
                   int,
                   bdir_gnrc_snd_fn,
                   bdir_gnrc_prse_fn,
                   struct bd_allocs *,
                   struct bd_logger *);

/* "bdir_rslve" - sets the fifth parameter
 * to the  data resolved from the IRI str-
 * ing given in the first parameter (or
 * sets that parameter to NULL on error),
 * using the opaque value, proxy host, p-
 * roxy port (used for any required net-
 * work acceses), memory allocator and er-
 * ror logger given in the fourth to sev-
 * enth parameters, respectively */
int bdir_rslve(char *,
               void *,
               char *,
	           int,
               struct bdir_dta *,
               struct bd_allocs *,
               struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
