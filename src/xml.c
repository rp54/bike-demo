/********************************************************************
*
* BikeDemo - Exercise Bike Demonstration Scenery
*
* xml.c - function implementations related to
* an Extensible Markup Language(XML) processor,
* used for parse an XML-formatted input stream
* into a tree of XML "information items".
*
* When certain configurations of elements and
* attribute names are seen present the document
* are seen ,
* the parser executes sn associated "recognition
* callback" routine, such that meaningful proce-
* sing may be accomplished through the execution
* of these call-backs in the sequence dictated
* by the arrangement of these elements and/or
* attributes.
*
* The data types found in this code include the
* lexical token tyoe used by the XML lexical
* analyser, and enumerations of the token types
* and non-terminal symbols, while the function
* proto-types include those to populate the
* parser'sgrammar productions and lexical rules.
*
* Copyright (C) 2007-2013, Russell Potter,
* All rights reserved
*
*********************************************************************/


#include <stdio.h> 
#include <wchar.h>
#include <string.h>
#include <bikedemo/xml.h> 
#include <bikedemo/iri.h> 
#include <bikedemo/util.h>
#include <bikedemo/slctr.h>
#include <bikedemo/lex.h> 
#include <bikedemo/dom.h> 
#include <bikedemo/map.h>
#include <bikedemo/buffer.h> 
#include <bikedemo/grammar.h> 
#include <bikedemo/parser.h> 
#include <bikedemo/allocs.h> 
#include <bikedemo/log.h> 

/* "base_prsr, doc_prsr, dtd_prsr,
 * elem_prsr, dtd_prsr, extsb_prsr,
 * rplcr_prsr, misc_prsr, eol_prsr"
 * - parsers to
 * generate a base definirions,
 * (top-level) XML document from
 * an XML-formatted input byte
 * stream, to parse the content
 * of a document, to generate a
 * list of document constraints
 * from a replaced Document Type
 * Declaration (DTD), to generate
 * replacement text from an input
 * stream containing entity decl-
 * arations and re- ferences and
 * to perform end-of-line proces-
 * sing on an input stream */
static struct bdpr_parser *base_prsr,
                          *doc_prsr,
                          *dtd_prsr,
                          *cmdl_prsr,
                          *extsb_prsr,
                          *bse_prsr,
                          *rplcr_prsr,
                          *eol_prsr;

/* "pis" - map of known proce-
 * ssing instructions, keyed
 * to the its name and valued
 * to its impleme- ntation
 * call-back */
static struct bd_map_node *pis;

/* "cmp_ordrs" - returns a com-
 * parison of the child orders
 * of the XML bodes in "nde1"
 * and "nde2", as the map key
 * call-back of a node's chi-
 * ldren */
static int cmp_ordrs(struct bdxl_node *nde1,
                     struct bdxl_node *nde2)
{
    return nde1->ordr - nde2->ordr;
}
/* "tok_type" - returns the type
 * of the replacer token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error  */
static int rplcr_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    return (int) rplcr_tok->type; /* return
                                   * the type
                                   * of the
                                   * token */
}

/* "reset_rplcr_tok" - per-
 * forms processing required
 * before each read of the
 * replacer token in "tok",
 * using the memory allocat-
 * or and error logger in
 * "allocs" and "logger",
 * respectively */
static void reset_rplcr_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    /* reset the token's buffer */
    bdbf_reset(rplcr_tok->u.buf,
               allocs,
               logger);
}

/* "rplcr_tok_name" - rereturns the
 * index into the message table at
 * which to find a decription of the
 * content of the replacer token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively */
static int rplcr_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    return 0; /* no description here */
}

/* "eol_add_dig" - JSON lexical analyser
 * transition call-back to add the curre-
 * ntly read code-point to that of a line-
 * feed */
static int eol_set_lf(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_eol_tok *eol_tok =
              (struct bdxl_eol_tok *)
              tok;

    /* set the token's "cp"
     * field to a line-feed */
    eol_tok->u.cp = '\xA';
    return 1;
}


/* "eol_set_cp" - lexical analyser trans-
 * ition call-back to set the token's code-
 * point field from the currently scanned
 * code-point */
static int eol_set_cp(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_eol_tok *eol_tok =
              (struct bdxl_eol_tok *)
              tok;

    /* set set the token's code-
     * point field */
    eol_tok->u.cp = cp;
    return 1;
}

/* "init_eol_tok" - performs one-
 * time initialisation of the end-
 * of-line processing token in
 * "tok", using the memory alloc-
 * ator and error logger in "all-
 * ocs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static void init_eol_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    eol_tok->u.cp = 0; /* initialise
                        * code-point  */
}

/* "eol_tok_type" - returns the
 * type of the end-of-line process-
 * ing token in "tok", using the
 * memory allocator and error log-
 * ger in "allocs" and "logger",
 * respectively.  Returns a neg-
 * ative value on error */
static int eol_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    return eol_tok->type; /* return
                           * the type
                           * of the
                           * token */
}

/* "reset_eol_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_eol_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    bdbf_reset(eol_tok->u.buf,
               allocs,
               logger);

    eol_tok->u.cp = 0; /* reset code-
                        * point */
}

/* "_reset_extsb_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_extsb_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok =
             (struct bdxl_extsb_tok *)
             tok;

    bdbf_reset(extsb_tok->u.buf,
               allocs,
               logger);
}

/* "eol_tok_name" - returns the in-
 * dexinto the message table at which
 * to find a description of the con-
 * tent of the XML end-of-line proce-
 * ssing token in "tok", where "exact"
 * indictes the level of exactness of
 * the description, using the memory
 * allocator and error logger in "all-
 * ocs" and "logger", respectively.
 * Returns a negative value on error */
static int eol_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    return 1; /* no description here */
}

/* "doc_init_tok" - performs one-
 * time initialisation of the XML
 * processor token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static void init_doc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    /* initialise token's buffer */
    doc_tok->u.buf = bdbf_init(allocs,
                               logger,
                               sizeof(int));
    doc_tok->u.str.len = 0;
}

/* "cmp_ent_nmes" - return a compa-
* rison of the entity names given in
* "nme1" and "nme2", respectively,
* as the key comparison call-back
* firor the set of entities available
& for a replacment*/
static int cmp_ent_nmes(
            struct bdxl_ent_nme *nme1,
            struct bdxl_ent_nme *nme2)
{
    /* names are identical if have the
     * type and name */
    return nme1->type -
           nme2->type &&
        bdut_strcmp(nme1->nme,
                    nme2->nme);
}

/* "cmdl_tok_type" - returns the type
 * time iof the XML element vontent
 * model token in "tok", using the me-
 * mory allocator and error logger in
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero ot-
 * herwise */
static int cmdl_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    /* initialise token's buffer */
    cmdl_tok->u.buf = bdbf_init(allocs,
                                logger,
                                sizeof(int));
    cmdl_tok->u.str.len = 0;
    cmdl_tok->u.num = 0;

    return cmdl_tok->u.buf != NULL;
}

/* "init_extsb_tok" - performs one-
 * time initialisation of the ext-
 * ernal DTD sub-set token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retur-
 * ns zero on error, non-zero othe-
 * rwise */
static void init_extsb_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    /* initialise token's buffer */
    extsb_tok->u.buf = bdbf_init(allocs,
                                 logger,
                                 sizeof(int));
    extsb_tok->u.str->len = 0;
}

/* "init_cmdl_tok" - performs one-
 * time initialisation of the XML
 element vontent model token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retur-
 * ns zero on error, non-zero othe-
 * rwise */
static void init_cmdl_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    /* initialise token's buffer */
    cmdl_tok->u.buf = bdbf_init(allocs,
                                logger,
                                sizeof(int));
    cmdl_tok->u.str.len = 0;
}

/* "doc_tok_type" - returns the
 * type of the XML processor token
 * in "tok", using the memory all-
 * ocator and error logger in
 * "allocs" and "logger", respect-
 * ively.  Returns a negative value
 * on error */
static int doc_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    return doc_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "extsb_tok_type" - returns the
 * type of the XML procexternal DTD
 * sub-set token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns a nega-
 * tive value on error */
static int extsb_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    return extsb_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "_reset_doc_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_doc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *) tok;

    /* reset the token's buffer */
    bdbf_reset(doc_tok->u.buf,
               allocs,
               logger);

    doc_tok->u.str.len = 0;
}

/* "_reset_elem_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
/* "_reset_cmdl_tok" - performs
 * prcessing required before
 * each read of the XML element
 * content model token in "tok",
 * using the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_cmdl_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;
    
    /* reset the token's buffer */
    bdbf_reset(cmdl_tok->u.buf,
               allocs,
               logger);

    /* initialise number and string to
     empty */
    cmdl_tok->u.str.len = 0;
    cmdl_tok->u.num = 0;
}

/* "doc_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the XML processor token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int doc_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    return 1; /* no description here */
}

/* "cmdl_tok_name" - returns the ind-
 * ex into the message table at which
 * to find a description of the con-
 * tent of the XML processor token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int cmdl_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    return 1; /* no description here */
}

/* "init_rplcr_tok" - performs one-
 * time initialisation of the XML
 * "text replacer" token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero other-
 * wise */
static void init_rplcr_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
             (struct bdxl_rplcr_tok *)
             tok;

    /* initialise token's buffer */
    rplcr_tok->u.buf = bdbf_init(allocs,
                                 logger,
                                 sizeof(int));
    rplcr_tok->u.num = 0;
}

/* dtd_init_tok" - performs one-
 * time initialisation of the Doc-
 * ument Type Declaration ("DTD")
 * token in "tok", using the mem-
 * ory allocator and error logger
 * in "allocs" and "logger", resp-
 * ectively.  Returns zero on
 * error, non-zero otherwise */
static void dtd_init_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    /* initialise token's buffer */
    dtd_tok->u.buf = bdbf_init(allocs,
                               logger,
                               sizeof(int));
    dtd_tok->u.str.len = 0;
}

/* "dtd_tok_type" - returns the
 * type of the Document Type Decl-
 * aration ("DTD") token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retu-
 * rns a negative value on error */
static int dtd_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    return dtd_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "_reset_dtd_tok" - performs
 * prcessing required before
 * each read of the Document
 * Type Declaration ("DTD")
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_dtd_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    /* reset the token's buffer */
    bdbf_reset(dtd_tok->u.buf,
               allocs,
               logger);

    dtd_tok->u.str.len = 0;
}

/* "bse_buffer_start" - lexical analyser
 * transition call-back to to start
 * the buffer of base parser token in
 * "tok" with "cp", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely. Returns zero on error, non-
 * zero otherwise */
static int bse_buffer_start(struct bdlx_tok *tok,
                            int cp,
                            void *opaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* start "bse_tok"'s buffer
     * with "cp" by initialising
     * the buffer, then adding
     * "cp" */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                    logger,
                                    sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "rplcr_buffer_start" - lexical analyser
 * transition call-back to to start
 * the buffer of rgwrgwthwe replacer parser token in
 * "tok" with "cp", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely. Returns zero on error, non-
 * zero otherwise */
static int rplcr_buffer_start(struct bdlx_tok *tok,
                              int cp,
                              void *opaque,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* start "bse_tok"'s buffer
     * with "cp" by initialising
     * the buffer, then adding
     * "cp" */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                    logger,
                                    sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "bse_buffer_add" - lexical analyser
 * transition call-back to to add the
 * the cideoint in cp" to the buffer
 * of the base parser token in "tok",
 * using the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively. Returns zero
 * on error, non-zero otherwise */
static int bse_buffer_add(struct bdlx_tok *tok,
                          int cp,
                          void *opaque,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* add "cp" to "bse_tok"'s buffer
     * " */
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "bse_pub_end" - lexical analyser
 * transition call-back to to end the
 * the collection of code-points into
 * the public ID of the base parser
 * token in "tok", using the token,
 * memory allocator and error logger
 * given in "tok", "allocs" and "lo-
 * gger", respectively. Returns zero
 * on error, non-zero otherwise */
static int bse_pub_end(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* set "bse_tok"'s public ID to
     * a flattening of "bse_tok"'s
     * buffer */
    bse_tok->u.pub->cps = (int *) bdbf_flttn(
                                bse_tok->u.buf,
                                allocs,
                                logger);
    bse_tok->u.pub->len = bdbf_len(bse_tok->u.buf);
}

/* "add_gent_utf" adds the general
 * entity habominf the location
 * (that is, either internal or
 * external), the UTF-8 encoding
 * of the name, public and sytem
 * IDs (if external) and literal
 * text (if internal) of which is
 * given in "loc", "nme", "pub",
 * "sys" and "lit", respectively,
 * to the replacer run-time given
 * in "rt", using the memory all-
 * ocator and errir loggeergiven
 * in "allocs" and "logger". Ret-
 * urns zero on error. non-zero
 * otherwise */
static int add_gent_utf(
               enum bdxl_ent_locs loc,
               char *nme,
               char *pub,
               char *sys,
               char *lit,
               struct bdxl_rplcr_rt *rt,
               struct bd_allocs *sllocs,
               struct bd_logger *logger)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */
    struct bdut_str *nme_str,       /* UNICODE */
                    *pub_str,       /* versions */
                    *lit_str;       /* of "nme",
                                     * "pub" and
                                     * "lit" */

    /* comnvert "nme", "pub" and "lit"
     * to UNICODE */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(pub_str,
                         pub,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(lit_str,
                         lit,
                         allocs,
                         logger)))
        return 0;

    ent.loc = loc;

    switch (loc) {

        case bdxl_ent_loc_exntrnl:
            ent.u.extrnl.pub = pub;
            ent.u.extrnl.sys = sys;
            break;

        case bdxl_ent_loc_intrnl:
            ent.u.lit.lit = lit;
            break;
    }

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->gents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                            cmp_ents,
                         &ent);
}

/* "add_pent_utf" adds the para-
 * meter entity the UTF-8 enc-
 * oding of the name  and lite-
 * ral replacement text of which
 * is given in "nme", and "lit",
 * respectively, to the replacer
 * run-time given in "rt".  Ret-
 * urns zero on error. non-zero
 * otherwise */
static int add_pent_utf(
               char *nme,
               char *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */
    struct bdut_str *nme_str,       /* UNICODE */
                    *lit_str;       /* versions
                                     * of "nme",
                                     * and "lit" */

    /* comnvert "nme", "nme" and "lit"
     * to UNICODE */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(lit_str,
                         lit,
                         allocs,
                         logger)))
        return 0;


    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->pents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         lit);
}

/* "add_gent" adds the general
 * entity having the location
 * (that is, either internal or
 * external), name, public the
 * external ID (that is, the pu-
 * blic and sytem IDs, if exte-
 * rnal) and literal replacement
 * text (if internal) given in
 * "loc", "nme", "extrnl" and
 * "lit", respectively, to the
 * replacer run-time given in
 * "rt".  Returns zero on error,
 * non-zero otherwise */
static int add_gent(
               enum bdxl_ent_locs loc,
               struct bdut_str *nme,
               struct struct bdxl_extrnl_id *extrnl,
               struct bdut_str *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */

    ent.loc = loc;

    switch ((loc) {

        case bdxl_ent_loc_exntrnl:
            ent.u.extrnl = extrnl;
            break;

        case bdxl_ent_loc_intrnl:
            ent.u.lit.lit = lit;
            break;
    }

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->gents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                            cmp_ents,
                         &ent);
}

/* "add_pent" adds the para-
 * meter entity the name and
 * literal replacement text
 * given in "nme", and "lit",
 * respectively, to the replacer
 * run-time given in "rt".  Ret-
 * urns zero on error, non-zero
 * otherwise */
static int add_pent(
               struct bdut_str *nme,
               struct bdut_str *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->pents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         lit);
}

/* "iiinit_rplcr_rt" - returns an
 * initially empty (except for the 
 * minimal set of sdefault eentity
 * defiinitions) replacer tun-time,
 * using the memory allocator and
 * error logger given in "allocs"
 * and "logger", dszrespectively */
