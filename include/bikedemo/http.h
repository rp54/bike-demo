/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* "html.h" - data types and function proto-
* types related to the Hyper-Text Markup
* Language (HTML), a legacy, SGML-like for-
* mat popular on the web prior to the advent
* of XML.
*
* This implementation parses an input HTML
* document, using the parsing rules detailed
* at "http://www.w3.org/html/wg/drafts/html/
* master/", and * returns an XHTML document
* having equivalent content to the that document
* 
* Copyright (C) 2007-2014, Russell Potter
* All rights reserved
*
*********************************************************************/

#ifndef __BD_HTML
#define __BD_HTML

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>

/* "bdhm_cnfdnces" - enumeration -
 * the possible "confidences" of
 * a "sniffed" HTML encoding: na-
 * mely, either "tentative", "ce-
 * rtain" or "irrelevant" */
enum bdhm_cnfdnces {

    bdhm_cnfdnce_tnttve,
    bdhm_cnfdnce_crtn,
    bdhm_cnfdnce_irrlvnt
};

/* "bdhm_ins_modes" enumeration -
 * the possible "insertion modes"
 * (as defined - and documented -
 * in the HTML spec) of an HTML
 * parser */
enum bdhm_ins_modes {

    bdhm_ins_mode_initl,
    bdhm_ins_mode_bfre_html,
    bdhm_ins_mode_bfre_hd,
    bdhm_ins_mode_in_hd,
    bdhm_ins_mode_in_hd_noscrpt,
    bdhm_ins_mode_aftr_hd,
    bdhm_ins_mode_in_bdy,
    bdhm_ins_mode_txt,
    bdhm_ins_mode_in_tble,
    bdhm_ins_mode_in_tble_txt,
    bdhm_ins_mode_in_cptn,
    bdhm_ins_mode_in_rw,
    bdhm_ins_mode_in_cll,
    bdhm_ins_mode_in_slct,
    bdhm_ins_mode_in_slct_in_tble,
    bdhm_ins_mode_in_frgn_cnt,
    bdhm_ins_mode_aftr_bdy,
    bdhm_ins_mode_in_frmset,
    bdhm_ins_mode_aftr_frmset,
    bdhm_ins_modes_num
};

/* "bdhm_scpes" enumeration - the
 * possible "scopes" into which
 * all HTML elements fall, namely:
 * "general" (actually un-named
 * in the spec.), "list item",
 * "button", "table" and "select" */
enum bdhm_scpes {

    bdhm_scpe_gnrl,
    bdhm_scpe_lst_itm,
    bdhm_scpe_bttn,
    bdhm_scpe_tble,
    bdhm_scpe_slct
};

/* "bdhm_toks" enumeration - the
 * possible types of tokens for
 * an HTML parser, as defined by
 * the spec., namely: DOC-TYPE,
 * start tag, end tag, CDATA se-
 * ction, comment, attributes
 * and character*/
enum bdhm_toks {

    bdhm_tok_doc_type,  /* DOC-TYPE */
    bdhm_tok_strt_tg,   /* start tag */
    bdhm_tok_end_tg,    /* end tag */
    bdhm_tok_cmt,       /* comment */
    bdhm_tok_attrs,     /* attributes */
    bdhm_tok_cdta,      /* CDATA section */
    bdhm_tok_chr        /* character */
};

/* "bdhm_elem_types" enumeration -
 * the possible types into which
 * HTML elements may be categor-
 * ised,ser, namely: either, "sp-
 * ecial" , "formaatting" or "or-
 * dinary" */
enum bdhm_elem_types {

    bdhm_elem_type_spcl,    /* special */
    bdhm_elem_type_frmttng, /* formatting */
    bdhm_elem_type_ordnry,  /* ordinary */
    bdhm_elem_types_num     /* number of
                             * types */
};

/* "bdhm_enc_cnfdncs" enumeration - the
 * presumed "confidence" that a predicted
 * encoding of an HTML fdovuument is the
 * actual encioding.  This confidence may
 * be, syt any point in time, either ten-
 * tative, certain or irrelevant */
enum bdhm_enc_cnfdncs {

    bdhm_enc_cnfdnce_tntve,  /* tentative  */
    bdhm_enc_cnfdnce_crtn,   /* certain  */
    bdhm_enc_cnfdnce_irrlvnt /* tentative  */
};

/* "bdhm_tok" - an HTML parser lexi-
 * cal token, capable of analysing
 * an returning smty og og the tokens
 * appearing in an HTML-formatted
 * byte stream, consisting of a
 * generic roknen, the token type,
 * the currently scanned DOC-TYPE,
 * scanned attributes, current cha-
 * racter, start tag and end tag */
struct bdhm_tok {

    struct bdlx_tok tok; /* generic
                          * token */
    enum bdhm_toks type; /* token type */

    union u {

        struct bdhm_doctype *doctype; /* scanned
                                       * DOC-TYPE */
        struct bdbf_buffer *buf;      /* storage
                                       * buffer */
        struct bd_map_node *attrs;    /* scanned
                                       * attributes */
        int chr;                      /* current
                                       * character */
        struct bdut_str *start,       /* scanned  */
                        *end;         /* start and 
                                       * end tags */
    }
};

/* "bdhm_elems" structure - the
 * "element pointers" (as defined in
 * the spec.) of sn HTNKML parser
 * state, consisrting of s "form"
 * element and a "head" element */
struct bdhm_elem_ptrs {

    struct bdxl_nodelem *hd,  /* "head" and */
                        *frm; /* "form" ele-
                               * ments */
};

/* "bdhm_flgs" structure = the "pars-
 * ing state flags" of an HTML paser's
 * state, condsisrting of the "scri-
 * pting abnd deframeset-ok" dlagds */
struct bdhm_flgs {

    int scrptng,  /* "scripting" and */
        frmst_ok; /* frameset-ok" flags */
};

/* "bdhm_opn_ends" structure - a two
 * "short-cuts" into the stack of open
 * elements, consisting of ointers to
 * the current rop and bottom of the
 * stack */
struct bdhm_opn_ends {

    struct bdhm_opn_node *top,  /* top and */
                         *bttm; /* bottom of
                                 * stack */
};

/* "bdhm_tmplte_node" structure - a single
 * node in the stack of template insert-
 * ion modes, consisting of that node's
 * mode and a pointer to the next node in
 * the stack */
struct bdhm_tmplte_node {

    enum bdhm_ins_modes ins_mode, /* nodse's mode */

