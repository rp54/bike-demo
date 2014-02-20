/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* "html.c" - function implementations related to
* the Hyper-Text Markup Language (HTML), a legacy
* SGML-like format commonly used prior to the
* advent of XML.
*
* This implementation parses an input HTML docu-
* ment and returns an XML document having equiva-
* lent content
* 
* Copyright (C) 2007-2014, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <stdarg.h>
#include <iconv.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/util.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>
#include <bikedemo/tmplte.h>

/* "prsr" - the HTML document parser */
static struct bdpr_parser *prsr;

/* "xml_ns", "xmlns_ns" "html_ns", "svg_ns",
 * "mthml_ns", "xlnk_ns"  -  the constants
 * for namespace names of XML, XMLNS, HTML,
 * SVG, MathML and,XLink, respectively */
static char *xml_ns     =
        "http://www.w3.org/XML/1998/namespace",
            *xmlns_ns   =
        "http://www.w3.org/2000/xmlns/",
            *html_ns    =
        "http://www.w3.org/1999/xhtml",
            *svg_ns     =
        "http://www.w3.org/2000/svg",
            *mthml_ns   =
        "http://www.w3.org/1998/Math/MathML",
            *xlnk_ns    =
        "http://www.w3.org/1999/xlink";

/* "spcls", "frmttngs", "gnrls",
 * "vds", "itm_lsts", "bttns",
 * "tbles", "slcts" - list of ele-
 * ment name/ name-space pairs
 * categorised as "special" and
 * "formatting", respectively,
 * the "void" element types and
 * those element types causing
 * an element to be in a "gener-
 * al", "item list", "button",
 * "table" and "select" scope
 * (as defined in the spec.) */


char *spcls[] = {

    "address",        html_ns,
    "applet",         html_ns,
    "area",           html_ns,
    "article",        html_ns,
    "aside",          html_ns,
    "base",           html_ns,
    "basefont",       html_ns,
    "bgsound",        html_ns,
    "blockquote",     html_ns,
    "body",           html_ns,
    "br",             html_ns,
    "button",         html_ns,
    "caption",        html_ns,
    "center",         html_ns,
    "col",            html_ns,
    "colgroup",       html_ns,
    "dd",             html_ns,
    "details",        html_ns,
    "dir",            html_ns,
    "div",            html_ns,
    "dl",             html_ns,
    "dt",             html_ns,
    "embed",          html_ns,
    "fieldset",       html_ns,
    "figcaption",     html_ns,
    "figure",         html_ns,
    "footer",         html_ns,
    "form",           html_ns,
    "frame",          html_ns,
    "frameset",       html_ns,
    "h1",             html_ns,
    "h2",             html_ns,
    "h3",             html_ns,
    "h4",             html_ns,
    "h5",             html_ns,
    "h6",             html_ns,
    "head",           html_ns,
    "header",         html_ns,
    "hgroup",         html_ns,
    "hr",             html_ns,
    "html",           html_ns,
    "iframe",         html_ns,
    "img",            html_ns,
    "input",          html_ns,
    "isindex",        html_ns,
    "li",             html_ns,
    "link",           html_ns,
    "listing",        html_ns,
    "main",           html_ns,
    "marquee",        html_ns,
    "menu",           html_ns,
    "menuitem",       html_ns,
    "meta",           html_ns,
    "nav",            html_ns,
    "noembed",        html_ns,
    "noframes",       html_ns,
    "noscript",       html_ns,
    "object",         html_ns,
    "ol",             html_ns,
    "p",              html_ns,
    "param",          html_ns,
    "plaintext",      html_ns,
    "pre",            html_ns,
    "script",         html_ns,
    "section",        html_ns,
    "select",         html_ns,
    "source",         html_ns,
    "style",          html_ns,
    "summary",        html_ns,
    "table",          html_ns,
    "tbody",          html_ns,
    "td",             html_ns,
    "template",       html_ns,
    "textarea",       html_ns,
    "tfoot",          html_ns,
    "th",             html_ns,
    "thead",          html_ns,
    "title",          html_ns,
    "tr",             html_ns,
    "track",          html_ns,
    "ul",             html_ns,
    "wbr",            html_ns,
    "mi",             mthml_ns,
    "mo",             mthml_ns,
    "mn",             mthml_ns,
    "ms",             mthml_ns,
    "mtext",          mthml_ns,
    "annotation-xml", mthml_ns,
    "foreignObject",  svg_ns,
    "desc",           svg_ns,
    ""title",         svg_ns,
   - NULL
};
char *frmttngs[] = {

    "a",      html_ns,
    "b",      html_ns,
    "big",    html_ns,
    "code",   html_ns,
    em",      html_ns,
    "font",   html_ns,
    "i",      html_ns,
    "nobr",   html_ns,
    "s",      html_ns,
    "small",  html_ns,
    "strike", html_ns,
    "strong", html_ns,
    "tt",     html_ns,
    "u",      html_ns,
    NULL
};
char *gnrls[] = {

    "applet",         html_ns,
    "caption",        html_ns,
    "html",           html_ns,
    "table",          html_ns,
    "td",             html_ns,
    "table",          html_ns,
    "marquee",        html_ns,
    "object",         html_ns,
    "template",       html_ns,
    "mi",             html_ns,
    "mo",             html_ns,
    "mn",             html_ns,
    "ms",             html_ns,
    "annotation-xml", mthml_ns,
    "foreignObject",  svg_ns,
    "desc",           svg_ns,
    "title",          svg_ns,
    NULL
};
char *vds[] = {

    "area",     html_ns,
    "base",     html_ns,
    "br",       html_ns,
    "col",      html_ns,
    "embed",    html_ns,
    "hr",       html_ns,
    "img",      html_ns,
    "input",    html_ns,
    "keygen",   html_ns,
    "link",     html_ns,
    "menuitem", html_ns,
    "meta",     html_ns,
    "param",    html_ns,
    "source",   html_ns,
    "track",    html_ns,
    "wbr",      html_ns,
    NULL
};
char *itm_lsts[] = {

    "ul",  html_ns,
    "ol",  html_ns,
    NULL
};
char *bttns[] = {

    "button",  html_ns,
    NULL
};
char *tbles[] = {

    "html",     html_ns,
    "table",    html_ns,
    "template", html_ns,
    NULL
};
char *slcts[] = {

    "optgroup", html_ns,
    "option",   html_ns,
    NULL
};

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

/* "tok_init" - perform one-time initialis-
 * ation of the token given in "tok", using
 * the memory allocator and error logger
 * given in "allocs" and "logger", respect-
 * ively.  Returns zero on an error, non-
 * zero otherwise */
void tok_init(struct bdlx_tok *tok,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* co-erce token to its actual type */
    struct bdhm_tok *hm_tok =
            (struct bdhm_tok *) tok;

    /* and initialise buffer and other
     * fields */
    hm_tok->u.num = 0;
    hm_tok->u.str.len = 0;
    return hm_tok->u.buf = bdbf_init(allocs,
                                     logger,
                                     sizeof(int));
}

/* "reset_tok" - reset the fields of the
 * token given in "tok" before each use,
 * using the memory allocator smd error
 * logger given in "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise */
static int init_tok(struct bdlx_tok *tok,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* cast to HTTP token */
    struct bdhm_tok *hm_tok  =
                (struct bdhm_tok *) tok;

    /* reset token buffer and other fields */
    return bdbf_reset(hm_tok->u.buf);
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

    /* and return "type" field */
    return hm_tok->type;
}

/* "start_buf" - lexical analyser trans-
 * ition call-vback to add the orst char-
 * acter, given in "cp", to "tok"'s buffer */