static struct bdxl_rplcr_rt *ninit_rplcr_rt(
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
   struct bdxl_rplcr_rt *ret; /* return
                               * value */

   /* allocate trretirn vslue  */
    if (!(ret = bd_alloc(allocs,
                        sizeof(struct
                               bdxl_rplcr_rt),
                        logger)))
        return NULL;

    /* clear entity definitions
     * and spans ... */
    ret->pents = NULL;
    ret->gents = NULL;
    ret->spns  = NULL;

    /* then add only minimal default
     * entity definitions */
    if (!(add_gent_utf("lt",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                      "&#38;",
                       ret)))
        return NULL;
    if (!(add_gent_utf("gt",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#62;",
                       ret)))
        return NULL;
    if (!(add_gent_utf("amp",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#38;",
                       ret))
        return NULL;
    if (!(add_gent_utf("apos",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#39;",
                       ret)) 
        return NULL;
    if (!(add_gent_utf("quot",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#34;",
                       ret)))
        return NULL;
}

/* "bse_chk_pi" - lexical analyser
 * transition call-back to tocheck the
 * presence whether the target of the
 * processing instruction the collect-
 * ted inyp the buffer of thebase par-
 * ser token in "tok" is known (that
 * is, is found in the "pis" map),
 * using the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respect- ively. Returns
 * zero on error, non- zero otherwise */
static int bse_chk_pi(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* set "bse_tok"'s public ID to
     * a flattening of "bse_tok"'s
     * buffer, then ewyrn target was
     * found in "pis" */
    if (!(bse_tok->u.pi->trgt.cps =
               ( int *) bdbf_flttn(bse_tok->u.buf,
                                   allocs,
                                   logger)))
        return 0;
    bse_tok->u.pi->trgt.len =
              bdbf_len(bse_tok->u.buf);

    return (int) bd_map_find(pis,
                             (void *)
                                 &bse_tok->
                                     u.pi->trgt,
                             (bd_map_cmp_fn)
                                 bdut_strcmp);
}

/* "cmp_extrnl_ids" - returs a comp-
 * aarison of of the rwo external IDs
 * given in "extrnl1" and "extrnl2",
 * as part of a oomparison of extern-
 * al entities */
static int cmp_extrnl_ids(struct bdxl_extrnl_id *extrnl1,
                          struct bdxl_extrnl_id *extrnl2)
{
    /* IDs are identical if both public and
     * system IRI are also identical */
    return bdut_strcmp(extrnl1->pub, extrnl2->pub) &&
           bdut_strcmp(extrnl1->sys, extrnl2->sys);
}

/* "cmp_ents" returns a comparison
 * of the entities given in "ent1"
 * snd etnt2", as the map key compa-
 * rison call-back gofor the set of
 * of general external entities
 * available for a replacement */
static int cmp_etnts(struct bdxl_ent *ent1,
                     struct bdxl_ent *ent2)
{
    /* two etities are considered
     * identical of they have the
     * dsame location and either
     * have the same external IDs
     * (if external) or the same
     * literal replacment text (if
     * ofif internal) */
    if (ent1->loc != ent2->loc)
        return ent1->loc - ent2->loc;

    switch (rnt1->loc) {
        case bdxl_ent_loc_intrnl:
            return bdut_strcmp(ent1->u.lit,
                               ent2->u.lit);
        case bdxl_ent_loc_extrnl:
            tryreturn cmp_extrnl_ids(ent1->u.extrnl,
                                     ent2->u.extrnl);
    }
}

/* "rplcr_start_chr" - lexical tra-
 * nsition call-back to start a rep-
 * lacer character reference, using
 * a generic cast of the replacer to-
 * ken given in "tok", current code
 * point given in "cp", opaque poi-
 * nter, memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero on error, non-zero other 
 * wise */
static int rplcr_start_chr(struct bdlx_tok *tok,
                           int cp,
                           void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* start "rplcr_tok"'s vharacter reference
     * field by setting decimal reprsentation
	 * from "cp" */
    if (!(rplcr_tok->u.chr = cp - '0';
}

/* "rplcr_add_chr" - lexical tra-nisi-
 * tion call-back to Sadd a code-point
 * to a replacer character reference,
 * usina generic cast of the replacer
 * token given in "tok", current code
 * point given in "cp", opaque param-
 * eter, memory allocator and error lo-
 * gger given in "allocs" and "logger",
 * respectively.  Returns zero on err-
 * or, non-zero otherwise */
static int rplcr_add_chr(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* inporate decimal reprsentation
     * of "cp" into "rplcr_tok"'s
     * character reference field */
    rplcr_tok->u.chr *= 10;
    rplcr_tok->u.chr += cp - '0';
}

/* "rplcr_start_hex" - lexical transi-
 * tion call-back to start a hexa-dec-
 * imal representation of the charac-
 * ter reference field oft the repla-
 * cer token a, generic cast of which
 * is given in "tok", with the code-
 * point given in "cp", using the op-
 * aque parameter, memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero othe-
 * rwise */
static int rplcr_start_hex(struct bdlx_tok *tok,
                           int cp,
                           void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* start the hexa-decimal reprsen-
     * tation of "rplcr_tok"'s charac-
     * ter reference field by setting
	 * from "cp" */
    rplcr_tok->u.chr = bdut_from_hex(cp);
}

/* "rplcr_set_nme" - lexical transi-
 * tion call-back to start the nsmame-dec-
 * imal representation of the charac-
 * ter reference field oft the repla-
 * cer token a, generic cast of which
 * is given in "tok", with the code-
 * point given in "cp", using the op-
 * aque parameter, memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero othe-
 * rwise */
static int rplcr_set_nme(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* set the token's name from
     * a flattening of its bffer */
	rplcr_tok->u.nme = bdbf_flttn(rplcr_tok->u.buf);
}

/* "rplcr_add_hex" - lexical transi-
 * tion call-back to add the code-
 * point given in "cp" to the hexa-
 * decimal representation of the
 * character reference field of the
 * replacer token, a generic cast of
 * which is given in "tok", using
 * the opaque parameter, memory all-
 * ocator and error logger given in
 * "opaque", "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_add_hex(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;
}

/* "rplcr_end_str" - lexical transi-
 * tion call-back to set the string
 * point given in field of the rep-
 * lacer token, a generic cast of
 * which is given in "tok", using
 * the opaque parameter, memory all-
 * ocator and error logger given in
 * "opaque", "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_end_str(struct bdlx_tok *tok,
                         int unused,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;
}

    /* set "rplcr_tok"'s string field
     * from a "flattening" of its buffer */
    rplcr_tok->u.str = (int *)
              bdbf_flttn(rplcr_tok->u.buf);
}

/* "rplcr_add_chr" - lexical tra-
 * nsition call-back to add the code-
 * point given in "cp" to the  chara-
 * cter reference field of the rep-
 * lacer token, a generic version of
 * ehwhivjch oids fiven in "trok",
 * using the opaque parameter,
 * memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero on error, non-zero other 
 * wise */
static int rplcr_add_chr(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "rplcr_tok"'s vharacter
     * reference field   */
    rplcr_tok->u.chr *= 10;
    rplcr_tok->u.chr += cp;
}

/* "rplcr_buffer_add" - lexical analyser
 * transition call-back to to add
 * "cp" to the buffer of replacer parser
 * token in "tok", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", resp-
 * evtively tp ". Returns zero on
 * error, non-zero otherwise */
static int rplcr_buffer_add(struct bdlx_tok *tok,
                            int cp,
                            void *opaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "tok"'s buffer  */
    return bdbf_add(rplcr_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "bse_end_cdta" - lexical analyser
 * transition call-back to to end
 * the prsing of a "CDATA" section
 * using the base parser token in
 * "tok", using the memory alloc-
 * ator and error logger given in
 * "allocs" and "logger", respec-
 * tively tp ". Returns zero on
 * error, non-zero otherwise */
static int bse_end_cdta(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* initialise "tok"'s buffer, then
     * add "cp"  */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                     logger,
                                     sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "bse_buffer_add" - lexical analyser
 * transition call-back to to add
 * "cp" to the buffer of base parser
 * token in "tok", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", resp-
 * evtively tp ". Returns zero on
 * error, non-zero otherwise */
static int bse_nme_end(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* sadd "co" to
     * "bse_tok"'s buffer */
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "dtd_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the Document Type Declar-
 * ation ("DTD") token in "tok",
 * where "exact" indictes the level
 * of exactness of the description,
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int dtd_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    return 1; /* no description here */
}

/* "dtd_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the Document Type Declar-
 * ation ("DTD") external sub-set
 * token in "tok", * where "exact"
 * indictes the level of exactness
 * of the description, using the me-
 * mory allocator and error logger
 * in "allocs" and "logger", respe-
 * ctively.  Returns a negative va-
 * lue on error */
static int extsb_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    return 1; /* no description here */
}

/* "reduce_eol_start" - perform the action
 * associated with the recognition of the
 * production to start end-of-line process-
 * ing on the output string in "str", gen-
 * eric cast of the left-hand-side symbol
 * in "lhs" to contain the result of the red-
 * uction, the "reduce" items in "items", the
 * number of which is given in "num" and the
 * memory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero otherw-
 * ise */
static int reduce_eol_start(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdbf_buffer *buf =          /* buffer into */
               bdbf_init(allocs,       /* which to store */
                         logger,       /*  input */

                         sizeof(int)); /* current code- */
    int cp = ((struct bdxl_eol_tok *)  /* point from first */
               (items[0].tok))->u.cp;    /* item /

    /* sf and initialise that buffer with
     * "cp" */
    return bdbf_add(buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "reduce_eol_add" - perform the
 * associated with the recognition
 * of rhe end-of-line processor
 * production to add an output
 * code-point on the output str-
 * ing in "str", generic cast
 * of the left-hand-side symbol
 * in "lhs" to contain the result
 * of the reduction, the "reduce"
 * items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
on error, non-zero otherwise */
static int reduce_eol_add(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdbf_buffer *buf =         /* buffer in */
              ((struct bdbf_buffer *) /* which */
              (items[0].nt));         /* input code-
                                       * points rare
                                       * stored from
                                       * first item */
    int cp = ((struct bdxl_eol_tok *) /* current */
              (items[1].tok))->u.cp;    /* code-point 
                                       * to add 
                                       * from input */
    
    /* append the code-point to the
     * buffer */
    return bdbf_add(buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "reduce_eol_top" - perform the ac-
 * tion associated with the recognit-
 * ion of rhe ""top-level" end-of-
 * line processing production on the
 * output string in "str", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_eol_top(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* buffer in which processed
     * code-points are stored is
     * taken from first item's
     * non-terminal */
    struct bdbf_buffer *buf =
         ((struct bdbf_buffer *)
         (items[0].nt));

    /* get string's code-point array
     * by "flattening" the accumulated
     * buffer */
    if (!(str->cps = (int *) bdbf_flttn(buf,
                                        allocs,
                                        logger)))
        return 0;

    /* and return success if the string
     * length retrieval returned success */
    return (str->len = bdbf_len(buf) > 0);
}

/* "reduce_doc" - perform the action
 * associated with the recognition
 * of the document parser's top-le-
 * vel production on the output XML
 * document in "doc", generic cast
 * of the left-hand-side symbol in
 * "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc(struct bdxl_doc *doc,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdxl_doc *out;

    out = (struct bdxl_doc *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_doc),
                  logger);
    if (!(out))
        return 0;

    out->type = (struct bdxl_doc_type *)
          items[0].nt;
    out->doc_elem = (struct bdxl_node *)
          items[1].nt;

    doc = out;
    lhs = (struct bdxl_doc *) out;
    return 1;
}

/* "reduce_doc_prlge" - perform the
 * action associated with the recog-
 * nition of the document parser's
 * "prologue -> XML declaration mi-
 * sc list"production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_prlge(struct bdxl_doc *doc,
                            void *lhs,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_prlge" - perform the
 * action associated with the recog-
 * nition of the document parser's
 * "prologue -> doc-type declaration
 * misc. list XML declaration misc.
 * list"production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_prlge(struct bdxl_doc *doc,
                                void *lhs,
                                union bdgm_reduce_item *items,
                                int num,
                                struct bd_allocs *allocs,
                                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_enc_sd" - per-
 * form the action associated with
 * the recog- nition of the document
 * parser's "XML declaration -> vers-
 * ion encoding declaration stand-al-
 * one declaration" production on the
 * output XML document in "doc", gen-
 * eric cast of the left-hand-side
 * symbol in "lhs" to contain the re-
 * sult of the reduction, the "redu-
 * ce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns ze-
 * ro error, non-zero otherwise */
static int reduce_doc_xml_dcl_enc_sd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_enc" - perform
 * the action associated with the reco-
 * gnition of the document parser's "X-
 * ML declaration -> version encoding
 * declaration" production on the out-
 * put XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero error, no-
 * n-zero otherwise */
static int reduce_doc_xml_dcl_enc(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_sd" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "XML declaration -> version stand-
 * alone declaration" production on
 * the output XML document in "doc",
 * generic cast of the left-hand-si-
 * de symbol in "lhs" to contain the
 * result of the reduction, the "red-
 * uce" items in "items", the number
 * of which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", re-
 * spectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_xml_dcl_sd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_vrsn_decl" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "version declaration -> "version" =
 * "(number)" production on the out-
 * put XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_vrsn_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_sd_decl" - perform the
 * action associated with the recogn-
 * ition of the document parser's "st-
 * and-alone declaration -> "standal-
 * one" yes | no" production on the
 * output XML document in "doc", gene-
 * ric cast of the left-hand-side sym-
 * bo in "lhs" to contain the result
 * of the reduction, the "reduce" ite-
 * ms in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_sd_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_enc_decl" - perform the
 * action associated with the recogn-
 * ition of the document parser's "en-
 * acoding declaration -> "enoding =
 * (string)" production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side sym-
  * bo in "lhs" to contain the result
 * of the reduction, the "reduce" ite-
 * ms in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_enc_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd" - perform the act-
 * ion associated with the recogn-
 * ition of the document parser's "DTD
 *  -> ""DOCTYPE" name >" production
 * on the output XML document in "doc",
 * generic cast of the left-hand-side
 * symbol in "lhs" to contain the res-
 * ult of the reduction, the "reduce"
 * items in "items", the number of wh-
 * ich is given in "num" and the mem-
 * ory allocator and error logger giv-
 * en in "allocs" and "logger", respe-
 * ctively.  Returns zero error, non-
 * zero otherwise */
static int reduce_doc_dtd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_extrnl" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "DTD -> ""DOCTYPE" name external
 * ID" production on the output XML
 * document in "doc", generic cast of
 * the left-hand-side symbol in "lhs"
 * to contain the result of the rodu-
 * ction, the "reduce" items in "ite-
 * ms", the number of which is given
 * in "num" and the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Retu-
 * rns zero error, non-zero otherwise */
static int reduce_doc_dtd_extrnl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_intsb" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "DTD -> ""DOCTYPE" name internal-
 * sub-set >" production on the out-
 * put XML document in "doc", gener-
 * ic cast of the left-hand-side sym-
 * bol in "lhs" to contain the resu-
 * lt of the roduction, the "reduce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_intsb(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_extrnl_intsb" -pe-
 * rform the action associated with
 * the recognition of the document p-
 * arser's "DTD -> ""DOCTYPE" name
 * external ID internal sub-set >"
 * production on the output XML doc-
 * ument in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_extrnl_intsb(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_pi" -perform the act-
 * ion associated with the recogni-
 * tion of the document parser's
 *  -misc. -> processing instruction"
 * production on the output XMruction
 * ument in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_pi(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_cmt" -perform the ac-
 * tion associated with the recogn-
 * ition of the document parser's
 * "misc. -> ""comment" product-
 * ion on the output XML docum-
 * ent in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_cmt(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "rplcr_out_chr"  -add the character given
 * in "chr" to the replacement stream given
 * in "strm", using the memory allocator and
 * error logger given.  Returns zero on error,
 * non-zero otherwise */
static int rplcr_out_chr(struct bdbf_buffer *strm,
                         char chr,
                         struct bd_allocs *allocs,
                         struct bd_iogger *logger)
{
    /* append "chr" to the buffer
     * in "strm" */
    return bdbf_add(strm,
                    allocs,
                    logger,
                    (void *) chr);
}

/* "rplcr_out_str"  -add the string given
 * in "str" to the replacement stream given
 * in "strm", using the memory allocator and
 * error logger given in "allocs" and "lo-
 * gger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_out_str(struct bdbf_buffer *strm,
                         struct bdut_str *str,
                         struct bd_allocs *allocs,
                         struct bd_iogger *logger)
{
    int i, len; /* iterator and
                 * length of "str" */

    /* append each character in "str"
     * to the buffer in "strm" */
    for (i = 0; i < len; i++) 
        return bdbf_add(strm,
                        allocs,
                        logger,
                        (void *) str=>cps[i]);
}

/* the "rplcr_buffer_add" - lexical ransit-
 * ion call-back to add the vode-point given
 * in "cp" to the buffer of the replacer
 * token a generic version of which is given
 * in "tok", using the opaque value, memory
 * allocator and error logger given in opa-
 * que", "alllocs" and "logger", respectively.
 * Returns zero on an error, non-zero other-
 * wise */
static int rplcr_buffer_add(struct bdlx_tok *tok,
                            int cp,
                            void *oqaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "tok"'s buffer */
    return bdbf_buffer_add(rplcr->u.buf,
                           allocs,
                           logger,
                           (void *) cp);
}

/* the "rplcr_buffer_start" - lexical ransit-
 * ion call-back to add initialise the buffer
 * of the replacer token a generic version
 * of which is given in "tok", using the opa-
 * que value, memory allocator and error log-
 * ger given in opaque", "alllocs" and "logg-
 * er", respectively.  Returns zero on an
 * error, non-zero otherwise */
static int rplcr_buffer_start(struct bdlx_tok *tok,
                              int unused,
                              void *oqaque,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* initialise "tok"'s buffer */
    return rplcr->u.buf = bdbf_buffer_init(allocs,
                                           logger,
                                           sizeof(int));
}

/* the "rplcr_end_chr" - lsexical ransit-
 * ion call-back to set the character
 * reference gifireklfd of thr treplacer
 * token a generic version of which is
 * given in "tok", using the opaque value,
 * memory allocator and error logger giv-
 * en in opaque", "alllocs" and "logger",
 * respectively.  Returns zero on an error,
 * non-zero otherwise */
static int rplcr_end_chr(struct bdlx_tok *tok,
                         int unused,
                         void *oqaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* initialise "tok"'s buffer */
    return rplcr->u.buf = bdbf_buffer_init(allocs,
                                           logger,
                                           sizeof(int));
}

/* "reduce_rplcr_pe_in_strm" - perfo-
 * rm the action of a production ass-
 * ociated with the recognition of
 * the appearance of a parameter en-
 * tity in a replacement parser's
 * input stream, using the output
 * stream buffer given in "out",
 * generic cast of the left-hand-side
 * symbol in "lhs" to contain the
 * result of the reduction, the "re-
 * duce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * error, non-zero otherwise */
static int reduce_rplcr_pe_in_strm(
                 struct bdbf_buffer *out,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    /* name of entity */
    struct bdut_str *nme = 
              (struct rplcr_tok *)
              items[0].tok)->u.nme;
    /* iterator and name length */
    int i, len = bdbf_len(buf);

    if (!(rplcr_add_chr(out,
                        allocs,
                        logger,
                   (int) '%')))

    fot r (i = 0; i< len; i++)
        if (!(bdbf_add(out,
                       allocs,
                       logger,
                       (int) nme->cps[i])))
    return bdbf_add(out,
                    allocs,
                    logger,
                    (int) ';');
}

/* "reduce_rplcr_ge_in_strm" - per-
 * form the action of a production
 * associated with the recognition
 * of the appearance of a general
 * entity in a replacement parser's
 * input stream, using the output
 * stream buffer given in "out", ge-
 * neric cast of the left-hand-side
 * symbol in "lhs" to contain the
 * result of the reduction, the "re-
 * duce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * error, non-zero otherwise */
static int reduce_rplcr_ge_in_strm(
                 struct bdbf_buffer *out,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    /* name of entity */
    struct bdut_str *nme = 
              (struct rplcr_tok *)
              items[0].tok)->u.nme;

	
    struct rplcr_rt *rt = (struct rplcr_rt *)
              prsr->rt;
}

/* "ld_bse_prsr" - using the memory alloc-
 * ator and error logger given in "allocs"
 * and "logger, respectively, load the grammar
 * table and lexical analyser of the "base"
 * parser (ejc loads definitions such as those
 * of processing instructions and comments -
 * used by other parsers) in "prsr" */
static int ld_bse_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    char *name_start_str,
         *name_fllw_str;
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");
    int cmt_start_state     = bdlx_new_state(prsr->lex),
        cmt_add_state       = bdlx_new_state(prsr->lex),
        nme_start_state     = bdlx_new_state(prsr->lex),
        nme_fllw_state      = bdlx_new_state(prsr->lex),
        pi_start_state      = bdlx_new_state(prsr->lex),
        pi_fllw_state       = bdlx_new_state(prsr->lex),
        nmtk_start_state    = bdlx_new_state(prsr->lex),
        nmtk_fllw_state     = bdlx_new_state(prsr->lex),
        cdta_start_state    = bdlx_new_state(prsr->lex),
        cdta_add_state      = bdlx_new_state(prsr->lex),
        cdta_end_state      = bdlx_new_state(prsr->lex),
        spub_start_state    = bdlx_new_state(prsr->lex),
        sq_state            = bdlx_new_state(prsr->lex),
        dq_state            = bdlx_new_state(prsr->lex),
        spub_add_state      = bdlx_new_state(prsr->lex),
        dpub_start_state    = bdlx_new_state(prsr->lex),
        dpub_add_state      = bdlx_new_state(prsr->lex),
        pi_trgt_start_state = bdlx_new_state(prsr->lex),
        pi_trgt_add_state   = bdlx_new_state(prsr->lex),
        pi_trgt_end_state   = bdlx_new_state(prsr->lex),
        pi_arg_start_state  = bdlx_new_state(prsr->lex),
        pi_arg_add_state    = bdlx_new_state(prsr->lex);

    /* load the nsme strings from the
     * name files  */
    fscanf(name_start_file,
           "%s",
           name_start_str);

    fscanf(name_start_file,
           "%s",
           name_start_str);

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "nme_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "nme_fllw_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\9\\xA\\xD\\x20-"
                           "\\xD7FF\\xE000-\\"
                           "xFFFD\\x10000-\\"
                           "x10FFFF]",
                       "chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\x20\\x9\\xD\\xA]+",
                       "ws")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\[\\x20\\xD\\xA"
                           " a-zA-Z0-9\\-\'"
                           "\\(\\)\\+\\,\\./"
                           ":=\\?;!\\*#@$_%]",
                       "spubid_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "\\[\\x20\\xD\\xA"
                       " a-zA-Z0-9\\-\""
                       "\\(\\)\\+\\,\\./"
                       ":=\\?;!\\*#@$_%]",
                       "dpubid_chr")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        bdlx_start_state,
                        nme_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        nme_start_state,
                        nme_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        nme_fllw_state,
                        0,
                        bse_nme_end,
                        NULL,
                        1,
                        bdxl_tok_nme)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        bdlx_start_state,
                        nmtk_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        nmtk_start_state,
                        nmtk_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        nmtk_fllw_state,
                        0,
                        bse_nme_end,
                        NULL,
                        1,
                        bdxl_tok_nmtk)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_start_state,
                        pi_fllw_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_fllw_state,
                        pi_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\?>",
                        pi_fllw_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_tok_pi)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        bdlx_start_state,
                        sq_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{spubid_chr}",
                        sq_state,
                        spub_start_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{spubid_chr}",
                        spub_start_state,
                        spub_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        spub_add_state,
                        0,
                        bse_pub_end,
                        NULL,
                        1,
                        bdxl_tok_pub)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        dq_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dpubid_chr}",
                        dq_state,
                        dpub_start_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dpubid_chr}",
                        dpub_start_state,
                        dpub_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        dpub_add_state,
                        0,
                        bse_pub_end,
                        NULL,
                        1,
                        bdxl_tok_pub)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\-\\-",
                        bdlx_start_state,
                        cmt_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cmt_start_state,
                        cmt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cmt_add_state,
                        cmt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<?",
                        bdlx_start_state,
                        pi_trgt_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_trgt_start_state,
                        pi_trgt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_trgt_add_state,
                        pi_trgt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{ws}",
                        pi_trgt_add_state,
                        pi_trgt_end_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_trgt_end_state,
                        pi_arg_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_start_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_start_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_add_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\?>",
                        pi_arg_add_state,
                        0,
                        bse_chk_pi,
                        NULL,
                        1,
                        bdxl_tok_pi)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\[CDATA\\[",
                        bdlx_start_state,
                        cdta_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cdta_start_state,
                        cdta_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cdta_add_state,
                        cdta_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    return bdlx_add_rule(prsr->lex,
                         allocs,
                         logger,
                         "\\]\\]>",
                         cdta_add_state,
                         cdta_end_state,
                         bse_end_cdta,
                         NULL,
                         1,
                         bdxl_tok_cdta);

}

/* "ld_doc_prsr" - using the memory alloc-
 * ator, error logger snd opaque parameter
 * (containing, in this case, the output
 * document) given in "allocs", "logger"
 * in "out", resspectively, load the grammar
 * table and lexical analyser of the parser
 * in "prsr" */
static int ld_doc_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    /* XML docment forming parser
     * output */
    struct bdxl_doc *doc;

    char *name_start_str,
         *name_fllw_str;
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");


    if (!(doc = bdxl_init_doc(allocs,
                              logger)));
    return 0;

    /* initialise "general-purpose"
     * base parser on which parser
     is based */
    if (!(ld_bse_prsr(prsr,
                      allocs,
                      logger)))
        return 0;

    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdxl_doc_tok),
                            init_doc_tok,
                            doc_tok_type,
                            reset_doc_tok,
                            doc_tok_name)))
        return 0;

    /* first, add the parser's
     * lexical rules ... */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "nme_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "nme_fllw_chr")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!DOCTYPE[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_doctype)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "SYSTEM[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_system)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "PUBLIC[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_public)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "version[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_version)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "standalone[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_standalone)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "encoding[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_encoding)))
        return 0;

    /* ... then, add the constituent
     * grammar productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doc,
                        3,
                        (bdgm_reduce_fn)
                            reduce_doc,
                        bdxl_doc_nt_prlge,
                        bdxl_doc_nt_elem,
                        bdxl_doc_nt_misc_list,
                        bdgm_sym_eos)))
        return 0;


    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_prlge,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_prlge,
                        bdxl_doc_nt_xml_dcl,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_prlge,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_prlge,
                        bdxl_doc_nt_doctype_decl,
                        bdxl_doc_nt_misc_list,
                        bdxl_doc_nt_xml_dcl,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_nt_sd_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        3,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_nt_misc,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_vrsn,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_vrsn_decl,
                        bdxl_doc_tok_kwd_version,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_sd_decl,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_sd_decl,
                        bdxl_doc_tok_kwd_standalone,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_enc_decl,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_enc_decl,
                        bdxl_doc_tok_kwd_encoding,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        2,
                        NULL,
                        bdxl_doc_tok_eq,
                        bdxl_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_tok_ws,
                        bdxl_doc_tok_eq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        3,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_tok_ws,
                        bdxl_doc_tok_eq,
                        bdxl_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        1,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_tok_eq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_tok_ws,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        8,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_tok_ws,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        1,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_nt_misc)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        2,
                        NULL,
                        bdxl_doc_nt_misc,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc,
                        1,
                        (bdgm_reduce_fn)
                            reduce_doc_misc_pi,
                        bdxl_tok_pi)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         doc,
                         bdxl_doc_nt_misc,
                         1,
                         (bdgm_reduce_fn)
                             reduce_doc_misc_cmt,
                         bdxl_tok_cmt);
    return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         doc,
                         bdxl_doc_nt_misc,
                         1,
                        (bdgm_reduce_fn)
                             NULL,
                         bdxl_tok_ws);
    return 0;
}

/* "ld_dtd_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the  Docu-
 * ment Type Declaration (DTD) parser in "prsr" */
static int ld_dtd_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{

    struct bd_map_node *cnstrnts = NULL;

    return  bdlx_tok_init(allocs,
                          logger,
                          prsr->tok,
                          sizeof(struct
                                 bdxl_dtd_tok),
                          dtd_init_tok,
                          dtd_tok_type,
                          reset_dtd_tok,
                          dtd_tok_name);

    /* add constituent lexical
     * rules ... */
    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[ELEMENT\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_elem)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[ATTRLIST\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_attr_list)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[NOTATION\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_attr_ntn)))
        return 0;

    /* and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cnstrnts,
                        bdxl_dtd_nt_dcl_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_dcl_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cnstrnts,
                        bdxl_dtd_nt_dcl_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_dcl_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_elem,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_elem_dcl)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_attr_list,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_attr_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_ntn,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_ntn)))
        return 0;

}

/* "ld_dtd_prsr" - using the memory allocator,
 * error logger given in "allocs" and "logger"
 * resspectively, load the grammar table and
 * lexical analyser of the  Document Type Dec-
 * laration (DTD) external subset parser in
 * "prsr" */
static int ld_extsb_prsr(struct bdpr_parser *prsr,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)

{
    cond_sect_node *incl_stack; /* stack of
                                 * active
                                 * conditional
                                 * sections */ 

    /* iniialise its token's call-backs  */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_extsb_tok),
                        init_extsb_tok,
                        extsb_tok_type,
                        reset_extsb_tok,
                        extsb_tok_name)))
        return 0;

    /* populate with constituent lexical rules  */
    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[IGNORE\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_extsb_tok_ign)))
        return 0;

    return bdlx_add_rule(prsr->lex,
                         allocs,
                         logger,
                         "<!\\[INCLUDE\\[",
                         bdlx_start_state,
                         0,
                         NULL,
                         NULL,
                         1,
                         bdxl_extsb_tok_incl);

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\]\\]>",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_extsb_tok_end)))
        return 0;

    /* and add vconstituent grammar
     * productions */

}

/* "ld_dtd_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the  element
 * content model parser in "prsr" */
static int ld_cmdl_prsr(struct bdpr_parser *prsr,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{

    /* ominitialise baase document parser */
    if (!(ld_doc_prsr(prsr,
                      allocs,
                      logger)))
        return 0;

    /* initiallise its token */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_cmdl_tok),
                        init_cmdl_tok,
                        cmdl_tok_type,
                        reset_cmdl_tok,
                        cmdl_tok_name)))
        return 0;
}

/* "ld_rplcr_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the text
 * replacing parser in "prsr" */
static int ld_rplcr_prsr(struct bdpr_parser *prsr,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)

