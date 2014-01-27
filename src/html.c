/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* html.c - function implementationa related to
* the Hyper-Text Markup Language (HTML), a legacy,
* SGML-like format commonly * used prior to the
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
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

/* "prsr" - the HTML document
 * parser */
static struct bdpr_parser *prsr;

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

    /* return "type" field */
    return hm_tok->type;
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

    /* returns non-zero if
     * "chr" matches at all
     * in the list */
}

/*  "get_attr" - returns the value
 * of the '='-separated name-value
 * attribute located after the
 * contents of the pointer given
 * in "pos", and setting the name
 * of the attribute name to "nme" */ 
 * tic char *get_attr(char **pos,
                      char **nme)
{
        char b; /* utility
                 * character */

    /* if rogress to next byte
     * if"**pos" is either white-
     * space or '/' */
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
     * set it and the
     * attribute value to
     * empty strings  */
    else {
        pos = nme;
        *nme = "";
        return "";
    }

name:
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

        /* then, if "**pos"
         * is any of the
         * following ... */
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
         * alphabetic, add 0x20
         * to value to append
         * to name  */

        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*nme)++ = ((**pos)++ + 0x20);
            break;

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
rpt:
            (pos)++;

            if (b == **pos) {
                (*pos)++;
            return ret;

           } else

    /* set up character to
     * append to attribute
     * value  */
    dst = **pos;

    /* and. id of if position
     * is alhabetic, add 0x20
     * to that value  */
 
    if (**pos > 'A' &&
        **pos < 'Z')
        dst += 0x20;

    /* apprnebnd hthe value
     * to the attribute value
     * (the return value) */
    (*ret)++ = dst;


    if (**pos == '>')
        return "";

    return ret;
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
struct enc_cnfdnce *mke_enc_cnfdnce(
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

/* "sniff_enc" - "sniffs" (to use the
 * terminology of the spec.) and ret-
 * urns the name of, the encoding of
 * the HTML formatted array of bytes
 * given in "buf", setting the con-
 * fidence level of the detection in
 * "cnfdnce", uasing the content-type
 * (where known: NULL if not).  Retu-
 * rns NULL on error. Note that, acc-
 * ording to the spec., the length
 * of "buf" is guaranteed to be less
 * than 1024 */
int char *sniff_enc(char *buf,
                    char *ctype,
                    enum bdhm_cnfdnces *cnfdnce)
{
    return "";
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
    return bdlx_add_def(prsr->lex,
                        allocs,
                        logger,
                        "nme_chr",
                        "[a-zA-Z1-9]");
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
                     NULL;        * of "input" */
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

    ret = NULL;

    bdpr_run(allocs,
             logger,
             prsr,
             (void *)
                 ret,
             str);

    /* .. and return the generated 
     * document */
    return ret;
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
/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* html.c - function implementationa related to
* the Hyper-Text Markup Language (HTML), a legacy,
* SGML-like format commonly * used prior to the
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
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

/* "prsr" - the HTML document
 * parser */
static struct bdpr_parser *prsr;

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

    /* return "type" field */
    return hm_tok->type;
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

    /* returns non-zero if
     * "chr" matches at all
     * in the list */
}

/*  "get_attr" - returns the value
 * of the '='-separated name-value
 * attribute located after the
 * contents of the pointer given
 * in "pos", and setting the name
 * of the attribute name to "nme" */ 
 * tic char *get_attr(char **pos,
                      char **nme)
{
        char b; /* utility
                 * character */

    /* if rogress to next byte
     * if"**pos" is either white-
     * space or '/' */
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
     * set it and the
     * attribute value to
     * empty strings  */
    else {
        pos = nme;
        *nme = "";
        return "";
    }

name:
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

        /* then, if "**pos"
         * is any of the
         * following ... */
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
         * alphabetic, add 0x20
         * to value to append
         * to name  */

        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*nme)++ = ((**pos)++ + 0x20);
            break;

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
rpt:
            (pos)++;

            if (b == **pos) {
                (*pos)++;
            return ret;

           } else

    /* set up character to
     * append to attribute
     * value  */
    dst = **pos;

    /* and. id of if position
     * is alhabetic, add 0x20
     * to that value  */
 
    if (**pos > 'A' &&
        **pos < 'Z')
        dst += 0x20;

    /* apprnebnd hthe value
     * to the attribute value
     * (the return value) */
    (*ret)++ = dst;


    if (**pos == '>')
        return "";

    return ret;
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
struct enc_cnfdnce *mke_enc_cnfdnce(
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

/* "sniff_enc" - "sniffs" (to use the
 * terminology of the spec.) and ret-
 * urns the name of, the encoding of
 * the HTML formatted array of bytes
 * given in "buf", setting the con-
 * fidence level of the detection in
 * "cnfdnce", uasing the content-type
 * (where known: NULL if not).  Retu-
 * rns NULL on error. Note that, acc-
 * ording to the spec., the length
 * of "buf" is guaranteed to be less
 * than 1024 */
int char *sniff_enc(char *buf,
                    char *ctype,
                    enum bdhm_cnfdnces *cnfdnce)
{
    return "";
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
    return bdlx_add_def(prsr->lex,
                        allocs,
                        logger,
                        "nme_chr",
                        "[a-zA-Z1-9]");
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
                     NULL;        * of "input" */
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

    ret = NULL;

    bdpr_run(allocs,
             logger,
             prsr,
             (void *)
                 ret,
             str);

    /* .. and return the generated 
     * document */
    return ret;
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
/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* html.c - function implementationa related to
* the Hyper-Text Markup Language (HTML), a legacy,
* SGML-like format commonly * used prior to the
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
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

/* "prsr" - the HTML document
 * parser */
static struct bdpr_parser *prsr;

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

    /* return "type" field */
    return hm_tok->type;
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

    /* returns non-zero if
     * "chr" matches at all
     * in the list */
}

/*  "get_attr" - returns the value
 * of the '='-separated name-value
 * attribute located after the
 * contents of the pointer given
 * in "pos", and setting the name
 * of the attribute name to "nme" */ 
 * tic char *get_attr(char **pos,
                      char **nme)
{
        char b; /* utility
                 * character */

    /* if rogress to next byte
     * if"**pos" is either white-
     * space or '/' */
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
     * set it and the
     * attribute value to
     * empty strings  */
    else {
        pos = nme;
        *nme = "";
        return "";
    }

name:
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

        /* then, if "**pos"
         * is any of the
         * following ... */
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
         * alphabetic, add 0x20
         * to value to append
         * to name  */

        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*nme)++ = ((**pos)++ + 0x20);
            break;

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
rpt:
            (pos)++;

            if (b == **pos) {
                (*pos)++;
            return ret;

           } else

    /* set up character to
     * append to attribute
     * value  */
    dst = **pos;

    /* and. id of if position
     * is alhabetic, add 0x20
     * to that value  */
 
    if (**pos > 'A' &&
        **pos < 'Z')
        dst += 0x20;

    /* apprnebnd hthe value
     * to the attribute value
     * (the return value) */
    (*ret)++ = dst;


    if (**pos == '>')
        return "";

    return ret;
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
struct enc_cnfdnce *mke_enc_cnfdnce(
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

/* "sniff_enc" - "sniffs" (to use the
 * terminology of the spec.) and ret-
 * urns the name of, the encoding of
 * the HTML formatted array of bytes
 * given in "buf", setting the con-
 * fidence level of the detection in
 * "cnfdnce", uasing the content-type
 * (where known: NULL if not).  Retu-
 * rns NULL on error. Note that, acc-
 * ording to the spec., the length
 * of "buf" is guaranteed to be less
 * than 1024 */
int char *sniff_enc(char *buf,
                    char *ctype,
                    enum bdhm_cnfdnces *cnfdnce)
{
    return "";
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
    return bdlx_add_def(prsr->lex,
                        allocs,
                        logger,
                        "nme_chr",
                        "[a-zA-Z1-9]");
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
                     NULL;        * of "input" */
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

    ret = NULL;

    bdpr_run(allocs,
             logger,
             prsr,
             (void *)
                 ret,
             str);

    /* .. and return the generated 
     * document */
    return ret;
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
/********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* html.c - function implementationa related to
* the Hyper-Text Markup Language (HTML), a legacy,
* SGML-like format commonly * used prior to the
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
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>
#include <bikedemo/html.h>
#include <bikedemo/parser.h>

/* "prsr" - the HTML document
 * parser */
static struct bdpr_parser *prsr;

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

    /* return "type" field */
    return hm_tok->type;
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

    /* returns non-zero if
     * "chr" matches at all
     * in the list */
}

/*  "get_attr" - returns the value
 * of the '='-separated name-value
 * attribute located after the
 * contents of the pointer given
 * in "pos", and setting the name
 * of the attribute name to "nme" */ 
 * tic char *get_attr(char **pos,
                      char **nme)
{
        char b; /* utility
                 * character */

    /* if rogress to next byte
     * if"**pos" is either white-
     * space or '/' */
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
     * set it and the
     * attribute value to
     * empty strings  */
    else {
        pos = nme;
        *nme = "";
        return "";
    }

name:
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

        /* then, if "**pos"
         * is any of the
         * following ... */
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
         * alphabetic, add 0x20
         * to value to append
         * to name  */

        case 'A': case 'B': case 'C':
        case 'D': case 'E': case 'F':
        case 'G': case 'H': case 'I':
        case 'J': case 'K': case 'L':
        case 'M': case 'N': case 'O':
        case 'P': case 'Q': case 'R':
        case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X':
        case 'Y': case 'Z': case 'Z':
            (*nme)++ = ((**pos)++ + 0x20);
            break;

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
rpt:
            (pos)++;

            if (b == **pos) {
                (*pos)++;
            return ret;

           } else

    /* set up character to
     * append to attribute
     * value  */
    dst = **pos;

    /* and. id of if position
     * is alhabetic, add 0x20
     * to that value  */
 
    if (**pos > 'A' &&
        **pos < 'Z')
        dst += 0x20;

    /* apprnebnd hthe value
     * to the attribute value
     * (the return value) */
    (*ret)++ = dst;


    if (**pos == '>')
        return "";

    return ret;
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
struct enc_cnfdnce *mke_enc_cnfdnce(
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

/* "sniff_enc" - "sniffs" (to use the
 * terminology of the spec.) and ret-
 * urns the name of, the encoding of
 * the HTML formatted array of bytes
 * given in "buf", setting the con-
 * fidence level of the detection in
 * "cnfdnce", uasing the content-type
 * (where known: NULL if not).  Retu-
 * rns NULL on error. Note that, acc-
 * ording to the spec., the length
 * of "buf" is guaranteed to be less
 * than 1024 */
int char *sniff_enc(char *buf,
                    char *ctype,
                    enum bdhm_cnfdnces *cnfdnce)
{
    return "";
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
    return bdlx_add_def(prsr->lex,
                        allocs,
                        logger,
                        "nme_chr",
                        "[a-zA-Z1-9]");
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
                     NULL;        * of "input" */
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

    ret = NULL;

    bdpr_run(allocs,
             logger,
             prsr,
             (void *)
                 ret,
             str);

    /* .. and return the generated 
     * document */
    return ret;
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
s