    struct bdhm_tmplte_node nxt;  /* next node */
};

/* "bdhm_opn_node" structure - a single
 * node in the trun-time's stack of open
 * elements, consisting of the node's
 * element and ointers to the next and
 * previous nodes in the stack */
struct bdhm_opn_node {

    struct bdxl_elem *elem;    /* node's element */
    struct bdhm_opn_node *nxt, /* next and prev- */
                         *prv; /* ious in stack */
};

/* "bdhm_fmt_node" structure - a single
 * inode in the stack of list of "active
 * formatting elements" in an HTML par-
 * ser's state, consisting of the form-
 * atting element, aits associated tok-
 * en, an indication as to whether the
 * node is a "marker" (as defined in the
 * spec.) and a reference to the next
 * node in the stack */
struct bdhm_fmt_node {

    struct bdhm_tag *tag;      /* the node's
                                * tag */
    struct bdhm_tok *tok;      /* associated
                                * token */
    int mrkr;                  /* ewhether is
                                * a marker */
    struct bdhm_fmt_node *nxt; /* next ele-
                                * ment in list */
};

/* "bdhm_rt" structure - the run-time
 * environment of an HTML parser (equ-
 * ivalent to the "parser state" defi-
 * ned in the spec.), consisting of an
 * "insertion mode", the stack of curr-
 * ently open elements, a list of "act-
 * ive formatting elements", the stack
 * of "template insertion modes", par-
 * sing state flags and the current
 * "element pointers" */
struct bdhm_rt {

    enum bdhm_ins_modes ins_mode;     /* insertion
                                       * mode */
    struct bdhm_opn_ends *opns;       /* ends of
                                       * stack of
                                       * open ele-
                                       * ments */
    struct bdhm_fmt_node *fmts;       /* list of
                                       * active fo-
                                       * rmatting
                                       * elements */
    struct bdhm_tmplte_node *tmpltes; /* stack of
                                       * "template-
                                       * insertion
                                       * modes" */
    int is_frag;                      /* whether is
                                       * "fragment
                                       * case" */
    struct bdhm_elem_ptrs *elem_ptrs; /* "element
                                       * pointers" */
    struct bdhm_flgs *flgs;           /* parser
                                       * state flags */
};

/* "bdhm_doctype" structure -
 * an JHYYTML DOC-TYPE (as def-
 * ined in the spec.), xconsi-
 * sting of a name, public ID,
 * system ID and (any of which
 * nmauy be "missing", or
 * NULL) and a "quirks mode"
 * flag */
ztruct bdhm_doctype {

    struct bdut_str *nme, /* name, pub- */
                    *pub, /* lic ID and */
                    *sys; /* system ID */
    int frce_qrks;        /* forcing "qu-
                           * irks mode" */
};

/* "bdhm_tag" structure an HTML
 * "tag", consisting of the
 * tag name and name-space, an
 * indication as to whether the
 * tag is "self closing" (doe-
 * sn't require an end tag) and
 * a set of attributes */
struct bdhm_tag {

    struct bdut_str *nme,      /* tag name and */
                     ns;       /* name-space  */
    int slf_clse;              /* is self- 
                                * closing */
    struct bd_map_node *attrs; /* attributes */
};

/* "bdhm_tok" structure - the
 * HTML document parsing token,
 * consisting of a generic
 * token, the a current token
 * type, snd a union of the 
 * currently scanned DOC-TYPE,
 * the current character, a
 * buffer into which input is
 * collected as well as ano-
 * ther temporary buffer abns
 * the currently scanned start
 * tag, end tags and stry set
 * of attributes */

struct bdhm_tok {

    struct bdlx_tok tok;         /* generic
                                  * token */
    enum bdhm_toks type;         /* token type */

    union {
        struct bdhm_doctype *doctype; /* scanned
                                       * DOC-TYPE */
        struct bdhm_tag *strt,        /* start and */
                        *end;         /* end tags */
        int chr;                      /* current
                                       * character */
        struct bd_map_node *attrs;    /* attributes */
        struct bdbf_buffer *buf,      /* input and */
                           *tmp;      /* temp. buf-
                                       * fers */
    } u;
};

/* "bdhm_elem" structure  a single HTML
 * element, consisting of emciedded XML
 * element, its ID, title, language,
 & direction and class name */
struct bdhm_elem {

    struct bdxl_elem elem;     /* ebedded
                                * XML element */

    struct bdut_str *id,       /* ID, title, */
                    *ttle,     /* language */
                    *lng,      /* direction */
                    *drctn,    /* and class &*/
                    *clss_nme; /* name */
};

/* "bdhm_doc" structure - an HTML document,
 * consisting of an embedded XML document,
 * t,"head" and "body" elements and collec-
 * tions of the doocument's links, images
 * and cookies (amongst others) */
struct bdhm_doc {

    struct bdxl_doc doc;       /* embedded
                                * XML doc-
                                * ument */
   struct bdut_str *ttle,      /* title, */
                   *rfrr,      /* referrer */
                   *dmn,       /* domain and */
                   *url;       /* URL */
   struct bdhm_elem *bdy;      /* document
                                * body */
   struct bd_map_node *imgs,   /* document's */
                      *applts, /* images, */
                      *lnks,   /* applets, */
                      *frms,   /* flinks, */
                      *anchrs, /* orms, */
                      &ckies;  /* anchors
                                * and
                                * cookies */
}

/* "bdhm_html_elem structure - a docum-
 * ent's maub "HTML" element, consisting
 * of a an embedded HTML element and a
 * version string */
struct bdhm_html_elem {

    struct bdhm_elem elem; /* embedded
                            * HTML ele-
                            * ment */
    struct bdut_str *vrsn; /* version
                            * tring */
};

/* "bdhm_hd_elem structure - a "head"
 * element, consisting of an embedded
 * HTML element and the document profile
 * string */
struct bdhm_hd_elem {

    struct bdhm_elem elem;    /* embedded
                               * HTML ele-
                               * ment */
    struct vsbdut_str *prfle; /* document's
                               * profile
                               * tring */
};

/* "bdhm_lnk_elem" structure - a "iink"
 * element, consisting of an embedded
 * HTML element, a reflection as to
 * whether the link is disabled and
 * strings relecting the document's
 * "charset" and "media" links, (am-
 * ongst others) */
struct bdhm_lnk_elem {

