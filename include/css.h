/********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery 
*
* "css.h" - data types and function proto-
* types related to the W3C'S Cascading Style
* Sheet (CSSS) specification (version 3),
* which is used to associate "properties"
* with the elements of an XML file (which
* may, but need not, be related to the
* presentation of the document), which
* may also then "cascade"  upward to its
* parent elements as necessary.
* 
* Copyright (C) 2007-2012, Russell Potter,
* All rights reserved
*
*********************************************************************/

#ifndef __BD_CSS_H
#define __BD_CSS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/util.h>
#include <bikedemo/map.h>
#include <bikedemo/lex.h>
#include <bikedemo/parser.h>
#include <bikedemo/grammar.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "bdcs_sht_types" enumeration - the
 * ways in ehich a CSS style-syeet can
 * be specified (ich affects the prec-
 * edence of its contained properties):
 * either by the style-sheet author,
 * the user or directly by the user-
 * agent */
enum bdcs_sht_types {

    bdcs_sht_type_usr,  /* user-specified */    
    bdcs_sht_type_athr, /* author-specified */
    bdcs_sht_type_ua    /* via user-agent */
};

/* "bdcs_toks" enumeration - the lexical
 * tokens of the CSS grammar: includes
 * string literals, numeric literals,
 * (dhuchmsy optionally include a mea-
 * surement unit) abd ibe if several
 * directive keywords such as "@import",
 * ""@media and "@charset" */
enum bdcs_toks {

    bdcs_tok_ws,        /* white-space */
    bdcs_tok_lbr,       /* left brace ("{") */
    bdcs_tok_rbr,       /* right brace ("}") */
    bdcs_tok_cln,       /* colon (":")*/
    bdcs_tok_str,       /* string */
    bdcs_tok_id,        /* identifier */
    bdcs_tok_at_kwd,    /* "at" keyword */
    bdcs_tok_nbr,       /* unit-less number */
    bdcs_tok_pcnt,      /* percentage */
    bdcs_tok_msrmnt,    /* numeric mesurement
                         * (with optional unit) */
    bdcs_tok_fn,        /* function ("{id}(") */
    bdcs_tok_uc_rnge,   /* Unicode range ("U+{nn}-
                         * {nn}" or "U+{nn}???") */
    bdcs_tok_url,       /* URL ("url({url})") */
    bdcs_tok_clr,       /* colour(keyword or
                         * ("rgb(<xx>, <xx>,
                         * <xx>)") */
    bdcs_tok_kwd_mda,   /* "@media" keyword */
    bdcs_tok_kwd_imprt, /* "@import" keyword */
    bdcs_tok_kwd_none,  /* "none" keyword */
    bdcs_tok_kwd_chrst, /* "@charset" keyword */
    bdcs_tok_kwd_inhrt, /* "inherit" keyword */
    bdcs_tok_num        /* number of tokens */
};

/* "bdcs_nts" enumeration - non-
 * terminals of the CSS grammar,
 * including */
enum bdcs_nts {

    bdcs_nt_sheet = bdcs_tok_num + 1,
    bdcs_nt_chrst,
    bdcs_nt_imprt,
    bdcs_nt_imprts,
    bdcs_nt_dcl,
    bdcs_nt_rlst,
    bdcs_nt_at_drctv
};

/* bdcs_imprt" structure - a record of
 * an "import" of a CSS style-sheet from
 * another style-sheet dsuch that, when
 * the import iccurs, declarations from
 * the imported style-sheet ceome avail-
 * able to the importing style-sheet.
 * It consists of the URL string of the
 * import and a reference to the import-
 * ing style-sheet */
struct bdcs_imprt {

	struct bdut_str url; /* URL of this
                          * document */

    struct bdcs_imprt *imprtr;
              /* importing document */
};

/* "bdcs_pr_env" structure - a CSS "par-
 * sing environment": a set of values
 * that nay sffect in some way the
 * parsing of a CSS-formatted data stream,
 *huch consists of the character set in
 * use, the stacl of current imports and
 * whether to be case sensitive */
struct bdcs_pr_env {

    enum bdcs_sht_types type;  /* how sheet was
                                * specified */
	int igncse;                /* whether to
                                * be case ins-
                                * ensitive */
    int ws;                    /* whether white-
                                * space id impo-
                                * rtnant */
};

/* "bdcs_fn" structure - represents a
 * CSS "function", which consists of the 
 * function  nsme and the fuction's arg-
 * ument */
struct bdcs_fn {

    struct bdut_str name, /* function name */
                    arg;  /* and argument
                           * strings */

};

/* "bdcs_msrmnt" structure - a CSS "meas-
 * urement": a numeric value accompanied
 * by that value's unit of measurement
 * (such as "pixels" or "inches") */
struct bdcs_msrmnt {

    double value;          /* magnitude of
                            * value */
	struct bdut_str units; /* units string
                            * value */
};