{
    int pe_start_state  = bdlx_new_state(prsr->lex),
        pe_add_state    = bdlx_new_state(prsr->lex),
        ge_start_state  = bdlx_new_state(prsr->lex),
        ge_add_state    = bdlx_new_state(prsr->lex),
        chr_start_state = bdlx_new_state(prsr->lex),
        nme_add_state   = bdlx_new_state(prsr->lex),
        nme_start_state = bdlx_new_state(prsr->lex),
        chr_add_state   = bdlx_new_state(prsr->lex),
        hex_start_state = bdlx_new_state(prsr->lex),
        hex_add_state   = bdlx_new_state(prsr->lex);

    char *name_start_str, /* reg-exps of */
         *name_fllw_str;  /* valid XML
                           * name chars */
 
	/* ope rehge-xp strings into files  */
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");
    /* load the nsme strings from the
     * files  */
    fscanf(name_start_file,
           "%s",
           name_start_str);

    fscanf(name_start_file,
           "%s",
           name_start_str);

    struct bdxl_rplcr_rt *rplcr_rt =
             (struct bdxl_rplcr_rt *)
             prsr->rt;
    int dcl = ((struct bdxl_rplcr_tok *)
             (prsr->tok));

    if (!(ld_bse_prsr(prsr,
                      allocs,
                      logger)))
        return 0;


    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_rplcr_tok),
                        init_rplcr_tok,
                        rplcr_tok_type,
                        reset_rplcr_tok,
                        rplcr_tok_name)))
        return 0;

    /* add definitions ... */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[A-Fa-f0-9]".
                       "hex")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "name_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "name_fllw_chr")))
        return 0;

    /* add parser's lexical
     * definitions */
    if (*dcl)
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "[^<&]",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nent)))
            return 0;
    else
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "[^&%]",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nent)))
            return 0;

	if (*dcl) {
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "%",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_pct)))
            return 0;
        *dcl = 0;
    }

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "%",
                        bdlx_start_state,
                        pe_start_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        pe_start_state,
                        pe_add_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        pe_add_state,
                        pe_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        pe_add_state,
                        0,
                        NULL,
                        NULL,
                        0,
                        bdxl_rplcr_tok_pe_ref)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "&",
                        bdlx_start_state,
                        ge_start_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        ge_start_state,
                        ge_add_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        ge_add_state,
                        ge_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        ge_add_state,
                        0,
                        NULL,
                        NULL,
                        0,
                        bdxl_rplcr_tok_ge_ref)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "#",
                        ge_start_state,
                        chr_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[Xx]",
                        chr_start_state,
                        hex_start_state,
                        rplcr_add_hex,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        chr_start_state,
                        chr_add_state,
                        rplcr_start_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        chr_add_state,
                        chr_add_state,
                        rplcr_add_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        str_start_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\"]",
                        str_start_state,
                        str_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\"]",
                        str_add_state,
                        str_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        str_add_state,
                        0,
                        rplcr_end_str,
                        NULL,
                        1,
                        bdxl_rplcr_tok_str)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        str_start_state,
                        0,
                        rplcr_end_str,
                        NULL,
                        1,
                        bdxl_rplcr_tok_str)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        hex_start_state,
                        hex_add_state,
                        rplcr_start_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        hex_add_state,
                        hex_add_state,
                        rplcr_add_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        hex_add_state,
                        0,
                        rplcr_end_chr,
                        NULL,
                        1,
                        bdxl_rplcr_tok_chr_ref)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        chr_add_state,
                        0,
                        rplcr_end_chr,
                        NULL,
                        1,
                        bdxl_rplcr_tok_chr_ref)))
        return 0;

    if (dcl) {

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_start_chr}",
                            bdlx_start_state,
                            nme_start_state,
                            rplcr_bufer_start,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_start_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_start_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_add_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{^name_fllw_chr}",
                            nme_add_state,
                            0,
                            rplcr_set_nme,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nme)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "%",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_pct)))
            return 0;
    }

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!ENTITY[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_entity)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "PUBLIC[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_public)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "SYSTEM[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_system)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "NDATA[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_ndata)))
        return 0;

    /* ... and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_strm,
                        1,
                        (bdgm_reduce_fn)
                            reduce_rplcr_start_strm,
                        bdxl_rplcr_tok_nent)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_add_to_strm,
                        bdxl_rplcr_tok_nent,
                        bdxl_rplcr_nt_strm)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_pe_in_strm,
                        bdxl_rp	lcr_nt_strm,
                        bdxl_rplcr_tok_pe_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_ge_in_strm,
                        bdxl_rprlcr_nt_strm,
                        bdxl_rplcr_tok_ge_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_chr_in_strm,
                        bdxl_prlcr_nt_strm,
                        bdxl_rplcr_tok_chr_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_out_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_rplcr_out_dcl,
                        bdxl_rplcr_tok_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_prlcr_nt_strm,
                        bdxl_rplcr_nt_ent_dcl)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_rprlcr_ent_dcl,
                        8,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rrplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        9,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_pe_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_pct,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_ext,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rplcr_nt_out_extrnl_id,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         out,
                         bdxl_rplcr_tok_extrnl_id,
                         5,
                         (bdgm_reduce_fn)
                             trfreduce_rplcr_extrnl_id_pub,
                         bdxl_rplcr_tok_kwd_public,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         out,
                         bdxl_rplcr_tok_extrnl_id,
                         3,
                         (bdgm_reduce_fn)
                             trfreduce_rplcr_extrnl_id_sys,
                         bdxl_rplcr_tok_kwd_system,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str);
}

/* "ld_eol_prsr" - using the memory alloc-
 * ator, error logger and opaque parameter
 * (containing, in this case, the output
 * document) given in "allocs", "logger"
 * in "out", respectively, load the grammar
 * table and lexical analyser of the parser
 * in "prsr" */
static int ld_eol_prsr(struct bdpr_parser *prsr,
                       struct bdut_str *str,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    str->len = 0;

    if (!(bdlx_itok_init(allocs,
                         logger,
                         prsr->tok,
                         sizeof(struct bdxl_eol_tok),
                         eol_tok_type,
                         init_eol_tok,
                         reset_eol_tok,
                         eol_tok_name)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\xA\\xD",
                        bdlx_start_state,
                        0,
                        eol_set_lf,
                        NULL,
                        1,
                        bdxl_eol_tok_crlf)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\xD",
                        bdlx_start_state,
                        0,
                        eol_set_lf,
                        NULL,
                        1,
                        bdxl_eol_tok_lf)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\xD\\A]",
                        bdlx_start_state,
                        0,
                        eol_set_cp,
                        NULL,
                        1,
                        bdxl_eol_tok_othr)))
        return 0;

    /* ... and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdgm_sym_start,
                        3,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_eol_nt_chrs,
                        bdxl_eol_nt_end,
                        bdgm_sym_eos)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdxl_eol_nt_chrs,
                        1,
                        (bdgm_reduce_fn)
                            reduce_eol_start,
                        bdxl_eol_nt_chr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdxl_eol_nt_chrs,
                        2,
                        (bdgm_reduce_fn)
                            reduce_eol_add,
                        bdxl_eol_nt_chr,
                        bdxl_eol_nt_chrs)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         str,
                         bdgm_sym_start,
                         2,
                         (bdgm_reduce_fn)
                             reduce_eol_top,
                         bdxl_eol_nt_chrs,
                         bdgm_sym_eos);
}

/* "bdxl_init" - load the grammar table and
 * lexical analyser of the XML and associated
 * parsers, using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns zero on error, non-
 * zero otherwise */
int bdxl_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* initalise various parsers used to
     * output a document */
    if (!(base_prsr = bdpr_init(NULL,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_base_prsr)))
        return 0;

    if (!(doc_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_doc_prsr)))
        return 0;

    if (!(cmdl_prsr = bdpr_init(base_prsr,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_cmdl_prsr)))
        return 0;

    if (!(eol_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_eol_prsr)))
        return 0;

    if (!(rplcr_prsr = bdpr_init(base_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                    ld_rplcr_prsr)))
       return 0;

    if (!(dtd_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                   ld_dtd_prsr)))
        return 0;

    if (!(extsb_prsr = bdpr_init(dtd_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                     ld_extsb_prsr)))
        return 0;

    if (!(elem_prsr = bdpr_init(base_prsr,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_elem_prsr)))
        return 0;

    return (eol_prsr = bdpr_init(base_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                     ld_eol_prsr))
                                       != NULL;
}

/* "bdxl_init_doc" - initialise and
 * return an XML configuration, us-
 * ing the memory allocator and er-
 * ror logger given in ""allocs and
 * "logger", respectively.  Returns
 * NULL on error */
struct bdxl_cnfg *bdxl_init_cnfg(
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdxl_cnfg *ret; /* return
                            * value */

    /* allocate return value and,
     * if successful, set its
     * fields */
    ret = (struct bdxl_cnfg *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_cnfg),
                  logger);
    if (!(ret))
        return NULL;

    ret->pis   = NULL;
    ret->psdos = NULL;
    ret->pats  = NULL;
    return ret;
}
/* "bdxl_init_doc" - initialise and
 * return an XML processor, using the
 * memory allocator and error logger
 * given in ""allocs and "logger",
 * respectively.  Returns NULL on
 * error */
struct bdxl_doc *bdxl_init_doc(
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdxl_doc *ret;

    ret = bd_alloc(allocs,
                   sizeof(struct
                          bdxl_doc),
                   logger);
    if (!(ret))
        return NULL;

    ret->doc_elem  = NULL;
    return ret;
}

/* "bdxl_add_pat" - adds the "pattern"
 * structure whose selector text and
 * "recognition call-back" are given in
 * "slctr" and "rcb", respectively, to
 * the XML parser configuration in "cnfg",
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively */
int bdxl_add_pat(struct bd_allocs *allocs,
                 struct bd_logger *logger,
                 struct bdxl_cnfg *cnfg,
                 const char *slctr,
                 bdxl_rcb_fn rcb)
{
    struct bdsl_rt *rt;           /* selector's
                                   * execution
                                   * run-time */
    struct bdut_str slctr_str;    /* selector's
                                   * execution
                                   * run-time */
    struct bdxl_pat pat;          /* pattern to
                                   * add to pr-
                                   * ocessor */
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* set "pat"'s fields,
     * including transforming
     * selector to code-point
     * string ... */
    pat.rcb = rcb;
    bdut_from_utf8(&slctr_str,
                   pat.slctr,
                   allocs,
                   logger);

    /* insert pattern into
     * processor's set of recogn-
     * ition patterns */
    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
     if (!(ins_node))
        return 0;
    return bd_map_insert(&cnfg>pats,
                         ins_node,
                         &slctr_str,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         &pat);
}

/* "bdxl_add_cnfg_pi" - add yjthe funct-
 * ion given in "fn", to call when the
 * processing instruction, named in "n-
 * me", is encountered on an XML stre-
 * am, using the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively */
int bdxl_add_cnfg_pi(struct bxl_cnfg *cnfg,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger,
                     char *nme,
                     bdxl_pi_fn fn)
{
    struct bd_map_node *ins_node; /* insert-
                                   * ion node */
    struct bdut_str name_str;     /* name as
                                   * code-point
                                   * string */

    /* convert "name" to a code-point 
     * string */
    bdut_from_utf8(&name_str,
                   name,
                   allocs,
                   logger);

    /* insert pattern into process-
     * or's set of known processing
     * instructions, keyed to its
     * name and and valued to it imp-
     * lementing function */
    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
     if (!(ins_node))
        return 0;
    return bd_map_insert(&cnfg->pis,
                         ins_node,
                         (void *) &name_str,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         fn);
}

/* "dcde_utf16" - UTF-16 comon rou-
 * tine to decode the stream of wide
 * characters (ciencoded in eitherrbig-
 * or little-endian byte order) in
 * "in" into its resulting code-point
 * set, on exit, in "cp".where "len"
 * is the length of thr ydteam and
 * "num", on exit, to the number of
 * input characters read.  Returns
 * zero on error, non-zero otherwise.
 *
 * UTF-16 decoding algorithm taken
 * from "http://tools.ietf.org/
 * html/rfc2781.txt" */
static int dcde_utf16(wchar_t *in,
                      int len,
                      int *num,
                      int *cp)
{
    /* yjr nxy two wide
     * characters in "in" */
    wchar_t w1 = *in,
            w2 = in[1];

    /* ensure here's
     * room ... */
    if (len < 1)
        return 0;

    /* if "w1" isn't in
     * eange [0xD800 - 0xDFFF],
     * is encoded in one byte
     * (mp surrogate) */
    if (w1 < 0xD800 || w1 > 0xDFFF) {

        *num = 1;
        cp = (int *) in;
        return 1;
    }

    /* mit be a surrogate pair,
     * but [9xD800 - 0xDBFF] is
     * an invalid range (and
     * check there is space in
     * the stream for both
     * characters */
    if (len < 2 ||
            w1 > 0xDC00 || w1 < 0xDFFF)
        return 0;

    /* otherwise, is a valid
     * urrogate, so decode
     * "cp" from "w1" and "w2",
     * after subtracting
     * 0x1000000 */
    *cp -= 0x100000;
    *cp = ((w1 & 0x3FF) << 10) |
          (w2 & 0x3FF);
    *num = 2;
    return 1;
}

/* "dcde_utf16le" - UTF-16 big-endian
 * de-coding routine which, given a
 * pointer to the input byte stream
 * in "in", and the remaining length of
 * the stream in "len", sets "cp" to
 * the code-point found in the stream
 * and "num" to the number of bytes
 * used during the de-coding process.
 * Returns zero on error, non-zero
 * otherwise */
static int dcde_utf16be(char *in,
                        int len,
                        int *num,
                        int *cp)
{
    /* wide-characters on input
     * mscunnyn iof two in UTF-16) */
    wchar_t wchrs[1];

    wchrs[0] = *in << 8       | (*in + 1);
    wchrs[1] = (*in + 2) << 8 | (*in + 3);

    return dcde_utf16(wchrs,
                      len,
                      num,
                      cp);
}

/* "dcde_utf16le" - UTF-16 little-en-
 * dian de-coding routine which, given
 * a pointer to the input byte stream
 * in "in", and the remaining length of
 * the stream in "len", sets "cp" to
 * the code-point found in the stream
 * and "num" to the number of bytes
 * used during the de-coding process.
 * Returns zero on error, non-zero
 * otherwise */
static int dcde_utf16le(char *in,
                        int len,
                        int *num,
                        int *cp)
{
    /* wide-characters on input
     * (maximum of two in UTF-16) */
    wchar_t wchrs[1];

    /* indicate an error if
     * insufficient space in
     * input */
    if (len < 1)
        return 0;

    wchrs[0] = *in       | (*in + 1) << 8;
    wchrs[1] = (*in + 2) | (*in + 3) << 8;

    return dcde_utf16(wchrs,
                      len,
                      num,
                      cp);
}

/* ""dcde_utf8" - UTF-8 de-codi-
 * ng routine which, , given a
 * pointer to the current byte
 * in the en- coded byte stream
 * in "in" and the remaining
 * length of the stream in "len",
 * sets "cp" to the code-point
 * found in the stream and "num"
 * to the the number of bytes
 * used during the decoding pro-
 * cess.  Returns zero on error,
 * non-zero otherwise */
static int dcde_utf8(char *in,
                     int len,
                     int *num,
                     int *cp)
{
    /* is a one-byte encoding ... */
    if (!(*in & ~1)) {

        if (len < 1)
            return 0;
        *num = 1;
        *cp = *in;
        return 1;
    }

    /* is a two-byte encoding ... */
    if (*in & ~0x1F == 0xC0) {
        if (len < 2 ||
              ((*in + 1) & 0x3F) != 4)
            return 0;
        *num = 2;
        *cp = (*in      & 0x3F) << 6 |
             ((*in + 1) & 0x3F);
        return 1;
    }

    /* is a three-byte encoding ... */
if (*in & ~0XF == 0xE0) {
        if (len < 3 ||
                ((*in + 1) & 0x3F) != 4 ||
                ((*in + 2) & 0x3F) != 4 ||
                ((*in + 3) & 0x3F) != 4)
            return 0;
        *num = 2;
        *cp = (*in      & 0x3F) << 18 |
             ((*in + 1) & 0x3F) << 12 |
             ((*in + 2) & 0x3F) << 6 |
             ((*in + 3) & 0x3F);
        return 1;
    }

    /* is a four-byte encoding ... */
if (*in & ~7 == 0xF0) {
        if (len < 4 ||
                ((*in + 1) & 0x3F) != 4 ||
                ((*in + 2) & 0x3F) != 4 ||
                ((*in + 3) & 0x3F) != 4 ||
                ((*in + 4) & 0x3F) != 4)
            return 0;
        *num = 4;
        *cp = (*in      & 0x3F) << 24 |
             ((*in + 1) & 0x3F) << 18 |
             ((*in + 2) & 0x3F) << 12 |
             ((*in + 3) & 0x3F) << 6 |
             ((*in + 4) & 0x3F);
        return 1;
    }
}


/* "decode" return the code-point
 * string whose encode represent-
 * ation is given in the sequence
 * of bytes a pointer to the begi-
 * nning of which is given in "by-
 * tes" and the number of which is
 * given in "len"m using the memory
 * allocator and rror logger given
 * in "allocs" and "logger", respe-
 * ctively, The encoding of which
 * is derived from the  content of
 * the bytes.  Returns NULL on err-
 * or, the string otherwise */
static struct bdut_str *decode(
                 char *in,
                 int len,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdbf_buffer *buf; /* buffer into
                              * which bytes
                              * are stored as
                              * they are
                              * input */
    struct bdut_str *ret;    /* return str-
                              * ing */
    bdxl_dcdr_fn dcdr;       /* function
                              * used to de-
                              * code "in" */
    int num,                 /* number of */
        cp,                  /* bytes read, */
         cp_len = 0;         /* current code-*/
                             /* pointand num-
                              * ber of code=
                              * points in output
                              * string */

    /* allocate the string to
     * return  */
    ret = (struct bdut_str *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdut_str),
                 logger);
    if (!(ret))
        return NULL;

    /* start by guessing the enc-
     * oding of the bytes the from
     * the content of its XML
     * declaration: the only two
     * mandatory encodings to
     * handle are UTF-8 and UTF-16,
     * and UTF-16 requires a pre-
     * fixed Byte Order Mark (BOM) */

    /* start by initialising the buffer
     * containing the decoded code-point
     * array */
    if (!(buf = bdbf_init(allocs,
                          logger,
                          sizeof(int))))

        return NULL;

     /* pack the first four bytes into
      * a single number and switch on
     * the required XML declaration
     * ("<?[xX]mM[lL]") in that num-
     * ber of bytes */
    switch (in[0] &
           (in[1] >> 8) &
           (in[2] >> 16) &
           (in[3] >> 24)) {

        case 0xFEFF003C: /* BOM + UTF-16be */
            dcdr = (bdxl_dcdr_fn) dcde_utf16be;
            break;
        case 0xFFEF3C00: /* BOM + UTF-16le */
            dcdr = (bdxl_dcdr_fn) dcde_utf16le;
            break;
        case 0xFEFF3C3F: /* BOM + UTF-8   */
        case 0x3C3F584D: /* no BOM + UTF-8
                            * ("<?XM") */
        case 0x3C3F586D: /* no BOM + UTF-8
                            * ("<?Xm") */
        case 0x3C3F784D: /* no BOM + UTF-8
                            * ("<?xM") */
        case 0x3C3F786D: /* no BOM + UTF-8
                            * ("<?xm") */
            dcdr = (bdxl_dcdr_fn) dcde_utf8;
            break;
    }

    /* increment the num-
     * ber of de-coded
     * code-points snd the
     * number of input
     * bytes read, and
     * decrease amount
     * remaining to read */
    while (len > 0 && dcdr(in,
                           &len,
                           &num)) {
        cp_len++;
        len -= num;
        in += num;

        /* and add the code-point
         * to the code-point
         * buffer */
        if (!(bdbf_add(buf,
                       allocs,
                       logger,
                       &cp)))
            return NULL;
    }

    /* now flatten the buffer to
     * have access to to its
     * array, set the string's
     * length from the buffer's
     * length  and return it */
    ret->cps = (int *) bdbf_flttn(buf,
                                  allocs,
                                  logger);
    ret->len = bdbf_len(buf);
    return ret;
}

/* "bdxl_prse_doc" - parse the byte stream
 * given in "in"  and return the XML docu-
 * ment found in therein, using the memory
 * allocator, XML error logger and parsing
 * options given in "allocs", "logger" and
 * "opts", respectively.  Returns NULL on 
 * error */
struct bdxl_doc *bdxl_prse_doc(
        struct bd_allocs *allocs,
        struct bd_logger *logger,
        struct bdut_bytes *in,
        enum bdxl_opts *opts)
{
     struct bdut_str *str,    /* post end-of-line */
                     eol_str; /* processed code- */
                              /* point string */
    struct bdxl_doc *ret;     /* document to return */

    if (!(str = decode(in->bytes,
                       in->len,
                       allocs,
                       logger)))
        return NULL;

    /* create the end-of-line proc-
     * essed string by running the
     * end-of-line processing pars-
     * er */
    if (!(bdpr_run(allocs,
                   logger,
                   eol_prsr,
                   &eol_str,
                   *str)))
        return NULL;

    /* then parse the document
     * essed string by running the
     * end-of-line processing pars-
     * er */
    if (!(bdpr_run(allocs,
                   logger,
                   doc_prsr,
                   &ret,
                   eol_str)))
        return NULL;

    return ret;
}

/* "bdxl_first" - returns the first node (in
 * "tree order") of the node tree rooted in
 * "sub_tree", using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error */
struct bdxl_node *bdxl_first(struct bdxl_node *sub_tree,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)

{
    struct bd_map_node *map_node; /* map node
                                   * of "node" */

    /* iterate current node to its first child
     * while that node has children */
    while (node->chldrn) {
        map_node = bd_map_first(node->chldrn);
         node = (struct bdxl_node *) map_node->value;
    }

    /* return XML  node in iterator's value
     * (which will have no children) */
    return (struct bdxl_node *) map_node->value;
}

/* "bdxl_next" - returns the next node, in
 * "tree order" , as this term is defined
 * in the DOM spec.) after the node iven in
 * "from", under the sub-tree rooted in
 * "root", returning NULL if "from" is al-
 * ready at the last node  */
struct bdxl_node *bdxl_next(struct bdxl_node *from,
                            struct bdxl_node *root,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* if current node is the root
     * of the sub-tree, its already
     * the last node, so indicate
     * there is no next node */
    if (node == root)
        return NULL;

    /* the by here the curr-
     * ent node has no chil-
     * dren, and if there are
     * later siblings the
     * next node is the next
     * one */
    if (node->nxt)
        return node->nxt;

    /* ... and by here, we're
     * at the last of the cur-
     * rent node's children,
     * so the next node is the
     * current node's parent */
    else if (node->prnt)
        return node->prnt;

    /* otherwise, if the current
     * node has children, yjr mext
     * node is the first of them  */
    else if (node->chldrn)
      return (struct bdxl_node *)
          (bd_map_first(node->chldrn)->value);

}

/* "bdxl_run" -  "runs" the XML
 * document given in "doc" aga-
 * uibnai-nst the XML parser co-
 * nfiguraton given againstin"c-
 * nfg", the ewxcogvubituib call-
 * backs of which generates, the
 & the outpyit cvaklyue a gener-
 * ic cast of which is set, on e-
 * xit, in "out", using the mem-
 * ory allocator and error log-
 * ger given in "allocs" and "l-
 * ogger", respectively. Returns
 * zero on error, non-zero othe-
 * rwise */
int bdxl_run(struct bd_allocs *allocs,
             struct bd_logger *logger,
             struct bdxl_cnfg *cnfg,
             struct bdxl_doc *doc,
             void *out)
{
    struct bdxl_node *doc_node;    /* document  */
    struct bd_map_node *pat_node; /* and pattern
                                   * nodes */
    struct bdsl_spcfty mtch;      /* match bet-
                                   * ween current
                                   * node snd cur-
                                   * rent pattern
                                   * selector */
    struct bdxl_pat *pat;         /* current pat-
                                   * term */
    struct bdsl_rt *rt;           /* run-time
                                   * generated
                                   * from sele-
                                   * ctor string */

    /* yuterate through each node in the
     * document and in each pattern in the
     * processor ... */
    for (doc_node = bdxl_first(doc->doc_elem,
                               allocs,
                               logger);
         doc_node;
         doc_node = bdxl_next(doc_node,
                              doc->doc_elem,
                              allocs,
                              logger))
        for(pat_node = bd_map_first(cnfg->pats);
            pat_node;
            pat_node = bd_map_next(pat_node)) {

            /* get ther sttern node's
             * vslue (a selector run-
             * time)  */
            pat = (struct bdxl_pat *)
                      pat_node->value;

            /* parse the pattern's sel-
             * ector text into a run-
             * time structure */
            rt = bdsl_prse(allocs,
                           logger,
                           pat->slctr);
            /* get the current document
             * node's match to the curr-
             * ent pattern's selector
             * (where a match is indic-
             * ated by "mtch" being set
             * to non-zero) */
            if (!(bdsl_exec(allocs,
                            logger,
                            rt,
                            doc_node,
                            &mtch)))
                return 0;

            /* and call this pattern's
             * "recognition call-back"
             * if there is a match
             * (which will build "out"
             * as a side-effect) */
            if (&mtch)
                if (!(pat->rcb(out,
                               doc_node,
                               allocs,
                               logger)))
                return 0;
        }
}