    struct bdhm_elem elem;    /* embedded
                               * HTML ele-
                               * ment */
    int dsbld                 /* disabled
                               * flag */
    struct bdut_str *chrst,   /* charset, */
                    *hrf,     /* URL, */
                    *hrf_lng, /* language */
                    *mda,     /* target media, */
                    *rel,     /* forward */
                    *rev,     /* link, rev- */
                    *trgt,    /* rerse link,*/
                    *type;    /* target frame
                               * and advisory
                               * type */
};

/* "bdhm_ttle_elem" structure - a "title"
 * element, consisting of an embedded HTML
 * element smd rthe title text */
struct bdhm_ttle_elem {

    struct bdhm_elem elem;    /* embedded
                               * HTML ele-
                               * ment */
    struct bdut_str *txt;     /* title text */
};

/* "bdhm_mta_elem" structure - a "meta"
 * element, consisting of an embedded HTML
 * element, associated information, respon-
 * se header name, meta information name
 * and the select form of content */
struct bdhm_mta_elem {

    struct bdhm_elem elem;      /* embedded
                                 * HTML ele-
                                 * ment */
    struct bdut_str *cnt,       /* associated */
                                /* information */
                     *http_eqv; /* response
                                 * header name */
                     *nme,      /* meta infor-
                                 * mation name */
                     *schme;    /* select form
                                 * of content */
};

/* "bdhm_bse_elem" structure - a "base"
 * element, consisting of an embedded HTML
 * element, base URI and the name of the
 * default target frame */
struct bdhm_bse_elem {

    struct bdhm_elem elem; /* embedded
                            * HTML ele-
                            * ment */
    struct bdut_str *hrf,  /* bsase URI */
                    *trgt; /* default
                            * target frame */
};

/* "bdhm_style_elem" structure - a "style"
 * element, consisting of an embedded HTML
 * element, base URI the style-sheet's
 * enabler/disabler, media and type */
struct bdhm_style_elem {

    struct bdhm_elem elem; /* embedded
                            * HTML ele-
                            * ment */
    int *dsbld;            /* enabler/dis-
                            * abler */
    struct bdut_str *mda,  /* media */
                    *type; /* type */
};

/* "bdhm_bdy_elem" structure - a "body"
 * element, consisting of an embedded
 * HTML element, the colour of active
 * links, the URI of background texture tile
 * image, the document background colour,
 * the colour of links that are not
 * active and unvisited, the document
 * text colour and the colour of links
 * that htave been visited by the user */
struct bdhm_bdy_elem {

    struct bdhm_elem elem;   /* embedded
                              * HTML element */
    struct bdut_str *alnk,   /* active links
                              * colour */
                    *bg,     /* background
                              * texture image */
                    *bg_clr, /* background
                              * colour */
                    *lnk,    /* non-active,
                              * unvisited
                              * link colour */
                    *txt,    /* document
                              * text colour */
                    &*vlnk;  /* visited links
                              * colour */
};

/* "bdhm_slct_elem" structure - a "select"
 * element, consisting of an embedded HTML
 * element, the element's selection type,
 * the value of the currently selected
 * option, the name when submitted with
 * a form, zero-based ordinal index of
 * the selected option (uadsing -1 if
 * not selected),  the number o visible
 * options, the index of element's pos-
 * ition in the tabbing order, true, mu-
 * ltiple "option" elements may be sel-
 * ected ansd a collection erepresesnt-
 * ing the available options */
 * 
struct bdhm_slct_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    struct bdut_str *type,    /* "select" type */
                    *val,     /* value of selected
                               * option */
                    *nme,     /* name of selected
                               * option */
    int slctd_idx,            /* index of sele-
                               * ected option */
        sz,                   /* number of visible
                               * options */
        tb_idx,               /* position in
                               * tab order */
        mltple;               /* can have multiple
                               * options */
    struct bd_map_node *opts; /* list o options */
};

/* "bdhm_optgrp_elem" structure - an
 * "optiongroup" element, consisting of
 * an embedded HTML element, an indica-
 * tion as to whrether the element is
 * disabled amnd the element's label */
struct bdhm_optgrp_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    int dsbld,                /* whether
                               * disabled */
        lbl;                  /* label */
};

/* "bdhm_opt_elem" structure - an
 * "option" element, consisting of
 * an embedded HTML element. the
 * value of the "selected" attr-
 * ibute, the "form" element co-
 * ntaining this rlement,  whe-
 * ther the element is unavail-
 * able  */
struct bdhm_opt_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form element */
    int dsbld,                 /* whether unav-
                                * ailablable */
        slctd,                 /* cuttrrent
                                * selected
                                * state */
        dflt,                  /* bvalue of
                                * "selected"
                                * attribute */
        idx;                   /* ordinal index
                                * of option */
    struct bdut_str *txt,      /* text of option */
                    *lbl,      /* element's label */
                    *val;      /* current element
                                * value * /
};

/* "bdhm_inpt_elem" structure - an "input"
 * element, consisting of an embedded HTML
 * element, the value of the "value" attr-
 * ibute, the list of "acceptable" content
 * types, the access key to give access
 * within the containing form, the string
 * to control alignment this object with
 * respect to the surrounding text, Alte-
 * rnate text (for user agents not render-
 * ing normal content), the name as subm-
 * itted with a form., an indication as
 * to control's unavailability, position
 * in the tabbing order, The type of con-
 trol created */
struct bdhm_inpt_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdut_str *dflt_vl,  /* contents of
                                * "value" at-
                                * tribute */
                    *accpt,    /* "acceptable"
                                * content types */
                    *accss_ky, /* key giving
                                * access to
                                * form control */
                    *algn,     /* aligns con-
                                * trol respect
                                * to surrounding
                                * text */
                    *alt,      /* Alternate text
                                * for rendering
                                * user agents */
                    *nme,      /* name for form
                                * submission */
                    *src,      /* control image
                                * location */
                    *type,     /* basic control
                                * type */
                    *use_mp,   /* should use
                                * image map */
                    *vl;       /* contents of
                                * "checked"
                                *  attribute */
    struct bdhm_frm_elem *frm; /* contain-
                                * ing form */
    int dflt_chkd,             /* current
                                * state of
                                * control */
        chkd,                  /* checked
                                * state */
        dsbld,                 /* whrether
                                * unavailable */
        frd_only,              /* is read-only */
        sz,                    /* ize info. */
        tb_udx,                /* position in
                                * tabbing order */

};