/* "bdcs_clr" structure - a CSS colour,
 * consisting of its three double-
 * valued red green, blue and alpha
 * components in the range 0 to 1 */ 
 struct bdcs_clr {

    double r, g, b, a; /* magnitude of
                        * each colour
                        * components, and
                        * alpha channel
                        * (in the range 0
                        * to 1) */
};

/* "bdcs_urnge" structure - represents a range
 * of code-points between a lower and upper
 *  bounds */
struct bdcs_urnge {

    int lowr, uppr; /* code-points of lower and
                     * upper bounds, respectiv-
                     * ely, of range */
};

/* "bdcs_tok" structure - lexical token for
 * the parser of the CSS format: contains,
 * together with a generic tokenrefernce,
 * the currently scanned dentifier, string,
 * number and measurement */
struct bdcs_tok {

    struct bdlx_tok tok;      /* generic token */

    enum bdcs_toks type;      /* token type */

	int rdx,                  /* for numeric types,
                               * multiplicand of
                               * current digit */
        idx;                  /*  current index
                               * into string or
                               * identifier */ 
    struct bdbf_buffer *buf;  /* buffer holding
                               * map of allocated
                               * memory segments
				              /* used to build
                               * current string
		                       * or identifier */
    union {

    	struct bdut_str id, str;   /* currently */
    	                           /* scanned  */
                                   /* identifier
                                    * and string */
        int cp;                    /* currently
                                    * scannned
                                    * code-point */
        struct bdcs_clr *clr;      /* colour */
        struct bdcs_urnge range;   /* current code-
                                    * point range */
	    double num;                /* number  */
        struct bdcs_msrmnt msrmnt; /* measurement */
        struct bdcs_fn fn;         /* function */     
    } u;
};

/* "bdcs_prop" structure - a single CSS
 * "property" withtin a style-sheet, which
 * contains callback to read property's
 * value, whose first parameter is the
 * property in question, second is a map of
 * he CSS tokens consituting the contents
 * of the property, the third is an opaque
 * value snd fourth and fifth are the
 * memory allocator and error logger to
 * use in the call, and whose return
 * value is a type-neutral represe-
 * ntation of the ptoperty's value, or
 * NULL on error */
struct bdcs_prop {

    void *(* get)(struct bdcs_prop *,
                  struct bd_map_node *,
                  void *,
                  struct bd_allocs *,
                  struct bd_logger *);
};

/* "bdcs_mda" structure - a single CSS
 * media type, which consists a routine
 * to return the implementation-depend-
 * ent cirrent value of the name given
 * by the code-point array given in the
 * routine's first parameter, abd a rou-
 * tine to "render" to the media's
 * device using the values of the prop-
 * erty types returned from the first
 * routine using the opaque value in
 * the first parameter */
struct bdcs_mda {

    void *( *prp)(int *);
        /* return value of named
         * property */
    void ( *rndr)(void *);
        /* render to device using
         * opaque parameter */
};

/* "bdcs_sheet" structure - a CSS
 * "stylesheet", rhivu contains a mapp-
 * ing between CSS properties abd their
 * selector strings, indicating the cond-
 * itions under which each becomes "sel-
 * ected" (or "activated") */
struct bdcs_sheet {

    struct bd_map_node *by_slctr, /* prop-
                      * erites for each
                      * selector */
                       *by_prop; /* selec-
                      * tors for each prop-
                      * erty*/
};


/***  Function proto-types ***/

/* "bdcs_add_prop_type" - adds the CSS property
 * named in the third parameter, having a media
 * type named in the fourth parameter and being
 * a sub-property of the property given in the
 * second parameter to the style-sheet given in
 * the first parameter, using the memory alloc-
 * ator and error logger given in the sixth and
 * seventh parameters, respectively */
 void *bdcs_add_prop_type(struct bdcs_sheet *,
                          struct bdcs_prop *,
                          struct bdut_str,
                          struct bdut_str,
                          struct bd_allocs *,
                          struct bd_logger *); 

/* "bdcs_parse" - psrses the CSS-formatted stream
 * of bytes given in the first parameter and
 * using the parsing environment given in the
 * fourth parameter, returning the style-sheet
 * found in the stream, using the memory alloc-
 * ator and error logger given in the second
 * and third parameters, respectively */
struct bdcs_sheet *bdcs_parse(struct bdut_bytes,
                              struct bd_allocs *,
                              struct bd_logger *,
                              struct bdcs_pr_env *);
 
/* "bdcs_init_sheet" - returns and initialises
 * ab (initially empty) CSS style-sheet, using
 * the memory allocator and error logger given
 * in the first and second parameters, respecti-
 * vely */
struct bdcs_sheet *bdcs_init_sheet(
                         struct bd_allocs *,
                         struct bd_logger *); 

/* "bdcs_init" - initialises the CSS sub-system,
 * using the memory allocator and error logger
 * given in the first and second parameters,
 * respectively.  Returns on error, non-zero
 * otherwise */
int bdcs_init(struct bd_allocs *,
              struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