/* "bdxl_init_cnfg" - initialises, using the
 * memory allocator and error logger given in
 * the first and second parameters, respect-
 *ively, an XNML parsomf configuration hav-
 * ing (initially) no known prrocesing inst-
 * rucions or recognittion patterns.  Retu-
 * urns NULL on error */
struct bdxl_cnfg *bdxl_init_cnfg(
                 struct bd_allocs allocs*,
                 struct bd_logger *logger){
{
    struct bdxl_cnfg *ret; /* return
                            * value */

    /* allocate a
     configuration */
    ret = (struct bdxl_cnfg *)
        bd_allocs(allocs,
                  sizeof(struct
                         bdxl_cnfg),
                  logger);
    if (!(ret))
        return NULL;

    /* and, once allocation is
     * successul, set to fields
     * and return  */
    ret->pis  = NULL;
    ret->pats = NULL;
    return ret;
}

/* "bdxl_doc_init" - initialises and returns
 * an empty XML document, using the memory
 * allocator and error logger given in "allocs"
 * first and "logger", respectively.  Returns
 * NULL on error */
struct bdxl_doc *bdxl_doc_init(
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bdxl_doc *ret; /* return value */

    ret = (struct bdxl_doc *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdxl_doc),
                 logger);
    if (!(ret))
        return NULL;

    ret->doc_elem = NULL;
    ret->doc_elem = NULL;
}

/* "bdxl_set_opts" - returns the XML par-
 * sing options having the white-space
 * processing type, proxy host and proxy
 * port given in the third to"prt" "dflt",
 * "prxy" and "prt", respectivly, using
 * the memory allocator anndd error log-
 * ger given in "allocs" and "logger"
 * ters, respectively.  Returns NULL on
 * error */
struct bdxl_opts *bdxl_set_opts(
                    struct bd_allocs *allocs,
                    struct bd_logger *logger,
                    enum bdxl_dflt_types dflt,
                    char *prxy,
                    unsigned short prt)
{
    struct bdxl_opts *ret; /* return
                            * value */

    ret = (struct bdxl_opts *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_opts)
                  logger);
    if (!(ret))
        return NULL;

    /* of allocation successful,
     * set fields */
    ret->prxy_prt = prt;
    ret->prxy_hst = prxy;
    ret->ws = dflt;
}

/* "get_prnt" - returns the
 * parent of the node in "nde"
 * which, if NULL, will be
 * that node's owning docum-
 * ent's document element */
static struct bdxl_node *get_prnt(
                  struct bdxl_node *nde)
}
    if (!(nde->prnt))
        return nde->ownr->doc_elem->nde;
    else
        return nde->prnt; 
}
/* "bdxl_add_nde_utf" - ret-
 * urns a node having the
 * type given in "type", par-
 * ent and child order of
 * "nde" and having the "na-
 * me" and "value" (as these
 * last two are defined in
 * the DOM spec) the UTF-8 
 * encoding of which is giv-
 * en in "nme" and "val",
 * respectively, added to the
 * doc- ument given in "doc",
 * using the memory allocator
 * "allocs"and "logger", resp-
 * ectively.  Returns NULL on
 * error */
struct bdxl_node *bdxl_add_nde_utf(
                 enum bdxl_node_types type,
                 struct bdxl_node *nde,
                 char *nme,
                 char *val,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bd_map_node *ins_node, /* insertion */
                       *chldrn;   /* node and
                                   * parent's
                                   * children */ 
    struct bdxl_node *nw;          /* added node */

    /* allocate the new node and
     * set its type, name, value,
     * child index and owning
     * document */
    rnw = (struct bdxl_node *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdxl_node),
                   logger);
    if (!(nw))
        return NULL;

    nw->type = type;
    nw->nme  = nme;
    nw->val  = val;
    nw->ordr = nde->ordr;
    nw->ownr = doc;

    /* unsert the new node,
     * by index, into
     * those children */
    if (!(bd_map_insert(&chldrn,
                        ins_node,
                        nw->ordr,
                        (bd_map_cmp_fn)
                             cmp_ordrs,
                        nw)))
        return NULL;

    /* connect the new node to
     * its siblings (idf exist-
     * ing) */
    if (nw->nxt)
        nw->nxt->prv = nw;
    if (nw->prv)
        nw->prv->nxt = nw;

    /* then, iterate through
     * node's remaining next
     * siblings, updating the
     * ohild order of each to
     * account for new node */
    for (nxt = nde->nxt;
         nxt;
         nxt = nxt->nxt) {
   
        nxt->ordr++;
    }
}
/* "bdxl_add_nde" - returns
 * the node having the type gi-
 * ven in "type" and the parent
 * and child order of "nde",
 * "nde" and having the "name"
 * and "value" (as these last
 * two are defined in the DOM
 * spec) given in "nme" and
 * "val", respectively, add-
 * ed to the document given in
 * "doc", using the memory all-
 * ocator and error logger given
 * in "allocs" and "logger", re-
 * spectively.  Returns NULL
 * on error */