/* "bdhm_txta_elem" structure - an "text-
 * area" element, consisting o an embed-
 * ded HTML element, a string representing
 * the contents of the element, the conta-
 * ining form, the key giving access to
 * the control, name used in submission of
 * the form, the basic form type, the cur-
 * rent value of the corresponding form con-
 * trol, the number of rews and columns in
 * the control, respectively, whrether of
 * the control is unavailable or unmodifi-
 * sable, respectively and the control's
 * position in the form's tabbing order */
struct bdhm_txta_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
    struct bdut_str *acc_ky,   /* key giving
                                * access to
                                * form */
                    *dflt_vl,  /* contents of
                                * element */
                    *nme,      /* name used
                                * in form 
                                * submission */
                    *type,     /* basic form
                                * type */ 
                    *vl;       /* control
                                * vaiue */
    int rws,                   /* Number of
                                * text rows */
        cols,                  /* Number of
                                * text columns */
        dsbld,                 /* is unava-
                                * ilable */
        rd_only,               /* is unmod-
                                * ifiable */
        tb_idx;                /* positlon
                                * in tabbing
                                * order */
};

/* "bdhm_bttn_elem" structure - an "button"
 * element, consisting of an embedded HTML
 * element, the containing form, the key
 * giving access to the control, the name
 * used in submission of the form, the basic
 * form type, the current value of the corr-
 * esponding form control, snd whether the
 * control is unavailable and its position
 * in the containing form's tabbing order */
struct bdhm_bttn_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
    struct bdut_str *acc_ky,   /* key giving
                                * access to
                                * form */
                    *nme,      /* name used
                                * in form 
                                * submission */
                    *type,     /* basic form
                                * type */
                    *vl;       /* control
                                * vaiue */
    int dsbld,                 /* is unava-
                                * ilable */
        tb_idx;                /* positlon
                                * in tabbing
                                * order  */
};

/* "bdhm_bttn_elem" structure - an "button"
 * element, consisting of an embedded HTML
 * element, the containing form and the at-
 * ribute linking this label with another
 * form control */
struct bdhm_bttn_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
    struct bdut_str *html_fr;  /* linking
                                * attribute */
};

/* "bdhm_fldst_elem" structure - a "fieldset"
 * element, consisting of an embedded HTML e-
 * lement and its containing form */
struct bdhm_fldst_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
};

/* "bdhm_lgnd_elem" structure - an "legend"
 * element, consisting of an embedded HTML
 * element, the xcontainining form, the key
 * giving access to the form and the alig-
 * nment relative to the  containing "fie-
 * ldset" element */
struct bdhm_lgnd_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
    struct bdut_str *acc_ky,   /* kry to give
                                * access to
                                * form */
                    *algn;     /* aligment
                                * relative to
                                * containing
                                * "fieldset" */
};

/* "bdhm_ulst_elem" structure - an "ulist"
 * element, consisting of an embedded HTML
 * element, an indicstion as to whether to
 * reduce spacing between list items, the
 * starting sequence number and the num-
 * bering style */
struct bdhm_ulst_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    int cmpct,             /* Reduce spacing
                            * between list
                            * items */
        strt;              /* Starting
                            * sequence
                            * number */
    struct bdst_str *type; /* bullet
                            * numbering
                            * style */
};

/* "bdhm_dlst_elem" structure - an "defin-
* ition list" ("dl", "dd", and "dt") ele-
* ments, consisting of an embedded HTML
 * element and an indicstion as to whether
 * to reduce spacing between list items */
struct bdhm_dlst_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    int cmpct;             /* Reduce spacing
                            * between list
                            * items */
};

/* "bdhm_dir_elem" structure - a "dire-
 * otory" element, consisting of an em-
 * bedded HTMLelement, an indicstion as
 * to whether to reduce spacing between
 * list items */
struct bdhm_dir_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    int cmpct;             /* Reduce spacing
                            * between list
                            * items */
};

/* "bdhm_div_elem" structure - a gen-
 * eric block container, the "div"
 * element, consisting of an embedded
 * HTMLelement ans the horizontal text
 * alignment */
struct bdhm_div_elem {

    struct bdhm_elem elem;  /* embedded HTML
                             * element */
    struct bdut_str *aklgn; /* horizontal
                             * text alignment */
};

/* "bdhm_li_elem" structure - a "li"
 * element, consisting of an embedded
 * HTMLelement, whether to reset the
 * sequence numvbe r and the List item
 * bullet style */
struct bdhm_li_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *type; /* list item
                            * bullet style */
    int vl;                /* reset sequ-
                            * ence number */
};

/* "bdhm_para_elem" structure - a para-
 * graph", consisting of an embedded
 * HTMLelement ans the horizontal text
 * alignment */
struct bdhm_para_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *algn; /* horizontal
                            * text alignment */
};

/* "bdhm_hdg_elem" structure - a "hea-
 * ding" ("h1", "h2" "h3", "h4", "n5"
 * and "h6") elements, consisting of
 * an embedded HTMLelement ans the
 * horizontal text alignment */
struct bdhm_hdg_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *algn; /* horizontal
                            * text alignment */
};

/* "bdhm_qte_elem" structure - a "qu-
 * ote" ("q") element, consisting of
 * an embedded HTMLelement and a URI
 * designating the souce document */
struct bdhm_qte_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *cte;  /* source URI */
};

/* "bdhm_pre_elem" structure - a "pre"
 * element, consisting of an embedded
 * HTMLelement and its fixed width for
 ( content */
struct bdhm_pre_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    int wdth;              /* width for
                            * content */
};

/* "bdhm_br_elem" structure - a "br"
 * element, consisting of an embedded
 * HTMLelement and an indication as
 * or to the type of flow of text
 * aro-und floats */
struct bdhm_br_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *clr;  /* Control flow
                            * of text aro-
                            * und floats */
};

/* "bdhm_bsefnt_elem" structure - a
 * "basefont" element, consisting of
 * an embedded HTML element its col-
 * our, font face ID and font size */
struct bdhm_bsefnt_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *clr,  /* colour */
                    *fce,  /* font face ID */
    int sz;                /* font size */
};

/* "bdhm_fnt_elem" structure - a
 * "font" element, consisting of
 * an embedded HTML element, its
 * colour, font face ID and font
 * size */
struct bdhm_fnt_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *clr,  /* colour */
                    *fce,  /* font face ID */
                    *sz;   /* font size */
};

/* "bdhm_hr_elem" structure - a
 * "horizontal rule" ("hr") ele-
 * ment, consisting of an embed-
 * ded HTML element, an indic-
 * ation as to hoe to align the
 * rule, the width and height,
 * respectively, of the rule
 * and whether should be no
 * shading when rendering the
 * rule */