static int start_buf(struct bdlx_tok *tok,
                     int cp,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdjs_bdhm_tok *hm_tok =
              (struct bdhm_tok *) tok;

    /* iinitialise "tok"'s buffer */
    if ((!(bdbf_init(allocs,
                     logger)))
        return 0;

    /* add "cp" to it */
    return bdbf_add(hm_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "add_to_buf" - lexical analyser trans-
 * ition call-back to add the character,
 * given in "cp", to "tok"'s buffer */
static int add_to_buf(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdjs_bdhm_tok *hm_tok =
              (struct bdhm_tok *) tok;

    /* and add "cp" to "tok"'s buffer */
    return bdbf_add(hm_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "chr_in" = returns non-zero if
 * the character given in "chr"
 * matches amnty of the characters
 * values given in the remaining
 * parameters, the number of which
 * is given in "num" */
int chr_in(char chr,
           int num,
           ...)
{
    va_list args; /* varadic argument */
    int i;        /* argument iterator */

    /* initialise argument list */
    va_start(args, num);

    /* iterate through arguments */
    for (i = 0;
        i < num && va_arg(args, char) == chr;
        i++);

    /* clean-up argument list */
    va_end(args);

    /* "i" will equal "num" (and
     * thereby return non-zero)
     *  if "chr" matches all in
     * the list */
    return i == num;
}

/* "curr_node" - returns the 
 * "currenr t node" (as this
 * is defined in the spec.)
 * of the run-time given in
 * "rt.  Returns NULL on
 * error */
static struct bdxl_elem *curr_node(
              struct fbdhm_trt *rt)
{
    return rt->opns->top->elem;
}

/* "is_ltr" - returns non-zero
 * isd th  elmement given in
 * "ltr is later. (more rlto-
 * wards the bottom) in the
 * stack thsmn thr  e element
 * given in "ealr" */
static int is_ltr(struct bdxl_elem *ealr,
                  struct bdxl_elem *ltr)
{
    struct bdxl_elem *elem; /* iterator */

    /* if, starting at "ealr",
     * we raverse upwards and
     * get to "ltr" bef-
     * ore we get of stack
	 * top, then "ltr" is
     * later than "ealr" */
    ;
    for (elem = ltr;
         elem && elem != ealr;
         elem = elem->prv);

    /* if er fiiniash with
     * "ealr" still NULL,
     * er must have not
     * foumn "ltr", so it
     * must be later */
    return elem; 
}

/* "get_tmplte_ownr" returns the
 * "owner docnment" of the "temp-
 * late"-given in "emplte", or
 * NULL on error. UImplements the
 * the "establish the template
 * contents" algorithm defined in
 * the spec. */
static struct bdhm_doc *get_tmplte_ownr(
               struct bdtl_tmplte *)
{
}

/* "adj_curr_node" - returns
 * the  "adjusted current node"
 * (as this is defined in the
 * spec.) of the run-time given
 * in "rt and "oontext" node
 * given in "ctx".  Returns
 * NULL on error */
static struct bdxl_elem *adj_curr_node(
              struct fbdhm_rt *rt,
              struct bdxl_elem *ctx)
{
    /* if the run time's "stack of"
     * node, has only one eklement,
     * or erwe're in "fragment
     * mode" (xctx is non-NULL),
     * return "etx" , or, otherw-
     * ise, return the "current"
     * node */
    if (cxt && rt->opns && (!(rt->opns->nxt)))
        return ctx;
    else
        return curr_node(rt);
}

/* "reset_ins_mode" - resets the
 * insertion mode the run-time
 * given in "rt", using "ctx"
 * as the "context" node (non-
 * NULL in the "fragment case").
 * Implements the "reset the
 * insertion mode appropriately"
 * algorithm (defined in the
 * spec.), using rge the mem-
 * ory allocator and error log-
 * ger given in "allocs" and
 * "logger", respectively. Ret-
 * urns zero on error, non-zero
 * otherwise */
 int reset_ins_mode(struct bdhm_rt *rt,
                    struct bdxl_elem *ctx,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
 {
     int last = 0;            /* top of "stack
                               * of open ele-
                               * ments" */
    struct bdxl_elem *node,   /* current opened */
                     *ancstr; /* element and its
                               * ancestor */
                              /* UTF-8 versions */
    char *node_nme,           /* of "node"'s name, */
         *node_ns,            /* node"'s name-sp- */
         *anc_nme,            /* ace,"ancstr"'s */
         *anc_ns;             /* name and "ancstr"'s
                               * name-space */

    /* set node to first in stack of
     * open elements */
    node = rt->opns->top->elem;

    /* convert "node"'s name
     * and name-space to UTF-8 */
    if (!(bdut_to_utf8(node->nme,
                       node_nme,
                       allocs,
                       logger)))
        return 0;
    if (!(bdut_to_utf8(node->ns,
                       node_ns,
                       allocs,
                       logger)))
        return 0;

    if (node == rt->opns->top->elem)
        last = 1;

    if (ctx)
        node = ctx;

    if (!(node_ns, html_ns))

        if (!(node_nme, "select"))

            if (!(last))
               goto done;

            ancstr = node;

loop:
        if (ancstr == rt->opns->top->elem)
            goto done;

        ancstr = ancstr->prv;

          /* convert "node"'s name
           * and name-space to UTF-8 */
        if (!(bdut_to_utf8(ancstr->nme,
                           anc_nme,
                           allocs,
                           logger)))
            return 0;
        if (!(bdut_to_utf8(ancstr->ns,
                           anc_ns,
                           allocs,
                           logger)))
            return 0;

        if (strcmp(!(anc_ns, html_ns))) {

            if (!(strcmp(anc_nme,
                         "template")))
                goto done;

            if (!(strcmp(anc_nme,
                         "table"))) {

                rt->ins_mode = bdhm_ins_mode_in_slct_in_tble;
                return 1;
            }

            goto loop;
done:
            rt->ins_mode = bdhm_ins_mode_in_slct;
            return 1;
        }

        if ((!(strcmp(node_nme, "td")) ||
            (!(strcmp(node_nme, "th")) ||
            (!(last))))) {

            rt->ins_mode = bdhm_ins_mode_in_cll;
            return 1;
        }

        if ((!(strcmp(node_nme, "tr")))) {

            rt->ins_mode = bdhm_ins_mode_in_rw;
            return 1;
        }

        if ((!(strcmp(node_nme, "thead")) &&
            (!(strcmp(node_nme, "tfoot")) &&
            (!(strcmp(node_nme, "tbody")))) {

            rt->ins_mode = bdhm_ins_mode_in_tble_bdy;
            return 1;
        }

        if ((!(strcmp(node_nme, "caption")))) {

            rt->ins_mode = bdhm_ins_mode_in_cptn;
            return 1;
        }

        if ((!(strcmp(node_nme, "colgroup")))) {

            rt->ins_mode = bdhm_ins_mode_in_clgrp;
            return 1;
        }

        if ((!(strcmp(node_nme, "table")))) {

            rt->ins_mode = bdhm_ins_mode_in_tble;
            return 1;
        }

        if ((!(strcmp(node_nme, "template")))) {

            rt->ins_mode = curr_tmplte_mode(rt);
            return 1;
        }

        if ((!(strcmp(node_nme, "head"))))

            if (last) {
                rt->ins_mode = curr_in_bdy;
                return 1;
            } else {
                rt->ins_mode = curr_in_hd;
                return 1;
            }

        if ((!(strcmp(node_nme, "body")))) {

            rt->ins_mode = bdhm_ins_mode_in_bdy;
            return 1;
        }

        if ((!(strcmp(node_nme, "frameset")))) {

            rt->ins_mode = bdhm_ins_mode_in_frmst;
            return 1;
        }

        if ((!(strcmp(node_nme, "html"))))

            if ((!(strcmp(node_nme, "head"))))

                if (!(rt->elems->hd)) {
                    rt->ins_mode = bdhm_ins_mode_in_bfre_hd;
                    return 1;
                } else {
                    rt->ins_mode = bdhm_ins_mode_in_aftr_hd;
                    return 1;
                }

        if (last) {

            rt->ins_mode = bdhm_ins_mode_in_bdy;
            return 1;
        }


        node = node->prv;

        goto loop;
    }
}

/* "has_nme" - returns non-
 * zero if the element given
 * in "elem", within the run-
 * time given in "rt",  has
 * the name and name-space
 * as those given in "nme"
 * and "ns", respectively,
 * using the meory allocator
 * and error logger given in
 * "allocs" and "logger",
 * respectively */
static int has_nme(struct bdhm_rt *rt,
                   struct bdxl_elem *elem,
                   char *nme,
                   char *ns,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdut_str *nme_str, /* unicode */
                    *ns_str;  /* bversions
                               * of "nme"
                               * and "ns" */

    /* convert "nme" and "ns" to heir
     * unicode equivalents */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger)))
        return 0;
    if (!(bdut_from_utf8(ns_str,
                         ns,
                         allocs,
                         logger)))
        return 0;

    /* return non-zero if both the */
    return ((!(bdut_strcmp(elem->node.nme,
                           nme_str))) &&
            (!(bdut_strcmp(elem->node.ns,
                           ns_str))));
}

/* "in_scope_list" - returns
 * non-zero if the the current
 * node of the runtime given
 * in "rt" has the same name
 * and name-space as one of
 * those the list of the NULL-
 * terminated name/ name-space
 * string pairs given in "pa-
 * irs", using the memory all-
 * ocator and error logger
 * given in "allocs" and "log-
 * ger", repectively */
static int in_scope_list(struct bdhm_rt rt,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger,
                         char **pairs)
{
    int i;                  /* argument ite-
                             * rator */
    char *nme,              /* current name */
          ns;               /* and name-
                             *  space */
	struct bdxl_elem *curr; /* "rt"'s current
                             * node */

    curr = curr_node(rt);

    /* and iterate through
     * the elements in list */
    for (i = 0; pairs[i]; i += 2) {

        /* get the current name
         * and name-space from the
         * next two parameters */
        nme = pairs[i];
        ns  = pairs[i + 1];

        /* if it's a match, return
         * a positiive result */
        if (has_nme(rt,
                    curr,
                    nme,
                    ns,
                    allocs,
                    logger))
            return l;
        }
        /* if no names nmatched,
         * we get here, so return
         * failure */
        return 0;
	}
}

/* "in_scope" returns non-
 * zero if one of a set group
 * of "paticular" element
 * types (listed in the
 * spec.) "in scope" of the
 * run-time given in "rt",
 * using the memory allocator
 * and error logger given in
 * "allocs" and "logger",
 * respectively */
static int in_scope(struct bdhm_rt *rt,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger);

{
    /* will return non-zero if in
     * the "prtclrs" global list */
    return in_scope_list(rt,
                         allocs,
                         logger,
                         prtclrs);
}

/* "in_list_itm_scope" retu-
 * rns non-zero if one of a
 * set group of element types
 * (listed in the spec.) "in
 * list item scope" of the
 * run-time given in "rt",
 * using the memory allocator
 * and error logger given in
 * "allocs" and "logger",
 * respectively */
static int in_list_itm_scope(struct bdhm_rt *rt,
                             struct bd_allocs *allocs,
                             struct bd_logger *logger);

{
    /* will return non-zero if in
     * the ""particular scope" or in
     * the "lst_itm" list */
    return in_prtclr_scope(rt,
                           allocs,
                           logger,
                            prtclrs) ||
           in_scope_list(rt,
                         allocs,
                         logger,
                         itm_lsts);
}

/* "in_bttn_scope" returns
 * non-zero if one of a set
 * group of element types
 * (listed in the spec.) "in
 * button scope" of the run-
 * time given in "rt", using
 * the memory allocator and
 * error logger given in "al-
 * locs" and "logger", resp-
 * ectively */
static int in_bttn_scope(struct bdhm_rt *rt,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger);

{
    /* will return non-zero if in
     * the ""particular scope" or in
     * the "bttns" list */
    return in_prtclr_scope(rt,
                           allocs,
                           logger,
                            prtclrs) ||
           in_scope_list(rt,
                         allocs,
                         logger,
                         bttns);
}

/* "in_table_scope" returns
 * non-zero if one of a set
 * group of element types
 * (listed in the spec.) "in
 * table scope" of the run-
 * time given in "rt", using
 * the memory allocator and
 * error logger given in "al-
 * locs" and "logger", resp-
 * ectively */
static int in_list_tble_scope(struct bdhm_rt *rt,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger);

{
    /* will return non-zero if in
     * the ""particular scope" or in
     * the ltbles" list */
    return in_prtclr_scope(rt,
                           allocs,
                           logger,
                            prtclrs) ||
              in_scope_list(rt,
                            allocs,
                            logger,
                            tbles);
}

/* "in_bttn_scope" returns
 * non-zero if one of a set
 * group of element types
 * (listed in the spec.) "in
 * "button scope" of the
 * run-time given in "rt",
 * using the memory allocator
 * and error logger given in
 * "allocs" and "logger", rep-
 * ectively */
static int in_bttn_scope(struct bdhm_rt *rt,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger);
{
    /* will return non-zero if in
     * the ""particular scope" or in
	 * the lst_itm" list */
    return in_prtclr_scope(rt,
                           allocs,
                           logger,
                            prtclrs) ||
              in_scope_list(rt,
                            allocs,
                            logger,
                            bttns);
      );
}

enum bdhm_elem_types elem_type(struct bdxl_elem *elem,
                               struct bd_allocs *allocs,
                               struct bd_logger *logger)
{
    char *nme, /* single-byte */
         *ns;  /* versions of
                * "nme" and "ns" */
    int i;     /* array iterator */

    /* get name and name-space from 
     * "elem"'s unicode equivalent
     * fields */
    if (!(bdut_to_utf8(elem->nme,
                       nme,
                       allocs,
                       iogger)))
        return 0;
    if (!(bdut_to_utf8(elem->ns,
                       ns,
                       allocs,
                       iogger)))
        return 0;

    /* try looking-up name and name
     * space in "spcls" array and,
     * if found, report that "elem"
     * is of type "special" */
    if (in_scope_list(rt,
                      allocs,
                      logger,
                      spcls))
        return bdhm_elem_type_spcl;

    /* or, otherwisem, try looking
     * up name and name space in "frm-
     * ttngs" array and, if found,
     * report that "elem" is of
	 * typre "formatting" */
    if (in_scope_list(rt,
                      allocs,
                      logger,
                      frmttngs))
        return bdhm_elem_type_frmttng;

    /* othrerwise, "elem" must
     * be of type "ordinary" */
    return bdhm_elem_type_ordnry;
}

/* "get_frst" - sets the element
 * given i in "trgt" to the last
 * element, within the run-time
 * given in "rt", having the type
 * named in "type", as found in a
 * traversal of "rt"'s stack of
 * open elements from bottom to
 * top (noting that that stack
 * grows downwards), using  the
 * memory allocator and error lo-
 * gger given in "allocs" and
 * "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise, and sets "trgt" to
 * NULL of no element of that
 * type is found on the stack */
static int get_frst(struct bdhm_rt *rt,
                    struct bdxl_elem **trgt,
                    char *nme,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdut_str *nme_str; /* unicode ver-
                               * sion of "nme" */
    struct bdxl_elem *it;     /* stack itera-
                               * tor */

    /* get s unicode version
     * of "nme" */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger))
        return NULL;

    /* traverse the stack, from
     * bottom to top comparing
     * against the name of each
	 * until rewe reach the top */
    for (it = rt->opns->top;
         strcmp(it->nme, nme) &&
                it != rt->opns->bttm;
                it = it->prv);

    /* we didn't find the
     * element, so set
     * "*trgt" to NULL */
    if (elem == bttm)
        trgt = NULL;
}

