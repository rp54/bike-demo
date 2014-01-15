/**************************************************************
*
* BikeDemo - Exercise Bike Demonstration Sc-
* enery
*
* xml.h - data types and function proto-types
* related to the Extensible Markup Language
* (XML) parser, used for parsing data in an
* XML format.
*
* When certain configurations of elements and
* attribute names are seen on the input, the
* parser executes the pattern's associated
* "recognition call-back" routine, such that
* meaningful processing may be accomplished
* through the execution of these call-backs
* in the sequence dictated,h by the arrangement
* of elements and/or attribute names found
* on the input.
*
* Copyright (C) 2007-2012, Russell Potter,
* All rights reserved
*
***************************************************************/

#ifndef __BD_XML
#define __BD_XML

#ifdef __cplusplus
extern "C" {
#endif

struct bdxl_ent;

#include <bikedemo/util.h>
#include <bikedemo/slctr.h>
#include <bikedemo/lang.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/lex.h>
#include <bikedemo/dom.h>
#include <bikedemo/evt.h>
#include <bikedemo/grammar.h>


/* "bdxl_ws_types" enumeration - whether to
 * ignore or preserve or use default proess-
 * ing of input stream */
enum bdxl_ws_types {

    bdxl_ws_prsrve, /* preserve white-apace */
    bdxl_ws_dflt    /* default procssing */
};

/* "bdxl_ws_dflt_types" enumeration - whether
 * the default behaviour of an XML input str-
 * eam is to ignore or preserve white-space */
enum bdxl_ws_dflt_types {

    bdxl_ws_dflt_prsrve, /* preserve white-apace */
    bdxl_ws_dflt_ignre   /* ignore white-apace */
};

/* "bdxl_ent_locs" enumeration - the
 * possible location types of an
 * entity: either "internal" (defi-
 * nition appears in-line with the
 * entity) or "external" (refers to
 * an IRI containing the entity
 * definition)
 *  */
enum bdxl_ent_locs {

    bdxl_ent_intnl,
    bdxl_emt_extrnl
};

/* "bdxl_cnstrnt_types" enumeration - types of
 * constraints to apply in a Documeht Type
 * Definition (DTD), ehich may be element type.
 * srreinute list and notation type */
enum bdxl_cnstrnt_types {

    bdxl_ws_cntrnt_elem,  /* element constraint */
    bdxl_ws_cntrnt_attrs, /* attribute constraint */
    bdxl_ws_cntrnt_notn   /* notation constraint */
};

/* "bdxl_attr_dflt_types" enumeration - types of
 * DTD default attribute types, which may be either
 * required (mandatory with any value), impied
 * (presence optional) or fixed (mandatory with
 * fixed value)  */  
enum bdxl_dtd_attr_dflt_types {

    bdxl_dtd_attr_dflt_rqrd,  /* presence mandatory,
                               * no fixed value */
    bdxl_dtd_attr_dflt_impld, /* presence optional */
    bdxl_dtd_attr_dflt_fxd    /* presence mandatory,
                               * fixed value */
};

/* "bdxl_attr_types" enumeration - types of
 * DTD attribute types, which may be an ID
 * (unique across eleemnt type), IDREF (refers
 * to an ID, IDREFS (space-separated list of ID
 * references), ENTITY (is an entity name) ENTITIES
 * (space-separated list of entity names),
 * NMTOKEN), NMTOKENS (space-separated list of
 * NMTOKENs) */ 
enum bdxl_attr_types {

    bdxl_attr_attr_enum,  /* one of list of
                           * enumerated
                           * values */
    bdxl_attr_attr_cdta,  /*  no fixed format */
    bdxl_attr_attr_id,    /* unique across element
                           * type */
    bdxl_attr_attr_idrf,  /* refers to an ID */
    bdxl_attr_attr_idrfs, /* list of ID references */
    bdxl_attr_attr_ent,   /* entity name */
    bdxl_attr_attr_ents,  /* list of entity names */
    bdxl_attr_attr_nmtk,  /* is an NMTOKEN */
    bdxl_attr_attr_nmtks  /* list of NMTOKENs */
};

/* "bdxl_node_types" enumeration - types of
 * "nodes", which are programmatic structures
 * encompassing elements, attributes, comments
 * and processing instructions */
enum bdxl_node_types {

    bdxl_node_elem = 1, /* element */
    bdxl_node_attr,     /* attribute */
    bdxl_node_txt,      /* free-form text */
    bdxl_node_cdta,     /* "CDATA" section */
    bdxl_node_ent_rf,   /* entity reference */
    bdxl_node_ent_dcl,  /* entity declaration */
    bdxl_node_pi,       /* processing instruction */
    bdxl_node_cmt,      /* comment */
    bdxl_node_doc,      /* document */
    bdxl_node_doc_type, /* document type*/
    bdxl_node_doc_frg,  /* document fragment */
    bdxl_node_doc_ntn   /* notation */
};

/* "bdxl_ent_types" enumeration - possible types of
 * XML "entities", which may be "general" (may app-
 * ear anywhere in a document), "parameter" (may
 * appear only in a DTD) or "character" (is repl-
 * aced by the decimal or hexadecimal numeric value
 * of the character) */
enum bdxl_ent_types {

    bdxl_ent_type_gnrl,
    bdxl_ent_type_prm,
    bdxl_ent_type_chr
};

/* "bdxl_toks" enumeration - possible tokens
 * returned by the "general-purpose" lexical
 * analyser, used to load rules for use by
 * other XML parsers */
enum bdxl_toks {

    bdxl_tok_ws,   /* white-space */
    bdxl_tok_eq,   /* equals sign
                    * ("=") */
    bdxl_tok_cmt,  /* comment (deli-
                    * mited by "<!--"
                    * and "-->")*/
    bdxl_tok_cdta, /* character data
                    * block */
    bdxl_tok_txt,  /* general text */
    bdxl_tok_nme,  /* name */
    bdxl_tok_nmtk, /* name-token */
    bdxl_tok_str,  /* string ("\"abc\""
                    *           or
                    *        'abc')*/
    bdxl_tok_pi,   /* processing
                    * instruction
                    * ("<?abc?>") */
    bdxl_tok_pub,  /* public ID */
    bdxl_toks_num, /* number of tokens */
};

/* "bdxl_rplcr_toks" enumeration - possible tokens
 * returned by the "replacement text" lexical anal-
 * yser, reads entity declarations and references
 * and substitutes each entity refernce with its
 * replacement text) */
enum bdxl_rplcr_toks {

    bdxl_rplcr_tok_pe_ref =    /* parameter */
            bdgm_sym_nres,     /* entity ref
                                * ("%abc;") */
    bdxl_rplcr_tok_ge_ref,     /* general entity
                                * ref ("&abc;") */
    bdxl_rplcr_tok_chr_ref,    /*char. entity
                                * ref. ("&#NN;" or
                                       ("&#xNN;")*/
    bdxl_rplcr_tok_nent,       /* non-replacement-
                                * related charac-
                                * ter ("[^<%&]") */
    bdxl_rplcr_tok_kwd_public, /* "PUBLIC" keyword */
    bdxl_rplcr_tok_kwd_system, /* "SYSTEM" keyword */
    bdxl_rplcr_tok_kwd_entity, /* "<!ENTITY" keyword */
    bdxl_rplcr_tok_kwd_ndata,  /* "<!NDATA" keyword */
    bdxl_rplcr_toks_num        /* number of replacer
                                * tokens */
};

/* "bdxl_elem_toks" enumeration - possible tokens
 * returned by the parser that generates an elem-
 * ent structure from a stream containing an
 * XML-formatted element.  These tokens are the
 * its start and end tags, the "empty element"
 * symbol snf uthe eelement content, as returned
 * by the content parser*/
enum bdxl_elem_toks {

    bdxl_elem_tok_strt_tg = /* start
                             * tag */
            bdgm_sym_nres,
    bdxl_elem_tok_end_tg,   /* end tag */
    bdxl_elem_tok_empty,    /* empty
                             * element */
    bdxl_elem_tok_cnt,      /* element
                             * content */
    bdxl_elem_toks_num      /* number of
                             * tokens */
};

/* "bdxl_dtd_toks" enumeration - possible tokens
 * returned by the "Document Type Declaration"
 * (DTD) lexical analyser, which is capable of
 * reading element, attribute list and notation
 * declrations, and white-space */
enum bdxl_dtd_toks {

	bdxl_dtd_tok_kwd_elem =    /* keyword "<!ELEMENT" */
            bdgm_sym_nres,
    bdxl_dtd_tok_cmdl,         /* content model
                                * character */
    bdxl_dtd_tok_kwd_rqrd,     /* keyword "#REQUIRED" */
    bdxl_dtd_tok_kwd_impld,    /* keyword "#IMPLIED" */
    bdxl_dtd_tok_kwd_fxd,      /* keyword "#FIXED" */
    bdxl_dtd_tok_kwd_attrlist, /* keyword "<!ATTRLIST" */
    bdxl_dtd_tok_kwd_ntn,      /* keyword "<!NOTATION" */
    bdxl_dtd_tok_kwd_cdata,    /* keyword "CDATA" */
    bdxl_dtd_tok_kwd_id,       /* keyword "ID" */
    bdxl_dtd_tok_kwd_idrf,     /* keyword "IDREF" */
    bdxl_dtd_tok_kwd_idrfs,    /* keyword "FDREFS" */
    bdxl_dtd_tok_kwd_ent,      /* keyword "ENTITY" */
    bdxl_dtd_tok_kwd_ents,     /* keyword "ENTITES" */
    bdxl_dtd_tok_gt,           /* end of declaration (">") */
    bdxl_dtd_toks_num          /* number of DTD tokens */
};

/* "bdxl_cmdl_toks" enumeration - possible tokens
 * returned by the "DTD content model lexical ana-
 * lyser, which is capable of reading commas,
 * left and right parentheses, pipe symbols,
 * white-soace, "mixed content" indicator and
 * lement names */
enum bdxl_cmdl_toks {

	bdxl_cmdl_tok_lpr =       /* left parenthesis  */
            bdxl_toks_num,    /* ("(") */
    bdxl_cmdl_tok_rpr,        /* right parenthesis
                              /* (")") */
    bdxl_cmdl_tok_cma,        /* comma (",") */
    bdxl_cmdl_tok_bar,        /* pipe ("|") */
    bdxl_cmdl_tok_ast,        /* asterisk ("*") */
    bdxl_cmdl_tok_pls,        /* plus ("+") */
    bdxl_cmdl_tok_qstn,       /* question mark
                               * ("?") */
    bdxl_cmdl_tok_mxd,        /* mixed content
                               * indicator (")*") */
    bdxl_cmdl_tok_kwd_pcdata, /* keyword "#PCDATA" */
    bdxl_cmdl_tok_kwd_any,    /* keyword "ANY" */
    bdxl_cmdl_tok_kwd_emty,   /* keyword "EMPTY" */
    bdxl_cmdl_toks_num        /* number of content
                               * model tokens */
};

/* "bdxl_doc_toks" enumeration - the possible
 * tokens types that may be returned bythe (top-
 * level) XML document lexical analyser */
enum bdxl_doc_toks {

    bdxl_doc_tok_xml_dcl =       /* XML declaration */
           bdgm_sym_nres,        /* ("<?[xX][mM][lL]") */
    bdxl_doc_tok_end_pi,         /* end of
                                  * processing
                                  * instruction ("?>") */
    bdxl_doc_tok_stg,            /* first char. of
                                  * element start tag
                                  * ("<[^!?-]") */
    bdxl_doc_tok_gt,             /* greater-than sign
                                  * (">") */
    bdxl_doc_tok_int_sb,         /* DTD internal sub-
                                  * set character
                                  * (["[^>]") */
    bdxl_doc_tok_cnt,            /* remainder of docu-
                                  * ment
                                  * content */
    bdxl_doc_tok_eq,             /* equal sign
                                  * ("=") */
    bdxl_doc_tok_lsq,            /* left square bracket
                                  * ("[") */
    bdxl_doc_tok_rsq,            /* right square bracket
                                  * ("]") */
    bdxl_doc_tok_kwd_doctype,    /* keyword "<!DOCTYPE" */
    bdxl_doc_tok_kwd_public,     /* keyword "PUBLIC" */
    bdxl_doc_tok_kwd_system,     /* keyword "SYSTEM" */
    bdxl_doc_tok_kwd_standalone, /* keyword "standalone" */
    bdxl_doc_tok_kwd_version,    /* keyword "version" */
    bdxl_doc_tok_kwd_encoding,   /* keyword "encoding" */
    bdxl_doc_toks_num            /* number of document
                                  * tokens */
};

/* "bdxl_extsb_toks" enumeration - the possible
 * token types that may be returned by the DTD
 * "external sub-set" lexical analyser, which
 * accepts all DTD tokens, as well as conditi-
 * onal sections */
enum bdxl_extsb_toks {

    bdxl_extsb_tok_kwd_ignore  = /* keyword "<![INCLUDE[" */
        bdxl_dtd_toks_num,
    bdxl_extsb_tok_kwd_include,  /* keyword "<![IGNORE[" */
    bdxl_extsb_tok_end,          /* end cond. sect. */
                                 /*  ("]]>") */
    bdxl_extsb_toks_num,         /* numer if external
                                  * DTD sub-set tokens */
};

/* "bdxl_bdxl_cndsct_types" enumeration - the
 * possible types of a conditional section:
 * namely, either to include or ignore */
enum bdxl_cndsct_types {

    bdxl_cndsct_type_ignore, /* ignores */
    bdxl_cndsct_type_include /* includes */
};

/* "bdxl_eol_toks" enumeration - the possible
 * tokens types that may be returned by an end-of-
 * line processing's lexical analyser */
enum bdxl_eol_toks {

    bdxl_eol_tok_crlf =      /* carriage-return */
              bdgm_sym_nres, /* ("xA") */
    bdxl_eol_tok_cr,         /* carriage-return ('\xA') */
   bdxl_eol_tok_lf,          /* line-feed ('\xD') */
    bdxl_eol_tok_othr,       /* any other char-
                              * acter (^\xD\xA]") */
    bdxl_eol_toks_num        /* number of tokens */

};

/* "bdxl_doc_nts" enumeration - the non-
 * terminals of the XML document gramm-
 * ar, which generates a document from
 * XML-formatted input.  It basically con-
 * sists of s document prologue and a and
 * opaque document content, ewhich is sc-
 * anned for replacement text and parsed
  *with the "element" parser */
enum bdxl_doc_nts {

    bdxl_0oc_nt_doc =
          bdxl_doc_toks_num + 1,

    bdxl_doc_nt_prlge,
    bdxl_doc_nt_doc,
    bdxl_doc_nt_elem,
    bdxl_doc_nt_misc,
    bdxl_doc_nt_misc_list,
    bdxl_doc_nt_xml_dcl,
    bdxl_doc_nt_doctype_decl,
    bdxl_doc_nt_extrnl_id,
    bdxl_doc_nt_int_sb,
    bdxl_doc_nt_int_sb_chrs,
    bdxl_doc_nt_int_sb_end,
    bdxl_doc_nt_sd_decl,
    bdxl_doc_nt_enc_decl,
    bdxl_doc_nt_vrsn,
    bdxl_doc_nt_vers_num,
    bdxl_doc_nt_eq,
    bdxl_doc_nt_cnt,
    bdxl_doc_nt_end_cnt,
    bdxl_doc_num_syms
};

/* "bdxl_cnt_nts" enumeration - the non-
 * terminals of ythe XML "document con-
 * tent" grammar, which generates a do-
 * cument element of a document from
 * an XML-formatted element, as it appe-
 * ars on an input source after process-
 * ing for replacement text */
enum bdxl_cnt_nts {

    bdxl_cnt_nt_elem =
          bdxl_toks_num + 1,
    bdxl_cnt_nt_misc,
    bdxl_cnt_nt_misc_list,
    bdxl_cnt_nt_misc_num_syms
};

/* "bdxl_rplr_nts" enumeration - the non-
 * terminals of the XML "replacement text"
 * grammar */
enum bdxl_rplcr_nts {

    bdxl_rplcr_nt_strm =
        bdxl_toks_num + 1,
    bdxl_rplcr_nt_ntn,
    bdxl_rplcr_nt_pe_ref,
    bdxl_rplcr_nt_ge_ref,
    bdxl_rplcr_nt_chr_ref,
    bdxl_rplcr_nt_ge_intn_dcl,
    bdxl_rplcr_nt_ge_extn_dcl,
    bdxl_rplcr_nt_pe_intn_dcl,
    bdxl_rplcr_nt_pe_extn_dcl,
    bdxl_rplcr_num_syms
};

/* "bdxl_eol_nts" enumeration - the non-
 * terminals of the XML "end-of-line
 * processing" grammar */
enum bdxl_eol_nts {

    bdxl_eol_nt_chr =
          bdxl_eol_toks_num + 1,
    bdxl_eol_nt_chrs,
    bdxl_eol_nt_end,
    bdxl_eol_num_syms
};

/* "bdxl_cmdl_nts" enumeration - the non-
 * terminals of the XML element content
 * model grammar */
enum bdxl_cmdl_nts {

    bdxl_cmdl_nt_cp =
          bdgm_sym_nres,
    bdxl_cmdl_nt_chce,
    bdxl_cmdl_nt_seq,
    bdxl_cmdl_nt_mxd,
    bdxl_cmdl_num_syms

};

/* "bdxl_dtd_nts" enumeration - the non-
 * terminals of the Document Type Declar-
 * ation grammar */
enum bdxl_dtd_nts {

    bdxl_dtd_nt_attr_list =
          bdxl_dtd_toks_num + 1,
    bdxl_dtd_nt_elem,
    bdxl_dtd_nt_elem_cnt,
    bdxl_dtd_nt_notn,
    bdxl_dtd_num_syms
};

/* "bdxl_elem_nts" enumeration - the non-
 * terminals of the ekement
 * ation grammar */
enum bdxl_elem_nts {

    bdxl_elem_nt_elem =
          bdxl_doc_toks_num + 1,
    bdxl_elem_nt_start_ag,
    bdxl_elem_nt_end_tag,
    bdxl_elem_nt_empty_tag,
    bdxl_elem_nt_cnt_list,
    bdxl_elem_nt_attr,
    bdxl_elem_nt_attr_list,
    bdxl_elem_num_syms
};

/* "bdxl_extsb_nts" enumeration - the non-
 * terminals of the Document Type Declar-
 * ation external sub-set grammar */
enum bdxl_extsb_nts {

    bdxl_extsb_ignre_sct =
          bdxl_dtd_toks_num + 1,
    bdxl_extsb_inclde_sct,
    bdxl_extsb_num_syms
};

/* "bdxl_rcb_fn" typedef - the proto-type
 * of the "recognition call-back" function
 * called when a pattern of elements and/or
 * attribute names is seen on the input.
 *
 * the first parameter is an opaque data
 * pointer passed with the call, the
 * second is the current node and the
 * third and fourth are the memory alloc-
 * ator and error logger to use.   Retu-
 * rns zero on error, non-zero other-
 * wise */
typedef int (* bdxl_rcb_fn)(void *,
                            struct bdxl_node *,
                            struct bd_allocs *,
                            struct bd_logger *);

/* "bdxl_dcdr_fn" typedef -  the proto-type
 * of the function called to "decode" a str-
 * eam of bytes into a code-point for input
 * to an XML processor the gorst pparameter
 * contains a pointer to the current
 * byte in the input stream , the second is
 * set to the numbr of bytes n the decode,
 * while the third is set on exit to the
 * code-point found on the input, or NULL
 * if bytes do not form a valid encoding
 * of a code-point */
typedef int (* bdxl_dcdr_fn)(char *,
                             int *,
                             int *);

/* "bdxl_notn_fn" typedef - the proto-type
 * for resading a sequence of characters
 * from a ono-XML encoded data source.
 *
 * the first parameter contains the
 * number of byte to  read, while the
 * second is filled with the XML-equi-
 * valent of the non-XML data contain-
 * ed in the notation's format and the
 * second and third parameters contain
 * the memory allocator and error log-
 * ger to use.  Returns zero on error
 * non-zero otherwise */
typedef int (* bdxl_notn_fn)(struct bdut_str *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdxl_pi_fn" typedef - proto-type of
 * the function called to perform the
 * action associated with a particular
 * type of XML processing instruction
 * (PI)
 *
 * The first parameter contains an indi-
 * cation as to whether the PI was refer-
 * enced in the document's prologue (for
 * those PIs needing to differentiate
 * behaviour based on placement), the
 * second the PIs argument and the third
 * and fourth parameters contain the
 * memory allocator and error logger
 * to use, respectively.  Returns zero
 * on error, non-zero otherwise */
typedef int (* bdxl_pi_fn)(int,
                           struct bdut_str *,
                           struct bd_allocs *,
                           struct bd_logger *);

/* "bdxl_ent_nme" combination of an entity's
 * name and its type, ised as the key for the
 * set of entities abailable for a replacement */
struct bdxl_ent_nme {

    struct bdut_str *nme;      /*  entity's name */
    enum bdxl_ent_types type;  /*  entity's type */
};

/* bdxl_cnfg" structure -  the configuration
 * of an XML parser during a parsing operation,
,* consisting of a set of the its known pro-
 * essing instructions, assdditional relevant
 * CSS pseudo-classes and set of current rec-
 * ognition patterns */
struct bdxl_cnfg {

    struct bd_map_node *pis,   /* known proc-
                                * essomg in-
                                * structions */
                       *psdos, /* current
                                * pseudo-
                                * classes */
                       *pats;  /* current rec-
                                * ognition pat-
                                * terns */
};

/* "bdxl_vldte_ops" structure -  used an en-
 * tity replacer to "validate" the legality
 * of entity references and non-reference s-
 * tring, respectively, and consists of a
 * "state" field dictating the correct ref-
 * erence placement, sn initialisation call-
 * back (which id usually used to initiali-
 * se the state), a call-back called when a
 * reference is found, a call-back called
 * when a run of text without references
 * is found on the input and a call-back
 & called when an entity declaration is
 * found*/
struct bdxl_vldte_ops {

    void *state; /* opaque state */

    int (* init)(struct bdxl_vldte_ops *); /* initialise */

    int (* ref)(struct bdxl_vldte_ops *,   /* validate */
                struct bdxl_ent *);        /* entity refere-
                                            * nce given in
                                            * second para-
                                            * meter*/

    int (* txt)(struct bdxl_vldte_ops *,   /* validate text */
                struct bdut_str *);        /* run, given in
                                            * second param-
                                            * eter, containing
                                            * no references */

    int (* ent)(struct bdxl_vldte_ops *,   /* validate */
                struct bdut_ent *);        /* entity declar-
                                            * ation given in
                                            * second parameter */
};

/* "bdxl_rplcmt" structure - the "replace-
 * ment text" (as this is defined in the
 * XML spec.) for a single entity in the
 * entity stack used when recursively
 * generating replacement text, consist-
 * ing of the currently entity and the
 * entity being replaced */
struct bdxl_rplcmt {

    struct bdxl_ent *ent;      /* current */
    struct bdxl_rplcmt *rplcd; /* and repl-
                                * aced enti-
                               /* ties  */
};

/* "vsbdxl_rplcr_rt" structure - the run-
 * time used by the replacement text par-
 * ser, consisting of the operative val-
 * idator, the set of the currently active
 * parameter and general entities, respe-
 * ctively, a replacing stack (used to
 * detect infinite recursion), a set of
 * offsets of output generated from
 * each entity replacement and whether
 * to calculate such spans */
struct bdxl_rplcr_rt {

    struct bdxl_vldte_ops *vldtr; /* validator */
    int do_spns;                  /* whether to
                                   * calculate
                                   * spans */
    struct bd_map_node *spns,     /* entitiy */
                       *gents,    /* spans and */
                       *pents;    /* active par-
                                   * ameter and
                                   * general
                                   * entities */
    struct bdxl_rplcmt *stck;     /* base of
                                   * replacement
                                   * stack */
};

/* bdxl_logger" structure - an XML
 * logger, which, in addition of generic
 * logging functionality, kalso logs XML-
 * related either nnon-fatal, fatal or
 * notification messages, using call-backs
 * each passing the logger snd the message-
 * table index of the message to log and
 * returning zero on error, non-zero other-
 * wise */
struct bdxl_logger {

    struct bd_logger logger;          /* for gener-
                                       * ic logging */
    int (* err)(struct bdxl_logger *, /* non-fatal */
                int);                 /* error */
    int (* ftl)(struct bdxl_logger *, /* fatal */
                int);                 /* error */
    int (* ntf)(struct bdxl_logger *, /* notific- */
                int);                 /* ation */
};

/* "bdxl_extrnl_id" structure - an
 * entity "external ID", consisting
 * of a a "system ID" and a "public
 * ID", which is NULL if not supp-
 * lied (both code-point strings) */
struct bdxl_extrnl_id {

    struct bdut_str *sys, *pub; /* system and
                                 *  public IDs
                                 *  (sd code-
                                 *  point arrays) */
};


/* "bdxl_rplcr_tok" structure -  The token
 * returned by the "replacer" lexical anal-
 * yser.  The analyser is capable of reco-
 * gnising either string or integer litera-
 * ls and contains the roken's type, the
 * currently scanned entity, a buffer in
 * which to collect input and an indication
 * as to whether parsing is currently in
 * "declaration mode ", in which operce-
 * ntage signs are recognised as separate
 * tokens */
struct bdxl_rplcr_tok {

    struct bdlx_tok tok;          /* generic
                                   * token */
    enum bdxl_ent_types type;     /* token
                                   * type */
    union {
        struct bdbf_buffer *buf;  /* buffer in
                                   * which to
                                   * collect in-
                                   * put */
        struct bdut_str *str,     /* string  */
                        *txt,     /* literal, */
                        *nme;     /* non-replac-
                                   * eable text
                                   * and refere
                                   * nce name */
        int num, dcl;             /* integer  */
                                  /* literal and
                                   * whether in
                                   * "declaration
                                   * mode" */
    } u;
};

/* "bdxl_prxy" structure - proxy infor-
 * mation, used in resolving external
 * entities, consisting of  A a proxy
 * host to contact and a port on the
 * host */
struct bdxl_prxy {

    char *hst;            /* proxy host*/
    unsigned short prt;   /* proxy port */
};

/* "bdxl_opts" structure - options influ-
 * encing how an XML document is parsed:
 * namely, the type of white-space proce-
 * ssing (either preserve or ignore) and
 * the proxy information (for use in ent-
 * ity resolution) */
struct bdxl_opts {

    enum bdxl_ws_dflt_types ws; /* white-space
                                 * processing
                                 * type*/
    struct bdxl_prxy *prxy;     /* proxy info. */
};

/* "bdxl_cmdl_tok" structure -  The token
 * returned by the element content model
 * lexical analyser.  The analyser is cap-
 * able of recognising either rlrmrnt names
 * or numerio literals and contains the tok-
 * en's type, currently scanned nsme or
 * umeric value and a buffer in which to
 * collect input */
struct bdxl_cmdl_tok {

    struct bdlx_tok tok;           /* generic token */
    enum bdxl_cmdl_toks type;      /* token type */
    union {
        struct bdbf_buffer *buf;   /* buffer in
                                    * which to
                                    * collect in-
                                    * put */
        struct bdut_str str,       /* string  */
                        nme;       /* literal and */
                                   /* element bame */
        int num, cp;               /* numeric literal
                                    *  and turrent
                                    * code-point */
    } u;
};

/* "bdxl_extsb_item" structure a single
 * item in a Document Type Definition
 * external sub-set's stack of current
 * conditional sections, any one of wh-
 * ich may ignore for a declaration not
 * to be added to a document's list of
 * constraints. Each consists of an
 * indication as to whether the sect-
 * ion is to be included or ignored and
 * a pointer to the referencing condit-
 * ional section */
struct bdxl_extsb_item {

	int incl;                       /* whether
	                                 * included
	                                 * or ignored */
    struct bdxl_extsb_item *incldr; /* including
                                     * section */
};

/* "bdxl_bse_tok" structure -  The token
 * returned by the "base" parser's lexi-
 * cal analyser.  The analyser is cap-
 * able of recognising names,public IDs
 * and processing instructions, and con-
 * sists of an embedded generic token,
 * ge token type, a buffer into which
 * to collect input the currently scan-
 * ned name, public IO and processing
 * instruction */
struct bdxl_bse_tok {

    struct bdlx_tok tok;           /* generic
                                  * token */
    enum bdxl_toks type;         /* token
                                  * type */
    union {
    	struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect
                                  * input */
        struct bdut_str *pub,    /* currently */
                        *nme;    /* scanned
                                  * Public ID
                                  * and name */
        struct bdxl_pi *pi;      /* currently
                                  * scanned
                                  * norocesing
                                  * instruction */
    } u;
};

/* "bdxl_rplcr_tok" structure -  The token
 * returned by the "base" parser's lexi-
 * cal analyser.  The analyser is cap-
 * able of recognising names,public IDs
 * and processing instructions, and con-
 * sists of an embedded generic token,
 * ge token type, a buffer into which
 * to collect input the currently scan-
 * ned name, public IO and processing
 * instruction */
struct bdxl_rplcr_tok {

    struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdxl_rplcr_toks type;   /* token
                                  * type */
    union {
    	struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect
                                  * input */
        int chr;                 /* currently */
        struct bdut_str *str;    /* scanned
                                  * character
                                  * reference
                                  * and string */
    } u;
};

/* "bdxl_dtd_tok" structure -  The token
 * returned by the DTD lexical analyser.
 * The analyser is capable of recognising
 * either string or integer literals and
 * any one of several keywords and contains
 * the token's type and a buffer in which
 * to collect input */
struct bdxl_dtd_tok {
	
    struct bdlx_tok tok;           /* generic
                                  * token */
    enum bdxl_dtd_toks type;     /* token
                                  * type */
    union {
    	struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect
                                  * input */
        struct bdut_str str,     /* string */
                        nme;     /* literal
                                  * and name*/
        int num;                 /* integer
                                  * literal */
    } u;
};

/* "bdxl_eol_tok" structure -  The token
 * returned by the end-of-line processor's
 * lexical analyser, wjuc contains the
 * generic, embedded, token, its type and
 * the currently scanned code-point */
struct bdxl_eol_tok {

    struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdxl_eol_toks type;     /* token
                                  * type */
    union {
        struct bdbf_buffer *buf; /* inpiut
                                  * buffer 
                                  * type */
       int cp;                   /* currently
                                  * scanned
                                  * code-point */
    } u;   
};

/* "bdxl_doc_tok" structure -  The token
 * returned by the XML document lexical
 * analyser.  The analyser is capable
 * of recognising either string or integer
 * literals any one of several keywords
 * and contains the token's type and a
 * buffer in which to collect input, the
 * currently scanned number, currently
 * ly scanned string literal snd proces-
 * sing instruction */
struct bdxl_doc_tok {

	struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdxl_doc_toks type;     /* token
                                  * type */
    union {
        struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect in-
                                  * put */
        struct bdut_str str;     /* string lit-
                                  * eral */
        struct bdxl_pi *pi;      /* processing
                                  * instruction */
        int num;                 /* numeric
                                  * literal */
    } u;
};

/* "bdxl_cnt_tok" structure -  The token
 * returned by the XML document content
 * lexical analyser.  The analyser is
 * capable of recognising either strings,
 * or integer literals, names or any one
 * of several keywords and contains the
 * token's type and a buffer in which to
 * collect input, the currently scanned
 * number, currently ly scanned string
 * literal snd processing instruction */
struct bdxl_cnt_tok {

	struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdxl_cnt_toks type;     /* token
                                  * type */
    union {
        struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect in-
                                  * put */
        struct bdut_str *str,    /* string lit- */
                        *nme;    /* eral and name */
        struct bdxl_pi  *pi;     /* processing
                                  * instruction */
        int num;                 /* numeric
                                  * literal */
    } u;
};

/* "bdxl_extsb_tok" structure -  The token
 * returned by the DTD external sub-set
 * lexical analyser.  The analyser is
 * capable of recognising either strings,
 * or integer literals, names or any one
 * of several keywords and contains the
 * token's type and a buffer in which to
 * collect input, the currently scanned
 * number, currently ly scanned string
 * literal snd processing instruction */
struct bdxl_extsb_tok {

	struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdxl_extsb_toks type;   /* token
                                  * type */
    union {
        struct bdbf_buffer *buf; /* buffer in
                                  * which to
                                  * collect in-
                                  * put */
        struct bdut_str *str,    /* string lit- */
                        *nme;    /* eral and name */
        struct bdxl_pi *pi;      /* processing
                                  * instruction */
        int num;                 /* numeric
                                  * literal */
    } u;
};

/* "bdxl_eol_cp_buf" structure - the comb-
 * ination of a buffer into which code-
 * points are to added and the code-point
 * to add as the ubuffer's first, as used
 * in the end-of-line processing parser */
struct bdxl_eol_cp_buf {

	struct bdbf_buffer *buf; /* buffer
                              * component */
	int cp;                  /* code-point
	                          * component */

};

/* "bdxl_node" structure - a single "node"
 * in an XML document, which must be one
 * of the types listed in the "bdxl_node-
 * _types" enumeration
 *
 * A node consists of an embedded event
 * target (so that such nodes may behave
 * as objects in such targets), the num-
 * ber of nodes so far parsed (used to
 * compute the node's document order),
 * a set of attributes, each of which
 * consist of a map of name/value pairs,
 * a reference to the node's parent (wh-
 * ich (NULL for the document node), a
 * name and value string and name-space
 * information, which contains the node's
 * name-space URI and the prefix with
 * which it is associated and the node's
 * local name.
 *
 * A node also contains a pointer to
 * user-defined data, as well as refer-
 * ences to the node's attributes,child
 * nodes, parent and its next and prev-
 * ious siblings, an indication of
 * the node's index within its parent's
 * children and the node's owning docum-
 * ent */
struct bdxl_node {

    struct bdet_trgt trgt;      /* embedded event
                                 * target */
    int num;                    /* number of nodes
                                 * so far parsed
                                 * in document */
    enum bdxl_node_types type;   /* node type, */
    struct bdut_str nme, val,   /* node name, value */
                    ns, prfx,   /* name-space URI, */
                    lcl;        /* name-space Prefix
                                 * name-pace URI and
                                 * local name */
	int ordr;                   /* order within */
                                /* parent */
    struct bd_map_node *attrs,  /* attributes and */
                       *chldrn; /* children */
                                /* next and pre- */
    struct bdxl_node *nxt,       /* vious sibl- */
                    *prv,       /* ings and par- */
                    *prnt;      /* ent */
    struct bdxl_doc *ownr;      /* owning document */
    void *usr;                  /* user data pointer */
};

/* "bdxl_cmt" structure  - a represents an
 * XML comment, consisting of its constit-
 * uent characters */
struct bdxl_cmt {

    struct bdut_str *dta; /* charaters */

/* "bdxl_cdta" structure  - a represents an
 * XML "CDATA section", consisting of an
 * embredded DOM Node (since it is a type
 * of that structure) and its constituent
 * characters */
struct bdxl_cdta {

    struct bdxl_node nde;   /* embedded DOM
                            * Node */
    struct bdut_str *chrs; /* target */
};

/* "bdxl_txt" structure  - a node node corresponding
 * to block of characters (that is, a run of non-
 * mark-up text), found in an XML document which con-
 * sists of an embedded CDATA section (since it is a
 * type of that structure), and the characters of
 * the text */
struct bdxl_txt {

    struct bdxl_cdta cdta; /* embedded node */
    struct bdut_str *chrs; /* character content */
};

/* "bdxl_dtd" structure  - a "Document Type Decla-
 * ration", which consists of a nap id the constr-
 * aints on the allowed element and attribute
 * configurations and the bame if the document */
struct bdxl_dtd {

    struct bd_map_node *cnstrnts; /* list of document
                                   * constraints */
    struct bdut_str *nme;         /* document name */
};

/* "bdxl_pi" structure  - a represents an XML
 * "processing instruction", consisting of an
 * string of embredded character data (since
 * it is a type of that structure) and its
 * target string */
struct bdxl_pi {

    struct bdxl_cdta cdta; /* embedded character
                             * data */
    struct bdut_str *trgt;  /* target */
                    
};

/* "bdxl_ntn" structure  - an XML "notation",
 * which consists of an embedded node (since
 * it is a type of node, and an external ID */
struct bdxl_ntn {

    struct bdxl_node nde;      /* embedded node */
    struct bdxl_extrnl_id id; /* external ID */
};

/* "bdxl_impl" structure  - an XML "implement-
 * ation", which consists of an embedded node,
 * (since it i's a type of node), the docu-
 * ment constituting the implementation and
 * a set of the features the implementati-
 * on implements */
struct bdxl_impl {

    struct bdxl_node nde;       /* embedded node */
	struct bd_map_node *ftres; /* set of implem-
                                * ented features */
    struct bdxl_doc *doc;      /* implemented
                                * document */
};

/* "bdxl_doc_type" structure  - a "Document
 * Type", which consists of an embedded node,
 * (since it's a type of node), it's contained
 * entities and notations, its external ID
 * (i.e., its public and system IDs) and the
 * text if its internal subset */
struct bdxl_doc_type {

    struct bdxl_node nde;        /* embedded
                                 * node */
    struct bdut_str nme;        /* document
                                 * name */
    struct bdxl_extrnl_id extn; /* public and
                                 * system IDs */
    struct bd_map_node *ents,   /* contained */
                       *ntns;   /* entities
                                 * and notat-
                                 * ions */
    struct bdut_str *int_sb;    /* internal
                                 * subset 
                                 * (excluding
                                 * surrouding
                                 * '[' and ']') */
};

/* "bdxl_attr" structure  - a single "attribute"
 * of an XML element, which consists of an embed-
 * ded node, since it is a type of node, and this
 * member makes all node functionality apply to
 * the structure, and the attribute's nsmespace
 * URI, prefix and local name */
struct bdxl_attr {

    struct bdxl_node nde;       /* embedded
                                * node */
    struct bdut_str *ns,       /* namespace URI,*/
                    *prfx,     /* prefix and*/
                     lcl;      /* local name */
};

/* "bdxl_ent" structure  - an XML "entity",
 * which consists of an embedded node, since
 * it is a type of node, and this member
 * makes all node functionality apply to
 * the structure, a "document ID" (ystem and
 * public IDs), serving as the root of the
 * element tree, and a set of the constraints
 * imposed upon those elements (and the rela-
 * tionships between them) within the document,
 * known as the "Document Type Definition"
 * (DTD) */
struct bdxl_ent {

    /* location type of entity */
    enum bdxl_ent_locs loc;

    union {
       struct bdut_str *lit;           /* literal
                                        * text */
       struct bdxl_extrnl_id *extrnl;  /* public and
                                        * system IDs */
    } u;
};

/* "bdxl_doc" structure  - a "top-level" XML
 * "document node", which consists of a document
 * type node, a "document element", serving as
 * the "root" of the element tree, and a set of
 * the constraints imposed on those elements
 * (and the relationships between them) within
 * the document, otherwise known as the "Docum-
 * ent Type Definition" (DTD), whether the docu-
 * ment is "stand-alone", the document's URI,
 * its XML and input encoding names, version
 * and whether strict error checking is being
 * enforced */
struct bdxl_doc {

    struct bdxl_node node;        /* embedded
                                   * node */
    struct bdxl_doc_type *type;   /* document
                                   * type */
    struct bdxl_elem *doc_elem;   /* document
                                   * element  */
    struct bdxl_bdxl_dtd *dtd;    /* document
                                   * type decla-
                                   * ration */
    int stdlne;                   /* whether stand-
                                   * alone */
    struct bdut_str *uri;         /* doc. URI, */
    int vrsn, strct_errs;         /* version str-
                                   * ing and whet-
                                   * her enacting
                                   * strict error
                                   * checking */
    struct bdut_str *in_enc,      /* input and */
                    *xml_enc;     /* XML encod-
                                   * ing names */
};

/* "bdxl_elem" structure  - an XML "element",
 * which consists of an embedded node (since
 * it's a sub-type of node), a set of the
 * element's attributes, an indication as
 * to whether white-space in the element is
 * ignored or preserved by the processor
 * and the introduced namespace mappings */
struct bdxl_elem {

    struct bdxl_node nde;      /* embedded
                                * node */
    struct bd_map_node *attrs, /* element */
                       *nss;   /* attriutes
                                * and name-
                                * space mapp-
                                * ings */
    enum bdxl_ws_types ws;     /* how white-
                                * space is
                                * processed */ 
};

/* "bdxl_wlkr" structure  - a DOM "TreeWa-
 * lker", consisting of an associated tree
 * root, "what to show" bit-mask, Node
 * filter call-back and current node */
struct bdxl_wlkr {

    struct bdxl_doc *doc;  /* relevant 
	                        * document */
    struct bdxl_node *rt,   /* root */
                    *curr; /* and current
                            * nodes */
    enum bddm_whats what;  /* what to show
                            * bit-mask */
    bddm_fltr_fn fltr;     /* node filter */
};

/* "bdxl_it" structure  - a DOM "Node It-
 * erator", consisting of a document, its
 * tree root, "what to show" bit-mask,
 * reference node, "before" node and fil-
 * ter call-back */
struct bdxl_it {

    struct bdxl_doc *doc;  /* relevant doc- */
    struct bdxl_node *rt,   /* ument and its */
                    *ref,  /* root, ref- */
                    *bfre; /* erence and */
                           /*"before" nodes */
    enum bddm_whats what;  /* what to show
                            * bit-mask */
    bddm_fltr_fn fltr;     /* node filter */
};

 /* "bdxl_mut_rec"- a "mutation record", de-
  * scribing some change (or "mutation") in
  * a node within a document tree, such as
  * node additions or deletions, or attrib-
  * ute changes.  It consists of a decr-
  * iption of the mutation type, thethe
  * mutated node, a list of the nodes that
  * were added and deleted, respectively,
  * the node brefore and after, respect-
  * ively, the added or deleted node and
  * the mutated attribute's local name,
  * name-space URI and old value */
struct bdxl_mut_rec {

    struct bdut_str *type;    /* type descri-
                               * ption */
    struct bdxl_node *trgt,    /* mutated node, */
                    *prev,    /* previous sib- */
                    *next;    /* ling and next */
    struct bd_map_node *adds, /* sibling, */
                       *dels; /* added nodes and */
                              /* sdeleted nodes */
    struct bdut_str *attr,    /* mutated atrib */
                    *ns,      /* tute's local*/
                    *old;     /*  name, name- */
                              /* space and old
                               * name */
};

/* "bdxl_rnge" structure  - an XML "range",
 * representing aban range of nodes within
 * some document, consisting of  the start
 * and end containiner nodes and offsets */
struct bdxl_rnge {

    struct bdxl_node *strt_cntnr,  /* start and end */
                    *end_cntnr;   /* container nodes */
    int strt_off,                 /* start and end */
        end_off;                  /* offsets */
};

/* "bdxl_pat" structure  - the combination
 * of the text to select a CSS-style sele-
 * ctor string (matched against the curr-
 * ent element) and with its associated
 * recognition call-back" routine to be ca-
 * lled when that selector programme mat-
 * ches thqt element */
struct bdxl_pat {

    char *slctr;     /* selector
                     /* text */
    bdxl_rcb_fn rcb; /* recognition
                      * call-back */
};

/*** function proto-types ***/

/* "bdxl_init" - loads the grammar table and
 * lexical analyser of the XML parser, using
 * the memory allocator and error logger given
 * in the first and second parameters, respect-
 * ively.  Returns zero on error, non-zero
 * otherwise */
int bdxl_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdxl_doc_init" - initialises and returns
 * an empty XML document, using the memory
 * allocator and error logger given in the
 * first and second parameters, respectively.
 * Returns NULL on error */
struct bdxl_doc *bdxl_doc_init(struct bd_allocs *,
                               struct bd_logger *);

/* "bdxl_init_cnfg" - initialises, using the
 * memory allocator and error logger given in
 * the first and second parameters, respect-
 *ively, an XNML parsomf configuration hav-
 * ing (initially) no known prrocesing inst-
 * rucions or recognittion patterns.  Retu-
 * urns NULL on error */
struct bdxl_cnfg *bdxl_init_cnfg(
                     struct bd_allocs *,
                     struct bd_logger *);

/* "bdxl_add_pat" - adds the "pattern"
 * structure whose selector text and
 * "recognition call-back" are given
 * in the fourth and fifth parameters,
 * respectively, to the configuration
 * given in the third parameter, using
 * the memory allocator and error log-
 * ger given in the first and second
 * parameters, respectively */
int bdxl_add_pat(struct bd_allocs *,
                 struct bd_logger *,
                 struct bdxl_cnfg *,
                 const char *,
                 bdxl_rcb_fn);

/* "bdxl_add_cnfg_pi" - adds the proc-
 * essing instruction whose name and
 * call-back are given in the fourth and
 * fifth parameters, respectively, to
 * the configuration given in the third
 * parameter, using the memory allocator
 * and error logger given in the first
 * and second parameters, respectively */
int bdxl_add_cnfg_pi(struct bd_allocs *,
                     struct bd_logger *,
                     struct bdxl_cnfg *,
                     const char *,
                     bdxl_pi_fn);

/* "bdxl_first" - returns the first node  (in
 * "tree order", as this term is defined
 * in the DOM spec.) of the node tree rooted
 * in the node given in the first parameter */
struct bdxl_node *bdxl_first(struct bdxl_node *);

/* "bdxl_next" - returns the next node, in
 * "tree order", (as this term is defined
 * in the DOM spec.) after the node given
 * in the second parameter under the sub-
 * tree rooted in the first parameter */
struct bdxl_node *bdxl_next(struct bdxl_node *,
                            struct bdxl_node *);

/* "bdxl_set_opts" - returns the XML par-
 * sing options having the white-space
 * processing type, proxy host and roxy
 * port given in the third to fifth par-
 * ameters, respectivly, using the mem-
 * ory allocator anndd error logger gi-
 * ven in the first and second parame-
 * ters, respectively.  Returns NULL on
 * error  */
struct bdxl_opts *bdxl_set_opts(
                    struct bd_allocs *,
                    struct bd_logger *,
                    enum bdxl_dflt_types,
                    char *,
                    unsigned short);

/* "bdxl_prse_doc" - parse the byte
 * stream given in the third parame-
 * ter and return the XML-formatted
 * document found in that stream,
 * using the memory allocator, XML
 * error logger and parsing options
 * given in the first, second and
 * fourth parameters, respectively.
 * Returns the parsed XML document
 * or NULL on error */
struct bdxl_doc *bdxl_prse_doc(
                    struct bd_allocs *,
                    struct bd_logger *,
                    struct bdut_bytes *,
                    struct bdxl_opts *);

/* "bdxl_run" -  "runs", against the
 * XML document given in the fourth
 * parameter, the configuration given
 * in the third parameter, the "recogn-
 * ition call-backs" of which collecti-
 * vely assembles the output value, a
 * generic cast of which is set in the
 * fifth parameter, using the memory
 * allocator and XML error logger given
 * in the first and second parameters,
 * respectively.  Returns zero on error,
 * non-zero otherwise */
int bdxl_run(struct bd_allocs *,
             struct bd_logger *,
             struct bdxl_cnfg *,
             struct bdxl_doc *,
             void *);

/* "bdsl_cmbtr" - implements the "com-
 * binator call-back" for an XML docum-
 * ent.  The first parameter contains
 * the parsed simple selector to run
 * against the node given in the second
 * parameter, linked by the combinator
 * type given in the third parameter ,
 * using the opaque value, memory allo-
 * cator and error logger to use, and
 * the value to set, on exit, to an in-
 * dication as to whether the combina-
 * tion should match given in the fou-
 * rth to seventh parameters, resepec-
 * tively.  Returns zero on error , non-
 * zero otherwise */
tint bdxl_cmbtr(struct bdsl_smple,
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
int (* bdsl_smple(enum bdsl_smple_types,
                  void *,
                  struct bdxl_node *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdxl_add_nde" - returns
 * the node having the type
 * given in the first parame-
 * ter and child placement and
 * parent of the second para-
 * meter and having the "name"
 * and "value" (as these last
 * two are defined in the DOM
 * spec) given in the and thi-
 * rd and fourth parameters,
 * respectively, added to the
 * document given in the fifth
 * parameter, using the memory
 * allocator and error logger
 * given in the sixth and sev-
 * enth parameters, respect-
 * ively.  Returns NULL on
 * error */
struct bdxl_node *bdxl_add_nde(
                 enum bdxl_node_types,
                 struct bdxl_node *,
                 struct bdut_str *,
                 struct bdut_str *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_nde_utf" - returns
 * the node having the type
 * given in the first parameter
 * and parent and child order
 * of the second parameter and
 * having the "name" and "val-
 * ue" (as these last two are
 * defined in the DOM spec) the
 * UTF-8 encoding of which are
 * given in the third and four-
 * th parameters, respectively,
 * added to the document given
 * in the fifth parameter, us-
 * ing the memory allocator and
 * error logger given in the si-
 * xth and seventh parameters,
 * resp- ectively.  Returns
 * NULL on error */
struct bdxl_node *bdxl_add_nde_utf(
                 enum bdxl_node_types,
                 struct bdxl_node *,
                 char *,
                 char *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_elem" - returns the
 * element, having the node (co-
 * ntaining its parent and child
 * links), attributes, tag name,
 * namespace URI, local name and
 * prefix given in the first to
 * sixth parameters, espectiv-
 * ely, to the document given in
 * the seventh parameter, using
 * the memory allocator and err-
 * or logger given in the the fi-
 * seighth aand ninth parameters,
 * respectively. Returns NULL on
 * error */
struct bdxl_elem *bdxl_add_elem(
                  struct bdxl_node *,
                  struct bd_map_node *,
                  struct bdut_str *,
                  struct bdut_str *,
                  struct bdut_str *,
                  struct bdut_str *,
                  struct bdcxl_doc *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdxl_add_elem_utf" - retu-
 * rns the element, having the
 * node (containing its parent
 * and and child links), attr-
 * ibutes, tag name, namespace
 * URI, local name and prefix
 * the UTF-8 encodng of which
 * are given in the first to
 * sixth parameters, respect-
 * ively, added to the docu-
 * ment given in the seventh
 * parameter, using the memory
 * allocator and error logger
 * given in the eighth and ni-
 * nth parameters, respectively.
 * Returns NULL on error */
struct bdxl_elem *bdxl_add_elem_utf(
                  struct bdxl_node *,
                  struct bd_map_node *,
                  char *,
                  char *,
                  char *,
                  char *,
                  struct bdxl_doc *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdxl_add_cmt" - returns the
 * comment, having the node (co-
 * ntaining its parent and child
 * links) given in the first par-
 * ameter and content given in
 * the second parameter added to
 * the document given in the th-
 * ird parameter, using the mem-
 * ory allocator and error logg-
 * er given in the the fourth
 * and fifth parameters, respe-
 * ctively. Returns NULL on
 * error */
struct bdxl_cmt *bdxl_add_cmt(
                 struct bdxl_node *,
                 struct bdut_str *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_cmt_utf" - returns
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
struct bdxl_cmt *bdxl_add_cmt_utf(
                 struct bdxl_node *,
                 char *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_doc_type" - returns
 * the document type having the
 * node (containing its parent
 * and child links) given in the
 * first parameter and content
 * given in the second parameter,
 * added to the document given
 * in the third parameter, using
 * the memory allocator and err-
 * or logger given in the the
 * fourth and fifth parameters,
 * respectively. Returns NULL
 * on error */
struct bdxl_doc_type *bdxl_add_doc_type(
                 struct bdxl_node *,
                 struct bdut_str *,
                 struct bdut_str *,
                 struct bdut_str *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_doctype_utf" - ret-
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
                 struct bdxl_node *,
                 char *,
                 char *,
                 char *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

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
                 struct bdxl_node *,
                 struct bdut_str *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

/* "bdxl_add_txt)utf" - returns
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
                 struct bdxl_node *,
                 char *,
                 struct bdxl_doc *,
                 struct bd_allocs *,
                 struct bd_logger *);

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
                struct bdxl_node *,
                struct bdut_str *,
                struct bdut_str *,
                struct bdxl_doc *,
                struct bd_allocs *,
                struct bd_logger *);

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
                struct bdxl_node *,
                char *,
                char *,
                struct bdxl_doc *,
                struct bd_allocs *,
                struct bd_logger *);

/* "bdxl_rplce" - sets the second
 * parameter to the "replacement
 * text" (as this is defined in
 * the XML spec.) of the text
 * given in the first parameter,
 * using the recursive replace-
 * ment flag, validation opera-
 * tions, proxy information (us-
 * ed in resolving external ent-
 * ities), memory allocator and
 * error logger given in the
 * third to eighth parameters,
 * respectively.  Returns zero
 * on error, non-zero otherwise */
int bdxl_rplce(struct bdut_str *,
               struct bdut_str **,
               int,
               struct bdxl_vldte_ops *,
               struct bdxl_prxy *,
               struct bd_allocs *,
               struct bd_logger *);

/* "bdxl_get_attr" - returns the
 * value (or NULL on error) of the
 * attribute of the node given in
 * the second parameter the UTF-8
 * encoding of which is given in
 * the first parameter, using the
 * memory allocator and error
 * logger given in the third
 * parameters, respectively*/
struct bdut_str *bdxl_get_attr(
                   char *,
                   struct bdxl_node *,
                   struct bd_allocs *,
                   struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