struct bdhm_hr_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *algn, /* how to align
                            * rule */
                    *sz,   /* height of
                            * rule */
                    *wdth; /* width of
                            * rule */
    int no_shde;           /* should be
                            * no shading */
};

/* "bdhm_mod_elem" structure - a
 * "modification" ("mod") element,
 * consisting of an embedded HTML
 * element, the designating docu-
 * ment URL and the date and time
 * of the change */
struct bdhm_mod_elem {

    struct bdhm_elem elem;   /* embedded HTML
                              * element */
    struct bdut_str *cte,    /* designating
                              * document URL */
                    *dtetme; /* date and time
                              * of change */
};

/* "bdhm_anchr_elem" structure - an
 * "anchor" ("a") element, consisting
 * of an embedded HTML element, the
 * key used to gain access to the
 * element's containing form, the
 * character encoding of the linked
 * resource, the comma-separated
 * list of lengths defining the act-
 * ive region geometry, the absolute
 * URI, the language code of the
 * resource, the anchor name, the
 * forward and reverse, respectively,
 * link types, the shape of the
 * active area, the frame in which
 * to render the resource and the
 * position in the containing form's
 * tabbing order */
struct bdhm_anchr_elem {

    struct bdhm_elem elem;   /* embedded HTML
                              * element */
    struct bdut_str *acc_ky, /* key to access
                              * containing form */
                    *chrst,  /* char. encoding */
                    *coords, /* region geometry */
                    *hrf,    /* absolute URI */
                    *hrflng, /* language code of
                              * resource */
                    *nme,    /* anchor name */
                    *rel,    /* forward link type */
                    *rev,    /* reverse link type */
                    *shpe,   /* shape of active
                              * area */
                    *trgt,   /* frame in which
                              * to render resource */
                    *type;   /* acontent type  */
    int tb_idx;              /* position in
                              * tabbing order */
};

/* "bdhm_img_elem" structure - an image
 * ("img") element, consisting of an emb-
 * edded HTML element, name used in the
 * co- ntaining form submission, the cor-
 * responding control's horizontal align-
 * ment, top and boottom, and left and
 * rightspace and border thickness (res-
 * pectively), the alternate text to use
 * when not rendering, whether the elem-
 * ent's image map is located on the cl-
 * ient and whether on the server */
struct bdhm_img_elem {

    struct bdhm_elem elem;   /* embedded HTML
                              * element */
    struct bdut_str *nme,    /* name used in
                              * form submission */
                    *algn,   /* horizontal
                              * alignment */
                    *alt,    /* alternate text
                              * when not rendering */
                    *brdr,   /* thickness of border */
                    *lngdsc, /* long description
                              * of image */
                    *src;    /* source URI */
    int hght,                /* height in pixels */
        wdth,                /* width in pixels */
        hspce,               /* space left and right
                              * in pixels */
        vspce,               /* space top and bottom
                              * in pixels */
        is_mp,               /* Use server-side image
                              * map */
        use_mp;              /* Use client-side image
                              * map */
};

/* "bdhm_obj_elem" structure - an object
 * ("imgobject") element, consisting of
 * an embedded HTML element, the containing
 * form, the applet class file, the, the
 * horizontal alignment, the space-separa-
 * ted list of constituent archives, the
 * width of border around the object, the
 * Base URI for the "classid", "data",
 * and "archive" attributes, theontent type
 * for downloaded data, the document this
 * object contains, the a URI specifying
 * the location of the object's data, the
 * content type for downloaded data, whe-
 * ther to declare, but not instantiate,
 * the object, the height and width (resp-
 * ectively) of the object, the horizontal
 * and vertical spacing (respectively)
 * around the object, the element's pos-
 * ition in the tabbing order, the Cont-
 * ent type for data downloaded via the
 * "data" attribute, the name when subm-
 * itted with a form, the rthe message
 * to render while loading the object,
 * whrether to ise a client-side image
 * map */
struct bdhm_obj_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bdhm_frm_elem *frm; /* containing
                                * form */
    struct bdut_str *cde,      /* applet class
                                * file  */
                    *algn,     /* horizontal
                                * alignment */
                    *archve,   /* list of arc-
                                * hives */
                    *brdr,     /* border
                                * thickness */
                    *nme,      /* name for form
                                * submssion */
                    *cde_type, /* Content type
                                * for downloa-
                                * ded data  */
                    *hgt,      /* height */
                    *stndby,   /*  loading message */
                    *type,     /* download
                                * content type */
                    *use_mp,   /* use client-side
                                * image map */
                    *wdth;     /* width  */
    int dclre,                 /* Declare, but
                                * don't instan-
                                * otiate, bject */
        tb_idx,                /* position in
                                * tabbing order */
        hspce,                 /* space to left
                                * and right in
                                * pixels */
        hspce;                 /* space to left
                                * and right in
                                * pixels */
        vspce;                 /* space to top
                                * and bottom in
                                * pixels */
    struct bdghm_doc *doc;     /* containing
                                * document */
};

/* "bdhm_param_elem" structure - an object
 * parameter element, consisting of an emb-
 * edded HTML element, the name, content
 * type and run-time value of the parameter
 * and Information about its meaning */
struct bdhm_param_elem {

    struct bdhm_elem elem;      /* embedded HTML
                                 * element */
    struct bdut_str *nme,       /* parameter
                                 * name  */
                    *type,      /* type */
                     *val,      /* run-time
                                 * value */
                     *val_type; /* Information
                                 * about 
                                 * meaning */
};

/* "bdhm_applt_elem" structure - an "applet"
 * element, consisting of an embedded HTML
 * element, the horizontal alignment, alter-
 * nate text to use when not rendering, a
 * comma-separated list of constituent
 * archives, the Applet class file name,
 * the base URI of the applet, its name,
 * width, height a serialized representat-
 * ion of an applet's state, the left,
 * right, top and bottom space around the
 * appplet */ 
struct bdhm_applt_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    struct bdut_str *algn,    /* horizontal
                               * salignment */
                    *alt,     /* alternate text
                               * to use when
                               * not rendering */
                    *archve,  /* list of
                               * archives */
                    *cde,     /*  applet URI */
                    *cde_bse, /* base URI */
                    *hght,    /* height */
                    *hspce,   /* top and
                               * bottom space */
                    *nme,     /* applet name */
                    *obj,     /* applet's state */
                    *vspce,   /* left and
                               * right space */
                    *wdth;    /* width */
};