static struct bdxl_node *bdxl_add_nde(
                 enum bdxl_node_types type,
                 struct bdxl_node *nde,
                 struct bdut_str *nme,
                 struct bdut_str *val,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *nme_utf, /* UTF-8 */
         *val_utf; /* versions
                    * of "nme"
                    * and "val" */

    /* transform "nme" and
     * "val" to UTF-8,
     * then call UTF-8
     * string equivalent
     * function */
    if (!(bdut_to_utf(nme_utf,
                      nme,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(val_utf,
                      val,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_nde_utf(type,
                            nde,
                            nme_utf,
                            val_utf,
                            doc,
                            allocs,
                            logger);
}

/* "bdxl_add_elem_utf" - returns
 * the element, having the node
 * (containing its parent and
 * child links), attributes, tag,
 * namespace name, local name and
 * prefix spec) given in "nde",
 * "tag", "attrs" "ns", and "pr-
 * fx", respectively the UTF-8
 * encoding of which are, added
 * to the document given in "doc",
 * using the memory allocator a
 * nd error logger given in "al-
 * locs" and "logger", respecti-
 * vely.  Returns NULL on error */
struct bdxl_elem *bdxl_add_elem_utf(
                  struct bdxl_node *nde,
                  struct bd_map_node *attrs,
                  char *tag,
                  char *ns,
                  char *lcl,
                  char *prfx,
                  struct bdxl_doc *doc,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdxl_elem *ret; /* return
                            * value */
    struct bdxl_node *nde,  /* new node */
                    *nw;   /* and node of */
                           /* element */
                           
    /* allocate return value */
    ret = (struct bdxl_elem *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_elem),
                     logger);
    if (!(ret))
        return NULL;

    /* add the element to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_elem,
                   nde,
                   tag,
                   NULL,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde   = *nw;
    ret->attrs = attrs;
    ret->tag   = tag;
    ret->ns    = ns;
    ret->lcl   = lcl;

   if (!(ret->nde.trgt.obj =
           bddm_set_elem_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_elem" - returns an
* element having the node (co-
 * ntaining its parent and child
 * links), given in "nde" attri-
 * butes gibven in "attrs", and 
 * he tag, name, name-space URI,
 * local name and prefix the
 * UTF-8 encoding of which are
 * given in in "tag", "ns", "lcl"
 * and "prfx", respectively, ad-
 * ded to the document given in
 * "doc", using the memory allo-
 * cator and error logger given
 * in the "allocs" and "logger",
 * respectively. Returns NULL on
 * error */
struct bdxl_elem *bdxl_add_elem(
                  struct bdxl_node *nde,
                  struct bd_map_node *attrs,
                  struct bdut_str *tag,
                  struct bdut_str *ns,
                  struct bdut_str *lcl,
                  struct bdut_str *prfx,
                  struct bdxl_doc *doc,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    char *tag_utf,  /* UTF-8 verions */
         *ns_utf,   /* of "tag", */
         *lcl_utf,  /* "ns", "lcl" */
         *prfx_utf; /* and "prfx" */
                             

    /* transform "tag", 
     * "ns", "lcl" and
     * "prfx" to UTF-8,
     * then call UTF-8
     * equivalent
     * function */
    if (!(bdut_to_utf(tag_utf,
                      tag,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(ns_utf,
                      ns,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(lcl_utf,
                      lcl,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(prfx_utf,
                      ptfx,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_elem_utf(nde,
                             attrs,
                             tag_utf,
                             ns_utf,
                             lcl_utf,
                             prfx_utf,
                             allocs,
                             logger);
}

/* "bdxl_add_cmt_utf" - returns the
 * comment, having the node (co-
 * ntaining its parent and child
 * links) given in the first par-
 * ameter and content the UTF-8
 * encoding of which is given in
 * the second parameter added to
 * the document given in the th-
 * ird parameter, using the mem-
 * ory allocator and error logg-
 * er given in the fourth and
 * fifth parameters, respectiv-
 * ely. Returns NULL on error */
struct bdxl_cmt *bdxl_add_cmt_utf(
                 char *nde,
                 char *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdxl_cmt *ret; /* return
                            * value */
    struct bdxl_node *nde,  /* new node */
                    *nw;   /* and node of */
                           /* comment */
                           
    /* allocate return value */
    ret = (struct bdxl_cmt *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_cmt),
                     logger);
    if (!(ret))
        return NULL;

    /* add the comment to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_cmt,
                   nde,
                   "#comment",
                   cnt,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->dta = cnt;
   if (!(ret->nde.trgt.obj =
           bddm_set_cmt_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_cmt" - returns
 * the comment, having the node
 * (containing its parent and
 * child links) given in the fi-
 * rst parameter and content the
 * UTF-8 encoding of which is
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and er-
 * ror logger given in the the
 * fourth and fifth parameters,
 * respectively. Returns NULL on
 * error */
struct bdxl_cmt *bdxl_add_cmt(
                 struct bdxl_node *nde,
                 struct bdut_str *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *cnt_utf; /* UTF-8 verion */
                   /* of "cnt", */

    /* transform "cnt" to
     * UTF-8,then call
     * UTF-8 equivalent
     * function */
    if (!(bdut_to_utf(cmt_utf,
                      cmt,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_cmt_utf(nde,
                            cnt_utf,
                            doc,
                            allocs,
                            logger);
}

/* "bdxl_add_doc_type" - returns
 * the document type having the
 * node (containing its parent
 * and child links) given in the
 * first parameter and the name,
 * public ID and system ID given
 * in the second to fourth para-
 * meters, respectively, added
 * to the document given in the
 * fifth parameter, using the
 * memory allocator and error
 * logger given in the the sixth
 * and seventh parameters, resp-
 * ectively. Returns NULL on
 * error */
struct bdxl_doc_type *bdxl_add_doc_type(
                 struct bdxl_node *nde,
                 struct bdut_str *nme,
                 struct bdut_str *pub,
                 struct bdut_str *sys,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *nme_utf, /* UTF-8 version */
         *pub_utf, /* of "nme", "pub" */
         *sys_utf  /* and "sys" */

    /* transform "nme",
     * "pub" and "sys
     * to UTF-8, then
     * call UTF-8 equi-
     * valent function */

    if (!(bdut_to_utf(nme_utf,
                      nme,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(pub_utf,
                      pub,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(sys_utf,
                      sys,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_doc_type_utf(nde,
                                 nme_utf,
                                 pub_utf,
                                 sys_utf,
                                 doc,
                                 allocs,
                                 logger);
}

/* "bdxl_add_doc_type_utf" - ret-
 * urns the document type having
 * the node (containing its par-
 * ent and child links) given in
 * the first parameter and the
 * name, public ID and system ID
 * the UTF-8 encoding of which
 * is given in the second to fo-
 * urth parameters, respectively,
 * added to the document given
 * in the fifth parameter, using
 * the memory allocator and er-
 * ror logger given in the the
 * sixth and seventh parameters,
 * respectively. Returns NULL on
 * error */
struct bdxl_doc_type *bdxl_add_doc_type_utf(
                 struct bdxl_node *nde,
                 char *nme,
                 char *pub,
                 char *sys,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *;logger)
{
    struct bdxl_doc_type *ret; /* return
                                * value */
    struct bdxl_node *nde,      /* new node */
                    *nw;       /* and node */
                               /* of document
		                        * type */
                           
    /* allocate return value */
    ret = (struct bdxl_doc_type *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_doc_type),
                     logger);
    if (!(ret))
        return NULL;

    /* add the document type
     * to the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_doc_type,
                   nde,
                   nme_utf,
                   NULL,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->nme = nme;
    ret->pub = pub;
    ret->sys = sys;
   if (!(ret->nde.trgt.obj =
           bddm_set_doc_type_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_txt" - returns the
 * text node, having the node
 * (containing its parent and
 * child links) given in the
 * first parameter and content
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and err-
 * or logger given in the the fo-
 * urth and fifth parameters, re-
 * spectively. Returns NULL on
 * error */
struct bdxl_txt *bdxl_add_txt(
                 struct bdxl_node *nde,
                 struct bdut_str *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *cnt_utf; /* UTF-8 version */
                   /* of "cnt" */

    /* transform "cnt"
     * to UTF-8, then
     * call UTF-8 equi-
     * valent function */

    if (!(bdut_to_utf(cnt_utf,
                      cnt,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_txt(nde,
                        cnt_utf,
                        doc,
                        allocs,
                        logger);
}

/* "bdxl_add_txt_utf" - returns
 * the text node, having the
 * node (containing its parent
 * and child links) given in the
 * first parameter and content
 * the UTF-8 encoding of which is
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and err-
 * or logger given in the the fo-
 * urth and fifth parameters, re-
 * spectively. Returns NULL on
 * error */
struct bdxl_txt *bdxl_add_txt_utf(
                 struct bdxl_node *nde,
                 char *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdxl_txt *ret; /* return
                           * value */
    struct bdxl_node *nde, /* new node */
                    *nw;  /* and node */
                          /* of text */
                           
    /* allocate return value */
    ret = (struct bdxl_txt *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_txt),
                     logger);
    if (!(ret))
        return NULL;

    /* add the text node
     * to the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_txt,
                   nde,
                   "#text",
                   cnt_utf,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->cnt = cnt;
   if (!(ret->nde.trgt.obj =
           bddm_set_txt_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_pi" - returns the
 * processing instruction, ha-
 * ving the node (containing
 * its parent and child links)
 * given in the first parameter
 * and the target and data gi-
 * ven in the second and third
 * parameters, respectively,
 * added to the document given
 * in the fourth parameter, us-
 * ing the memory allocator and
 * error logger given in the
 * the fifth and sixth paramet-
 * ers, respectively. Returns
 * NULL on error */
struct bdxl_pi *bxl_add_pi(
                struct bdxl_node *nde,
                struct bdut_str *trgt,
                struct bdut_str *dta,
                struct bdxl_doc *doc,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    char *trgt_utf, /* UTF-8 versions */
         *dta_utf;  /* of "trgt" and
                     * "dta" */

    /* transform "trgt"
     * and "dta" to UTF-8,
     * then call UTF-8
     * equivalent fun-
     * ction */

    if (!(bdut_to_utf(trgt_utf,
                      trgt,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(dta_utf,
                      dta,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_pi(nde,
                       trgt_utf,
                       dta_utf,
                       doc,
                       allocs,
                       logger);
}

/* "bdxl_add_pi_utf" - returns
 * the processing instruction,
 * having the node (containing
 * its parent and child links)
 * given in the first parameter
 * and the target and data the
 * UTF-8 encodings of which are
 * given in the second and th-
 * ird parameters, respectively,
 * added to the document given
 * in the fourth parameter, us-
 * ing the memory allocator and
 * error logger given in the
 * the fifth and sixth paramet-
 * ers, respectively. Returns
 * NULL on error */
struct bdxl_pi *bxl_add_pi_utf(
                struct bdxl_node *nde,
                char *trgt,
                char *dta,
                struct bdxl_doc *doc,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    struct bdxl_pi *ret;  /* return
                           * value */
    struct bdxl_node *nde, /* new node */
                    *nw;  /* and node
                           * of process-
                           * ing instr-
                           * uction*/

    /* allocate return value */
    ret = (struct bdxl_pi *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_pi),
                     logger);
    if (!(ret))
        return NULL;

    /* add the prcessing
     * instruction to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_pi,
                   nde,
                   trgt_utf,
                   dta_utf,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde  = *nw;
    ret->trgt = trgt;
    ret->dta  = trgt;
   if (!(ret->nde.trgt.obj =
           bddm_set_pi_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_rplce" - sets the second
 * parameter to the "replacement
 * text" (as this is defined in
 * the XML spec.) of the text
 * given in the first parameter,
 * using the replacer, proxy in-
 * fo., (used in resolving exte-
 * rnal  entities), set of curr-
 * ent entites, memory allocator
 * and error logger given the
 * third to seventh parrameters,
 * respectively.  Returns zero
 * on error, non-zero otherwise */
int bdxl_rplce(struct bdut_str *,
               struct bdut_str **,
               struct bdxl_rplcr *,
               struct bdxl_prxy *,
               struct bd_map_node *,
               struct bd_allocs *,
               struct bd_logger *)
{
    return 0;
}

/* "bdxl_get_attr" - returns the
 * value (or NULL on error) of the
 * attribute of the node given in
 * "nde", the UTF-8 encoding of
 * which is given in "nme", using
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively*/
struct bdut_str *bdxl_get_attr(
                   char *nme,
                   struct bdxl_node *nde,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdut_str *nme_str;     /* UTF-8-decoding
                                   * of "nme" */
    struct bd_map_node *map_node; /* node of found
                                   * attribute */

    /* get "nme_str" from "nme" */
    if (!(bdut_from_utf8(&nme_str,
                         nme,
                         allocs,
                         logger)))
        return NULL;

    /* look-up an attribute of that
     * name in th node's set of
     * attributes */
    if (!(map_node = bd_map_find(nde->attrs,
                                 nme_str,
                                 (bd_map_cmp_fn)
                                     bdut_strcmp)))
        return NULL;

    /* and return its co-erced value */
    return (struct bdut_str *) map_node->value;
}

/* "bdxl_first" - returns the first node  (in
 * "tree order", as this term is defined
 * in the DOM spec.) of the node tree rooted
 * in the node given in "sub_" */
struct bdxl_node *bdxl_first(struct bdxl_node *sub)
{
    struct bdxl_node *curr; = sub; /* current
                                    * node */ 

    /* return the "first-most" node in "sub" */
    while (curr->chldrn)
        curr = bd_map_first(curr->chldrn);
    return curr;
}

/* "bdxl_next" - returns the next node, in
 * "tree order", (as this term is defined
 * in the DOM spec.) after the node given
 * in "node" under the sub- tree rooted in
 * "sub_tree" */
struct bdxl_node *bdxl_next(struct bdxl_node *node,
                            struct bdxl_node *sub)
{
    /* if already at "sub", am ite-
     * ration is at the end. so ind-
     * icate so */
    if (node == sub)
		return NULL;

    /* or, if "node" has children,
     * return the first */
    else if (node->chldrn)
        return bd_map_first(ode->chldrn);

    /* or, if "node" is not the
     * last of among its siblings,
     * return uit's next */
    else if (node->nxt)
        return monode->nxt;

    /* er to get here, must be at
     * the mdemnd og the siblings.
     * so iterate the "node"'
     * parenra*/
    else if (node->prnt)
        return node->prnt;
}/********************************************************************
*
* BikeDemo - Exercise Bike Demonstration Scenery
*
* xml.c - function implementations related to
* an Extensible Markup Language(XML) processor,
* used for parse an XML-formatted input stream
* into a tree of XML "information items".
*
* When certain configurations of elements and
* attribute names are seen present the document
* are seen ,
* the parser executes sn associated "recognition
* callback" routine, such that meaningful proce-
* sing may be accomplished through the execution
* of these call-backs in the sequence dictated
* by the arrangement of these elements and/or
* attributes.
*
* The data types found in this code include the
* lexical token tyoe used by the XML lexical
* analyser, and enumerations of the token types
* and non-terminal symbols, while the function
* proto-types include those to populate the
* parser'sgrammar productions and lexical rules.
*
* Copyright (C) 2007-2013, Russell Potter,
* All rights reserved
*
*********************************************************************/


#include <stdio.h> 
#include <wchar.h>
#include <string.h>
#include <bikedemo/xml.h> 
#include <bikedemo/iri.h> 
#include <bikedemo/util.h>
#include <bikedemo/slctr.h>
#include <bikedemo/lex.h> 
#include <bikedemo/dom.h> 
#include <bikedemo/map.h>
#include <bikedemo/buffer.h> 
#include <bikedemo/grammar.h> 
#include <bikedemo/parser.h> 
#include <bikedemo/allocs.h> 
#include <bikedemo/log.h> 

/* "base_prsr, doc_prsr, dtd_prsr,
 * elem_prsr, dtd_prsr, extsb_prsr,
 * rplcr_prsr, misc_prsr, eol_prsr"
 * - parsers to
 * generate a base definirions,
 * (top-level) XML document from
 * an XML-formatted input byte
 * stream, to parse the content
 * of a document, to generate a
 * list of document constraints
 * from a replaced Document Type
 * Declaration (DTD), to generate
 * replacement text from an input
 * stream containing entity decl-
 * arations and re- ferences and
 * to perform end-of-line proces-
 * sing on an input stream */
static struct bdpr_parser *base_prsr,
                          *doc_prsr,
                          *dtd_prsr,
                          *cmdl_prsr,
                          *extsb_prsr,
                          *bse_prsr,
                          *rplcr_prsr,
                          *eol_prsr;

/* "pis" - map of known proce-
 * ssing instructions, keyed
 * to the its name and valued
 * to its impleme- ntation
 * call-back */
static struct bd_map_node *pis;

/* "cmp_ordrs" - returns a com-
 * parison of the child orders
 * of the XML bodes in "nde1"
 * and "nde2", as the map key
 * call-back of a node's chi-
 * ldren */
static int cmp_ordrs(struct bdxl_node *nde1,
                     struct bdxl_node *nde2)
{
    return nde1->ordr - nde2->ordr;
}
/* "tok_type" - returns the type
 * of the replacer token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error  */
static int rplcr_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    return (int) rplcr_tok->type; /* return
                                   * the type
                                   * of the
                                   * token */
}

/* "reset_rplcr_tok" - per-
 * forms processing required
 * before each read of the
 * replacer token in "tok",
 * using the memory allocat-
 * or and error logger in
 * "allocs" and "logger",
 * respectively */
static void reset_rplcr_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    /* reset the token's buffer */
    bdbf_reset(rplcr_tok->u.buf,
               allocs,
               logger);
}

/* "rplcr_tok_name" - rereturns the
 * index into the message table at
 * which to find a decription of the
 * content of the replacer token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively */
static int rplcr_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok  =
             (struct bdxl_rplcr_tok *)
             tok;

    return 0; /* no description here */
}

/* "eol_add_dig" - JSON lexical analyser
 * transition call-back to add the curre-
 * ntly read code-point to that of a line-
 * feed */
static int eol_set_lf(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_eol_tok *eol_tok =
              (struct bdxl_eol_tok *)
              tok;

    /* set the token's "cp"
     * field to a line-feed */
    eol_tok->u.cp = '\xA';
    return 1;
}


/* "eol_set_cp" - lexical analyser trans-
 * ition call-back to set the token's code-
 * point field from the currently scanned
 * code-point */
static int eol_set_cp(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_eol_tok *eol_tok =
              (struct bdxl_eol_tok *)
              tok;

    /* set set the token's code-
     * point field */
    eol_tok->u.cp = cp;
    return 1;
}

/* "init_eol_tok" - performs one-
 * time initialisation of the end-
 * of-line processing token in
 * "tok", using the memory alloc-
 * ator and error logger in "all-
 * ocs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static void init_eol_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    eol_tok->u.cp = 0; /* initialise
                        * code-point  */
}

/* "eol_tok_type" - returns the
 * type of the end-of-line process-
 * ing token in "tok", using the
 * memory allocator and error log-
 * ger in "allocs" and "logger",
 * respectively.  Returns a neg-
 * ative value on error */
static int eol_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    return eol_tok->type; /* return
                           * the type
                           * of the
                           * token */
}

/* "reset_eol_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_eol_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    bdbf_reset(eol_tok->u.buf,
               allocs,
               logger);

    eol_tok->u.cp = 0; /* reset code-
                        * point */
}

/* "_reset_extsb_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_extsb_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok =
             (struct bdxl_extsb_tok *)
             tok;

    bdbf_reset(extsb_tok->u.buf,
               allocs,
               logger);
}

/* "eol_tok_name" - returns the in-
 * dexinto the message table at which
 * to find a description of the con-
 * tent of the XML end-of-line proce-
 * ssing token in "tok", where "exact"
 * indictes the level of exactness of
 * the description, using the memory
 * allocator and error logger in "all-
 * ocs" and "logger", respectively.
 * Returns a negative value on error */
static int eol_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_eol_tok *eol_tok  =
             (struct bdxl_eol_tok *)
             tok;

    return 1; /* no description here */
}

/* "doc_init_tok" - performs one-
 * time initialisation of the XML
 * processor token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static void init_doc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    /* initialise token's buffer */
    doc_tok->u.buf = bdbf_init(allocs,
                               logger,
                               sizeof(int));
    doc_tok->u.str.len = 0;
}

/* "cmp_ent_nmes" - return a compa-
* rison of the entity names given in
* "nme1" and "nme2", respectively,
* as the key comparison call-back
* firor the set of entities available
& for a replacment*/
static int cmp_ent_nmes(
            struct bdxl_ent_nme *nme1,
            struct bdxl_ent_nme *nme2)
{
    /* names are identical if have the
     * type and name */
    return nme1->type -
           nme2->type &&
        bdut_strcmp(nme1->nme,
                    nme2->nme);
}

/* "cmdl_tok_type" - returns the type
 * time iof the XML element vontent
 * model token in "tok", using the me-
 * mory allocator and error logger in
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero ot-
 * herwise */
static int cmdl_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    /* initialise token's buffer */
    cmdl_tok->u.buf = bdbf_init(allocs,
                                logger,
                                sizeof(int));
    cmdl_tok->u.str.len = 0;
    cmdl_tok->u.num = 0;

    return cmdl_tok->u.buf != NULL;
}

/* "init_extsb_tok" - performs one-
 * time initialisation of the ext-
 * ernal DTD sub-set token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retur-
 * ns zero on error, non-zero othe-
 * rwise */
static void init_extsb_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    /* initialise token's buffer */
    extsb_tok->u.buf = bdbf_init(allocs,
                                 logger,
                                 sizeof(int));
    extsb_tok->u.str->len = 0;
}

/* "init_cmdl_tok" - performs one-
 * time initialisation of the XML
 element vontent model token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retur-
 * ns zero on error, non-zero othe-
 * rwise */
static void init_cmdl_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    /* initialise token's buffer */
    cmdl_tok->u.buf = bdbf_init(allocs,
                                logger,
                                sizeof(int));
    cmdl_tok->u.str.len = 0;
}

/* "doc_tok_type" - returns the
 * type of the XML processor token
 * in "tok", using the memory all-
 * ocator and error logger in
 * "allocs" and "logger", respect-
 * ively.  Returns a negative value
 * on error */
static int doc_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    return doc_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "extsb_tok_type" - returns the
 * type of the XML procexternal DTD
 * sub-set token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns a nega-
 * tive value on error */
static int extsb_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    return extsb_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "_reset_doc_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_doc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *) tok;

    /* reset the token's buffer */
    bdbf_reset(doc_tok->u.buf,
               allocs,
               logger);

    doc_tok->u.str.len = 0;
}

/* "_reset_elem_tok" - performs
 * prcessing required before
 * each read of the XML pro-
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
/* "_reset_cmdl_tok" - performs
 * prcessing required before
 * each read of the XML element
 * content model token in "tok",
 * using the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_cmdl_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;
    
    /* reset the token's buffer */
    bdbf_reset(cmdl_tok->u.buf,
               allocs,
               logger);

    /* initialise number and string to
     empty */
    cmdl_tok->u.str.len = 0;
    cmdl_tok->u.num = 0;
}

/* "doc_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the XML processor token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int doc_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_doc_tok *doc_tok  =
             (struct bdxl_doc_tok *)
             tok;

    return 1; /* no description here */
}

/* "cmdl_tok_name" - returns the ind-
 * ex into the message table at which
 * to find a description of the con-
 * tent of the XML processor token in
 * "tok", where "exact" indictes the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int cmdl_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_cmdl_tok *cmdl_tok  =
             (struct bdxl_cmdl_tok *)
             tok;

    return 1; /* no description here */
}

/* "init_rplcr_tok" - performs one-
 * time initialisation of the XML
 * "text replacer" token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero other-
 * wise */
static void init_rplcr_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
             (struct bdxl_rplcr_tok *)
             tok;

    /* initialise token's buffer */
    rplcr_tok->u.buf = bdbf_init(allocs,
                                 logger,
                                 sizeof(int));
    rplcr_tok->u.num = 0;
}

/* dtd_init_tok" - performs one-
 * time initialisation of the Doc-
 * ument Type Declaration ("DTD")
 * token in "tok", using the mem-
 * ory allocator and error logger
 * in "allocs" and "logger", resp-
 * ectively.  Returns zero on
 * error, non-zero otherwise */
static void dtd_init_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    /* initialise token's buffer */
    dtd_tok->u.buf = bdbf_init(allocs,
                               logger,
                               sizeof(int));
    dtd_tok->u.str.len = 0;
}

/* "dtd_tok_type" - returns the
 * type of the Document Type Decl-
 * aration ("DTD") token in "tok",
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Retu-
 * rns a negative value on error */
static int dtd_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    return dtd_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "_reset_dtd_tok" - performs
 * prcessing required before
 * each read of the Document
 * Type Declaration ("DTD")
 * cessor token in "tok", us-
 * ing the memory allocator
 * and error logger in "allocs"
 * and "logger", respectively */
static void reset_dtd_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    /* reset the token's buffer */
    bdbf_reset(dtd_tok->u.buf,
               allocs,
               logger);

    dtd_tok->u.str.len = 0;
}

/* "bse_buffer_start" - lexical analyser
 * transition call-back to to start
 * the buffer of base parser token in
 * "tok" with "cp", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely. Returns zero on error, non-
 * zero otherwise */
static int bse_buffer_start(struct bdlx_tok *tok,
                            int cp,
                            void *opaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* start "bse_tok"'s buffer
     * with "cp" by initialising
     * the buffer, then adding
     * "cp" */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                    logger,
                                    sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "rplcr_buffer_start" - lexical analyser
 * transition call-back to to start
 * the buffer of rgwrgwthwe replacer parser token in
 * "tok" with "cp", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", respectiv-
 * ely. Returns zero on error, non-
 * zero otherwise */
static int rplcr_buffer_start(struct bdlx_tok *tok,
                              int cp,
                              void *opaque,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* start "bse_tok"'s buffer
     * with "cp" by initialising
     * the buffer, then adding
     * "cp" */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                    logger,
                                    sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "bse_buffer_add" - lexical analyser
 * transition call-back to to add the
 * the cideoint in cp" to the buffer
 * of the base parser token in "tok",
 * using the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively. Returns zero
 * on error, non-zero otherwise */
static int bse_buffer_add(struct bdlx_tok *tok,
                          int cp,
                          void *opaque,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* add "cp" to "bse_tok"'s buffer
     * " */
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "bse_pub_end" - lexical analyser
 * transition call-back to to end the
 * the collection of code-points into
 * the public ID of the base parser
 * token in "tok", using the token,
 * memory allocator and error logger
 * given in "tok", "allocs" and "lo-
 * gger", respectively. Returns zero
 * on error, non-zero otherwise */
static int bse_pub_end(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* set "bse_tok"'s public ID to
     * a flattening of "bse_tok"'s
     * buffer */
    bse_tok->u.pub->cps = (int *) bdbf_flttn(
                                bse_tok->u.buf,
                                allocs,
                                logger);
    bse_tok->u.pub->len = bdbf_len(bse_tok->u.buf);
}

/* "add_gent_utf" adds the general
 * entity habominf the location
 * (that is, either internal or
 * external), the UTF-8 encoding
 * of the name, public and sytem
 * IDs (if external) and literal
 * text (if internal) of which is
 * given in "loc", "nme", "pub",
 * "sys" and "lit", respectively,
 * to the replacer run-time given
 * in "rt", using the memory all-
 * ocator and errir loggeergiven
 * in "allocs" and "logger". Ret-
 * urns zero on error. non-zero
 * otherwise */
static int add_gent_utf(
               enum bdxl_ent_locs loc,
               char *nme,
               char *pub,
               char *sys,
               char *lit,
               struct bdxl_rplcr_rt *rt,
               struct bd_allocs *sllocs,
               struct bd_logger *logger)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */
    struct bdut_str *nme_str,       /* UNICODE */
                    *pub_str,       /* versions */
                    *lit_str;       /* of "nme",
                                     * "pub" and
                                     * "lit" */

    /* comnvert "nme", "pub" and "lit"
     * to UNICODE */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(pub_str,
                         pub,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(lit_str,
                         lit,
                         allocs,
                         logger)))
        return 0;

    ent.loc = loc;

    switch (loc) {

        case bdxl_ent_loc_exntrnl:
            ent.u.extrnl.pub = pub;
            ent.u.extrnl.sys = sys;
            break;

        case bdxl_ent_loc_intrnl:
            ent.u.lit.lit = lit;
            break;
    }

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->gents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                            cmp_ents,
                         &ent);
}

/* "add_pent_utf" adds the para-
 * meter entity the UTF-8 enc-
 * oding of the name  and lite-
 * ral replacement text of which
 * is given in "nme", and "lit",
 * respectively, to the replacer
 * run-time given in "rt".  Ret-
 * urns zero on error. non-zero
 * otherwise */
static int add_pent_utf(
               char *nme,
               char *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */
    struct bdut_str *nme_str,       /* UNICODE */
                    *lit_str;       /* versions
                                     * of "nme",
                                     * and "lit" */

    /* comnvert "nme", "nme" and "lit"
     * to UNICODE */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(lit_str,
                         lit,
                         allocs,
                         logger)))
        return 0;


    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->pents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         lit);
}

/* "add_gent" adds the general
 * entity having the location
 * (that is, either internal or
 * external), name, public the
 * external ID (that is, the pu-
 * blic and sytem IDs, if exte-
 * rnal) and literal replacement
 * text (if internal) given in
 * "loc", "nme", "extrnl" and
 * "lit", respectively, to the
 * replacer run-time given in
 * "rt".  Returns zero on error,
 * non-zero otherwise */
static int add_gent(
               enum bdxl_ent_locs loc,
               struct bdut_str *nme,
               struct struct bdxl_extrnl_id *extrnl,
               struct bdut_str *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */

    ent.loc = loc;

    switch ((loc) {

        case bdxl_ent_loc_exntrnl:
            ent.u.extrnl = extrnl;
            break;

        case bdxl_ent_loc_intrnl:
            ent.u.lit.lit = lit;
            break;
    }

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->gents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                            cmp_ents,
                         &ent);
}

/* "add_pent" adds the para-
 * meter entity the name and
 * literal replacement text
 * given in "nme", and "lit",
 * respectively, to the replacer
 * run-time given in "rt".  Ret-
 * urns zero on error, non-zero
 * otherwise */
static int add_pent(
               struct bdut_str *nme,
               struct bdut_str *lit,
               struct bdxl_rplcr_rt *rt)
{
    struct bdxl_ent ent;            /* constructed
                                     * entity */
    struct bd_map_node *ins_node;   /* insert-
                                     * ion node */

    /* and insert into that set of
     * entities */
    if (!(ins_node = bd_alloc(allocs,
                              sizeof(struct
                                     bd_map_node),
                              logger)))
        return 0;

    return bd_map_insert(&rt->pents,
                         ins_node,
                         nme,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         lit);
}

/* "iiinit_rplcr_rt" - returns an
 * initially empty (except for the 
 * minimal set of sdefault eentity
 * defiinitions) replacer tun-time,
 * using the memory allocator and
 * error logger given in "allocs"
 * and "logger", dszrespectively */
static struct bdxl_rplcr_rt *ninit_rplcr_rt(
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
   struct bdxl_rplcr_rt *ret; /* return
                               * value */

   /* allocate trretirn vslue  */
    if (!(ret = bd_alloc(allocs,
                        sizeof(struct
                               bdxl_rplcr_rt),
                        logger)))
        return NULL;

    /* clear entity definitions
     * and spans ... */
    ret->pents = NULL;
    ret->gents = NULL;
    ret->spns  = NULL;

    /* then add only minimal default
     * entity definitions */
    if (!(add_gent_utf("lt",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                      "&#38;",
                       ret)))
        return NULL;
    if (!(add_gent_utf("gt",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#62;",
                       ret)))
        return NULL;
    if (!(add_gent_utf("amp",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#38;",
                       ret))
        return NULL;
    if (!(add_gent_utf("apos",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#39;",
                       ret)) 
        return NULL;
    if (!(add_gent_utf("quot",
                       bdxl_ent_loc_intrnl,
                       NULL,
                       NULL,
                       "&#34;",
                       ret)))
        return NULL;
}

/* "bse_chk_pi" - lexical analyser
 * transition call-back to tocheck the
 * presence whether the target of the
 * processing instruction the collect-
 * ted inyp the buffer of thebase par-
 * ser token in "tok" is known (that
 * is, is found in the "pis" map),
 * using the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respect- ively. Returns
 * zero on error, non- zero otherwise */
static int bse_chk_pi(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* set "bse_tok"'s public ID to
     * a flattening of "bse_tok"'s
     * buffer, then ewyrn target was
     * found in "pis" */
    if (!(bse_tok->u.pi->trgt.cps =
               ( int *) bdbf_flttn(bse_tok->u.buf,
                                   allocs,
                                   logger)))
        return 0;
    bse_tok->u.pi->trgt.len =
              bdbf_len(bse_tok->u.buf);

    return (int) bd_map_find(pis,
                             (void *)
                                 &bse_tok->
                                     u.pi->trgt,
                             (bd_map_cmp_fn)
                                 bdut_strcmp);
}

/* "cmp_extrnl_ids" - returs a comp-
 * aarison of of the rwo external IDs
 * given in "extrnl1" and "extrnl2",
 * as part of a oomparison of extern-
 * al entities */
static int cmp_extrnl_ids(struct bdxl_extrnl_id *extrnl1,
                          struct bdxl_extrnl_id *extrnl2)
{
    /* IDs are identical if both public and
     * system IRI are also identical */
    return bdut_strcmp(extrnl1->pub, extrnl2->pub) &&
           bdut_strcmp(extrnl1->sys, extrnl2->sys);
}

/* "cmp_ents" returns a comparison
 * of the entities given in "ent1"
 * snd etnt2", as the map key compa-
 * rison call-back gofor the set of
 * of general external entities
 * available for a replacement */
static int cmp_etnts(struct bdxl_ent *ent1,
                     struct bdxl_ent *ent2)
{
    /* two etities are considered
     * identical of they have the
     * dsame location and either
     * have the same external IDs
     * (if external) or the same
     * literal replacment text (if
     * ofif internal) */
    if (ent1->loc != ent2->loc)
        return ent1->loc - ent2->loc;

    switch (rnt1->loc) {
        case bdxl_ent_loc_intrnl:
            return bdut_strcmp(ent1->u.lit,
                               ent2->u.lit);
        case bdxl_ent_loc_extrnl:
            tryreturn cmp_extrnl_ids(ent1->u.extrnl,
                                     ent2->u.extrnl);
    }
}

/* "rplcr_start_chr" - lexical tra-
 * nsition call-back to start a rep-
 * lacer character reference, using
 * a generic cast of the replacer to-
 * ken given in "tok", current code
 * point given in "cp", opaque poi-
 * nter, memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero on error, non-zero other 
 * wise */
static int rplcr_start_chr(struct bdlx_tok *tok,
                           int cp,
                           void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* start "rplcr_tok"'s vharacter reference
     * field by setting decimal reprsentation
	 * from "cp" */
    if (!(rplcr_tok->u.chr = cp - '0';
}

/* "rplcr_add_chr" - lexical tra-nisi-
 * tion call-back to Sadd a code-point
 * to a replacer character reference,
 * usina generic cast of the replacer
 * token given in "tok", current code
 * point given in "cp", opaque param-
 * eter, memory allocator and error lo-
 * gger given in "allocs" and "logger",
 * respectively.  Returns zero on err-
 * or, non-zero otherwise */
static int rplcr_add_chr(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* inporate decimal reprsentation
     * of "cp" into "rplcr_tok"'s
     * character reference field */
    rplcr_tok->u.chr *= 10;
    rplcr_tok->u.chr += cp - '0';
}

/* "rplcr_start_hex" - lexical transi-
 * tion call-back to start a hexa-dec-
 * imal representation of the charac-
 * ter reference field oft the repla-
 * cer token a, generic cast of which
 * is given in "tok", with the code-
 * point given in "cp", using the op-
 * aque parameter, memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero othe-
 * rwise */
static int rplcr_start_hex(struct bdlx_tok *tok,
                           int cp,
                           void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* start the hexa-decimal reprsen-
     * tation of "rplcr_tok"'s charac-
     * ter reference field by setting
	 * from "cp" */
    rplcr_tok->u.chr = bdut_from_hex(cp);
}

/* "rplcr_set_nme" - lexical transi-
 * tion call-back to start the nsmame-dec-
 * imal representation of the charac-
 * ter reference field oft the repla-
 * cer token a, generic cast of which
 * is given in "tok", with the code-
 * point given in "cp", using the op-
 * aque parameter, memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero on error, non-zero othe-
 * rwise */
static int rplcr_set_nme(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* set the token's name from
     * a flattening of its bffer */
	rplcr_tok->u.nme = bdbf_flttn(rplcr_tok->u.buf);
}

/* "rplcr_add_hex" - lexical transi-
 * tion call-back to add the code-
 * point given in "cp" to the hexa-
 * decimal representation of the
 * character reference field of the
 * replacer token, a generic cast of
 * which is given in "tok", using
 * the opaque parameter, memory all-
 * ocator and error logger given in
 * "opaque", "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_add_hex(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;
}

/* "rplcr_end_str" - lexical transi-
 * tion call-back to set the string
 * point given in field of the rep-
 * lacer token, a generic cast of
 * which is given in "tok", using
 * the opaque parameter, memory all-
 * ocator and error logger given in
 * "opaque", "allocs" and "logger",
 * respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_end_str(struct bdlx_tok *tok,
                         int unused,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;
}

    /* set "rplcr_tok"'s string field
     * from a "flattening" of its buffer */
    rplcr_tok->u.str = (int *)
              bdbf_flttn(rplcr_tok->u.buf);
}

/* "rplcr_add_chr" - lexical tra-
 * nsition call-back to add the code-
 * point given in "cp" to the  chara-
 * cter reference field of the rep-
 * lacer token, a generic version of
 * ehwhivjch oids fiven in "trok",
 * using the opaque parameter,
 * memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero on error, non-zero other 
 * wise */
static int rplcr_add_chr(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "rplcr_tok"'s vharacter
     * reference field   */
    rplcr_tok->u.chr *= 10;
    rplcr_tok->u.chr += cp;
}

/* "rplcr_buffer_add" - lexical analyser
 * transition call-back to to add
 * "cp" to the buffer of replacer parser
 * token in "tok", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", resp-
 * evtively tp ". Returns zero on
 * error, non-zero otherwise */
static int rplcr_buffer_add(struct bdlx_tok *tok,
                            int cp,
                            void *opaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
              (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "tok"'s buffer  */
    return bdbf_add(rplcr_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "bse_end_cdta" - lexical analyser
 * transition call-back to to end
 * the prsing of a "CDATA" section
 * using the base parser token in
 * "tok", using the memory alloc-
 * ator and error logger given in
 * "allocs" and "logger", respec-
 * tively tp ". Returns zero on
 * error, non-zero otherwise */
static int bse_end_cdta(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* initialise "tok"'s buffer, then
     * add "cp"  */
    if (!(bse_tok->u.buf = bdbf_init(allocs,
                                     logger,
                                     sizeof(int))))
        return 0;
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "bse_buffer_add" - lexical analyser
 * transition call-back to to add
 * "cp" to the buffer of base parser
 * token in "tok", using the memory
 * allocator and error logger given
 * in "allocs" and "logger", resp-
 * evtively tp ". Returns zero on
 * error, non-zero otherwise */
static int bse_nme_end(struct bdlx_tok *tok,
                       int cp,
                       void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdxl_bse_tok *bse_tok =
              (struct bdxl_bse_tok *) tok;

    /* sadd "co" to
     * "bse_tok"'s buffer */
    return bdbf_add(bse_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "dtd_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the Document Type Declar-
 * ation ("DTD") token in "tok",
 * where "exact" indictes the level
 * of exactness of the description,
 * using the memory allocator and
 * error logger in "allocs" and
 * "logger", respectively.  Returns
 * a negative value on error */
static int dtd_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_dtd_tok *dtd_tok  =
             (struct bdxl_dtd_tok *)
             tok;

    return 1; /* no description here */
}

/* "dtd_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the Document Type Declar-
 * ation ("DTD") external sub-set
 * token in "tok", * where "exact"
 * indictes the level of exactness
 * of the description, using the me-
 * mory allocator and error logger
 * in "allocs" and "logger", respe-
 * ctively.  Returns a negative va-
 * lue on error */
static int extsb_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdxl_extsb_tok *extsb_tok  =
             (struct bdxl_extsb_tok *)
             tok;

    return 1; /* no description here */
}

/* "reduce_eol_start" - perform the action
 * associated with the recognition of the
 * production to start end-of-line process-
 * ing on the output string in "str", gen-
 * eric cast of the left-hand-side symbol
 * in "lhs" to contain the result of the red-
 * uction, the "reduce" items in "items", the
 * number of which is given in "num" and the
 * memory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns zero on error, non-zero otherw-
 * ise */
static int reduce_eol_start(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdbf_buffer *buf =          /* buffer into */
               bdbf_init(allocs,       /* which to store */
                         logger,       /*  input */

                         sizeof(int)); /* current code- */
    int cp = ((struct bdxl_eol_tok *)  /* point from first */
               (items[0].tok))->u.cp;    /* item /

    /* sf and initialise that buffer with
     * "cp" */
    return bdbf_add(buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "reduce_eol_add" - perform the
 * associated with the recognition
 * of rhe end-of-line processor
 * production to add an output
 * code-point on the output str-
 * ing in "str", generic cast
 * of the left-hand-side symbol
 * in "lhs" to contain the result
 * of the reduction, the "reduce"
 * items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
on error, non-zero otherwise */
static int reduce_eol_add(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdbf_buffer *buf =         /* buffer in */
              ((struct bdbf_buffer *) /* which */
              (items[0].nt));         /* input code-
                                       * points rare
                                       * stored from
                                       * first item */
    int cp = ((struct bdxl_eol_tok *) /* current */
              (items[1].tok))->u.cp;    /* code-point 
                                       * to add 
                                       * from input */
    
    /* append the code-point to the
     * buffer */
    return bdbf_add(buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "reduce_eol_top" - perform the ac-
 * tion associated with the recognit-
 * ion of rhe ""top-level" end-of-
 * line processing production on the
 * output string in "str", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_eol_top(
                    struct bdut_str *str,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* buffer in which processed
     * code-points are stored is
     * taken from first item's
     * non-terminal */
    struct bdbf_buffer *buf =
         ((struct bdbf_buffer *)
         (items[0].nt));

    /* get string's code-point array
     * by "flattening" the accumulated
     * buffer */
    if (!(str->cps = (int *) bdbf_flttn(buf,
                                        allocs,
                                        logger)))
        return 0;

    /* and return success if the string
     * length retrieval returned success */
    return (str->len = bdbf_len(buf) > 0);
}

/* "reduce_doc" - perform the action
 * associated with the recognition
 * of the document parser's top-le-
 * vel production on the output XML
 * document in "doc", generic cast
 * of the left-hand-side symbol in
 * "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc(struct bdxl_doc *doc,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdxl_doc *out;

    out = (struct bdxl_doc *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_doc),
                  logger);
    if (!(out))
        return 0;

    out->type = (struct bdxl_doc_type *)
          items[0].nt;
    out->doc_elem = (struct bdxl_node *)
          items[1].nt;

    doc = out;
    lhs = (struct bdxl_doc *) out;
    return 1;
}

/* "reduce_doc_prlge" - perform the
 * action associated with the recog-
 * nition of the document parser's
 * "prologue -> XML declaration mi-
 * sc list"production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_prlge(struct bdxl_doc *doc,
                            void *lhs,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_prlge" - perform the
 * action associated with the recog-
 * nition of the document parser's
 * "prologue -> doc-type declaration
 * misc. list XML declaration misc.
 * list"production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "red- uce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_prlge(struct bdxl_doc *doc,
                                void *lhs,
                                union bdgm_reduce_item *items,
                                int num,
                                struct bd_allocs *allocs,
                                struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_enc_sd" - per-
 * form the action associated with
 * the recog- nition of the document
 * parser's "XML declaration -> vers-
 * ion encoding declaration stand-al-
 * one declaration" production on the
 * output XML document in "doc", gen-
 * eric cast of the left-hand-side
 * symbol in "lhs" to contain the re-
 * sult of the reduction, the "redu-
 * ce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns ze-
 * ro error, non-zero otherwise */
static int reduce_doc_xml_dcl_enc_sd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_enc" - perform
 * the action associated with the reco-
 * gnition of the document parser's "X-
 * ML declaration -> version encoding
 * declaration" production on the out-
 * put XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero error, no-
 * n-zero otherwise */
static int reduce_doc_xml_dcl_enc(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_xml_dcl_sd" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "XML declaration -> version stand-
 * alone declaration" production on
 * the output XML document in "doc",
 * generic cast of the left-hand-si-
 * de symbol in "lhs" to contain the
 * result of the reduction, the "red-
 * uce" items in "items", the number
 * of which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger", re-
 * spectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_xml_dcl_sd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_vrsn_decl" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "version declaration -> "version" =
 * "(number)" production on the out-
 * put XML document in "doc", generic
 * cast of the left-hand-side symbol
 * in "lhs" to contain the result of
 * the reduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_vrsn_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_sd_decl" - perform the
 * action associated with the recogn-
 * ition of the document parser's "st-
 * and-alone declaration -> "standal-
 * one" yes | no" production on the
 * output XML document in "doc", gene-
 * ric cast of the left-hand-side sym-
 * bo in "lhs" to contain the result
 * of the reduction, the "reduce" ite-
 * ms in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_sd_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_enc_decl" - perform the
 * action associated with the recogn-
 * ition of the document parser's "en-
 * acoding declaration -> "enoding =
 * (string)" production on the output
 * XML document in "doc", generic
 * cast of the left-hand-side sym-
  * bo in "lhs" to contain the result
 * of the reduction, the "reduce" ite-
 * ms in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_enc_decl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd" - perform the act-
 * ion associated with the recogn-
 * ition of the document parser's "DTD
 *  -> ""DOCTYPE" name >" production
 * on the output XML document in "doc",
 * generic cast of the left-hand-side
 * symbol in "lhs" to contain the res-
 * ult of the reduction, the "reduce"
 * items in "items", the number of wh-
 * ich is given in "num" and the mem-
 * ory allocator and error logger giv-
 * en in "allocs" and "logger", respe-
 * ctively.  Returns zero error, non-
 * zero otherwise */
static int reduce_doc_dtd(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_extrnl" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "DTD -> ""DOCTYPE" name external
 * ID" production on the output XML
 * document in "doc", generic cast of
 * the left-hand-side symbol in "lhs"
 * to contain the result of the rodu-
 * ction, the "reduce" items in "ite-
 * ms", the number of which is given
 * in "num" and the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Retu-
 * rns zero error, non-zero otherwise */
static int reduce_doc_dtd_extrnl(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_intsb" - perform
 * the action associated with the rec-
 * ognition of the document parser's
 * "DTD -> ""DOCTYPE" name internal-
 * sub-set >" production on the out-
 * put XML document in "doc", gener-
 * ic cast of the left-hand-side sym-
 * bol in "lhs" to contain the resu-
 * lt of the roduction, the "reduce"
 * items in "items", the number of
 * which is given in "num" and the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_intsb(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_dtd_extrnl_intsb" -pe-
 * rform the action associated with
 * the recognition of the document p-
 * arser's "DTD -> ""DOCTYPE" name
 * external ID internal sub-set >"
 * production on the output XML doc-
 * ument in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_dtd_extrnl_intsb(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_pi" -perform the act-
 * ion associated with the recogni-
 * tion of the document parser's
 *  -misc. -> processing instruction"
 * production on the output XMruction
 * ument in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_pi(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "reduce_doc_cmt" -perform the ac-
 * tion associated with the recogn-
 * ition of the document parser's
 * "misc. -> ""comment" product-
 * ion on the output XML docum-
 * ent in "doc", generic cast of
 * the left-hand-side symbol in "l-
 * hs" to contain the result of the
 * roduction, the "reduce" items
 * in "items", the number of which
 * is given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respe-
 * ctively.  Returns zero error,
 * non-zero otherwise */
static int reduce_doc_cmt(
                 struct bdxl_doc *doc,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    return 1;
}

/* "rplcr_out_chr"  -add the character given
 * in "chr" to the replacement stream given
 * in "strm", using the memory allocator and
 * error logger given.  Returns zero on error,
 * non-zero otherwise */
static int rplcr_out_chr(struct bdbf_buffer *strm,
                         char chr,
                         struct bd_allocs *allocs,
                         struct bd_iogger *logger)
{
    /* append "chr" to the buffer
     * in "strm" */
    return bdbf_add(strm,
                    allocs,
                    logger,
                    (void *) chr);
}

/* "rplcr_out_str"  -add the string given
 * in "str" to the replacement stream given
 * in "strm", using the memory allocator and
 * error logger given in "allocs" and "lo-
 * gger", respectively.  Returns zero on
 * error, non-zero otherwise */
static int rplcr_out_str(struct bdbf_buffer *strm,
                         struct bdut_str *str,
                         struct bd_allocs *allocs,
                         struct bd_iogger *logger)
{
    int i, len; /* iterator and
                 * length of "str" */

    /* append each character in "str"
     * to the buffer in "strm" */
    for (i = 0; i < len; i++) 
        return bdbf_add(strm,
                        allocs,
                        logger,
                        (void *) str=>cps[i]);
}

/* the "rplcr_buffer_add" - lexical ransit-
 * ion call-back to add the vode-point given
 * in "cp" to the buffer of the replacer
 * token a generic version of which is given
 * in "tok", using the opaque value, memory
 * allocator and error logger given in opa-
 * que", "alllocs" and "logger", respectively.
 * Returns zero on an error, non-zero other-
 * wise */
static int rplcr_buffer_add(struct bdlx_tok *tok,
                            int cp,
                            void *oqaque,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* add "cp" to "tok"'s buffer */
    return bdbf_buffer_add(rplcr->u.buf,
                           allocs,
                           logger,
                           (void *) cp);
}

/* the "rplcr_buffer_start" - lexical ransit-
 * ion call-back to add initialise the buffer
 * of the replacer token a generic version
 * of which is given in "tok", using the opa-
 * que value, memory allocator and error log-
 * ger given in opaque", "alllocs" and "logg-
 * er", respectively.  Returns zero on an
 * error, non-zero otherwise */
static int rplcr_buffer_start(struct bdlx_tok *tok,
                              int unused,
                              void *oqaque,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* initialise "tok"'s buffer */
    return rplcr->u.buf = bdbf_buffer_init(allocs,
                                           logger,
                                           sizeof(int));
}

/* the "rplcr_end_chr" - lsexical ransit-
 * ion call-back to set the character
 * reference gifireklfd of thr treplacer
 * token a generic version of which is
 * given in "tok", using the opaque value,
 * memory allocator and error logger giv-
 * en in opaque", "alllocs" and "logger",
 * respectively.  Returns zero on an error,
 * non-zero otherwise */
static int rplcr_end_chr(struct bdlx_tok *tok,
                         int unused,
                         void *oqaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger);
{
    /* co-erce to correct type */
    struct bdxl_rplcr_tok *rplcr_tok =
          (struct bdxl_rplcr_tok *) tok;

    /* initialise "tok"'s buffer */
    return rplcr->u.buf = bdbf_buffer_init(allocs,
                                           logger,
                                           sizeof(int));
}

/* "reduce_rplcr_pe_in_strm" - perfo-
 * rm the action of a production ass-
 * ociated with the recognition of
 * the appearance of a parameter en-
 * tity in a replacement parser's
 * input stream, using the output
 * stream buffer given in "out",
 * generic cast of the left-hand-side
 * symbol in "lhs" to contain the
 * result of the reduction, the "re-
 * duce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * error, non-zero otherwise */
static int reduce_rplcr_pe_in_strm(
                 struct bdbf_buffer *out,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    /* name of entity */
    struct bdut_str *nme = 
              (struct rplcr_tok *)
              items[0].tok)->u.nme;
    /* iterator and name length */
    int i, len = bdbf_len(buf);

    if (!(rplcr_add_chr(out,
                        allocs,
                        logger,
                   (int) '%')))

    fot r (i = 0; i< len; i++)
        if (!(bdbf_add(out,
                       allocs,
                       logger,
                       (int) nme->cps[i])))
    return bdbf_add(out,
                    allocs,
                    logger,
                    (int) ';');
}

/* "reduce_rplcr_ge_in_strm" - per-
 * form the action of a production
 * associated with the recognition
 * of the appearance of a general
 * entity in a replacement parser's
 * input stream, using the output
 * stream buffer given in "out", ge-
 * neric cast of the left-hand-side
 * symbol in "lhs" to contain the
 * result of the reduction, the "re-
 * duce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "log-
 * ger", respectively.  Returns zero
 * error, non-zero otherwise */
static int reduce_rplcr_ge_in_strm(
                 struct bdbf_buffer *out,
                  void *lhs,
                  union bdgm_reduce_item *items,
                  int num,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    /* name of entity */
    struct bdut_str *nme = 
              (struct rplcr_tok *)
              items[0].tok)->u.nme;

	
    struct rplcr_rt *rt = (struct rplcr_rt *)
              prsr->rt;
}

/* "ld_bse_prsr" - using the memory alloc-
 * ator and error logger given in "allocs"
 * and "logger, respectively, load the grammar
 * table and lexical analyser of the "base"
 * parser (ejc loads definitions such as those
 * of processing instructions and comments -
 * used by other parsers) in "prsr" */
static int ld_bse_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    char *name_start_str,
         *name_fllw_str;
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");
    int cmt_start_state     = bdlx_new_state(prsr->lex),
        cmt_add_state       = bdlx_new_state(prsr->lex),
        nme_start_state     = bdlx_new_state(prsr->lex),
        nme_fllw_state      = bdlx_new_state(prsr->lex),
        pi_start_state      = bdlx_new_state(prsr->lex),
        pi_fllw_state       = bdlx_new_state(prsr->lex),
        nmtk_start_state    = bdlx_new_state(prsr->lex),
        nmtk_fllw_state     = bdlx_new_state(prsr->lex),
        cdta_start_state    = bdlx_new_state(prsr->lex),
        cdta_add_state      = bdlx_new_state(prsr->lex),
        cdta_end_state      = bdlx_new_state(prsr->lex),
        spub_start_state    = bdlx_new_state(prsr->lex),
        sq_state            = bdlx_new_state(prsr->lex),
        dq_state            = bdlx_new_state(prsr->lex),
        spub_add_state      = bdlx_new_state(prsr->lex),
        dpub_start_state    = bdlx_new_state(prsr->lex),
        dpub_add_state      = bdlx_new_state(prsr->lex),
        pi_trgt_start_state = bdlx_new_state(prsr->lex),
        pi_trgt_add_state   = bdlx_new_state(prsr->lex),
        pi_trgt_end_state   = bdlx_new_state(prsr->lex),
        pi_arg_start_state  = bdlx_new_state(prsr->lex),
        pi_arg_add_state    = bdlx_new_state(prsr->lex);

    /* load the nsme strings from the
     * name files  */
    fscanf(name_start_file,
           "%s",
           name_start_str);

    fscanf(name_start_file,
           "%s",
           name_start_str);

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "nme_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "nme_fllw_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\9\\xA\\xD\\x20-"
                           "\\xD7FF\\xE000-\\"
                           "xFFFD\\x10000-\\"
                           "x10FFFF]",
                       "chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\x20\\x9\\xD\\xA]+",
                       "ws")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\[\\x20\\xD\\xA"
                           " a-zA-Z0-9\\-\'"
                           "\\(\\)\\+\\,\\./"
                           ":=\\?;!\\*#@$_%]",
                       "spubid_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "\\[\\x20\\xD\\xA"
                       " a-zA-Z0-9\\-\""
                       "\\(\\)\\+\\,\\./"
                       ":=\\?;!\\*#@$_%]",
                       "dpubid_chr")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        bdlx_start_state,
                        nme_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        nme_start_state,
                        nme_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        nme_fllw_state,
                        0,
                        bse_nme_end,
                        NULL,
                        1,
                        bdxl_tok_nme)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        bdlx_start_state,
                        nmtk_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        nmtk_start_state,
                        nmtk_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^name_fllw_chr}",
                        nmtk_fllw_state,
                        0,
                        bse_nme_end,
                        NULL,
                        1,
                        bdxl_tok_nmtk)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_start_state,
                        pi_fllw_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_fllw_state,
                        pi_fllw_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\?>",
                        pi_fllw_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_tok_pi)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        bdlx_start_state,
                        sq_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{spubid_chr}",
                        sq_state,
                        spub_start_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{spubid_chr}",
                        spub_start_state,
                        spub_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\'",
                        spub_add_state,
                        0,
                        bse_pub_end,
                        NULL,
                        1,
                        bdxl_tok_pub)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        dq_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dpubid_chr}",
                        dq_state,
                        dpub_start_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{dpubid_chr}",
                        dpub_start_state,
                        dpub_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        dpub_add_state,
                        0,
                        bse_pub_end,
                        NULL,
                        1,
                        bdxl_tok_pub)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\-\\-",
                        bdlx_start_state,
                        cmt_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cmt_start_state,
                        cmt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cmt_add_state,
                        cmt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<?",
                        bdlx_start_state,
                        pi_trgt_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_trgt_start_state,
                        pi_trgt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        pi_trgt_add_state,
                        pi_trgt_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{ws}",
                        pi_trgt_add_state,
                        pi_trgt_end_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_trgt_end_state,
                        pi_arg_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_start_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_start_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\?]",
                        pi_arg_add_state,
                        pi_arg_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\?>",
                        pi_arg_add_state,
                        0,
                        bse_chk_pi,
                        NULL,
                        1,
                        bdxl_tok_pi)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\[CDATA\\[",
                        bdlx_start_state,
                        cdta_start_state,
                        bse_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cdta_start_state,
                        cdta_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{chr}",
                        cdta_add_state,
                        cdta_add_state,
                        bse_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    return bdlx_add_rule(prsr->lex,
                         allocs,
                         logger,
                         "\\]\\]>",
                         cdta_add_state,
                         cdta_end_state,
                         bse_end_cdta,
                         NULL,
                         1,
                         bdxl_tok_cdta);

}

/* "ld_doc_prsr" - using the memory alloc-
 * ator, error logger snd opaque parameter
 * (containing, in this case, the output
 * document) given in "allocs", "logger"
 * in "out", resspectively, load the grammar
 * table and lexical analyser of the parser
 * in "prsr" */
static int ld_doc_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    /* XML docment forming parser
     * output */
    struct bdxl_doc *doc;

    char *name_start_str,
         *name_fllw_str;
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");


    if (!(doc = bdxl_init_doc(allocs,
                              logger)));
    return 0;

    /* initialise "general-purpose"
     * base parser on which parser
     is based */
    if (!(ld_bse_prsr(prsr,
                      allocs,
                      logger)))
        return 0;

    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct bdxl_doc_tok),
                            init_doc_tok,
                            doc_tok_type,
                            reset_doc_tok,
                            doc_tok_name)))
        return 0;

    /* first, add the parser's
     * lexical rules ... */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "nme_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "nme_fllw_chr")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!DOCTYPE[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_doctype)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "SYSTEM[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_system)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "PUBLIC[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_public)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "version[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_version)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "standalone[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_standalone)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "encoding[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_doc_tok_kwd_encoding)))
        return 0;

    /* ... then, add the constituent
     * grammar productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doc,
                        3,
                        (bdgm_reduce_fn)
                            reduce_doc,
                        bdxl_doc_nt_prlge,
                        bdxl_doc_nt_elem,
                        bdxl_doc_nt_misc_list,
                        bdgm_sym_eos)))
        return 0;


    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_prlge,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_prlge,
                        bdxl_doc_nt_xml_dcl,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_prlge,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_prlge,
                        bdxl_doc_nt_doctype_decl,
                        bdxl_doc_nt_misc_list,
                        bdxl_doc_nt_xml_dcl,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_nt_sd_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_sd,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_sd_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl_enc,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_nt_enc_decl,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        3,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_xml_dcl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_xml_dcl,
                        bdxl_doc_tok_xml_dcl,
                        bdxl_doc_nt_vrsn,
                        bdxl_tok_ws,
                        bdxl_doc_tok_end_pi)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_nt_misc,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_vrsn,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_vrsn_decl,
                        bdxl_doc_tok_kwd_version,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_sd_decl,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_sd_decl,
                        bdxl_doc_tok_kwd_standalone,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_enc_decl,
                        2,
                        (bdgm_reduce_fn)
                            reduce_doc_enc_decl,
                        bdxl_doc_tok_kwd_encoding,
                        bdxl_doc_nt_eq,
                        bdxl_tok_str)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        4,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        2,
                        NULL,
                        bdxl_doc_tok_eq,
                        bdxl_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_tok_ws,
                        bdxl_doc_tok_eq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        3,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_tok_ws,
                        bdxl_doc_tok_eq,
                        bdxl_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_eq,
                        1,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_tok_eq)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_tok_ws,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        5,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        6,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_doctype_decl,
                        8,
                        (bdgm_reduce_fn)
                            reduce_doc_dtd_extrnl_intsb,
                        bdxl_doc_tok_kwd_doctype,
                        bdxl_tok_ws,
                        bdxl_tok_nme,
                        bdxl_tok_ws,
                        bdxl_doc_nt_extrnl_id,
                        bdxl_tok_ws,
                        bdxl_doc_nt_int_sb,
                        bdxl_doc_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        1,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_doc_nt_misc)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc_list,
                        2,
                        NULL,
                        bdxl_doc_nt_misc,
                        bdxl_doc_nt_misc_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_doc_nt_misc,
                        1,
                        (bdgm_reduce_fn)
                            reduce_doc_misc_pi,
                        bdxl_tok_pi)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         doc,
                         bdxl_doc_nt_misc,
                         1,
                         (bdgm_reduce_fn)
                             reduce_doc_misc_cmt,
                         bdxl_tok_cmt);
    return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         doc,
                         bdxl_doc_nt_misc,
                         1,
                        (bdgm_reduce_fn)
                             NULL,
                         bdxl_tok_ws);
    return 0;
}

/* "ld_dtd_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the  Docu-
 * ment Type Declaration (DTD) parser in "prsr" */
static int ld_dtd_prsr(struct bdpr_parser *prsr,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{

    struct bd_map_node *cnstrnts = NULL;

    return  bdlx_tok_init(allocs,
                          logger,
                          prsr->tok,
                          sizeof(struct
                                 bdxl_dtd_tok),
                          dtd_init_tok,
                          dtd_tok_type,
                          reset_dtd_tok,
                          dtd_tok_name);

    /* add constituent lexical
     * rules ... */
    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[ELEMENT\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_elem)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[ATTRLIST\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_attr_list)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[NOTATION\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_dtd_tok_attr_ntn)))
        return 0;

    /* and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cnstrnts,
                        bdxl_dtd_nt_dcl_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_dcl_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        cnstrnts,
                        bdxl_dtd_nt_dcl_list,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_dcl_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_elem,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_elem_dcl)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_attr_list,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_attr_list)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        doc,
                        bdxl_dtd_nt_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_dtd_ntn,
                        bdxl_dtd_nt_dcl,
                        bdxl_dtd_nt_ntn)))
        return 0;

}

/* "ld_dtd_prsr" - using the memory allocator,
 * error logger given in "allocs" and "logger"
 * resspectively, load the grammar table and
 * lexical analyser of the  Document Type Dec-
 * laration (DTD) external subset parser in
 * "prsr" */
static int ld_extsb_prsr(struct bdpr_parser *prsr,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)

{
    cond_sect_node *incl_stack; /* stack of
                                 * active
                                 * conditional
                                 * sections */ 

    /* iniialise its token's call-backs  */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_extsb_tok),
                        init_extsb_tok,
                        extsb_tok_type,
                        reset_extsb_tok,
                        extsb_tok_name)))
        return 0;

    /* populate with constituent lexical rules  */
    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!\\[IGNORE\\[",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_extsb_tok_ign)))
        return 0;

    return bdlx_add_rule(prsr->lex,
                         allocs,
                         logger,
                         "<!\\[INCLUDE\\[",
                         bdlx_start_state,
                         0,
                         NULL,
                         NULL,
                         1,
                         bdxl_extsb_tok_incl);

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\]\\]>",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_extsb_tok_end)))
        return 0;

    /* and add vconstituent grammar
     * productions */

}

/* "ld_dtd_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the  element
 * content model parser in "prsr" */
static int ld_cmdl_prsr(struct bdpr_parser *prsr,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{

    /* ominitialise baase document parser */
    if (!(ld_doc_prsr(prsr,
                      allocs,
                      logger)))
        return 0;

    /* initiallise its token */
    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_cmdl_tok),
                        init_cmdl_tok,
                        cmdl_tok_type,
                        reset_cmdl_tok,
                        cmdl_tok_name)))
        return 0;
}