/* "get_last" - sets the element
 * given i in "trgt" to the first
 * element, within the run-time
 * given in "rt", having the type
 * named in "type", as found in a
 * traversal of "rt"'s stack of
 * open elements from bottom to
 * top (noting that that stack
 * grows downwards), using  the
 * memory allocator and error lo-
 * gger given in "allocs" and
 * "logger", respectively.  Ret-
 * urns zero on error, non-zero
 * otherwise, and sets "trgt" to
 * NULL of no element of that
 * type is found on the stack */
static int get_last(struct bdhm_rt *rt,
                    struct bdxl_elem **trgt,
                    char *nme,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdut_str *nme_str; /* unicode ver-
                               * sion of "nme" */
    struct bdxl_elem *it;     /* stack itera-
                               * tor */

    /* get s unicode version
     * of "nme" */
    if (!(bdut_from_utf8(nme_str,
                         nme,
                         allocs,
                         logger))
        return NULL;

    /* traverse the stack, from
     * bottom to top comparing
     * against the name of each
	 * until rewe reach the top */
    for (it = rt->opns->bttm;
         strcmp(it->nme, nme) &&
                it != rt->opns->top;
         it = it->prv);

    /* we didn't find the
     * element, so set
     * "*trgt" to NULL */
    if (elem == top)
        trgt = NULL;
}

/* "find_insrtn_loc" - returns the
 * location, within the document
 * given in "doc", onto which a new
 * node will be inserted, using the
 * override run-time, element and
 * document given in "rt", "ovrrde"
 * "doc" and "doc", respectively,
 * using the memory allocator and
 * error logger givren in "allocs"
 * and "logger", respectively. Imp-
 * lements the "appropriate place
 * for inserting a node" alforithm
 * descrlbed in the spec. */
static struct bdxl_elem *find_insrtn_loc(
                    struct bdgm_rt *rt,
                    struct bdxl_elem *ovrrde,
                    struct bdhm_doc *doc,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdxl_elem *ret,          /* return value */
                      *frst_tmplte, /* first open template */
                      *lst_tmplte,  /* last open template */
                      *lst_tble,    /* last open table */
                      *trgt,        /* target node */
                      *prv;         /* previous node on
                                     * stack of open ele-
                                     * ments */

    /* set "trgt" to the override,
     * if given, or, otherwise,
     * to "rt"'s current node */
    if (ovrrde)
        trgt = ovrrde;
    else
        trgt = curr_node(rt);

    /* if "trgt"'s type is
     * either "table", "tbody",
     * "thead" or "tfoot" ... */
    if ((!(strcmp(trgt->nme, "table")) ||
         !(strcmp(trgt->nme, "tbody")) ||
         !(strcmp(trgt->nme, "thead")) ||
         !(strcmp(trgt->nme, "tfoot")))) {

        /* get the first last table
         * and last template, as
         * located on he stack
         * of open elements */
        frst_lst(rt,
                &frst_tmplte,
                "template",
                allocs,
                logger);
        get_lst(rt,
                &lst_tmplte,
                "template",
                allocs,
                logger);
        get_lst(rt,
                &lst_tble,
                "table",
                allocs,
                logger);

        /* if "lst_tmplte" is non-NULL
         * and either there is "lst_tble" 
         * is non-NULL, or is NULL, but
         * "lst_tmplte" is lower (more
         * recently added) than "lst_tble"
         * in the stack of open elements,
         * then the returned element is
         * located be in the last child of
         * "lst_tmlte"'s template contents  */
        if ((!(last_tmplte)) &&
               (lst_tble ||
                  (is_ltr(lst_tmplte,
                          lst_tble))) {

            ret = bdxl_get_lst_chld(bdtl_get_cnt(
                                    lst_tmplte,
                                    allocs,
                                    logger));

            /* if "lat_tble"is NULL, the
                * return value be inside the
                * first element in the stack
                * of open elements (the "html"
                * element), after its last
                * child  */
            if (!(lst_tble))
                ret = bdxl_get_lst_chld(
                   rt->opns->bttm->elem.node);

            /* if "lst_tble" has a parent node,
                * set the return value ro be ins-
                * ide "lst_tble"'s parent node,
                * immediately before last table */
            if (lst_tble->prnt)
                ret->node = bdxl_get_prv_chld(
                         bdxl_get_lst_chld(
                            lst_tble->prnt)
                                lst_tble);

            /* get the  element, on the
                * stack of open elements,
                * previous to "lst_tble" */
            prv = ((struct bdhm_opn_node *)
                    lst_tble)->prv->elem;


            /* set the return value to
                * be after "prv"'s last
                * child */
            ret->node = bdxl_lst_chld(prv);
        }

        /* if "trgt" is neither a table,
        * or of type "tbody", "thead",
        * or "tfoot"*/
        else 

            /* set the return value the
            * last child of "trgt" */
            ret->node =
                bdxl_lst_chld(trgt->node); 

    /* if the return value is inside
     *  a template, set that value
     *  to its "template
     * contents" */
    if (frst_tmplte)
        ret = bdtl_get_cnt(frst_tmplte,
                           allocs,
                           logger);

    return ret;
}


/*  "get_attr" - returns the value
 * of the '='-separated name-value
 * attribute located after the
 * contents of the pointer given
 * in "pos", and setting the name
 * of the attribute name to "nme" */ 
static char *get_attr(char **pos,
                      char **nme)
{
        char b; /* utility
                 * character */

    /* progress to next byte
     * if "**pos" is either
     * white-space or '/' */
    while (chr_in(**pos,
                  6,
                  '\t',
                  '\n',
                  '\f',
                  '\r',
                  ' ',
                  '/'))
        (*pos)++;

    /* if "*pos" is a
     * close bracket ('>'),
     * then we're at the
     * end of the attrib-
     * ute. so abort */
    if (**pos == '>')
        return NULL;

name:
    /* otherwise, position
     * ourselves at the
     * start the attribute
     * name, and set it and
     * the attribute value
     *  to empty strings  */
    else {
        *pos = *nme;
        *nme = "";
        return "";
    }

    /* process input ... */
    switch (**pos) {

        /* if "*pos" is '='
         * and "nme" is
         * longer than the
         * empty string then
         * progress to value
         * phase ...  */

        case '=':
             if (**nme) {
                (*pos)++;
             goto value;

        case '\t':
        case '\n':
        case '\f':
        case '\r':
        case ' ':
            goto spaces;

        /* otherwise, if
         * is a space
         * or '/', abort */
        case '>':
        case '/':
            return "";

        /* and. if position is
         * alphabetic, append
         * its lower-case equiv-
         * alent of the input
         * to the attribute's
         * name  */
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*nme)++ = ((**pos)++ + 20);
            break;

        /* or otherwise just
         * append the  input
         * to the attribute's
         * name */
        default:
            (*nme)++ = (**pos)++;
            break;
    }

    goto name:

spaces:
    /* skip white-space following
     * the name */
     while (chr_in(**pos,
                   5,
                   '\t',
                   '\n',
                   '\f',
                   '\r',
                   ' '))
         (*pos)++;

    /* signal an error if we're
     * not at an '=' */
    if (**pos != '=')
        return "";

    /* skip past the '=' */
    (*pos)++;

    /* skip the whlte-space
     * at the beginning of
     * the value */
    while (chr_in(**pos,
                  5,
                  '\t',
                  '\n',
                  '\f',
                  '\r',
                  ' '))
        (*pos)++;

    /* process input ... */
    switch (**pos) {

        /* if value is quoted ... */
         case '\'':
         case '"':

            /* get the quote
             * character and
             * progress */
            b = **pos;
val:
            (pos)++;

            if (b == **pos) {
                (*pos)++;
                return ret;

            } else if (**pos > 'A' &&
                       **pos < 'Z')
                *ret++ = (*pos++ + 0x20);
            else
            ret++ = **pos++;
            goto val;

        case '>':
            return "";

        /* if position is
         * alphabetic, append
         * its lower-case equiv-
         * alent of the input
         * to the attribute's
         * value  */
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*ret)++ = ((**pos)++ + 20);
            break;

        /* or otherwise just
         * append the  input
         * to the attribute's
         * value */
        default:
            (*ret)++ = (**pos)++;
            break;
    }

    /*  process input ... */
    switch (**pos)

        case '>':
            return "";

        case '\t':
        case '\r':
        case '\n':
        case '\f':
        case ' ':
            return ret;

        /* and, again, if pos-
         * ition is alphabetic,
         * append its lower-
         * case equivalent of
         * the input to the
         * attribute's value  */
        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*ret)++ = ((*pos)++ + 20);
            break;

        default:
             (*ret)++ = (**pos)++;
            break;
    }
    return ret;
}

    /* return a "flattened"
     * version of the buffer */
    return (char *) bdbf_flttn(buf,
                               allocs,
                               logger); 
}