/* "bdhm_mp_elem" structure - an "image mao"
 * element, consisting of an embedded HTML
 * element, the set of areas defined for
 * the map and its name */ 
struct bdhm_mp_elem {

    struct bdhm_elem elem;     /* embedded HTML
                                * element */
    struct bd_map_node *areas; /* list of
                                * areas */
    struct bdut_str *nme;      /* name of
                                * maop */
};

/* "bdhm_srea_elem" structure - an "ima-
 * ge map area" ("area") element, cons-
 * isting of an embedded HTML element,
 * the key giving access to the contai-
 * ning form, alternate text for user
 * agents not rendering the normal cont-
 * ent, a comma-separated list of leng-
 * ths defining an active region geomet-
 * ry, the URI of the linked resource, a
 * flag indicating that the area is ina-
 * ctive, the shape of the active area
 * (the co-ordinates of which are given
 * in"crds"), Index representing the ele-
 * ment's position in the tabbing order
 * and the frame into which to render the
 * resource  */ 
struct bdhm_area_elem {

    struct bdhm_elem elem;   /* embedded HTML
                              * element */
    struct bdut_str *acc_ky, /* form access
                              * key */
                    *alt,    /* alternate text
                              * when niot ren-
                              * dering */
                    *crds,   /* list o co-ord-
                              * inates */
                    *hrf,    /* URI of resource */
                    *shpe;   /* shape of area */
    int tb_idx,              /* pos. in tab-
                              * bing order */
         nohrf;              /* area is inactive */
};

/* "bdhm_scrpt_elem" structure - a script
 * element, consisting of an embedded HTML
 * element, the script content of the ele-
 * ment, the character encoding of the
 * linked resource URI of the script,
 * its language and anfd an Indication
 * that the user agent can defer proc-
 * essing of the script */ 
struct bdhm_scrpt_elem {

    struct bdhm_elem elem;  /* embedded HTML
                             * element */
    struct bdut_str *txt,   /* content of
                             * script */
                    *chrst, /* resoutce
                             * charset */
                    *src,   /* URI of
                             * resource */
                    *type;  /* script
                             * language */
    int dfr;                /* defer script
                             * processing */
};

/* "bdhm_tble_elem" structure - a "table"
 * element, consisting of an embedded HTML
 * element, the table'a caption, header,
 * footer, horizontal alignment, background
 * colour, border thickness, cell padding,
 * cell spacing, a n indixcation as to
 * which sides are visible and which bor-
 * ders to render, a Description about the
 * table's purpose and its desired width */ 
struct bdhm_tble_elem {

    struct bdhm_elem elem;            /* embedded HTML
                                       * element */
    struct bdhm_tble_cptn_elem *cptn; /* caption */
    struct bdhm_tble_sctn_elem *hdr,  /* header and */
                               *ftr;  /* footer  */
    struct bd_map_node *rws,          /* rows and */
                       *bdies;        /* bodies */
    struct bdut_str *algn,            /* horizontal
                                       * alignment */
                    *bg_clr,          /* background
                                       * colour */
                    *brdr,            /* border
                                       * thickness */
                    *pddng,           /* cell padding */
                    *spcng,           /* cell spacing */
                    *frme,            /* which sides
                                       * are visible */
                    *rles,            /* which borders
                                       * to render */
                    *smmry,           /* Description
                                       * about purpose */
                    *wdth;            /* desired width */
};

/* "bdhm_tble_cptn_elem" structure - a
 * "table caption" element, consisting
 * of an embedded HTML element and its
 * horizontal alignment */ 
struct bdhm_tble_cptn_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *algn; /* horizontal
                            * alignment */
};

/* "bdhm_tble_col_elem" structure - a
 * "table column" ("colgroup") element,
 * consisting of an embedded HTML ele-
 * ment, its horizontal alignment, the
 * alignment character, alignment char-
 * acter offset, the number of columns
 * in a group and default width */ 
struct bdhm_tble_col_elem {

    struct bdhm_elem elem;      /* embedded HTML
                                 * element */
    struct fnfbdut_str *algn,   /* horizontal
                                 * alignment */
                       *ch,     /* alignment
                                 * char.*/
                       *ch_off, /* alignment
                                 * char. offest */
                       *valgn,  /* vertical
                                 * alignment */
                       *wdth;   /* width */
    int span;                   /* number of
                                 * cols in group */
};

/* "bdhm_tble_sctn_elem" structure - a
 * "table section" element, consisting
 * of an embedded HTML element, its 
 * horizontaland vertical alignment,
 * constituent rows alignment character
 * and alignment character offset */
struct bdhm_tble_sctn_elem {

    struct bdhm_elem elem;      /* embedded HTML
                                 * element */
    struct bd_map_node *rws;    /* rows in
                                 * section */
    struct bdut_str *algn,      /* horizontal */
                    *valgn,     /* and vertical
                                 * alignment */
                    *ch,        /* alignment char. */
                    *ch_off;    /* alignment char.
                                 * offset */
};

/* "bdhm_tble_rw_elem" structure - a
 * "table row" "(tr)" element, consis-
 * ting of an embedded HTML element,
 * its  horizontal and vertical align-
 * ment, constituent cells, character
 * alignment, character alignment
 * offset, ordinal position within
 * its containing table and the ord-
 * inal position eithin its contain-
 * ing table section */
truct bdhm_tble_rw_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    struct bd_map_node *clls; /* cells in row */
    struct bdut_str *algn,    /* horizontal
                               * alignment */
                    *valgn,   /* vertical
                               * alignment */
                    *ch,      /* char. alignment */
                    *ch_off;  /* char. alignment
                               * offset */
    int rw_idx,               /* ordinal position */
        rw_sctn_idx;          /* ordinal position
	                           * within section */
};

/* "bdhm_tble_cll_elem" structure - a
 * "table cell" "(th, td)" element, co-
 * nsisting of an embedded HTML element,
 * abbreviation to use in header cells,
 * its height and width, the number of
 * rows and columns it spans, its char-
 * acter alignment, character and ali-
 * gnment offset, a list of ID attri-
 * butes for header cells, whether to
 * suppress wrapping, horizontal and
 * vertical alignment, the scope cov-
 * ered by header cells and the name
 * of this group of related headers
 * and the cell;'s background colour */