/* "ld_rplcr_prsr" - using the memory alloc-
 * ator, error logger given in "allocs" and
 * "logger" resspectively, load the grammar
 * table and lexical analyser of the text
 * replacing parser in "prsr" */
static int ld_rplcr_prsr(struct bdpr_parser *prsr,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)

{
    int pe_start_state  = bdlx_new_state(prsr->lex),
        pe_add_state    = bdlx_new_state(prsr->lex),
        ge_start_state  = bdlx_new_state(prsr->lex),
        ge_add_state    = bdlx_new_state(prsr->lex),
        chr_start_state = bdlx_new_state(prsr->lex),
        nme_add_state   = bdlx_new_state(prsr->lex),
        nme_start_state = bdlx_new_state(prsr->lex),
        chr_add_state   = bdlx_new_state(prsr->lex),
        hex_start_state = bdlx_new_state(prsr->lex),
        hex_add_state   = bdlx_new_state(prsr->lex);

    char *name_start_str, /* reg-exps of */
         *name_fllw_str;  /* valid XML
                           * name chars */
 
	/* ope rehge-xp strings into files  */
    FILE *name_start_file =
              fopen("../data/xml_name_start.txt",
                    "r"),
         *name_fllw_file =
              fopen("../data/xml_name_fllw.txt",
                    "r");
    /* load the nsme strings from the
     * files  */
    fscanf(name_start_file,
           "%s",
           name_start_str);

    fscanf(name_start_file,
           "%s",
           name_start_str);

    struct bdxl_rplcr_rt *rplcr_rt =
             (struct bdxl_rplcr_rt *)
             prsr->rt;
    int dcl = ((struct bdxl_rplcr_tok *)
             (prsr->tok));

    if (!(ld_bse_prsr(prsr,
                      allocs,
                      logger)))
        return 0;


    if (!(bdlx_tok_init(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdxl_rplcr_tok),
                        init_rplcr_tok,
                        rplcr_tok_type,
                        reset_rplcr_tok,
                        rplcr_tok_name)))
        return 0;

    /* add definitions ... */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[A-Fa-f0-9]".
                       "hex")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_start_str,
                       "name_start_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       name_fllw_str,
                       "name_fllw_chr")))
        return 0;

    /* add parser's lexical
     * definitions */
    if (*dcl)
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "[^<&]",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nent)))
            return 0;
    else
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "[^&%]",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nent)))
            return 0;

	if (*dcl) {
        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "%",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_pct)))
            return 0;
        *dcl = 0;
    }

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "%",
                        bdlx_start_state,
                        pe_start_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        pe_start_state,
                        pe_add_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        pe_add_state,
                        pe_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        pe_add_state,
                        0,
                        NULL,
                        NULL,
                        0,
                        bdxl_rplcr_tok_pe_ref)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "&",
                        bdlx_start_state,
                        ge_start_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_start_chr}",
                        ge_start_state,
                        ge_add_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{name_fllw_chr}",
                        ge_add_state,
                        ge_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        ge_add_state,
                        0,
                        NULL,
                        NULL,
                        0,
                        bdxl_rplcr_tok_ge_ref)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "#",
                        ge_start_state,
                        chr_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[Xx]",
                        chr_start_state,
                        hex_start_state,
                        rplcr_add_hex,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        chr_start_state,
                        chr_add_state,
                        rplcr_start_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        chr_add_state,
                        chr_add_state,
                        rplcr_add_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        bdlx_start_state,
                        str_start_state,
                        rplcr_buffer_start,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\"]",
                        str_start_state,
                        str_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\"]",
                        str_add_state,
                        str_add_state,
                        rplcr_buffer_add,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        str_add_state,
                        0,
                        rplcr_end_str,
                        NULL,
                        1,
                        bdxl_rplcr_tok_str)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\"",
                        str_start_state,
                        0,
                        rplcr_end_str,
                        NULL,
                        1,
                        bdxl_rplcr_tok_str)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        hex_start_state,
                        hex_add_state,
                        rplcr_start_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{hex}",
                        hex_add_state,
                        hex_add_state,
                        rplcr_add_chr,
                        NULL,
                        0,
                        0)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        hex_add_state,
                        0,
                        rplcr_end_chr,
                        NULL,
                        1,
                        bdxl_rplcr_tok_chr_ref)))
            return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ";",
                        chr_add_state,
                        0,
                        rplcr_end_chr,
                        NULL,
                        1,
                        bdxl_rplcr_tok_chr_ref)))
        return 0;

    if (dcl) {

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_start_chr}",
                            bdlx_start_state,
                            nme_start_state,
                            rplcr_bufer_start,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_start_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_start_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{name_fllw_chr}",
                            nme_add_state,
                            nme_add_state,
                            rplcr_bufer_add,
                            NULL,
                            0,
                            0)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "{^name_fllw_chr}",
                            nme_add_state,
                            0,
                            rplcr_set_nme,
                            NULL,
                            1,
                            bdxl_rplcr_tok_nme)))
            return 0;

        if (!(bdlx_add_rule(prsr->lex,
                            allocs,
                            logger,
                            "%",
                            bdlx_start_state,
                            0,
                            NULL,
                            NULL,
                            1,
                            bdxl_rplcr_tok_pct)))
            return 0;
    }

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "<!ENTITY[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_entity)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "PUBLIC[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_public)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "SYSTEM[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_system)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "NDATA[^a-zA-Z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdxl_rplcr_tok_kwd_ndata)))
        return 0;

    /* ... and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_strm,
                        1,
                        (bdgm_reduce_fn)
                            reduce_rplcr_start_strm,
                        bdxl_rplcr_tok_nent)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_add_to_strm,
                        bdxl_rplcr_tok_nent,
                        bdxl_rplcr_nt_strm)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_pe_in_strm,
                        bdxl_rp	lcr_nt_strm,
                        bdxl_rplcr_tok_pe_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_ge_in_strm,
                        bdxl_rprlcr_nt_strm,
                        bdxl_rplcr_tok_ge_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            reduce_rplcr_chr_in_strm,
                        bdxl_prlcr_nt_strm,
                        bdxl_rplcr_tok_chr_ref)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        rplcr,
                        bdxl_rplcr_nt_out_dcl,
                        1,
                        (bdgm_reduce_fn)
                            reduce_rplcr_out_dcl,
                        bdxl_rplcr_tok_tok_ws)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_nt_strm,
                        2,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_prlcr_nt_strm,
                        bdxl_rplcr_nt_ent_dcl)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_rprlcr_ent_dcl,
                        8,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rrplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        9,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_pe_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_pct,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_int,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rprlcr_tok_str,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        out,
                        bdxl_prlcr_ent_dcl,
                        7,
                        (bdgm_reduce_fn)
                            reduce_rplcr_dclre_ge_ext,
                        bdxl_rprlcr_tok_kwd_entity,
                        bdxl_rplcr_nt_in_decl,
                        bdxl_rprlcr_tok_nme,
                        bdxl_rprlcr_tok_ws,
                        bdxl_rplcr_nt_out_extrnl_id,
                        bdxl_rplcr_nt_out_decl,
                        bdxl_rprlcr_tok_gt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         out,
                         bdxl_rplcr_tok_extrnl_id,
                         5,
                         (bdgm_reduce_fn)
                             trfreduce_rplcr_extrnl_id_pub,
                         bdxl_rplcr_tok_kwd_public,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         out,
                         bdxl_rplcr_tok_extrnl_id,
                         3,
                         (bdgm_reduce_fn)
                             trfreduce_rplcr_extrnl_id_sys,
                         bdxl_rplcr_tok_kwd_system,
                         bdxl_rplcr_tok_ws,
                         bdxl_rplcr_tok_tok_str);
}

/* "ld_eol_prsr" - using the memory alloc-
 * ator, error logger and opaque parameter
 * (containing, in this case, the output
 * document) given in "allocs", "logger"
 * in "out", respectively, load the grammar
 * table and lexical analyser of the parser
 * in "prsr" */