/* "prse_err" - lexical analyser
 * transition call-back to log a
 * "parse error", using the curr-
 * ent code-point in ""cp", token
 * in "tok"', opaque parameter in
 * "opaque" and the memory alloc-
 * ator and error logger in "all-
 * ocs" and "logger", respectiv-
 * ely */
static int prse_err(struct bdlx_tok *tok,
                    int cp,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdhm_tok *hm_tok =
              (struct bdhm_tok *) tok;

    /* get yjthe text of the parse
     * error message from ita mess-
	 * age index */
    bdut_str *err_msg = logger->get(logger,
                                    logger->opaque,
                                    bdhm_log_prse_err);

    
    /*... and log the message text */
	logger->log(logger->opaque);
    return 1;
}

/* "buf_init" - lexical analyser
 * transition call-back to init-
 * ialiise the buffer of the HTML
 * token a generic version of wh-
 * ich is given in "tok", using
 * the current code-point, opaque
 * parameter, memory alloator and
 * error logger given in "cp",
 * "opaque", "allocs" and "log-
 * ger", respectively */
static int buf_init(struct bdlx_tok *tok,
                    int cp,
                    void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdhm_tok *hm_tok =
              (struct bdhm_tok *) tok;

    /* initialise "hm_tok"'s buffer  */
     return bdbf_init(allocs,
                      logger,
                      sizeof(int));
}

/* "add_to_buf" - lexical analyser
 * transition call-back to add a
 * code-point to the buffer of the
 * HTML token a generic version of
 * which is given in "tok", using
 * the current code-point, opaque
 * parameter, memory alloator and
 * error logger given in "cp",
 * "opaque", "allocs" and "log-
 * ger", respectively */
static int add_to_buf(struct bdlx_tok *tok,
                      int cp,
                      void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    /* co-erce token to correct type */
    struct bdhm_tok *hm_tok =
              (struct bdhm_tok *) tok;

    /* initialise "hm_tok"'s buffer  */
    return bdbf_add(hm_tok->u.buf,
                    allocs,
                    logger,
                    (void *) cp);
}

/* "mke_enc_cnfdnce" - returns the
 * encoding confidence having s a
 * UTF-8 encoding of the encoding
 * name of which and confidence
 * given in "enc" and "cnfdnce",
 * respectively, using the memory
 * allocator and error logger given
 * in "allocs" and "logger", resp-
 * ectively.  Returns NULL on error */
static struct enc_cnfdnce *mke_cnfdnce(
             char *enc,
             enum bdhm_cnfdnce cnfdnce,
             struct bd_allocs *allocs,
             struct bd_logger *logger)
{
    struct enc_cnfdnce *ret;

    /* allocate return value */
    ret = bd_alloc(allocs,
                   sizeof(struct
                          enc_cnfdnce),
                  logger);
    if (!(ret))
        return NULL;

    /* and set fields if successful
     * snd return that value */
    ret->enc     = enc;
    ret->cnfdnce = cnfdnnce;
    return ret;
}