truct bdhm_tble_cll_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    struct bdut_str *abbr,    /* abbreviation
                               * for header */
                    *algn,    /* horizontal
                               * alignment */
                    *valgn,   /* vertical
                               * alignment */
                    *ch,      /* char. alignment */
                    *ch_off,  /* char. alignment
                               * offset */
                    *hgt,     /* height */
                    *wdth,    /* width */
                    *bg_clr,  /* background
                               * colour */
                    *axs,     /* name of head-
                               * er group */
                    *scpe;    /* scope info. */
    struct bd_map_node *hdrs; /* list of header
                               * attribute values */
    int rw_spn,               /* row span */
        col_spn,              /* column spsan */
        nowrp,                /* whetcooxher to
                               * suppress wra-
                               * pping  */
};

/* "bdhm_frmest" structure - a "frameset"
* element, consisting of an embedded HTML
* element */
truct bdhm_frmest_elem {

    struct bdhm_elem elem; /* embedded HTML
                            * element */
    struct bdut_str *rws,  /* number of rows */
                    *cols, /* unumber of
                            * columns */
};

/* "bdhm_ifrme_elem" structure - an
 * "inline frame" "(iframe)" element,
 * consisting of an embedded HTML
 * element, the frame's name, horiz-
 * ontal alignment, an indication as
 * to whether the frame has scroll-
 * bars, the , the frame's width and
 * height, margin width and height,
 * a long description, a URI of the
 * frame's contents and the contain-
 * ing document */
s */
struct bdhm_ifrme_elem {

    struct bdhm_elem elem;      /* embedded HTML
                                 * element */
    struct bdut_str *nme,       /* frame name */
                    *algn,      /* horizontal
                                 * alignment */
                    *scrllng,   /* frame has
                                 * scrollbars */
                    *src,       /* URI of frame
                                 * contents */
                    *brdr,      /* whether to
                                 * render adjoi-
                                 * ning borders */
                    wdth,       /* width */
                    hght        /* height */
                    *mrgn_hgt,  /* margin height*/
                    *mrgn_wdth, /* margin width */
                    *lng_dsc;   /* long
                                 * description */
    struct bdhm_doc *doc;       /* containing
                                 * document */

};

/* "bdhm_frme_elem" structure - a
 * "frame" element, consisting of
 * an embedded HTML element, the
 * frame's name, horizontal ali-
 * gnment, an indication as to
 * whether the frame has scroll-
 * bars, the , the margin width
 * and height, a URI of whether
 * to render the frame's adjoin-
 * ing borders, a long descript-
 * ion, a URI of the frame's co-
 * ntents and the containing
 * document */
struct bdhm_frme_elem {

    struct bdhm_elem elem;      /* embedded HTML
                                 * element */
    struct bdut_str *nme,       /* frame name */
                    *algn,      /* horizontal
                                 * alignment */
                    *scrllng,   /* frame has
                                 * scrollbars */
                    *src,       /* URI of frame
                                 * contents */
                    *brdr,      /* whether to
                                 * render adjoi-
                                 * ning borders */
                    *mrgn_wdth, /* margin width */
                    *mrgn_hgt,  /* margin height*/
                    *lng_dsc;   /* long
                                 * description */
    struct bdhm_doc *doc;       /* containing
                                 * document */
    int norsz;                  /* wheher to
                                 * resizing is
                                 * forbidden */
};

/* "bdhm_tble_cll_elem" structure - a
 * "frame" element, consisting of an
 * embedded HTML element, and ithe number
 * of rows and columns it contains */
truct bdhm_tble_cll_elem {

    struct bdhm_elem elem;    /* embedded HTML
                               * element */
    struct bdut_str *rws,     /* number of rows */
                    *cols;    /* number of columns */
};

/*** function proto-types ***/

/* "bdhm_set_doc_prps" - returns 
 * a language-abstracted object
 * populated with DOM properties
 * of the HTML Document given in
 * the first parameter, using the
 * memory allocator and error lo-
 * gger given in the second and
 * third parameters, respectively.
 * Returns NUL on error */