static int ld_eol_prsr(struct bdpr_parser *prsr,
                       struct bdut_str *str,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    str->len = 0;

    if (!(bdlx_itok_init(allocs,
                         logger,
                         prsr->tok,
                         sizeof(struct bdxl_eol_tok),
                         eol_tok_type,
                         init_eol_tok,
                         reset_eol_tok,
                         eol_tok_name)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\xA\\xD",
                        bdlx_start_state,
                        0,
                        eol_set_lf,
                        NULL,
                        1,
                        bdxl_eol_tok_crlf)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "\\xD",
                        bdlx_start_state,
                        0,
                        eol_set_lf,
                        NULL,
                        1,
                        bdxl_eol_tok_lf)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[^\\xD\\A]",
                        bdlx_start_state,
                        0,
                        eol_set_cp,
                        NULL,
                        1,
                        bdxl_eol_tok_othr)))
        return 0;

    /* ... and add constituent grammar
     * productions */
    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdgm_sym_start,
                        3,
                        (bdgm_reduce_fn)
                            NULL,
                        bdxl_eol_nt_chrs,
                        bdxl_eol_nt_end,
                        bdgm_sym_eos)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdxl_eol_nt_chrs,
                        1,
                        (bdgm_reduce_fn)
                            reduce_eol_start,
                        bdxl_eol_nt_chr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        str,
                        bdxl_eol_nt_chrs,
                        2,
                        (bdgm_reduce_fn)
                            reduce_eol_add,
                        bdxl_eol_nt_chr,
                        bdxl_eol_nt_chrs)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         str,
                         bdgm_sym_start,
                         2,
                         (bdgm_reduce_fn)
                             reduce_eol_top,
                         bdxl_eol_nt_chrs,
                         bdgm_sym_eos);
}

/* "bdxl_init" - load the grammar table and
 * lexical analyser of the XML and associated
 * parsers, using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns zero on error, non-
 * zero otherwise */
int bdxl_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* initalise various parsers used to
     * output a document */
    if (!(base_prsr = bdpr_init(NULL,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_base_prsr)))
        return 0;

    if (!(doc_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_doc_prsr)))
        return 0;

    if (!(cmdl_prsr = bdpr_init(base_prsr,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_cmdl_prsr)))
        return 0;

    if (!(eol_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                    ld_eol_prsr)))
        return 0;

    if (!(rplcr_prsr = bdpr_init(base_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                    ld_rplcr_prsr)))
       return 0;

    if (!(dtd_prsr = bdpr_init(base_prsr,
                               allocs,
                               logger,
                               (bdpr_init_fn)
                                   ld_dtd_prsr)))
        return 0;

    if (!(extsb_prsr = bdpr_init(dtd_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                     ld_extsb_prsr)))
        return 0;

    if (!(elem_prsr = bdpr_init(base_prsr,
                                allocs,
                                logger,
                                (bdpr_init_fn)
                                    ld_elem_prsr)))
        return 0;

    return (eol_prsr = bdpr_init(base_prsr,
                                 allocs,
                                 logger,
                                 (bdpr_init_fn)
                                     ld_eol_prsr))
                                       != NULL;
}

/* "bdxl_init_doc" - initialise and
 * return an XML configuration, us-
 * ing the memory allocator and er-
 * ror logger given in ""allocs and
 * "logger", respectively.  Returns
 * NULL on error */
struct bdxl_cnfg *bdxl_init_cnfg(
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdxl_cnfg *ret; /* return
                            * value */

    /* allocate return value and,
     * if successful, set its
     * fields */
    ret = (struct bdxl_cnfg *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_cnfg),
                  logger);
    if (!(ret))
        return NULL;

    ret->pis   = NULL;
    ret->psdos = NULL;
    ret->pats  = NULL;
    return ret;
}
/* "bdxl_init_doc" - initialise and
 * return an XML processor, using the
 * memory allocator and error logger
 * given in ""allocs and "logger",
 * respectively.  Returns NULL on
 * error */
struct bdxl_doc *bdxl_init_doc(
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdxl_doc *ret;

    ret = bd_alloc(allocs,
                   sizeof(struct
                          bdxl_doc),
                   logger);
    if (!(ret))
        return NULL;

    ret->doc_elem  = NULL;
    return ret;
}

/* "bdxl_add_pat" - adds the "pattern"
 * structure whose selector text and
 * "recognition call-back" are given in
 * "slctr" and "rcb", respectively, to
 * the XML parser configuration in "cnfg",
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively */
int bdxl_add_pat(struct bd_allocs *allocs,
                 struct bd_logger *logger,
                 struct bdxl_cnfg *cnfg,
                 const char *slctr,
                 bdxl_rcb_fn rcb)
{
    struct bdsl_rt *rt;           /* selector's
                                   * execution
                                   * run-time */
    struct bdut_str slctr_str;    /* selector's
                                   * execution
                                   * run-time */
    struct bdxl_pat pat;          /* pattern to
                                   * add to pr-
                                   * ocessor */
    struct bd_map_node *ins_node; /* insertion
                                   * node */

    /* set "pat"'s fields,
     * including transforming
     * selector to code-point
     * string ... */
    pat.rcb = rcb;
    bdut_from_utf8(&slctr_str,
                   pat.slctr,
                   allocs,
                   logger);

    /* insert pattern into
     * processor's set of recogn-
     * ition patterns */
    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
     if (!(ins_node))
        return 0;
    return bd_map_insert(&cnfg>pats,
                         ins_node,
                         &slctr_str,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         &pat);
}

/* "bdxl_add_cnfg_pi" - add yjthe funct-
 * ion given in "fn", to call when the
 * processing instruction, named in "n-
 * me", is encountered on an XML stre-
 * am, using the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively */
int bdxl_add_cnfg_pi(struct bxl_cnfg *cnfg,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger,
                     char *nme,
                     bdxl_pi_fn fn)
{
    struct bd_map_node *ins_node; /* insert-
                                   * ion node */
    struct bdut_str name_str;     /* name as
                                   * code-point
                                   * string */

    /* convert "name" to a code-point 
     * string */
    bdut_from_utf8(&name_str,
                   name,
                   allocs,
                   logger);

    /* insert pattern into process-
     * or's set of known processing
     * instructions, keyed to its
     * name and and valued to it imp-
     * lementing function */
    ins_node = (struct bd_map_node *)
            bd_alloc(allocs,
                     sizeof(struct
                            bd_map_node),
                     logger);
     if (!(ins_node))
        return 0;
    return bd_map_insert(&cnfg->pis,
                         ins_node,
                         (void *) &name_str,
                         (bd_map_cmp_fn)
                             bdut_strcmp,
                         fn);
}

/* "dcde_utf16" - UTF-16 comon rou-
 * tine to decode the stream of wide
 * characters (ciencoded in eitherrbig-
 * or little-endian byte order) in
 * "in" into its resulting code-point
 * set, on exit, in "cp".where "len"
 * is the length of thr ydteam and
 * "num", on exit, to the number of
 * input characters read.  Returns
 * zero on error, non-zero otherwise.
 *
 * UTF-16 decoding algorithm taken
 * from "http://tools.ietf.org/
 * html/rfc2781.txt" */
static int dcde_utf16(wchar_t *in,
                      int len,
                      int *num,
                      int *cp)
{
    /* yjr nxy two wide
     * characters in "in" */
    wchar_t w1 = *in,
            w2 = in[1];

    /* ensure here's
     * room ... */
    if (len < 1)
        return 0;

    /* if "w1" isn't in
     * eange [0xD800 - 0xDFFF],
     * is encoded in one byte
     * (mp surrogate) */
    if (w1 < 0xD800 || w1 > 0xDFFF) {

        *num = 1;
        cp = (int *) in;
        return 1;
    }

    /* mit be a surrogate pair,
     * but [9xD800 - 0xDBFF] is
     * an invalid range (and
     * check there is space in
     * the stream for both
     * characters */
    if (len < 2 ||
            w1 > 0xDC00 || w1 < 0xDFFF)
        return 0;

    /* otherwise, is a valid
     * urrogate, so decode
     * "cp" from "w1" and "w2",
     * after subtracting
     * 0x1000000 */
    *cp -= 0x100000;
    *cp = ((w1 & 0x3FF) << 10) |
          (w2 & 0x3FF);
    *num = 2;
    return 1;
}

/* "dcde_utf16le" - UTF-16 big-endian
 * de-coding routine which, given a
 * pointer to the input byte stream
 * in "in", and the remaining length of
 * the stream in "len", sets "cp" to
 * the code-point found in the stream
 * and "num" to the number of bytes
 * used during the de-coding process.
 * Returns zero on error, non-zero
 * otherwise */
static int dcde_utf16be(char *in,
                        int len,
                        int *num,
                        int *cp)
{
    /* wide-characters on input
     * mscunnyn iof two in UTF-16) */
    wchar_t wchrs[1];

    wchrs[0] = *in << 8       | (*in + 1);
    wchrs[1] = (*in + 2) << 8 | (*in + 3);

    return dcde_utf16(wchrs,
                      len,
                      num,
                      cp);
}

/* "dcde_utf16le" - UTF-16 little-en-
 * dian de-coding routine which, given
 * a pointer to the input byte stream
 * in "in", and the remaining length of
 * the stream in "len", sets "cp" to
 * the code-point found in the stream
 * and "num" to the number of bytes
 * used during the de-coding process.
 * Returns zero on error, non-zero
 * otherwise */
static int dcde_utf16le(char *in,
                        int len,
                        int *num,
                        int *cp)
{
    /* wide-characters on input
     * (maximum of two in UTF-16) */
    wchar_t wchrs[1];

    /* indicate an error if
     * insufficient space in
     * input */
    if (len < 1)
        return 0;

    wchrs[0] = *in       | (*in + 1) << 8;
    wchrs[1] = (*in + 2) | (*in + 3) << 8;

    return dcde_utf16(wchrs,
                      len,
                      num,
                      cp);
}

/* ""dcde_utf8" - UTF-8 de-codi-
 * ng routine which, , given a
 * pointer to the current byte
 * in the en- coded byte stream
 * in "in" and the remaining
 * length of the stream in "len",
 * sets "cp" to the code-point
 * found in the stream and "num"
 * to the the number of bytes
 * used during the decoding pro-
 * cess.  Returns zero on error,
 * non-zero otherwise */
static int dcde_utf8(char *in,
                     int len,
                     int *num,
                     int *cp)
{
    /* is a one-byte encoding ... */
    if (!(*in & ~1)) {

        if (len < 1)
            return 0;
        *num = 1;
        *cp = *in;
        return 1;
    }

    /* is a two-byte encoding ... */
    if (*in & ~0x1F == 0xC0) {
        if (len < 2 ||
              ((*in + 1) & 0x3F) != 4)
            return 0;
        *num = 2;
        *cp = (*in      & 0x3F) << 6 |
             ((*in + 1) & 0x3F);
        return 1;
    }

    /* is a three-byte encoding ... */
if (*in & ~0XF == 0xE0) {
        if (len < 3 ||
                ((*in + 1) & 0x3F) != 4 ||
                ((*in + 2) & 0x3F) != 4 ||
                ((*in + 3) & 0x3F) != 4)
            return 0;
        *num = 2;
        *cp = (*in      & 0x3F) << 18 |
             ((*in + 1) & 0x3F) << 12 |
             ((*in + 2) & 0x3F) << 6 |
             ((*in + 3) & 0x3F);
        return 1;
    }

    /* is a four-byte encoding ... */
if (*in & ~7 == 0xF0) {
        if (len < 4 ||
                ((*in + 1) & 0x3F) != 4 ||
                ((*in + 2) & 0x3F) != 4 ||
                ((*in + 3) & 0x3F) != 4 ||
                ((*in + 4) & 0x3F) != 4)
            return 0;
        *num = 4;
        *cp = (*in      & 0x3F) << 24 |
             ((*in + 1) & 0x3F) << 18 |
             ((*in + 2) & 0x3F) << 12 |
             ((*in + 3) & 0x3F) << 6 |
             ((*in + 4) & 0x3F);
        return 1;
    }
}


/* "decode" return the code-point
 * string whose encode represent-
 * ation is given in the sequence
 * of bytes a pointer to the begi-
 * nning of which is given in "by-
 * tes" and the number of which is
 * given in "len"m using the memory
 * allocator and rror logger given
 * in "allocs" and "logger", respe-
 * ctively, The encoding of which
 * is derived from the  content of
 * the bytes.  Returns NULL on err-
 * or, the string otherwise */
static struct bdut_str *decode(
                 char *in,
                 int len,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdbf_buffer *buf; /* buffer into
                              * which bytes
                              * are stored as
                              * they are
                              * input */
    struct bdut_str *ret;    /* return str-
                              * ing */
    bdxl_dcdr_fn dcdr;       /* function
                              * used to de-
                              * code "in" */
    int num,                 /* number of */
        cp,                  /* bytes read, */
         cp_len = 0;         /* current code-*/
                             /* pointand num-
                              * ber of code=
                              * points in output
                              * string */

    /* allocate the string to
     * return  */
    ret = (struct bdut_str *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdut_str),
                 logger);
    if (!(ret))
        return NULL;

    /* start by guessing the enc-
     * oding of the bytes the from
     * the content of its XML
     * declaration: the only two
     * mandatory encodings to
     * handle are UTF-8 and UTF-16,
     * and UTF-16 requires a pre-
     * fixed Byte Order Mark (BOM) */

    /* start by initialising the buffer
     * containing the decoded code-point
     * array */
    if (!(buf = bdbf_init(allocs,
                          logger,
                          sizeof(int))))

        return NULL;

     /* pack the first four bytes into
      * a single number and switch on
     * the required XML declaration
     * ("<?[xX]mM[lL]") in that num-
     * ber of bytes */
    switch (in[0] &
           (in[1] >> 8) &
           (in[2] >> 16) &
           (in[3] >> 24)) {

        case 0xFEFF003C: /* BOM + UTF-16be */
            dcdr = (bdxl_dcdr_fn) dcde_utf16be;
            break;
        case 0xFFEF3C00: /* BOM + UTF-16le */
            dcdr = (bdxl_dcdr_fn) dcde_utf16le;
            break;
        case 0xFEFF3C3F: /* BOM + UTF-8   */
        case 0x3C3F584D: /* no BOM + UTF-8
                            * ("<?XM") */
        case 0x3C3F586D: /* no BOM + UTF-8
                            * ("<?Xm") */
        case 0x3C3F784D: /* no BOM + UTF-8
                            * ("<?xM") */
        case 0x3C3F786D: /* no BOM + UTF-8
                            * ("<?xm") */
            dcdr = (bdxl_dcdr_fn) dcde_utf8;
            break;
    }

    /* increment the num-
     * ber of de-coded
     * code-points snd the
     * number of input
     * bytes read, and
     * decrease amount
     * remaining to read */
    while (len > 0 && dcdr(in,
                           &len,
                           &num)) {
        cp_len++;
        len -= num;
        in += num;

        /* and add the code-point
         * to the code-point
         * buffer */
        if (!(bdbf_add(buf,
                       allocs,
                       logger,
                       &cp)))
            return NULL;
    }

    /* now flatten the buffer to
     * have access to to its
     * array, set the string's
     * length from the buffer's
     * length  and return it */
    ret->cps = (int *) bdbf_flttn(buf,
                                  allocs,
                                  logger);
    ret->len = bdbf_len(buf);
    return ret;
}

/* "bdxl_prse_doc" - parse the byte stream
 * given in "in"  and return the XML docu-
 * ment found in therein, using the memory
 * allocator, XML error logger and parsing
 * options given in "allocs", "logger" and
 * "opts", respectively.  Returns NULL on 
 * error */
struct bdxl_doc *bdxl_prse_doc(
        struct bd_allocs *allocs,
        struct bd_logger *logger,
        struct bdut_bytes *in,
        enum bdxl_opts *opts)
{
     struct bdut_str *str,    /* post end-of-line */
                     eol_str; /* processed code- */
                              /* point string */
    struct bdxl_doc *ret;     /* document to return */

    if (!(str = decode(in->bytes,
                       in->len,
                       allocs,
                       logger)))
        return NULL;

    /* create the end-of-line proc-
     * essed string by running the
     * end-of-line processing pars-
     * er */
    if (!(bdpr_run(allocs,
                   logger,
                   eol_prsr,
                   &eol_str,
                   *str)))
        return NULL;

    /* then parse the document
     * essed string by running the
     * end-of-line processing pars-
     * er */
    if (!(bdpr_run(allocs,
                   logger,
                   doc_prsr,
                   &ret,
                   eol_str)))
        return NULL;

    return ret;
}

/* "bdxl_first" - returns the first node (in
 * "tree order") of the node tree rooted in
 * "sub_tree", using the memory allocator and
 * error logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error */
struct bdxl_node *bdxl_first(struct bdxl_node *sub_tree,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger)

{
    struct bd_map_node *map_node; /* map node
                                   * of "node" */

    /* iterate current node to its first child
     * while that node has children */
    while (node->chldrn) {
        map_node = bd_map_first(node->chldrn);
         node = (struct bdxl_node *) map_node->value;
    }