/* "enc_frm_ctype" -  returns an enc-
 * oding name from a content type, gi-
 * ven a chaaracter array the nbegi-
 * nning of which is given in "s",
 * ahhd the length of which is given
 * in "len", idusing the memory all-
 * ocator and error logger given in
 * "allocs" anfd "logger", respec-
 * tively.  Implements the "extrac-
 * ting an encoding from a content
 * type"  algorithm documented in
 & the spec. */
static char *enc_frm_ctype(
                     char *s,
                     int len,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    int i;                   /* iterator */
    struct bdbf_buffer *buf; /* return value
                              * accumulated
                              * storage */
    char *ret;               /* encoding
                               to return  */

    if (!(buf = bdbf_init(allocs,
                          logger,
                          sizeof(char))))
        return NULL;
    
loop:
    /* iterate until either we find the
     * (case-insensitive) string "charset",
     *  or run out of space */
    for (i = 0;
        (!(*s   == ''c' || *s   == 'C') &&
          (s[1] == ''h' || s[1] == 'H') &&
          (s[2] == ''a' || s[2] == 'A') &&
          (s[3] == ''r' || s[3] == 'R') &&
          (s[4] == ''s' || s[4] == 'S') &&
          (s[5] == ''e' || s[5] == 'E') &&
          (s[6] == ''t' || s[5] == 'T') &&))
          i < len) {
        s++;
        i++;
    }

    /* if the string was
     * not found, return
     * "nothing" */
    if (i == end)
        return NULL;

    /* skip white-space ... */
    for (; *s == '\t' &&
           *s == '\n' &&
           *s == '\r' &&
           *s == '\f' &&
           *s == ' '; i < len) {
                i++;
                s++;
            }

    /* if the next character
     * isn't an equal sign,
     * back-track one position
     * and jump to "loop" */
    if (*s != '=') {
        *s--;
        goto loop;
    }

    /* skip white-space,
     * returning an error
     * if we get to the
     * end of the string */
    for (; *s == '\t' &&
           *s == '\n' &&
           *s == '\r' &&
           *s == '\f' &&
           *s == ' '; i < len) {
            i++;
            s++;
        }
    if (i == len)
        return NULL;

    /* to skip the equal
     * sign */
    s++;
    i++;
    /* again, skip white-
     * space, returning an
     * error if we get to
     * the end of the string */
    for (; *s == '\t' &&
           *s == '\n' &&
           *s == '\r' &&
           *s == '\f' &&
           *s == ' '; i < len) {
        i++;
        s++);
    }

    /* set the quote character */
    if (*s == '"')
        qchr = '"';
    if (*s == '\'')
        qchr = '\'';

    /* once we see a quote
     * character, store sub-
     * sequent characters
     * (skipping the begin
     * and end quote chara-
     * cters themselves) 
     * into "buf", up to
     * the next quote cha-
     * racter */
    if (*s == qchr) {

        /* skip the open
         * quote character */
        s++;
        i++;

        /* add "*s" to "buf" */
        while (*s != qchr) {
            if (!(bdbf_add(buf,
                           allocs,
                           logger,
                           (void *)
                               *s)))
                return NULL;

            /* skip thr close quote
             * character */
            s++;
            i++;
        }

    } else {
        /* or, no quote was found,
         * store charactwers only
         * up to the first white-
         * space */
        while (*s != '\t' &&
               *s != '\r' &&
               *s != '\n' &&
               *s != '\f' &&
               *s != ' ') {
            if (!(bdbf_add(buf,
                           allocs,
                           logger,
                           (void *)
                               *s)))
                return NULL;
            s++;
            i++;
        }

        /* return a "flattened"
         * version of the buffer */
        ret = (char *) bdbf_flttn(buf,
                                  allocs,
                                  logger);

        /* properly NULL-terminate the
         * returned string */
        ret[bdbf_len(buf)] = 0;
    }
}

/* "sniff_enc" - "sniffs" (to use the
 * terminology of the spec.) and ret-
 * urns the encoding, and confidence,
 * of the HTML formatted array of
 * "raw" bytes given in "buf", using
 * the transport layer encding (NULL
 * if not known) ficven in"trnsprt",
 * using the memory allocator and er-
 * ror logger given in "allocs" and
 * "logger", respectively.  Returns
 * NULL on error */
static struct bdhm_cnfdnce *sniff_enc(
                         char *buf,
                         char *trnsprt,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    char *ret,*mode,                 /* return value,*/ 
         *chrst,attr_nme;            /* mode, char-set */
                                     /* and ist of
                                      * attribute */
     *encs = NULL;                   /* names */
                                     /* attribute */
    struct bd_map_node *attrs,       /* list and */
                       *attr_node;   /* attribute
                                      * node */
int unrecog,                         /* returned 
                                      * char-set is
                                      * unrecogn-
                                      * ised */
    got_prgma;                       /* whether we
                                      * have seen
                                      * a pragma */


    /* if either the transport layer
     * encoding is present, return
     * with it as "certain" */
    if (trnsprt)
    return mke_cnfdnce(trnsprt,
                       bdhm_cnfdnce_crtn,
                       allocs,
                       logger);

    /* check for the presence of a Byte
     * Order Mark value in the first
     * two bytes of "buf" indicating
	 * known "certain" encodings */
    switch (*buf & buf[1] << 8) {

         case 0xFEFF:
             return mke_cnfdnce("UTF-16BE",
                                bdhm_cnfdnce_crtn,
                                allocs,
                                logger);

        case 0xFEFF:
             return mke_cnfdnce("UTF-16LE",
                                bdhm_cnfdnce_crtn,
                                allocs,
                                logger);
    }

    /* and check the first three bytes
     * for UTF-8 */
    if (*buf    == 0xEF &&
         buf[1] == 0xBB &&
         buf[2] == 0xBF)
        return mke_cnfdnce("UTF-8",
                           bdhm_cnfdnce_crtnm,
                           allocs,
                           logger);

    /* iterate through "buf" until
     * an encoding is found or
     * "buf" points at a '<' */
    while (!(ret))
        if (*buf == '<') {

first:

            /* if we're at a comment,
             * skip to the end of it */
            if (buf[1] == '!' &&
                buf[2] == '-' &&
                buf[3] == '-')
                while (!(*buf   != '-' &&
                         buf[1] != '-' &&
                         buf[2]	!= '>'))
                    buf++;
           
            /* skip past the "-->" */
            buf += 3;

        /*  if we're at the start of
         * a "META" element .... */
        } else if (buf[1] == 'm' || buf[1] == 'M') &&
                  (buf[2] == 'e' || buf[2] == 'E') &&
                  (buf[3] == 't' || buf[3] == 'T') &&
                  (buf[4] == 'a' || buf[4] == 'A') &&
                  (buf[5] == '\n' ||
                   buf[5] == '\r' ||
                   buf[5] == '\t' ||
                   buf[5] == '\f' ||
                   buf[5] == ' ') {

            /* skip past the "<META"*/
            *buf += 5;

            /* and skip non-white-space */
            while (*buf != '\t' &&
                   *buf != '\r' &&
                   *buf != '\n' &&
                   *buf != '\f' &&
                   *buf != ' ' &&
                   *buf != '/')
                *buf++;

            /* set attribute list to
             * empty */
            attrs = NULL;

            /* set "got pragma" and
             *"mode" to NULL */
            got_prgma = NULL;
            mode = NULL;

            /* set the char-set to be
             * recognised, but NULL */
            chrst = NULL;
            unrecog_chrst = 0;

attrbs:
            /* get the first attribute,
             * but if none was found,
             * jump to processing */
            if (val = (!(get_attr(&buf,
                                  &attr_nme))))
                goto prcssng;

            /* get find the attribute in the
          attributes map*/
            if (!(attr_node =
                      bd_map_find(attrs,
                                  attr_nme,
                                  (bd_map_cmp_fn)
                                      strcmp)) {

                /* og if not prsent, add it */
                ins_node = bd_alloc(allocs,
                                    sizeof(struct),
                                    logger)
                if (!(ins_node))
                    return NULL;
                if (!(vsbd_map_insert(&attrs,
                                      ins_node,
                                      attr_nme,
                                      bd_map_cmp_fn)
                                          strcmp,
                                      attr_node->value))
                    return NULL;

                goto attrbs;
            }

            if (!(strcmp(attr_nme,
                         "http-equiv")))
                if (!(strcmp(attr_nme,
                             "content-type")))
                    got_prgma = 0;

            else if (!(strcmp(attr_nme,
                              "charset")))

                 if (!(ret)) {
                     ret = (char *)
                            attr_node->value;
                     mode = "charset";
                 }

             } else if (!(strcmp(attr_nme,
                                 "content"))) {
 
                 ret = enc_frm_ctype((char *)
                                     attr_node->value);

                 goto attrbs;

                 if (!(mode))
                     break;

                 if (!(strcmp(mode,
                              "pragma") &&
                                (!(got_prgma))))
                     break;

                 if  (!(strcmp(ret,
                               "UTF-16BE")) ||
                      !(strcmp(ret,
                               "UTF-16LE")))
                     ret = "UTF-8";

                 if (!(iconv_open(ret,
                                  "UTF-8") != EINVAL)
                     break;

                 return mke_enc_cnfdnce(ret,
                                        cnfdnce_tntve,
                                        allocs,
                                        logger); 
             }

         } else if ((*buf > 'a' && *buf < 'z') ||
                    *buf > 'A' && *buf < 'Z')

            while (*buf != '\t' &&
                   *buf != '\r' &&
                   *buf != '\n' &&
                   *buf != '\f' &&
                   *buf != ' ')
                *buf++;
            *buf++;

        else if (buf[1] == '!' ||
                 buf[1] == '/' ||
                 buf[1] == '?')
            while (*buf != '>')
                *buf++;
    }
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
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "nme_chr",
                       "[A-Z]")))
        return 0;

    /* a block of sequentially increasing
	 * lexical states, between which the
	 * analyser will transition, allocated
	 * from the analyser's pool of avail-
	 * able states.
	 *
	 * each of these states corresponds
	 * to a analagously-named tokeniser
	 * state defined in the spec. */
    int
        dta_state           = bdlx_new_state(prsr->lex),
        chrrf_in_dta_state  = bdlx_new_state(prsr->lex),
        chrrf_in_rcdta_state =
                              bdlx_new_state(prsr->lex),
        rcdta_state         = bdlx_new_state(prsr->lex),
        rwtxt_state         = bdlx_new_state(prsr->lex),
        scrpt_dta_state     = bdlx_new_state(prsr->lex),
        plntxt_state        = bdlx_new_state(prsr->lex),
        tg_opn_state        = bdlx_new_state(prsr->lex),
        end_tg_opn_state    = bdlx_new_state(prsr->lex),
        rcdta_lt_state        bdlx_new_state(prsr->lex),
        rcdta_end_tg_opn_state =
                              bdlx_new_state(prsr->lex),
        rcdta_end_tg_nme_state =
                              bdlx_new_state(prsr->lex),
        rwtxt_lt_state      = bdlx_new_state(prsr->lex),
        rwtxt_end_tg_opn_state =
                              bdlx_new_state(prsr->lex),
        rwtxt_end_tg_nme_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_lt =        bdlx_new_state(prsr->lex),
        scrpt_dta_end_tg_opn_state = 
                              bdlx_new_state(prsr->lex),
        scrpt_dta_esc_strt_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_escd_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_escd_dsh_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_escd_lt_stat =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_escd_end_tg _opn_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_escd_end_tg_nme_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_strt_state = 
		                      bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_state = 
                              bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_dsh_state =
                              bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_dsh_dsh_state = 
                              bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_lt_state = 
                            = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_end_state
	                        = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_end_state
                            = bdlx_new_state(prsr->lex),
        bfre_attr_nme_state = bdlx_new_state(prsr->lex),
        attr_nme_state      = bdlx_new_state(prsr->lex),
        aftr_attr_nme_state = bdlx_new_state(prsr->lex),
        bfre_attr_val_state = bdlx_new_state(prsr->lex),
        attr_val_dq_state   = bdlx_new_state(prsr->lex),
        attr_val_sq_state   = bdlx_new_state(prsr->lex),
        attr_val_unq_state  = bdlx_new_state(prsr->lex),
        chr_rf_in_attr_val_state =
                              bdlx_new_state(prsr->lex),
        aftr_attr_val_unq_state = 
                              bdlx_new_state(prsr->lex),
        slf_clsng_strt_tg_state =
                              bdlx_new_state(prsr->lex),
        bgs_cmt_state       = bdlx_new_state(prsr->lex),
        mrkup_dcl_opn_state = bdlx_new_state(prsr->lex),
        cmt_strt_state      = bdlx_new_state(prsr->lex),
        cmt_strt_dsh_state  = bdlx_new_state(prsr->lex),
        cmt_state           = bdlx_new_state(prsr->lex),
        cmt_end_state       = bdlx_new_state(prsr->lex),
        cmt_end_dsh_state   = bdlx_new_state(prsr->lex),
        cmt_end_bng_state   = bdlx_new_state(prsr->lex),
        doctype_state       = bdlx_new_state(prsr->lex),
        bfre_doctype_state  = bdlx_new_state(prsr->lex),
        doctype_nme_state   = bdlx_new_state(prsr->lex),
        aftr_doctype_nme_state =
                              bdlx_new_state(prsr->lex),
        aftr_doctype_pub_kwd_state =
                              bdlx_new_state(prsr->lex),
        doctype_pub_id_dq_state =
                              bdlx_new_state(prsr->lex),
        doctype_pub_id_sq_state  =
                              bdlx_new_state(prsr->lex),
        aftr_doctype_pub_id_state =
                              bdlx_new_state(prsr->lex),
        btwn_doctype_pubsys_ids_state =
                              bdlx_new_state(prsr->lex),
        aftr_doctype_sys_kwd_state =
                              bdlx_new_state(prsr->lex),
        bfre_doctype_sys_id_state =
                              bdlx_new_state(prsr->lex),
        doctype_sys_id_dq_state =
                              bdlx_new_state(prsr->lex),
        doctype_sys_id_sq_state =
                              bdlx_new_state(prsr->lex),
        aftr_doctype_sys_id_state =
                              bdlx_new_state(prsr->lex),
        bgs_doctype_state   = bdlx_new_state(prsr->lex),
        cdta_sctn_state     = bdlx_new_state(prsr->lex);

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "&",
                        dta_state,
                        chrrf_in_dta_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "<",
                        dta_state,
                        tg_opn_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "\\0",
                        dta_state,
                        0,
                        NULL,
                        NULL,
	                    1,
                        bdhm_tok_chr)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "[^\\0&<"],
                        hcheerrfdta_state,
                        0,
                        NULL,
                        NULL,
	                    1,
                        bdhm_tok_chr)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "&",
                        chrrf_in_dta_state,
                        chrrf_in_rcdta_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "<",
                        chrrf_in_dta_state,
                        rcdta_lt_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        NULL,
                        chrrf_in_rcdta_state,
                        bdlx_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "<",
                        rwtxt_state,
                        rwtxt_lt_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "<",
                        scrpt_state,
                        scrpt_lt_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "!",
                        tg_opn_state,
                        mrkup_dcl_opn_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "/",
                        tg_opn_state,
                        end_tg_opn_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "[A-Z]",
                        tg_opn_state,
                        tg_nme_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "[a-z]",
                        tg_opn_state,
                        tg_nme_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "\\?",
                        tg_opn_state,
                        bgs_cmt_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "[A-Z]",
                        end_tg_opn_state,
                        tg_nme_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "[a-z]",
                        end_tg_opn_state,
                        tg_nme_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

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
                                  * of "input" */
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
    return bdpr_run(allocs,
                    logger,
                    prsr,
                    (void *)
                        ret,
                    str);
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

/* "get_elem_id" - implements the "ele-
 * ment"-class "id" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
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

/* "get_elem_ttle" - implements the "ele-
 * ment"-class "title" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_elem_ttle(
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

/* "get_elem_lng" - implements the "ele-
 * ment"-class "lang" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_elem_lng(
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

/* "set_elem_lng" - implements the "ele-
 * ment"-class "lang" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_elem_lng(
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

/* "get_elem_dr" - implements the "ele-
 * ment"-class "dir" attribute getter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_elem_dr(
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

/* "set_elem_dr" - implements the "ele-
 * ment"-class "dir" attribute setter,
 * using the language abstraction, "cont-
 * ext" object, array of arguments, number
 * of arguments, memory allocator, error
 * logger and object to set, on exit, to
 * any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_elem_dr(
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

/* "get_elem_clss_nme" - implements the
 * "element"-class "className" attribute
 * getter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
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

/* "set_elem_clss_nme" - implements the
 * "element"-class "className" attribute
 * setter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_elem_clss_nme(
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

/* "get_html_elem_vrsn" - implements the
 * "HTMLelement-class "version" attribute
 * getter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *get_html_elem_vrsn(
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

/* "set_html_elem_vrsn" - implements the
 * "HTMLelement-class "version" attribute
 * setter, using the language abstraction,
 * "context" object, array of arguments,
 * number of arguments, memory allocator,
 * error logger and object to set, on exit,
 * to any thrown exception in "lang", "ctx",
 * "args", "num", "allocs", "logger" and
 * "xcptn", respectively */
static struct bdlg_obj *set_html_elem_vrsn(
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

/* "bdhm_set_doc_prps" - returns 
 * a language-abstracted object
 * populated with DOM properties
 * of the HTML Document given in
 * "doc", using the memory allo-
 * cator and error logger given
 * in "allocs" and "logger", re-
 * spectively. Returns NULL on
 * error */
struct bdlg_obj *bdhm_add_doc_prps(
                       struct bdhm_doc *doc,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{

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

/* "bdhm_add_elem_prps" - returns a
 * language-abstracted object popul-
 * ated with DOM propertiesof the El-
 * ement given in "elem", using the
 * memory allocator and error logger
 * given in "allocs" and "logger",
 * respectively.  Returns NULL on
 * error */
struct bdlg_obj *bdhm_add_elem_prps(
                       struct bdhm_elem *elem,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdlg_obj *ret;          /* return value */
    struct bdxl_elem *elem = NULL; /* super-class
                                    *  object */

    /* initialise  object to return with
     * the properties of an XML Element"*/
    if (!(ret = bddm_add_elem_prps(&elem->elem,
                                   allocs,
                                   logger)))
        return NULL;

    /* ... then add the properties of an
     * HTML element */
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
                             set_elem_id,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "title",
                         (bdlg_cll_fn)
                             get_elem_ttle,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "title",
                         (bdlg_cll_fn)
                             set_elem_ttle,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "dir",
                         (bdlg_cll_fn)
                             get_elem_dr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "dir",
                         (bdlg_cll_fn)
                             set_elem_dr,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "lang",
                         (bdlg_cll_fn)
                             get_elem_lng,
                         allocs,
                         logger)))
        return NULL;

    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "lang",
                         (bdlg_cll_fn)
                             set_elem_lng,
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

    return bdlg_add_entry(ret,
                         bdlg_entry_attr_set,
                         "className",
                         (bdlg_cll_fn)
                             set_elem_clss_nme,
                         allocs,
                         logger);

}

/* "bdhm_set_html_elem_prps" - ret-
 * urns  a language-abstracted obj-
 * ect populated with DOM properties
 * of the HTML Element given in "ht-
 * ml", using the memory allocator
 * and error logger given in "allo-
 * cs" and "logger", respectively.
 * Returns NULL on error */
struct bdlg_obj *bdhm_add_html_elem_prps(
                       struct bdhm_html_elem *html,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdlg_obj *ret;          /* return value */
    struct bdxl_elem *elem = NULL; /* super-class
                                    *  object */

    /* initialise  object to return with
     * the properties of an XML Element"*/
    if (!(ret = bddm_add_elem_prps(&html->elem,
                                   allocs,
                                   logger)))
        return NULL;

    /* ... then add the properties of an
     * HTML element */
    if (!(bdlg_add_entry(ret,
                         bdlg_entry_attr_get,
                         "version",
                         (bdlg_cll_fn)
                             get_html_elem_vrsn,
                         allocs,
                         logger)))
        return NULL;

    return bdlg_add_entry(ret,
                          bdlg_entry_attr_set,
                          "version",
                          (bdlg_cll_fn)
                              set_html_elem_vrsn,
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
    /* load the parser with the
     * lexical rules and idispatch
     * call-back to enable it to
     * perform its task */
    prsr = bdpr_init(NULL,
                     allocs,
                     logger,
                     bdgm_sym_empty,
                     (bdpr_init_fn)
                         ld_prsr);

    return prsr != NULL;
}

/* "dsptch_initl" - "dispat-
 * ches", in a manner descri-
 * bed in the spec., the le-
 * xical token given in tok",
 * whilst parsing in the "in-
 * itial" insertion mode, us-
 * ing the parser, parser ou-
 * tput, memory allocator
 * and error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int dsptch_initl(struct bdlx_tok *tok,
                        struct bdpr_parser *prsr,
                        void *out,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;
    else
        node = (struct bdxl_doc *) out;

    switch (hm_tok->type) { /* switch on "tok"'s
                             * type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
	};
}

/* "dsptch_frgn_cnt" - "disp-
 * atches", in a manner desc-
 * ribed in the spec., the
 * lexical token given in
 * tok", whilst parsing in
 * the "foreign content" in-
 * sertion mode, using the
 * parser, parser output,
 * memory allocator and er-
 * ror logger given in "pr-
 * sr", "out", "allocs" and
 * "logger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_frgn_cnt(struct bdlx_tok *tok,
                           struct bdpr_parser *prsr,
                           void *out,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s
                             * type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
	};
}

/* "dsptch_bfre_html" - "di-
 * spatches", in a manner de-
 * scribed in the spec., the
 * HTML generic lexical tok-
 * en given in tok", whilst
 * parsing in the "before HT-
 * ML" insertion mode, us-
 * ing the parser, parser ou-
 * tput, memory allocator
 * and error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respectiv-
 * ely.  Returns zero on er-
 * ror, non-zero otherwise */
static int dsptch_bfre_html(struct bdlx_tok *tok,
                            struct bdpr_parser *prsr,
                            void *out,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s
                             * type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_bfre_hd" - "disp-
* atches", in a manner de-
 * scribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing in
 * the "before head" insert-
 * ion mode, using the par-
 * ser, parser output, mem-
 * ory allocator and error
 * logger given in "prsr",
 * "out", "allocs" and "lo-
 * gger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_bfre_hd(struct bdlx_tok *tok,
                          struct bdpr_parser *prsr,
                          void *out,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s
                             * type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_in_hd" - "dispat-
 * ches", in a manner descr-
 * ibed in the spec., the
 * lexical token given in
 * "tok", whilst parsing in
 * the "in-head" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr", "out",
 * "allocs" and "logger",
 * respectively.  Returns
 * zero on error, non-zero
 * otherwise */
static int dsptch_in_hd(struct bdlx_tok *tok,
                        struct bdpr_parser *prsr,
                        void *out,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_in_hd_noscrpt" -
 * "dispatches", in a mann-
 * er described in the sp-
 * ec., the lexical token gi-
 * ven in "tok" whilst pars-
 * ing in the "in-head, no-
 * script" insertion mode,
 * using the parser, parser
 * output, memory allocator
 * and error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int dsptch_in_hd_noscrpt(struct bdlx_tok *tok,
                                struct bdpr_parser *prsr,
                                void *out,
                                struct bd_allocs *allocs,
                                struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_aftr_hd" - "disp-
 * atches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "after head" inse-
 * rtion mode, using the pa-
 * rser, parser output, mem-
 * ory allocator and error
 * logger given in "prsr",
 * "out", "allocs" and "lo-
 * gger", respectively.  Re-
 * turns zero on error, non-
 * zero otherwise */
static int dsptch_aftr_hd(struct bdlx_tok *tok,
                          struct bdpr_parser *prsr,
                          void *out,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_in_bdy" - "disp-
 * atches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "in body" insert-
 * ion mode, using the par-
 * ser, parser output, mem-
 * ory allocator and error
 * logger given in "prsr",
 * "out", "allocs" and "lo-
 * gger", respectively.  Re-
 * turns zero on error, non-
 * zero otherwise */
static int dsptch_in_bdy(struct bdlx_tok *tok,
                         struct bdpr_parser *prsr,
                         void *out,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_txt" - "dispat-
 * ches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "text" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr",
 * "out", "allocs" and
 * "logger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_txt(struct bdlx_tok *tok,
                      struct bdpr_parser *prsr,
                      void *out,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_tble" - "dispat-
 * ches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "table" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr",
 * "out", "allocs" and
 * "logger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_tble(struct bdlx_tok *tok,
                       struct bdpr_parser *prsr,
                       void *out,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_tble_txt" - "disp-
 * atches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "table text" ins-
 * ertion mode, using the
 * parser, parser output,
 * memory allocator and er-
 * ror logger given in "pr-
 * sr", "out", "allocs" and
 * "logger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_tble_txt(struct bdlx_tok *tok,
                           struct bdpr_parser *prsr,
                           void *out,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_tble_cptn" - "dis-
 * patches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "caption" insert-
 * ion mode, using the par-
 * ser, parser output, mem-
 * ory allocator and error
 * logger given in "prsr",
 * "out", "allocs" and "lo-
 * gger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_cptn(struct bdlx_tok *tok,
                       struct bdpr_parser *prsr,
                       void *out,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_tble_rw" - "dis-
 * patches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "raw" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr",
 * "out", "allocs" and "lo-
 * gger", respectively.
 * Returns zero on error,
 * non-zero otherwise */
static int dsptch_rw(struct bdlx_tok *tok,
                     struct bdpr_parser *prsr,
                     void *out,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_tble_cll" - "dis-
 * patches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "cell" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr",
 * "out", "allocs" and "log-
 * ger", respectively.  Ret-
 * urns zero on error, non-
 * zero otherwise */
static int dsptch_cll(struct bdlx_tok *tok,
                      struct bdpr_parser *prsr,
                      void *out,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_slct" - "dispat-
 * ches", in a manner des-
 * cribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "select" insertion
 * mode, using the parser,
 * parser output, memory
 * allocator and error log-
 * ger given in "prsr", "out",
 * "allocs" and "logger",
 * respectively.  Returns
 * zero on error, non-zero
 * otherwise */
static int dsptch_slct(struct bdlx_tok *tok,
                       struct bdpr_parser *prsr,
                              void *out,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_slct_in_tble" -
 * "dispatches", in a mann-
 * er described in the spec.,
 * the lexical token given
 * in "tok", whilst parsing
 * in the "select in table"
 * insertion mode, using
 & the parser, parser ou-
 * tput, memory allocator
 * and error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
static int dsptch_slct_in_tble(struct bdlx_tok *tok,
                               struct bdpr_parser *prsr,
                               void *out,
                               struct bd_allocs *allocs,
                               struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_aftr_bdy" - "dis=
 * patches", in a manner de-
 * scribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "after body" ins-
 * ertion mode, using the
 * parser, parser output,
 * memory allocator and
 * error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respecti-
 * vely.  Returns zero on
 * error, non-zero other-
 * wise */
static int dsptch_aftr_bdy(struct bdlx_tok *tok,
                           struct bdpr_parser *prsr,
                           void *out,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_in_frmset" - "dis=
 * patches", in a manner de-
 * scribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing
 * in the "in frameset" ins-
 * ertion mode, using the
 * parser, parser output,
 * memory allocator and
 * error logger given in
 * "prsr", "out", "allocs"
 * and "logger", respecti-
 * vely.  Returns zero on
 * error, non-zero otherw-
 * ise*/
static int dsptch_in_frmset(struct bdlx_tok *tok,
                            struct bdpr_parser *prsr,
                            void *out,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

/* "dsptch_aftr_frmset" - "di-
 * spatches", in a manner de-
 * scribed in the spec., the
 * lexical token given in
 * "tok", whilst parsing in
 * the "after frameset" ins-
 * ertion mode, using the
 * parser, parser output,
 * memory allocator and er-
 * ror logger given in
 * "prsr", "out", "allocs"
 * and "logger", respecti-
 * vely.  Returns zero on
 * error, non-zero other-
 * wise */
static int dsptch_aftr_frmset(struct bdlx_tok *tok,
                              struct bdpr_parser *prsr,
                              void *out,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    struct bdhm_tok *hm_tok =    /* HTML version */
        (struct bdhm_tok *) tok; /*  of token */ 
    struct bdhm_rt *rt =         /* parser's */
        (struct bdhm_rt *)       /* HTML run- */
        prsr->rt;                /* time */
    struct bdxl_node *frag;      /* output fragment */
    struct bdxl_doc *doc;        /* output focument */

    /* get the output from its correct
     * source */
    if (rt->is_frag)
        frag = (struct bdxl_node *) out;

    switch (hm_tok->type) { /* switch on "tok"'s type ... */

        case bdhm_tok_doc_type:
            break;

        case bdhm_tok_strt_tg:
            break;

        case bdhm_tok_end_tg:
            break;

        case bdhm_tok_cmt:  
            break;

        case bdhm_tok_attrs:
            break;

        case bdhm_tok_cdta:
            break;

        case bdhm_tok_chr:

            switch (tok->u.chr) {
                case 'a':
            };
            break;
    };
}

static bdhm_dsptch_fn dsptch_fns[
                   bdhm_ins_modes_num] = {

]   dsptch_initl,
    dsptch_bfre_html,
    dsptch_bfre_hd,
    dsptch_in_hd,
    dsptch_in_hd_noscrpt,
    dsptch_aftr_hd,
    dsptch_in_bdy,
    dsptch_txt,
    dsptch_in_tble,
    dsptch_in_tble_txt,
    dsptch_in_cptn,
    dsptch_in_rw,
    dsptch_in_cll,
    dsptch in_slct,
    dsptch_in_slct_in_tble,
    dsptch in_frgn_cnt,
    dsptch_aftr_bdy,
    dsptch_in_frmset,
    dsptch_aftr_frmset
};

/* "dsptch" - within an HTML
 * parser, "dispatches", accor-
 * ding to the algorithm desc-
 * ribed in the spec., the ge-
 * neric token given in "tok",
 * as scanned from input, by
 * "re-dispatching" to the
 * dispatch routine corres-
 * ponding to each parser ins-
 * ertion mode", using the pa-
 * rser output, memory alloc-
 * ator and error logger giv-
 * en in "opaque", "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-
 * zero otherwise */
int dsptch(struct bdlx_tok *tok,
           struct bdpr_parser *prsr,
           void *out,
           struct bd_allocs *allocs,
           struct bd_logger *logger)
{
    struct bdhm_rt *rt =   /* parser's */
        (struct bdhm_rt *) /* HTML run-time */
        prsr->rt;
    bdhm_dsptch_fn fn;     /* current "ins-
                            * ertion mode"'s 
                            * dispatch func-
                            * tion */

    /* look-up dispatch function from
     * the current insertion mode */
	fn = dsptch_fns[rt->ins_mode];

    /* call the resulting function  */
    return fn(tok,
              prsr,
              out,
              allocs,
              logger);
}

/* "init_rt" - returns an an HTML pa-
 * rser run-time having the insertion
 * node, stack of open elements, list
 * of active formatting elements,
 * stack of template insertion modes,
 * "fragment case" flag, "form" ele-
 * ment pointer, "head", element poi-
 * nter, scripting flag annd "reset-
 * ok" flag given in "ins_mode", "fmts",
 * "tmpltes", "is_frag", "hd", "frm",
 * "scrptg" and "rstok", respectively,
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns NULL on
 * error */
static struct bdhm_rt *init_rt(
                enum bdhm_ins_modes ins_mode,
                struct bdhm_opn_node *opns,
                struct bdhm_fmt_node *fmts,
                struct bdhm_rtmplte_node *tmpltes,
                int is_frag,
                struct bdhm_elem *hd,
                struct bdhm_elem *frm,
                int scrptg,
                int rstok,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
    struct bdhm_rt *ret; /* return
                          * value */

    /* try to allocate a run-
     * time ... */
    ret = bd_alloc(allocs,
                   sizeof(struct
                          bdhm_rt),
                   logger);

    if (!(ret))
        return NULL;

    /* and, if successful, set
     * its fields and return
     * it */
    ret->ins_mode = ins_mode;
    ret->opns     = opns;
    ret->fmts     = fmts;
    ret->tmpltes  = tmpltes;
    ret->hd       = hd;
    ret->frm      = frm;
    ret->scrptng  = scrptng;
    ret->rstok    = rstok;
    return ret;
}

/* "bdhm_prse_doc" - parses, using the
 * memory allocator and error logger
 * given in "allocs" and "logger", re-
 * spectively, the HTML-formatted byte
 * stream given in "bytes" parameter,
 * and returns an XML document having
 * equivalent functionality to the
 * document found in that stream.  Re-
 * turns NULL on error */
struct bdxl_doc *bdhm_prse_doc(struct bd_allocs *allocs,
                               struct bd_logger *logger,
                               struct bdut_bytes *bytes)
{
    struct bdxl_doc *ret;
    struct bdhm_rt *rt;

    if (!(ret = bdxl_init_doc(allocs,
                              logger))))
        return NULL;

    if (!(rt = init_rt(bdhm_ins_mode_bfre_html,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       0,
                       0,
                       0,
                       allocs,
                       logger)))
        return NULL;

    prsr->rt = (void *) rt;

   of if (!(bdpr_run(allocs,
                     logger,
                     prsr,
                     (bdpr_dsptch_fn)
                         dsptch,
                     (void *)
                         ret,
                      bytes))) {
        bd_free(allocs,
                rt);
        return NULL;
    }

    return ret;
}

/* "bdhm_prse_frag" - parses, using the
 * memory allocator and error logger gi-
 * ven in "allocs" and "logger", respe-
 * ctively, the HTML-formatted byte st-
 * ream given in "bytes", and returns
 * an XML fragment having equivalent fu-
 * nctionality to the odocument fragme-
 * nt found in the HTML stream.  Returns
 * NULL on error */
struct bdxl_node *bdhm_prse_frag(struct bd_allocs *allocs,
                                 struct bd_logger *logger,
                                 struct bdut_bytes *bytes)
{
    struct bdxl_node *ret;
    struct bdhm_rt *rt;

    if (!(ret = bd_alloc(allocs,
                         sizeof(struct
                                bdxl_node),
                         logger))))
        return NULL;

    if (!(rt = init_rt(bdhm_ins_mode_initl,
                       NULL,
                       NULL,
                       NULL,
                       NULL,
                       1,
                       0,
                       0,
                       allocs,
                       logger)))
        return NULL;

    prsr->rt = (void *) rt;

   if (!(bdpr_run(allocs,
                  logger,
                  prsr,
                  (bdpr_dsptch_fn)
                      dsptch,
                  (void *)
                      ret,
                  bytes))) {
        bd_free(allocs,
                rt);

        return NULL;
    }
    return ret;
}