struct bdlg_obj *bdhm_add_doc_prps(
                       struct bdhm_doc *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_elem_prps" - returns 
 * a language-abstracted object
 * populated with DOM properties
 * of the HTML element given in
 * the first parameter, using
 * the memory allocator and
 * error logger given in the
 * second and third parameters,
 * respectively. Returns NULL
 * on error */
struct bdlg_obj *bdhm_add_elem_prps(
                       struct bdhm_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_html_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the main HTML element
 * given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_html_elem_prps(
                       struct bdhm_html_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_lnk_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Link elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_lnk_elem_prps(
                       struct bdhm_lnk_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_hd_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Head elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_hd_elem_prps(
                       struct bdhm_hd_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_ttle_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Head elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_ttle_elem_prps(
                       struct bdhm_ttle_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_mta_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Meta elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_mta_elem_prps(
                       struct bdhm_mta_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_bse_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Base elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_bse_elem_prps(
                       struct bdhm_bse_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_set_style_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Style elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_style_elem_prps(
                       struct bdhm_style_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_frm_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML HTML Form elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the seco-
 * nd and third parameters, respec-
 * tively. Returns NULL on error */
struct bdlg_obj *bdhm_add_frm_elem_prps(
                       struct bdhm_frm_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_frm_bdy_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML Body element
 * given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_bdy_elem_prps(
                       struct bdhm_bdy_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_slct_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM propert-
 * ies of the HTML Select elem-
 * en given in the first parameter,
 * using the memory allocator and
 * error logger given in the sec-
 * ond and third parameters, resp-
 * ectively. Returns NULL on error */
struct bdlg_obj *bdhm_add_slct_elem_prps(
                       struct bdhm_slct_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_slctoptgrp_elem_prps" - ret-
 * urns a language-abstracted object pop-
 * ulated with DOM properties of the HTML
 * OptGroup element given in the first
 * parameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bdhm_add_optgrp_elem_prps(
                       struct bdhm_optgrp_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_opt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTM Option
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_opt_elem_prps(
                       struct bdhm_opt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_inpt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTM Option
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns NULL
 * on error */
struct bdlg_obj *bdhm_add_inpt_elem_prps(
                       struct bdhm_inpt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_txta_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Text-
 * Area element given in the first para-
 * meter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.  Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_txta_elem_prps(
                       struct bdhm_txta_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_bttn_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Button
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_bttn_elem_prps(
                       struct bdhm_bse_bttn_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_lbl_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Label
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_lbl_elem_prps(
                       struct bdhm_lbl_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_fldst_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Field-
 * Set element given in the first parame-
 * ter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.  Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_fldst_elem_prps(
                       struct bdhm_bse_fldst_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_lgnd_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Legend
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_fldst_lgnd_elem_prps(
                       struct bdhm_bse_lgnd_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_ulst_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML UList
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_fldst_ulst_elem_prps(
                       struct bdhm_bse_ulst_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_olst_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML OList
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_fldst_olst_elem_prps(
                       struct bdhm_bse_olst_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_dlst_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML DList
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_fldst_dlst_elem_prps(
                       struct bdhm_bse_dlst_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_dir_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Dire-
 * ctory element given in the first par-
 * ameter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively.  Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_fldst_dir_elem_prps(
                       struct bdhm_bse_dir_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_mnu_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Menu
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively.  Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_fldst_mnu_elem_prps(
                       struct bdhm_bse_mnu_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_para_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Para-
 * graph element given in the first par-
 * ameter, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bdhm_add_para_elem_prps(
                       struct bdhm_bse_para_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_li_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML LI
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns NULL
 * on error */
struct bdlg_obj *bdhm_add_li_elem_prps(
                       struct bdhm_li_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_div_elem_prps" - returns a
 * language-abstracted object populated
 * with DOM properties of the HTML Div
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns NULL
 * on error */
struct bdlg_obj *bdhm_add_div_elem_prps(
                       struct bdhm_div_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_hdng_elem_prps" - returns a
 * language-abstracted object populated
 * with DOM properties of the HTML Head-
 * ing element given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_hdng_elem_prps(
                       struct bdhm_hdg_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_qte_elem_prps" - returns a
 * language-abstracted object populated
 * with DOM properties of the HTML Quote
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_qte_elem_prps(
                       struct bdhm_qte_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_pre_elem_prps" - returns a
 * language-abstracted object populated
 * with DOM properties of the HTML Pre
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_pre_elem_prps(
                       struct bdhm_pre_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_br_elem_prps" - returns a
 * language-abstracted object populated
 * with DOM properties of the HTML Br
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_br_elem_prps(
                       struct bdhm_br_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_bsefnt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Base-
 * Font element given in the first para-
 * meter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_bsefnt_elem_prps(
                       struct bdhm_bsefnt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_fnt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Font
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_fnt_elem_prps(
                       struct bdhm_fnt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_hr_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Hr ele-
 * ment given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_hr_elem_prps(
                       struct bdhm_hr_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_anchr_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Anchor
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_anchr_elem_prps(
                       struct bdhm_anchr_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_obj_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Object
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_obj_elem_prps(
                       struct bdhm_obj_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_img_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Image
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_img_elem_prps(
                       struct bdhm_anchr_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_img_prm_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Para-
 * meter element given in the first para-
 * meter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Retu-
 * rns NULL on error */
struct bdlg_obj *bdhm_add_prm_elem_prps(
                       struct bdhm_prm_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_applt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Applet
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_applt_elem_prps(
                       struct bdhm_applt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_map_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Map
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_map_elem_prps(
                       struct bdhm_map_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_area_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Area
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_area_elem_prps(
                       struct bdhm_area_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_scrpt_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Script
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_scrpt_elem_prps(
                       struct bdhm_scrpt_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tbl_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table
 * element given in the first parameter,
 * using the memory allocator and error
 * logger given in the second and third
 * parameters, respectively. Returns
 * NULL on error */
struct bdlg_obj *bdhm_add_tbl_elem_prps(
                       struct bdhm_tbl_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tblcptn_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table-
 * Caption element given in the first par-
 * ameter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Retu-
 * rns NULL on error */
struct bdlg_obj *bdhm_add_tblcptn_elem_prps(
                       struct bdhm_tblcptn_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tblcol_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table-
 * Col element given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Retu-
 * rns NULL on error */
struct bdlg_obj *bdhm_add_tblcol_elem_prps(
                       struct bdhm_tblcol_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tblcol_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table-
 * Col element given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Retu-
 * rns NULL on error */
struct bdlg_obj *bdhm_add_tblcol_elem_prps(
                       struct bdhm_tblcol_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tblrw_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table-
 * Row element given in the first param-
 * eter, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Retu-
 * rns NULL on error */
struct bdlg_obj *bdhm_add_tblrw_elem_prps(
                       struct bdhm_tblrw_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_tblcll_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Table-
 * Cell, using the memory allocator and
 * error logger given in the second and
 * third parameters, respectively. Ret-
 * urns NULL on error */
struct bdlg_obj *bdhm_add_tblcll_elem_prps(
                       struct bdhm_tblcolcll_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_frme_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML Frame
 * element, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bdhm_add_frme_elem_prps(
                       struct bdhm_frme_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_add_ifrme_elem_prps" - returns
 * a language-abstracted object populated
 * with DOM properties of the HTML IFrame
 * element, using the memory allocator
 * and error logger given in the second
 * and third parameters, respectively.
 * Returns NULL on error */
struct bdlg_obj *bdhm_add_ifrme_elem_prps(
                       struct bdhm_ifrme_elem *,
                       struct bd_allocs *,
                       struct bd_logger *);

/* "bdhm_init" = loads the lexical rules
 * and dispatching call-backs enabling an
 * HTML parser to generate the XML doc-
 * ument equivalent to the HTML document
 * serving as the parser's input, using
 * the memory allocator and error logger
 * given in the first and second parame-
 * ters respectively.  Returns zero on
 * error, non-zero otherwise */
int bdhm_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdhm_prse_doc" - parses, using the
 * memory allocator and error logger gi-
 * ven in the firar and second paramet-
 * ers, respectively, the HTML-formatt-
 * ed byte stream given in the third
 * parameter, and returns an XML docu-
 * ment having equivalent functionality
 * to the document found in the HTML
 * stream.  Returns NULL on error */
struct bdxl_doc *bdhm_prse_doc(struct bd_allocs *
                               struct bd_logger *,
                               struct bdut_bytes *);

/* "bdhm_prse_frag" - parses, using the
 * memory allocator and error logger gi-
 * ven in the firar and second paramet-
 * ers, respectively, the HTML-formatt-
 * ed byte stream given in the third
 * parameter, and returns an XML element-
 * ment having equivalent functionality
 * to the odocument fragment found in
 * the HTML stream.  Returns NULL on
 * error */
struct bdxl_node *bdhm_prse_frag(struct bd_allocs *
                                 struct bd_logger *,
                                 struct bdut_bytes *);

#ifdef __cplusplus
}
#endif

#endif