    /* return XML  node in iterator's value
     * (which will have no children) */
    return (struct bdxl_node *) map_node->value;
}

/* "bdxl_next" - returns the next node, in
 * "tree order" , as this term is defined
 * in the DOM spec.) after the node iven in
 * "from", under the sub-tree rooted in
 * "root", returning NULL if "from" is al-
 * ready at the last node  */
struct bdxl_node *bdxl_next(struct bdxl_node *from,
                            struct bdxl_node *root,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    /* if current node is the root
     * of the sub-tree, its already
     * the last node, so indicate
     * there is no next node */
    if (node == root)
        return NULL;

    /* the by here the curr-
     * ent node has no chil-
     * dren, and if there are
     * later siblings the
     * next node is the next
     * one */
    if (node->nxt)
        return node->nxt;

    /* ... and by here, we're
     * at the last of the cur-
     * rent node's children,
     * so the next node is the
     * current node's parent */
    else if (node->prnt)
        return node->prnt;

    /* otherwise, if the current
     * node has children, yjr mext
     * node is the first of them  */
    else if (node->chldrn)
      return (struct bdxl_node *)
          (bd_map_first(node->chldrn)->value);

}

/* "bdxl_run" -  "runs" the XML
 * document given in "doc" aga-
 * uibnai-nst the XML parser co-
 * nfiguraton given againstin"c-
 * nfg", the ewxcogvubituib call-
 * backs of which generates, the
 & the outpyit cvaklyue a gener-
 * ic cast of which is set, on e-
 * xit, in "out", using the mem-
 * ory allocator and error log-
 * ger given in "allocs" and "l-
 * ogger", respectively. Returns
 * zero on error, non-zero othe-
 * rwise */
int bdxl_run(struct bd_allocs *allocs,
             struct bd_logger *logger,
             struct bdxl_cnfg *cnfg,
             struct bdxl_doc *doc,
             void *out)
{
    struct bdxl_node *doc_node;    /* document  */
    struct bd_map_node *pat_node; /* and pattern
                                   * nodes */
    struct bdsl_spcfty mtch;      /* match bet-
                                   * ween current
                                   * node snd cur-
                                   * rent pattern
                                   * selector */
    struct bdxl_pat *pat;         /* current pat-
                                   * term */
    struct bdsl_rt *rt;           /* run-time
                                   * generated
                                   * from sele-
                                   * ctor string */

    /* yuterate through each node in the
     * document and in each pattern in the
     * processor ... */
    for (doc_node = bdxl_first(doc->doc_elem,
                               allocs,
                               logger);
         doc_node;
         doc_node = bdxl_next(doc_node,
                              doc->doc_elem,
                              allocs,
                              logger))
        for(pat_node = bd_map_first(cnfg->pats);
            pat_node;
            pat_node = bd_map_next(pat_node)) {

            /* get ther sttern node's
             * vslue (a selector run-
             * time)  */
            pat = (struct bdxl_pat *)
                      pat_node->value;

            /* parse the pattern's sel-
             * ector text into a run-
             * time structure */
            rt = bdsl_prse(allocs,
                           logger,
                           pat->slctr);
            /* get the current document
             * node's match to the curr-
             * ent pattern's selector
             * (where a match is indic-
             * ated by "mtch" being set
             * to non-zero) */
            if (!(bdsl_exec(allocs,
                            logger,
                            rt,
                            doc_node,
                            &mtch)))
                return 0;

            /* and call this pattern's
             * "recognition call-back"
             * if there is a match
             * (which will build "out"
             * as a side-effect) */
            if (&mtch)
                if (!(pat->rcb(out,
                               doc_node,
                               allocs,
                               logger)))
                return 0;
        }
}

/* "bdxl_init_cnfg" - initialises, using the
 * memory allocator and error logger given in
 * the first and second parameters, respect-
 *ively, an XNML parsomf configuration hav-
 * ing (initially) no known prrocesing inst-
 * rucions or recognittion patterns.  Retu-
 * urns NULL on error */
struct bdxl_cnfg *bdxl_init_cnfg(
                 struct bd_allocs allocs*,
                 struct bd_logger *logger){
{
    struct bdxl_cnfg *ret; /* return
                            * value */

    /* allocate a
     configuration */
    ret = (struct bdxl_cnfg *)
        bd_allocs(allocs,
                  sizeof(struct
                         bdxl_cnfg),
                  logger);
    if (!(ret))
        return NULL;

    /* and, once allocation is
     * successul, set to fields
     * and return  */
    ret->pis  = NULL;
    ret->pats = NULL;
    return ret;
}

/* "bdxl_doc_init" - initialises and returns
 * an empty XML document, using the memory
 * allocator and error logger given in "allocs"
 * first and "logger", respectively.  Returns
 * NULL on error */
struct bdxl_doc *bdxl_doc_init(
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    struct bdxl_doc *ret; /* return value */

    ret = (struct bdxl_doc *)
        bd_alloc(allocs,
                 sizeof(struct
                        bdxl_doc),
                 logger);
    if (!(ret))
        return NULL;

    ret->doc_elem = NULL;
    ret->doc_elem = NULL;
}

/* "bdxl_set_opts" - returns the XML par-
 * sing options having the white-space
 * processing type, proxy host and proxy
 * port given in the third to"prt" "dflt",
 * "prxy" and "prt", respectivly, using
 * the memory allocator anndd error log-
 * ger given in "allocs" and "logger"
 * ters, respectively.  Returns NULL on
 * error */
struct bdxl_opts *bdxl_set_opts(
                    struct bd_allocs *allocs,
                    struct bd_logger *logger,
                    enum bdxl_dflt_types dflt,
                    char *prxy,
                    unsigned short prt)
{
    struct bdxl_opts *ret; /* return
                            * value */

    ret = (struct bdxl_opts *)
         bd_alloc(allocs,
                  sizeof(struct
                         bdxl_opts)
                  logger);
    if (!(ret))
        return NULL;

    /* of allocation successful,
     * set fields */
    ret->prxy_prt = prt;
    ret->prxy_hst = prxy;
    ret->ws = dflt;
}

/* "get_prnt" - returns the
 * parent of the node in "nde"
 * which, if NULL, will be
 * that node's owning docum-
 * ent's document element */
static struct bdxl_node *get_prnt(
                  struct bdxl_node *nde)
}
    if (!(nde->prnt))
        return nde->ownr->doc_elem->nde;
    else
        return nde->prnt; 
}
/* "bdxl_add_nde_utf" - ret-
 * urns a node having the
 * type given in "type", par-
 * ent and child order of
 * "nde" and having the "na-
 * me" and "value" (as these
 * last two are defined in
 * the DOM spec) the UTF-8 
 * encoding of which is giv-
 * en in "nme" and "val",
 * respectively, added to the
 * doc- ument given in "doc",
 * using the memory allocator
 * "allocs"and "logger", resp-
 * ectively.  Returns NULL on
 * error */
struct bdxl_node *bdxl_add_nde_utf(
                 enum bdxl_node_types type,
                 struct bdxl_node *nde,
                 char *nme,
                 char *val,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bd_map_node *ins_node, /* insertion */
                       *chldrn;   /* node and
                                   * parent's
                                   * children */ 
    struct bdxl_node *nw;          /* added node */

    /* allocate the new node and
     * set its type, name, value,
     * child index and owning
     * document */
    rnw = (struct bdxl_node *)
          bd_alloc(allocs,
                   sizeof(struct
                          bdxl_node),
                   logger);
    if (!(nw))
        return NULL;

    nw->type = type;
    nw->nme  = nme;
    nw->val  = val;
    nw->ordr = nde->ordr;
    nw->ownr = doc;

    /* unsert the new node,
     * by index, into
     * those children */
    if (!(bd_map_insert(&chldrn,
                        ins_node,
                        nw->ordr,
                        (bd_map_cmp_fn)
                             cmp_ordrs,
                        nw)))
        return NULL;

    /* connect the new node to
     * its siblings (idf exist-
     * ing) */
    if (nw->nxt)
        nw->nxt->prv = nw;
    if (nw->prv)
        nw->prv->nxt = nw;

    /* then, iterate through
     * node's remaining next
     * siblings, updating the
     * ohild order of each to
     * account for new node */
    for (nxt = nde->nxt;
         nxt;
         nxt = nxt->nxt) {
   
        nxt->ordr++;
    }
}
/* "bdxl_add_nde" - returns
 * the node having the type gi-
 * ven in "type" and the parent
 * and child order of "nde",
 * "nde" and having the "name"
 * and "value" (as these last
 * two are defined in the DOM
 * spec) given in "nme" and
 * "val", respectively, add-
 * ed to the document given in
 * "doc", using the memory all-
 * ocator and error logger given
 * in "allocs" and "logger", re-
 * spectively.  Returns NULL
 * on error */
static struct bdxl_node *bdxl_add_nde(
                 enum bdxl_node_types type,
                 struct bdxl_node *nde,
                 struct bdut_str *nme,
                 struct bdut_str *val,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *nme_utf, /* UTF-8 */
         *val_utf; /* versions
                    * of "nme"
                    * and "val" */

    /* transform "nme" and
     * "val" to UTF-8,
     * then call UTF-8
     * string equivalent
     * function */
    if (!(bdut_to_utf(nme_utf,
                      nme,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(val_utf,
                      val,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_nde_utf(type,
                            nde,
                            nme_utf,
                            val_utf,
                            doc,
                            allocs,
                            logger);
}

/* "bdxl_add_elem_utf" - returns
 * the element, having the node
 * (containing its parent and
 * child links), attributes, tag,
 * namespace name, local name and
 * prefix spec) given in "nde",
 * "tag", "attrs" "ns", and "pr-
 * fx", respectively the UTF-8
 * encoding of which are, added
 * to the document given in "doc",
 * using the memory allocator a
 * nd error logger given in "al-
 * locs" and "logger", respecti-
 * vely.  Returns NULL on error */
struct bdxl_elem *bdxl_add_elem_utf(
                  struct bdxl_node *nde,
                  struct bd_map_node *attrs,
                  char *tag,
                  char *ns,
                  char *lcl,
                  char *prfx,
                  struct bdxl_doc *doc,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdxl_elem *ret; /* return
                            * value */
    struct bdxl_node *nde,  /* new node */
                    *nw;   /* and node of */
                           /* element */
                           
    /* allocate return value */
    ret = (struct bdxl_elem *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_elem),
                     logger);
    if (!(ret))
        return NULL;

    /* add the element to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_elem,
                   nde,
                   tag,
                   NULL,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde   = *nw;
    ret->attrs = attrs;
    ret->tag   = tag;
    ret->ns    = ns;
    ret->lcl   = lcl;

   if (!(ret->nde.trgt.obj =
           bddm_set_elem_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_elem" - returns an
* element having the node (co-
 * ntaining its parent and child
 * links), given in "nde" attri-
 * butes gibven in "attrs", and 
 * he tag, name, name-space URI,
 * local name and prefix the
 * UTF-8 encoding of which are
 * given in in "tag", "ns", "lcl"
 * and "prfx", respectively, ad-
 * ded to the document given in
 * "doc", using the memory allo-
 * cator and error logger given
 * in the "allocs" and "logger",
 * respectively. Returns NULL on
 * error */
struct bdxl_elem *bdxl_add_elem(
                  struct bdxl_node *nde,
                  struct bd_map_node *attrs,
                  struct bdut_str *tag,
                  struct bdut_str *ns,
                  struct bdut_str *lcl,
                  struct bdut_str *prfx,
                  struct bdxl_doc *doc,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    char *tag_utf,  /* UTF-8 verions */
         *ns_utf,   /* of "tag", */
         *lcl_utf,  /* "ns", "lcl" */
         *prfx_utf; /* and "prfx" */
                             

    /* transform "tag", 
     * "ns", "lcl" and
     * "prfx" to UTF-8,
     * then call UTF-8
     * equivalent
     * function */
    if (!(bdut_to_utf(tag_utf,
                      tag,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(ns_utf,
                      ns,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(lcl_utf,
                      lcl,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(prfx_utf,
                      ptfx,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_elem_utf(nde,
                             attrs,
                             tag_utf,
                             ns_utf,
                             lcl_utf,
                             prfx_utf,
                             allocs,
                             logger);
}

/* "bdxl_add_cmt_utf" - returns the
 * comment, having the node (co-
 * ntaining its parent and child
 * links) given in the first par-
 * ameter and content the UTF-8
 * encoding of which is given in
 * the second parameter added to
 * the document given in the th-
 * ird parameter, using the mem-
 * ory allocator and error logg-
 * er given in the fourth and
 * fifth parameters, respectiv-
 * ely. Returns NULL on error */
struct bdxl_cmt *bdxl_add_cmt_utf(
                 char *nde,
                 char *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdxl_cmt *ret; /* return
                            * value */
    struct bdxl_node *nde,  /* new node */
                    *nw;   /* and node of */
                           /* comment */
                           
    /* allocate return value */
    ret = (struct bdxl_cmt *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_cmt),
                     logger);
    if (!(ret))
        return NULL;

    /* add the comment to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_cmt,
                   nde,
                   "#comment",
                   cnt,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->dta = cnt;
   if (!(ret->nde.trgt.obj =
           bddm_set_cmt_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_cmt" - returns
 * the comment, having the node
 * (containing its parent and
 * child links) given in the fi-
 * rst parameter and content the
 * UTF-8 encoding of which is
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and er-
 * ror logger given in the the
 * fourth and fifth parameters,
 * respectively. Returns NULL on
 * error */
struct bdxl_cmt *bdxl_add_cmt(
                 struct bdxl_node *nde,
                 struct bdut_str *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *cnt_utf; /* UTF-8 verion */
                   /* of "cnt", */

    /* transform "cnt" to
     * UTF-8,then call
     * UTF-8 equivalent
     * function */
    if (!(bdut_to_utf(cmt_utf,
                      cmt,
                      allocs,
                      logger)))
        return NULL;

    return bdxl_add_cmt_utf(nde,
                            cnt_utf,
                            doc,
                            allocs,
                            logger);
}

/* "bdxl_add_doc_type" - returns
 * the document type having the
 * node (containing its parent
 * and child links) given in the
 * first parameter and the name,
 * public ID and system ID given
 * in the second to fourth para-
 * meters, respectively, added
 * to the document given in the
 * fifth parameter, using the
 * memory allocator and error
 * logger given in the the sixth
 * and seventh parameters, resp-
 * ectively. Returns NULL on
 * error */
struct bdxl_doc_type *bdxl_add_doc_type(
                 struct bdxl_node *nde,
                 struct bdut_str *nme,
                 struct bdut_str *pub,
                 struct bdut_str *sys,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *nme_utf, /* UTF-8 version */
         *pub_utf, /* of "nme", "pub" */
         *sys_utf  /* and "sys" */

    /* transform "nme",
     * "pub" and "sys
     * to UTF-8, then
     * call UTF-8 equi-
     * valent function */

    if (!(bdut_to_utf(nme_utf,
                      nme,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(pub_utf,
                      pub,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(sys_utf,
                      sys,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_doc_type_utf(nde,
                                 nme_utf,
                                 pub_utf,
                                 sys_utf,
                                 doc,
                                 allocs,
                                 logger);
}

/* "bdxl_add_doc_type_utf" - ret-
 * urns the document type having
 * the node (containing its par-
 * ent and child links) given in
 * the first parameter and the
 * name, public ID and system ID
 * the UTF-8 encoding of which
 * is given in the second to fo-
 * urth parameters, respectively,
 * added to the document given
 * in the fifth parameter, using
 * the memory allocator and er-
 * ror logger given in the the
 * sixth and seventh parameters,
 * respectively. Returns NULL on
 * error */
struct bdxl_doc_type *bdxl_add_doc_type_utf(
                 struct bdxl_node *nde,
                 char *nme,
                 char *pub,
                 char *sys,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *;logger)
{
    struct bdxl_doc_type *ret; /* return
                                * value */
    struct bdxl_node *nde,      /* new node */
                    *nw;       /* and node */
                               /* of document
		                        * type */
                           
    /* allocate return value */
    ret = (struct bdxl_doc_type *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_doc_type),
                     logger);
    if (!(ret))
        return NULL;

    /* add the document type
     * to the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_doc_type,
                   nde,
                   nme_utf,
                   NULL,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->nme = nme;
    ret->pub = pub;
    ret->sys = sys;
   if (!(ret->nde.trgt.obj =
           bddm_set_doc_type_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_txt" - returns the
 * text node, having the node
 * (containing its parent and
 * child links) given in the
 * first parameter and content
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and err-
 * or logger given in the the fo-
 * urth and fifth parameters, re-
 * spectively. Returns NULL on
 * error */
struct bdxl_txt *bdxl_add_txt(
                 struct bdxl_node *nde,
                 struct bdut_str *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    char *cnt_utf; /* UTF-8 version */
                   /* of "cnt" */

    /* transform "cnt"
     * to UTF-8, then
     * call UTF-8 equi-
     * valent function */

    if (!(bdut_to_utf(cnt_utf,
                      cnt,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_txt(nde,
                        cnt_utf,
                        doc,
                        allocs,
                        logger);
}

/* "bdxl_add_txt_utf" - returns
 * the text node, having the
 * node (containing its parent
 * and child links) given in the
 * first parameter and content
 * the UTF-8 encoding of which is
 * given in the second parameter
 * added to the document given
 * in the third parameter, using
 * the memory allocator and err-
 * or logger given in the the fo-
 * urth and fifth parameters, re-
 * spectively. Returns NULL on
 * error */
struct bdxl_txt *bdxl_add_txt_utf(
                 struct bdxl_node *nde,
                 char *cnt,
                 struct bdxl_doc *doc,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
    struct bdxl_txt *ret; /* return
                           * value */
    struct bdxl_node *nde, /* new node */
                    *nw;  /* and node */
                          /* of text */
                           
    /* allocate return value */
    ret = (struct bdxl_txt *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_txt),
                     logger);
    if (!(ret))
        return NULL;

    /* add the text node
     * to the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_txt,
                   nde,
                   "#text",
                   cnt_utf,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde = *nw;
    ret->cnt = cnt;
   if (!(ret->nde.trgt.obj =
           bddm_set_txt_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_add_pi" - returns the
 * processing instruction, ha-
 * ving the node (containing
 * its parent and child links)
 * given in the first parameter
 * and the target and data gi-
 * ven in the second and third
 * parameters, respectively,
 * added to the document given
 * in the fourth parameter, us-
 * ing the memory allocator and
 * error logger given in the
 * the fifth and sixth paramet-
 * ers, respectively. Returns
 * NULL on error */
struct bdxl_pi *bxl_add_pi(
                struct bdxl_node *nde,
                struct bdut_str *trgt,
                struct bdut_str *dta,
                struct bdxl_doc *doc,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    char *trgt_utf, /* UTF-8 versions */
         *dta_utf;  /* of "trgt" and
                     * "dta" */

    /* transform "trgt"
     * and "dta" to UTF-8,
     * then call UTF-8
     * equivalent fun-
     * ction */

    if (!(bdut_to_utf(trgt_utf,
                      trgt,
                      allocs,
                      logger)))
        return NULL;
    if (!(bdut_to_utf(dta_utf,
                      dta,
                      allocs,
                      logger)))
        return NULL;
    return bdxl_add_pi(nde,
                       trgt_utf,
                       dta_utf,
                       doc,
                       allocs,
                       logger);
}

/* "bdxl_add_pi_utf" - returns
 * the processing instruction,
 * having the node (containing
 * its parent and child links)
 * given in the first parameter
 * and the target and data the
 * UTF-8 encodings of which are
 * given in the second and th-
 * ird parameters, respectively,
 * added to the document given
 * in the fourth parameter, us-
 * ing the memory allocator and
 * error logger given in the
 * the fifth and sixth paramet-
 * ers, respectively. Returns
 * NULL on error */
struct bdxl_pi *bxl_add_pi_utf(
                struct bdxl_node *nde,
                char *trgt,
                char *dta,
                struct bdxl_doc *doc,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    struct bdxl_pi *ret;  /* return
                           * value */
    struct bdxl_node *nde, /* new node */
                    *nw;  /* and node
                           * of process-
                           * ing instr-
                           * uction*/

    /* allocate return value */
    ret = (struct bdxl_pi *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdxl_pi),
                     logger);
    if (!(ret))
        return NULL;

    /* add the prcessing
     * instruction to
     * the document */
    if (!(nw = bdxl_add_nde_utf(
                   bdxl_node_pi,
                   nde,
                   trgt_utf,
                   dta_utf,
                   doc,
                   allocs,
                   logger)))
        return NULL;

    /* set the structure's
     * fields and its lan-
     * guage object's prop-
     * erties and return */
    ret->nde  = *nw;
    ret->trgt = trgt;
    ret->dta  = trgt;
   if (!(ret->nde.trgt.obj =
           bddm_set_pi_prps(
                     ret,
                     allocs,
                     logger)))
       return NULL;
   return ret;
}

/* "bdxl_rplce" - sets the second
 * parameter to the "replacement
 * text" (as this is defined in
 * the XML spec.) of the text
 * given in the first parameter,
 * using the replacer, proxy in-
 * fo., (used in resolving exte-
 * rnal  entities), set of curr-
 * ent entites, memory allocator
 * and error logger given the
 * third to seventh parrameters,
 * respectively.  Returns zero
 * on error, non-zero otherwise */
int bdxl_rplce(struct bdut_str *,
               struct bdut_str **,
               struct bdxl_rplcr *,
               struct bdxl_prxy *,
               struct bd_map_node *,
               struct bd_allocs *,
               struct bd_logger *)
{
    return 0;
}

/* "bdxl_get_attr" - returns the
 * value (or NULL on error) of the
 * attribute of the node given in
 * "nde", the UTF-8 encoding of
 * which is given in "nme", using
 * the memory allocator and error
 * logger given in "allocs" and
 * "logger", respectively*/
struct bdut_str *bdxl_get_attr(
                   char *nme,
                   struct bdxl_node *nde,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdut_str *nme_str;     /* UTF-8-decoding
                                   * of "nme" */
    struct bd_map_node *map_node; /* node of found
                                   * attribute */

    /* get "nme_str" from "nme" */
    if (!(bdut_from_utf8(&nme_str,
                         nme,
                         allocs,
                         logger)))
        return NULL;

    /* look-up an attribute of that
     * name in th node's set of
     * attributes */
    if (!(map_node = bd_map_find(nde->attrs,
                                 nme_str,
                                 (bd_map_cmp_fn)
                                     bdut_strcmp)))
        return NULL;

    /* and return its co-erced value */
    return (struct bdut_str *) map_node->value;
}

/* "bdxl_first" - returns the first node  (in
 * "tree order", as this term is defined
 * in the DOM spec.) of the node tree rooted
 * in the node given in "sub_" */
struct bdxl_node *bdxl_first(struct bdxl_node *sub)
{
    struct bdxl_node *curr; = sub; /* current
                                    * node */ 

    /* return the "first-most" node in "sub" */
    while (curr->chldrn)
        curr = bd_map_first(curr->chldrn);
    return curr;
}

/* "bdxl_next" - returns the next node, in
 * "tree order", (as this term is defined
 * in the DOM spec.) after the node given
 * in "node" under the sub- tree rooted in
 * "sub_tree" */
struct bdxl_node *bdxl_next(struct bdxl_node *node,
                            struct bdxl_node *sub)
{
    /* if already at "sub", am ite-
     * ration is at the end. so ind-
     * icate so */
    if (node == sub)
		return NULL;

    /* or, if "node" has children,
     * return the first */
    else if (node->chldrn)
        return bd_map_first(ode->chldrn);

    /* or, if "node" is not the
     * last of among its siblings,
     * return uit's next */
    else if (node->nxt)
        return monode->nxt;

    /* er to get here, must be at
     * the mdemnd og the siblings.
     * so iterate the "node"'
     * parenra*/
    else if (node->prnt)
        return node->prnt;
}
