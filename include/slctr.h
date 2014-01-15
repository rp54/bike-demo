/********************************************************************
*
* Bike Demo - Exercise Bike Demonstration
* Scenery
*
* slctr.h - data types and function proto-
* types related to "selectors", which are
* used in two contexts: to specify the conf-
* iguration of XML elements that must be
* "selected" (i.e. that must match the
* current element) for a series of CSS
* properties to apply, and in the "selector
* text" for an XML "pattern" in which the
* selector text of the pattern must apply
* for the pttern to be "recognised", thereby
* calling its associated callback
*
* The selectors recognised nby the parser
* in this obode are fvconformant to the 
* W3C "selectors 4" standard
* 
* Copyright (C) 2007-2013, Russell Potter,
* All rights reserved
*
*********************************************************************/

#ifndef __BD_SLCTR_H
#define __BD_SLCTR_H

#ifdef __cplusplus
extern "C" {
#endif

struct bdxl_node;

#include <bikedemo/xml.h>
#include <bikedemo/eval.h>
#include <bikedemo/lex.h>
#include <bikedemo/grammar.h>
#include <bikedemo/util.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

struct bdsl_smple;

/* "bdsl_cmbntrs" enumeration the types of
 * simple selector combinators, as passed
 * to the combinator call-back */
enum  bdsl_cmbntrs {

    bdsl_cmbntr_dsc,  /* descendant */
    bdsl_cmbntr_chld, /* child */
    bdsl_cmbntr_adj,  /* adjacent */
    bdsl_cmbntr_gnrl, /* general */
    bdsl_cmbntr_ref   /* reference */
};

/* "bdsl_smple_types" enumeration - the
 * types of simple a selector, which may
 * be ID, class or attribute */
	 enum  bdsl_smple_types {

    bdsl_smple_id,   /* ID type */
    bdsl_smple_clss, /* class type */
    bdsl_smple_attr  /* attribute */
};

/* "bdsl_stk_types" enumeration - the poss-
 * ible types that may appear on the sele-
 * ctor evaluator's run-time stack, namely,
 * nodes, booleans, strings, numeric values,
 * return addresses or match results */
enum bdsl_stk_types {

    bdsl_stk_nde,  /* XML node */
    bdsl_stk_bln,  /* boolean */
    bdsl_stk_str,  /* string */
    bdsl_stk_rslt, /* match result */
    bdsl_stk_addr, /* return address */
    bdsl_stk_num   /* numeric */
};

/* "bdsl_cmbtr_fn" typedef - proto-ty-
 * pe of the function called when a co-
 * mbinator of simple selectors is en-
 * countered in a selector.  The first
 * parameter contains the parse simple
 * selector to run against the node
 * given in the second parameter, lin-
 * ked by the combinator type given in
 * thethird parameter , using the opa-
 * que value, memory allocator and error
 * logger to use, and the value to set,
 * on exit, to an indication as to whet-
 * her the combination should match
 * given in the fourth to seventh para-
 * meters, resepectively.  Returns zero
 * on error , non-zero otherwise */
typedef int (* bdsl_cmbtr_fn)(struct bdsl_smple,
                              struct bdxl_node,
                              enum bdsl_cmbtr_types,
                              void *,
                              struct bd_allocs *,
                              struct bd_logger *,
                              int *);

/* "bdsl_smple_fn" typedef - proto-
 * ty-pe of the funrtion called when
 * a simple selector property (such
 * as an ID, class or attribute sel-
 * ector) is encountered in a selec-
 * tor.  The first parameter is the
 * type of selector, the second an
 * opaque parameter, the third the
 * current node, the furth and fifth
 * the memory allocator and error
 * logger to use, and the sixth is
 * set, on exit, to an indication
 * as to whether the attribute is a
 * match.  Returns zero on error,
 * non-zero otherwise */
typedef int (* bdsl_smple_fn)(enum bdsl_smple_types,
                              void *,
                              struct bdxl_node *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdsl_toks" enumeration - the possible
 * token types returned by the selector's
 * lexical analyser */
enum bdsl_toks {

    bdsl_tok_nme =      /* element name */
        bdgm_sym_nres,
    bdsl_tok_ref,       /* reference ("/id/")
                         * combinator */
    bdsl_tok_id,        /* identifier */
    bdsl_tok_str,       /* string             (\'abc\')
                         *             or
                         *                    \"abc\")  */
    bdsl_tok_ws,        /* white-space */
    bdsl_tok_gt,        /* greater than       (">") */
    bdsl_tok_cma,       /* comma              (",") */
    bdsl_tok_int,       /* integral number    ("[0-9]+")*/
    bdsl_tok_pre,       /* prefix match       ("^=") */
    bdsl_tok_pst,       /* postfix match      ("$=") */
    bdsl_tok_sub,       /* sub-string
                         * match              ("*=") */
    bdsl_tok_eq,        /* equality           ("=")
                         * match */
    bdsl_tok_lpr,       /* left
                        /* parenthesis        ("(")  */
    bdsl_tok_rpr,       /* right */
                        /* parenthesis        (")") */
    bdsl_tok_lsq,       /* left square
                        /* bracket            ("[") */
    bdsl_tok_rsq,       /* right square
                        /* bracket            ("]") */
    bdsl_tok_ppe,       /* pipe               ("|") */
    bdsl_tok_clmn,      /* same column        ("||") */
    bdsl_tok_hsh,       /* hash               ("#") */
    bdsl_tok_clss,      /* class              (".") */
    bdsl_tok_incl,      /* includes           ("~=") */
    bdsl_tok_dm,        /* dash-match         ("|=") */
    bdsl_tok_plus,      /* plus               ("+") */
    bdsl_tok_cln,       /* colon              (":") */
    bdsl_tok_ast,       /* asterisk           ("*") */
    bdsl_tok_tlde,      /* tilde              (~") */
    bdsl_tok_kwd_n,     /* "n" (multiplier)   keyword */
    bdsl_tok_kwd_i,     /* "i" (ignore case)  keyword */
    bdsl_tok_kwd_rt,    /* "root"             keyword */
    bdsl_tok_kwd_mtchs, /* "matches"          keyword */
    bdsl_tok_kwd_not,   /* "not"              keyword */
    bdsl_tok_kwd_nc,    /* "nth-child"        keyword */
    bdsl_tok_kwd_nlc,   /* "nth-last-child"   keyword */
    bdsl_tok_kwd_mths,  /* "matches"          keyword */
    bdsl_tok_kwd_nm,    /* "nth-match"        keyword */
    bdsl_tok_kwd_nlm,   /* "nth-last-match"   keyword */
    bdsl_tok_kwd_fc,    /* "first-child"      keyword */
    bdsl_tok_kwd_lc,    /* "last-child"       keyword */
    bdsl_tok_kwd_nt,    /* "nth-of-type"      keyword */
    bdsl_tok_kwd_nlt,   /* "nth-last-of-type" keyword */
    bdsl_tok_kwd_fot,   /* "first-of-type"    keyword */
    bdsl_tok_kwd_lot,   /* "last-of-type"     keyword */
    bdsl_tok_kwd_oc,    /* "only-child"       keyword */
    bdsl_tok_kwd_oot,   /* "only-of-type"     keyword */
    bdsl_tok_kwd_mty,   /* "empty"            keyword */
    bdsl_tok_kwd_evn,   /* "even"             keyword */
    bdsl_tok_kwd_odd,   /* "odd"              keyword */
    bdsl_tok_kwd_lng,   /* "lang"             keyword */
    bdsl_tok_kwd_dir,   /* "dir"              keyword */
    bdsl_toks_num       /* number of tokens */
};

/* "bdsl_smple_nts" enumeration - the non-
 * terminalsof the selector grammar */
enum bdsl_smple_nts {

    bdsl_nt_slctr = bdsl_toks_num + 1,
    bdsl_nt_slctr_grp,
    bdsl_nt_ws,
    bdsl_nt_cmbntr,
    bdsl_nt_cmbntr_chld,
    bdsl_nt_cmbntr_grnd,
    bdsl_nt_cmbntr_gadj,
    bdsl_nt_cmbntr_sbl,
    bdsl_nt_cmbntr_ref,
    bdsl_nt_slctr_list,
    bdsl_nt_smple_slctr_strt,
    bdsl_nt_smple_slctr_end,
    bdsl_nt_slctr_end,
    bdsl_nt_smple_slctr,
    bdsl_nt_smple_slctr_seq,
    bdsl_nt_smple_slctr_prp,
    bdsl_nt_smple_slctr_prp_seq,
    bdsl_nt_clss,
    bdsl_nt_attr,
    bdsl_nt_psdo,
    bdsl_nt_psdo_plce,
    bdsl_nt_psdo_plce_type,
    bdsl_nt_psdo_plce_ntype,
    bdsl_nt_psdo_plce_ltype,
    bdsl_nt_psdo_plce_nltype,
    bdsl_nt_psdo_fot,
    bdsl_nt_psdo_lot,
    bdsl_nt_psdo_nt,
    bdsl_nt_psdo_nlt,
    bdsl_nt_psdo_nc,
    bdsl_nt_psdo_nlc,
    bdsl_nt_psdo_nm,
    bdsl_nt_psdo_nlm,
    bdsl_nt_psdo_lc,
    bdsl_nt_psdo_fc,
    bdsl_nt_psdo_lng,
    bdsl_nt_elem_nme,
    bdsl_nt_type,
    bdsl_nt_unvrsl,
    bdsl_nt_ns_prfx,
    bdsl_nt_expr,
    bdsl_nt_not,
    bdsl_nt_not_arg
};

/* "bdsl_cdes" enumeration - the possible
 * "codes" generated by the selector's run-
 * timre evaluator, as called by the main
 * selector code */
enum bdsl_cdes {

    bdsl_cde_attr,  /* push non-zero if the
                     * value of attribute of
                     * node in stack top has
                     * name in second stack
                     * top, and increment 
                     * run-time specifity's
                     * "attrs" count */
    bdsl_cde_lcl,   /* push non-zero
                     * if node on stack top
                     * has local name given
                     * on stack top and name-
                     * space ond tack top,
                     * then increment run-
                     * time specifity's
                     *  "ids" count */
    bdsl_cde_psdo,  /* call the call-back
                     * associated with
                     * pseudo-element named
                     * in stack top, and
                     * increment evaluator
                     * run-time specifity's
                     * "pseudos" count */
    bdsl_cde_cmbtr, /* call combinator func-
                     * tion in stack top,
                     * passing  combinator
                     * type in second stack
                     * top */
    bdsl_cde_smple, /* call simple selector
                     * function in stack top,
                     * passing  the type in
                     * second stack top */
    bdsl_cde_mtch,  /* push non-zero if node
                     * in stack top matches
                     * selector text in se-
                     * cond stack top */
    bdsl_cde_and,   /* push a boolean "and"
                     * of top two stack
                     * positions */
   bdsl_cde_ns,     /* push non-zero
                     * of node on stack top
                     * is in name-space
                     * given second stack top. */
    bdsl_cde_nns,   /* push non-zero if node
                     * on stack top is not in
                     * a name-space */
    bdsl_cde_a_ns,  /* push non-zero if attri-
                     * bute of node on stack
                     * top named second stack
                     * top is in name-space
                     * third stack top */
    bdsl_cde_a_nns, /* push non-zero if attr-
                     * ibute of node on stack
                     * top named in second
                     * stack top is not in a
                     * name-space */
    bdsl_cde_id,    /* push non-zero if node
                     * in stack top has ID
                     * named in second stack
                     * top. */
    bdsl_cde_clss,  /* push non-zero if node
                     * on stack top has class
                     * named in second stack
                     * top */
    bdsl_cde_strts, /* push the non-zero
                     * if value of attr, of
                     * node on top stack
                     * named in second on
                     * stack starts with
                     * string in third stack
                     * top */
    bdsl_cde_ends,  /* push the non-zero
                     * if value of attr, of
                     * node on top stack
                     * named in second on
                     * stack ends with
                     * string in third stack
                     * top */
    bdsl_cde_sbstr, /* push the non-zero
                     * if value of attr, of
                     * node on top stack
                     * named in second on
                     * stack is contained in
                     * string in third stack
                     * top */
    bdsl_cde_not,   /* invert truth value on
                     * stack top */
    bdsl_cde_sbr,   /* indicate that all codes
                     * up to the next "ret"
                     * will be placed into the
                     * sub-routine numbered on
                     * stack top at run-time */
    bdsl_cde_frst,  /* push non-zero if node
                     * on stack top has sibling
                     *index given in second
                     * stack top plus added to
                     * multiplication of third
                     * stack top and of same type
                     * as node if fourth stack
                     * top is non-zero */
    bdsl_cde_lst,   /* push non-zero if node
                     * on stack top has sibling
                     * index from,last given
                     * in second stack top added
                     * to multiplication of third
                     * stack top and of same type
                     * as node if fourth stack
                     * top is non-zero */
    bdsl_cde_rt,    /* push non-zero if node on
                     * stack top is the root node
                     * if a document */
    bdsl_cde_only,  /* push non-zero if node on
                     * stack top is an only child */
    bdsl_cde_empty, /* push non-zero if node on
                     * stack top has no children */
    bdsl_cde_ret,   /* return xecution to main eval-
                     * uator */
    bdsl_cde_nxt,   /* push the next sib-
                     * ling of the current
                     * node, or NULL if
                     * is last */
    bdsl_cde_prv,   /* push the previous
                     * sibling of the cur-
                     * rent node, or NULL
                     * if is first */
    bdsl_cde_prnt,  /* push the previous
                     * sibling of the cur-
                     * rent node, or NULL
                     * if is last */
    bdsl_cde_or,    /* push a boolean "or"
                     * of top two stack
                     * locations */
    bdsl_cde_end,   /* exit the run-time */
    bdsl_cdes_num   /* number of sub-rout-
                     * ine selector codes */
};

/* "bdsl_psdo_fn" typedef - proto-type of
 * the function called to implement the sem-
 * antics of a selector "pseudo class".
 * The first parameter is an opaque value
 * to pass with the call and the second and
 * third parameters contain the memory alloc-
 * ator and error logger to use */
typedef int (* bdsl_psdo_fn)(void *,
                             struct bd_allocs *,
                             struct bd_logger *);


/* "bdsl_tok" structure - the type of the kexical
 * token returned by the selector extca analyser.
 * It consists of the callbacks required by gen-
 * eric tokens, as well as the name, string lit-
 * eral and number scanned */
 struct bdsl_tok {

    struct bdlx_tok tok; /* generic
                          * token */
    enum bdsl_toks type; /* token
                          * type */
    union {
        int cp, num; /* code-point
                      * and currently
                      * scanned number */
        struct bdut_str *name,
                        *ref,
                        *str;
                     /* currently scanned */
                     /* name, reference or
                        string
                      * literal */
        struct bdbf_buffer *buf;
                     /* buffer into which
                      * to collect input */
    } u;
};

 /* "bdsl_spcfty" structure - an indication
  * of the degree to which a selector matches
  * an XML element:  in terms of its number
  * of IDs, attributes and element names */
 struct bdsl_spcfty {

     int num_ids,   /* number of IDs, */
         num_attrs, /* attributes */
         num_nms;   /* and element names, */
                    /* respectively, in
                    /* match */
};

 /* "bdsl_rt" structure - constitutes the
  * "run-time" if the selector parser, which
  * consists if the selector's specifity,
  * the currently active name-space mappings,
  * each oc which id keyed to the namespace's
  * prefix and valued to the namspace's IRI,
  * the currently active pseudo-element mapp-
  * ings each of which is keyed to the ele-
  * ment's nane and valued to the its imple-
  * mentation, an indication as to whether
  * the selector string is case-sensitive,
  * an evaluator used to parse the selector
  * and the combinator call-back in use */
 struct bdsl_rt {

    struct bd_map_node *nss,    /* current
 	                             * name-spa-
 	                             * ces */
                       *psdos;  /* current
 	                             * pseudo-
                                 * element
 	                             * mappings */
    int cse;                    /* whether
                                 * selector
                                 * string is
                                 * case-sensi-
                                 * tive */
    struct bdev_cdes *cdes;     /* codes with
                                 * which to
                                 * execute
                                 * selector */
                                /* string is
                                 * case-sensi-
                                 * tive */
    struct bdsl_spcfty *spcfty; /* specifity
                                 * of selector */
    struct bdev_cdes *main;     /* main selec-
                                 * tor codes */
    struct bd_map_node *smples; /* set of si-
                                 * mple sele-
                                 * ctor rout-
                                 * ines */
    bdsl_cmbtr_fn cmbtr;        /* combinator
                                 * call-back */
};

 /* "bdsl_sbr_rt" structure - a selector "sub-
  * routine" run-time, which consists of an
  * evaluator of the codes generated by the
  * parsing of a simple selector and a ref-
  * erence to the main selector run-time.
  * Each simple selector  is parsed into a
  * sub-routine since it may be called repeat-
  * edly in the combinator matching process */
  struct bdsl_sbr_rt {

         struct bdsl_rt *rt;     /* main sel-
                                  * ector run-
                                  * time */
         struct bdev_cdes *cdes; /* this sub-
                                  * routine
                                  * evaluator */
};

/*** Function proto-types ***/

/* "bdsl_init" - load the selector system
 * with its constituent lexical rules and
 * grammar productions using the memory
 * allocator and error logger given in the
 * first and second parameters, respectively
 * Returns zero on error, non-zero other-
 * wise */
int bdsl_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdsl_select" - sets the fifth parameter
 * to a non-NULL value if the selector text
 * the UTF-8 encoding of which is given in
 * the third parameter matches the element
 * given in the fourth parameter (with the
 * fields of the fifth parameter indicat-
 * ing the specifity of a successful mat-
 * ch), using memory allocator and error
 * logger given in the first and second
 * parameters, respectively.  The fourth
 * parameter indicates whether the selector
 * string is case-sensitive.  Returns zero
 * on error, non-zero otherwise */
int bdsl_select(struct bd_allocs *,
                struct bd_logger *,
                char *,
                struct bdxl_node *,
                struct bdsl_spcfty *);

/* "bdsl_add_psdo" - using the
 * memory allocator and error
 * logger given in the first and
 * second parameters, respecti-
 * vely, adds an associa-
 * tion between the pseudo-class
 * named in he third parameter
 * and its implementing call-back
 * it in the fourth parameter */
int bdsl_add_psdo(struct bd_allocs *,
		          struct bd_logger *,
		          struct bdsl_rt *,
                  char *,
                  bdsl_psdo_fn);

/* "bdsl_add_ns" - using the
 * memory allocator given in the
 * forst snd second parameters,
 * respectively, sad a name-space
 * whose prefix and IRI are given
 * in the fourth and fifth param-
 * eters, respectively, to the
 * selector run-time given in the
 * third parameter */
int bdsl_add_ns(struct bd_allocs *,
                struct bd_logger *,
                struct bdsl_rt *,
                char *,
                char *);

/* "bdsl_init_rt" - initialise
 * and retrirn a selector run-
 * time, using the memory all-
 * ocator given in the first
 * and second parameters, res-
 * pectively */
struct bdsl_rt *bdsl_init_rt(
                 struct bd_allocs *,
		         struct bd_logger *);


/* ""bdsl_prse" - convert and
 * return the an internal rep-
 * resentation ( a selector
 * run-time) from the selector
 * string given in the third,
 * paraneter, using the memory
 * allocator and error logger
 * given in the first and sec-
 * ond parameters, respectively.
 * The internal represeentat-
 * ion so created may then be
 * repetedly executed (during
 * the matching process) more
 * efficiently.  Returns NULL
 * on error */
struct bdsl_rt *bdsl_prse(
          struct bd_allocs *,
          struct bd_logger *,
          char *);

/* "bdsl_exec" - "executes" (by
 * running its avaluator against
 * the XML node in the fourth
 * parameter) the pre-parsed
 * selector run-time in the th-
 * ird parameter, and sets the
 * fifth parameter to the run-
 * time's specifity after exec-
 * ution (or to NULL if "nde"
 * does not match the selector),
 * using the memory allocator
 * and error logger given in the
 * first and second parameters,
 * respectively.  This routine
 * allows repeated matching of
 * selectors against nodes with-
 * out the over-head of prior
 * parsing */
int bdsl_exec(struct bd_allocs *,
              struct bd_logger *,
              struct bdsl_rt *,
              struct bdxl_node *,
              struct bdsl_spcfty *);

/* "bdsl_set_cmbntr" - sets the
 * "combinator call-back" of the
 * selector run-time given in
 * the third parameter ro the
 * routine given in the fourth
 * parameter, iusing the memory
 * allocator and error logger
 * given in the first and second
 * parameters, respectively */
int bdsl_set_cmbtr(struct bd_allocs *,
                   struct bd_logger *,
                   struct bdsl_rt *,
                   bdsl_cmbtr_fn);

/* "bdsl_set_smple" - sets the
 * "simple selector call-back"
 * of the selector run-time gi-
 * ven in the third parameter
 * to the routine given in the
 * fourth parameter, iusing the
 * memory allocator and error
 * logger given in the first
 * and second parameters, res-
 * pectively */
int bdsl_set_smple(struct bd_allocs *,
                   struct bd_logger *,
                   struct bdsl_rt *,
                   bdsl_smple_fn);

#ifdef __cplusplus
}
#endif

#endif
