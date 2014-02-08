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
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

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
    hm_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));
    hm_tok->u.num = 0;
    hm_tok->u.str.len = 0;

    return hm_tok->u.buf != NULL;
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
        i++) {

    /* "i" will equal "num" (and
     * thereby return non-zero)
     *  if "chr" matches all in
     * the list */
    return i == num;
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
                       "[a-zA-Z1-9]")))
        return 0;

    /* a block of sequentially increasing
     * allocated lexical states, between
     * which the analyser will transition,
     * allocated from the analyser's pool
     * of available states.
	 *
	 * each of these states corresponds
	 * to a similarly-named tokeniser
	 * state defined in the spec. */
    int chrrf_in_dta_state                = bdlx_new_state(prsr->lex),
        chrrf_in_rcdta_state              = bdlx_new_state(prsr->lex),
        rcdta_state                       = bdlx_new_state(prsr->lex),
        rwtxt_state                       = bdlx_new_state(prsr->lex),
        scrpt_dta_state                   = bdlx_new_state(prsr->lex),
        plntxt_state                      = bdlx_new_state(prsr->lex),
        tg_opn_state                      = bdlx_new_state(prsr->lex),
        end_tg_opn_state                  = bdlx_new_state(prsr->lex),
        rcdta_lt_state                    = bdlx_new_state(prsr->lex),
        rcdta_end_tg_opn_state            = bdlx_new_state(prsr->lex),
        rcdta_end_tg_nme_state            = bdlx_new_state(prsr->lex),
        rwtxt_lt_state                    = bdlx_new_state(prsr->lex),
        rwtxt_end_tg_opn_state            = bdlx_new_state(prsr->lex),
        rwtxt_end_tg_nme_state            = bdlx_new_state(prsr->lex),
        scrpt_dta_lt                      = bdlx_new_state(prsr->lex),
        scrpt_dta_end_tg_opn_state        = bdlx_new_state(prsr->lex),
        scrpt_dta_esc_strt_state          = bdlx_new_state(prsr->lex),
        scrpt_dta_escd_state              = bdlx_new_state(prsr->lex),
        scrpt_dta_escd_dsh_state          = bdlx_new_state(prsr->lex),
        scrpt_dta_escd_lt_state           = bdlx_new_state(prsr->lex),
        scrpt_dta_escd_end_tg_opn_state   = bdlx_new_state(prsr->lex),
        scrpt_dta_escd_end_tg_nme_state   = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_strt_state     = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_state         = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_dsh_state     = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_dsh_dsh_state = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_escd_lt_state      = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_end_state      = bdlx_new_state(prsr->lex),
        scrpt_dta_dble_esc_end_state      = bdlx_new_state(prsr->lex),
        bfre_attr_nme_state               = bdlx_new_state(prsr->lex),
        attr_nme_state                    = bdlx_new_state(prsr->lex),
        aftr_attr_nme_state               = bdlx_new_state(prsr->lex),
        bfre_attr_val_state               = bdlx_new_state(prsr->lex),
        attr_val_dq_state                 = bdlx_new_state(prsr->lex),
        attr_val_sq_state                 = bdlx_new_state(prsr->lex),
        attr_val_unq_state                = bdlx_new_state(prsr->lex),
        chr_rf_in_attr_val_state          = bdlx_new_state(prsr->lex),
        aftr_attr_val_unq_state           = bdlx_new_state(prsr->lex),
        slf_clsng_strt_tg_state           = bdlx_new_state(prsr->lex),
        bgs_cmt_state                     = bdlx_new_state(prsr->lex),
        mrkup_dcl_opn_state               = bdlx_new_state(prsr->lex),
        cmt_strt_state                    = bdlx_new_state(prsr->lex),
        cmt_strt_dsh_state                = bdlx_new_state(prsr->lex),
        cmt_state                         = bdlx_new_state(prsr->lex),
        cmt_end_state                     = bdlx_new_state(prsr->lex),
        cmt_end_dsh_state                 = bdlx_new_state(prsr->lex),
        cmt_end_bng_state                 = bdlx_new_state(prsr->lex),
        doctype_state                     = bdlx_new_state(prsr->lex),
        bfre_doctype_state                = bdlx_new_state(prsr->lex),
        doctype_nme_state                 = bdlx_new_state(prsr->lex),
        aftr_doctype_nme_state            = bdlx_new_state(prsr->lex),
        aftr_doctype_pub_kwd_state        = bdlx_new_state(prsr->lex),
        doctype_pub_id_dq_state           = bdlx_new_state(prsr->lex),
        doctype_pub_id_sq_state           = bdlx_new_state(prsr->lex),
        aftr_doctype_pub_id_state         = bdlx_new_state(prsr->lex),
        btwn_doctype_pubsys_ids_state     = bdlx_new_state(prsr->lex),
        aftr_doctype_sys_kwd_state        = bdlx_new_state(prsr->lex),
        bfre_doctype_sys_id_state         = bdlx_new_state(prsr->lex),
        doctype_sys_id_dq_state           = bdlx_new_state(prsr->lex),
        doctype_sys_id_sq_state           = bdlx_new_state(prsr->lex),
        aftr_doctype_sys_id_state         = bdlx_new_state(prsr->lex),
        bgs_doctype_state                 = bdlx_new_state(prsr->lex),
        cdta_sctn_state                   = bdlx_new_state(prsr->lex);

    if (!(bdlx_add_rule(js->lex,
                        allocs,
                        logger,
                        "&",
                        bdlx_start_state,
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
                        bdlx_start_state,
                        tg_opn_state,
                        NULL,
                        NULL,
                        0,
                        0)))
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
