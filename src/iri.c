/*********************************************************************
*
* BikeDemo - exercise bike demonstration scenery.
*
* iri.c - function definitions related to the pro-
* cessing of Internationalised Resource Identifiers
* (IRIs)
*
* Copyright (C) 2007-2013, Russell Potter. All
* rights reserved.
* 
***********************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <bikedemo/iri.h>
#include <bikedemo/lex.h>
#include <bikedemo/util.h>
#include <bikedemo/parser.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* "schms" - global map of IRI schemes,
 * keyed the the scheme name and valued
 * to the scheme's resolution function */
static struct bd_map_node *schms;

/*"prsr","gnrc_prsr" - top-level and
 * generic-formatted IRI parsers.  The
 * top-lebel parser is able to read an
 * IRI into a scheme name followed by a
 * buffer of scheme-speccific data (usu-
 * ally description as to how to resolve
 * the IRI), and the generic parser is
 * able to read an IRI in a "generic"
 * IRI formst into sb ITI strictire, wh-
 * ich is then passed to a resolution
 * callback */
static struct bdpr_parser *prsr,
                          *gnrc_prsr;

/* mke_gnrc_iri" - returns a generic IRI
 * having the hierarchical component and
 * fragment and query strings given in
 * "hr", "frg" and, "qry", respectively,
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error  */
static struct bdir_gnrc_iri *mke_gnrc_iri(
                        struct bdir_hier *hr,
                        struct bdut_str *qry,
                        struct bdut_str *frg,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    struct bdir_gnrc_iri *ret;  /* return
                                 * value */

    /* akixate return value ... */
    ret = (struct bdir_gnrc_iri *)
           bd_alloc(allocs,
                    sizeof(struct
                           bdir_gnrc_iri),
                    logger);
    if (!(ret))
        return NULL;

    /* and, if successful,
     * set its fields and
     * return it */
    ret->hier = hr;
    ret->frg  = frg;
    ret->qry  = qry;
    return ret;
}

/* mke_hier" - returns a generic IRI hiera-
 * rchical component having the authority
 * component and path string given in "auth"
 * and, "pth", respectively, using the
 * memory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns NULL on error  */
static struct bdir_hier *mke_hier(
                   struct bdir_auth *auth,
                   struct bdut_str *pth,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdir_hier *ret;  /* return
                             * value */

    /* akixate return value ... */
    ret = (struct bdir_hier *)
           bd_alloc(allocs,
                    sizeof(struct
                    		bdir_hier),
                    logger);
    if (!(ret))
        return NULL;

    /* and, if successful,
     * set its fields and
     * return it */
    ret->auth = auth;
    ret->pth  = pth;
    return ret;
}

/* mke_auth" - returns a generic IRI autho-
 * rity component having the user-info and
 * host host strings and port number given
 * in "usr", "hst" and "prt", respectively,
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively.  Returns NULL on error  */
static struct bdir_auth *mke_auth(struct bdut_str *usr,
                                  char *hst,
                                  int *prt,
                                  struct bd_allocs *allocs,
                                  struct bd_logger *logger)
{
    struct bdir_auth *ret;  /* return
                             * value */

    /* akixate return value ... */
    ret = (struct bdir_auth *)
           bd_alloc(allocs,
                    sizeof(struct
                    		bdir_auth),
                    logger);
    if (!(ret))
        return NULL;

    /* and, if successful,
     * set its fields and
     * return it */
    ret->usr = usr;
    ret->hst = hst;
    ret->prt = prt;
    return ret;
}

/* "init_tok" - performs one-time
 * initialisation of the JavaScript
 * token in "tok", using the memory
 * allocator and error logger in
 * "allocs" and "logger", respect-
 * ively.  Returns zero on error non-
 * zero otherwise */
static int init_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co0erce to correct type */
	struct bdir_tok *ir_tok  =
             (struct bdir_tok *) tok;

    /* initialise token's buffer */
    ir_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));

	return ir_tok->u.buf != NULL;
}

/* "tok_reset" - performs pro-
 * cessing required before
 * each readco0erce ot  of the
 * token in "tok", using the m-
 * emory allocator and error
 * logger in "allocs" and "log-
 * ger", respectively */
static int reset_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdir_tok *ir_tok  =
             (struct bdir_tok *) tok;

     /* reset the token's buffer */
     return bdbf_reset(ir_tok->u.buf,
                       allocs,
                       logger);
}

/* "tok_name" - returns the index
 * into the message table at which
 * to find a decription of the con-
 * tent of the JavaScript token in
 * "tok", where "exact" indicates
 * the level of exactness of the d-
 * escription, using the memory al=
 * locator and error logger in "al=
 * locs" and "logger", respectively */
static int tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdir_tok *ir_tok  =
             (struct bdir_tok *) tok;

	return 0; /* no description here */
}

/* "gnrc_tok_name" - returns the index
 * into the message table at which
 * to find a description of the con-
 * tent of the generic IRI token in
 * "tok", where "exact" indicates the
 * level of exactness of the descri-
 * ption, using the memory allocator
 * and error logger in "allocs" and
 * "logger", respectively */
static int gnrc_tok_name(
          struct bdlx_tok *tok,
          int exact,
          struct bd_allocs *allocs,
          struct bd_logger _logger)
{
    /* co-erce to correct type */
	struct bdir_gnrc_tok *gnrc_tok  =
             (struct bdir_gnrc_tok *)
             tok;

	return 0; /* no description here */
}

/* "pct1" - lexical transition call-back
 * to handle the first hexadecimal digit
 * (the numerical value of which is given
 * in "dig") of a "percentage escape",
 * using on the token in "tok", using
 * the memory allocator, error logger and
 * opaque value in "allocs", "logger" and
 * "opaque", respectively, Returns zero
 * on error, non-zero otherwise */
static int pct1(struct bdlx_tok *tok,
                int dig,
                void *opaque,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
	/* co-erce token to actal type */
	struct bdir_gnrc_tok *gnrc_tok =
			(struct bdir_gnrc_tok *) tok;

    /* set tower four bits of code-point
     * field from escape digit */
    gnrc_tok->u.cp = bdut_from_hex(dig) & 0xF;
    return 1;
}

/* "pct2" - lexical transition callback
 * to handle the second hexadecimal digit
 * (the numerical value of which is given
 * in "dig") of a "percentage escape",
 * using on the token in "tok", using
 * the memory allocator, error logger and
 * opaque value in "allocs", "logger" and
 * "opaque", respectively, Returns zero
 * on error, non-zero otherwise */
static int pct2(struct bdlx_tok *tok,
                int dig,
                void *opaque,
                struct bd_allocs *allocs,
                struct bd_logger *logger)
{
	/* co-erce token to actal type */
	struct bdir_gnrc_tok *gnrc_tok =
			(struct bdir_gnrc_tok *) tok;

    /* set upper four bits of code-point
     * field from escape digit */
    gnrc_tok->u.cp  &= bdut_from_hex(dig) >> 4;
    return 1;
}

/* "tok_type" - returns the type
 * of the IRI token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns a negat-
 * ive value on error  */
static int tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdir_tok *ir_tok  =
             (struct bdir_tok *) tok;

	return ir_tok->u.type; /* retuurn
                            * the type
                            * of the
                            * token */
}

/* "init_gnrc_tok" - performs one-time
 * initialisation of the generic IRI
 * token in "tok", using the memory
 * allocator and error logger in
 * "allocs" and "logger", respecti-
 * vely.  Returns zero on error,
 * non-zero otherwise */
static int init_gnrc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdir_gnrc_tok *gnrc_tok  =
             (struct bdir_gnrc_tok *)
             tok;

    /* initialise token's buffer */
    gnrc_tok->u.buf = bdbf_init(allocs,
                                logger,
                                sizeof(int));

	return gnrc_tok->u.buf != NULL;
}

/* "hmrc_tok_type" - returns the
 * type of the generically-format-
 * ted IRI token in "tok", using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively.  Returns a negat-
 * ive value on error  */
static int gnrc_tok_type(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co0erce to correct type */
	struct bdir_gnrc_tok *gnrc_tok =
             (struct bdir_gnrc_tok *) tok;

	return gnrc_tok->type; /* return
                            * the type
                            * of the
                            * token */
}

/* "_reset_gnrc_tok" - reperforms
 * prcessing required on the gene-
 * rically-formatted token in "tok"
 * before each token read, using
 * the memory allocator and error
 * logger in "allocs" and "logger",
 * respectively */
static int reset_gnrc_tok(
          struct bdlx_tok *tok,
          struct bd_allocs *allocs,
          struct bd_logger *logger)
{
    /* co-erce to correct type */
	struct bdir_gnrc_tok *gnrc_tok  =
             (struct bdir_gnrc_tok *) tok;

    /* resset the token's buffer and
     * initialise its port number */
    if (!(bdbf_reset(gnrc_tok->u.buf,
                     allocs,
                     logger)))
    	return 0;
    gnrc_tok->u.prt = 0;
    return 1;

}

/* "start_scheme" - lexical analyser
 * callback to start an IRI scheme name,
 * using the analyser, current code-
 * point, opaque parameter, memory allo-
 * cator and error logger given in "lex",
 * "cp", "opaque", "allocs" and "logger",
 * respectively.  Returns zero on error,
 * non-zero otherwise*/
static int start_scheme(struct bdlx_tok *tok,
                        int cp,
                        void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdir_tok *ir_tok  =
	             (struct bdir_tok *) tok;

    /* initialise buffer in which scheme
     * will be placed */
    ir_tok->u.buf = bdbf_init(allocs,
                              logger,
                              sizeof(int));

    return ir_tok->u.buf != NULL;
}

/* "add_to_scheme" - lexical analyser
 * callback to add a code-point to an
 * IRI scheme name, using the analyser,
 * current code-point, opaque parameter,
 * memory allocator and error logger
 * given in "lex", "cp", "opaque",
 * "allocs" and "logger", respectively.
 * Returns zero on error, non-zero ot-
 * herwise */
static int add_to_scheme(struct bdlx_tok *tok,
                         int cp,
                         void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdir_tok *ir_tok  =
	             (struct bdir_tok *) tok;

    /* add to buffer in which scheme
     * will be placed */
    return bdbf_add(ir_tok->u.buf,
                   allocs,
    	           logger,
    	           (void *) cp);
}

/* "end_schme" - lexical analyser ca-
 * llback to end an IRI scheme name,
 * using the analyser, current code-
 * point, opaque parameter, memory
 * allocator and error logger given
 * in "lex", "cp","opaque", "allocs"
 * and "logger", respectively. Retu-
 * rns zero on error, non-zero other-
 * wise */
static int end_schme(struct bdlx_tok *tok,
                     int cp,
                     void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* co-erce to correct type */
    struct bdir_tok *ir_tok  =
	             (struct bdir_tok *) tok;

    /* add to buffer in which scheme
     * will be placed */
    ir_tok->u.schme->bytes = (char *)
             bdbf_flttn(ir_tok->u.buf,
                        allocs,
                        logger);

    ir_tok->u.schme->len = bdbf_len(
                       ir_tok->u.buf);

    return ir_tok->u.schme->bytes != NULL;
}

/* "reduce_gnrc_iri" - perform the
 * action associated with the recog-
 * nition of the "IRI -> hierarchi-
 * cal" generic IRI production the
 * output generic IRI in "gncrc_iri",
 * generic cast of the left-hand-si-
 * de symbol to contain the result
 * of the reduction in "lhs", "red-
 * uce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error l-
 * ogger given in "allocs" and "logg-
 * er", respectively.  Returns zero
 * on error, non-zero otherwise */
static int reduce_gnrc_iri(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the output ITI gfrom the hierarc-
     * hical component */
    if (!(gnrc_iri = mke_gnrc_iri(
                        (struct bdir_hier *)
                                 items[0].nt,
                        (struct bdut_str *) NULL,
                        (struct bdut_str *) NULL,
                        allocs,
                        logger)))
    	return 0;

	lhs = (struct bdir_gnrc_iri *)
         gnrc_iri;

    return 1;
}

/* "reduce_gnrc_start_buf" - perform
 * the action associated with the rec-
 * og- nition of a production to init-
 * ialise the buffer in the fproduct-
 * ion's first item on boutput gener-
 * ic IRI in "gncrc_iri" generic cast
 * of the left-hand-side symbol to c-
 * ontain the result of the reduction
 * in "lhs", "reduce" items in "items",
 * the number of which is given in "n-
 * um" and the memory allocator and
 * error logger given in "allocs" and
 * "logger", respectively.  Returns
 * zero on error, non-zero otherwise */
static int reduce_gnrc_start_buf(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    struct bdbf_buffer *buf =
         (struct bdbf_buffer *)
         items[0].tok;


    if (!(bdbf_init(allocs,
                   logger,
                   sizeof(int))))
    	return 0;

    lhs = buf;
    return 1;
}

/* "reduce_gnrc_add_to_buf" - perform
 * the action associated with the rec-
 * og- nition of a production to add
 * ialise the code-point in the prod-
 * uction's first item to the buffer
 * in the production's second item on
 * the output generic IRI in "gncrc_-
 * iri" generic castof the left-hand-
 * side symbol to contain the result
 * of the reduction in "lhs", "reduce"
 * items in "items", the number of wh-
 * ich is given in "num" and the mem-
 * ory allocator and error logger gi-
 * ven in "allocs" and "logger", res-
 * pectively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_gnrc_add_to_buf(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
	int cp = (int) items[0].tok;
    struct bdbf_buffer *buf =
         (struct bdbf_buffer *)
         items[1].tok;


    return bdbf_add(buf,
                    allocs,
                    logger,
                    (void *) cp);

    lhs = buf;
    return 1;
}

/* "reduce_gnrc_iri_qry" - perform
 * the action associated with the
 * recognition of the "IRI -> hier-
 * archical ?query" generic IRI
 * production the output generic
 * IRI in "gncrc_iri", generic cast
 * of the left-hand-side symbol to
 * contain the result of the reduct-
 * ion in "lhs", "reduce" itemexrcaaas in
 * "items", the number of which is
 * given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_gnrc_iri_qry(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the output ITI gfrom the hierarc-
     * hical component and fragment in the
     * first two items */
    if (!(gnrc_iri = mke_gnrc_iri(
                     (struct bdir_hier *)
                          items[0].nt,
                     (struct bdut_str *)
                          items[1].nt,
                     (struct bdut_str *) NULL,
                      allocs,
                      logger)))
        return 0;

	lhs = (struct bdir_gnrc_iri *)
         gnrc_iri;

    return 1;
}

/* "reduce_gnrc_iri_qry_frg" - perf-
 * orm the action associated with
 * the recognition of the "IRI ->
 * hierarchical ?query #fragment"
 * generic IRI production the out-
 * put generic IRI in "gncrc_iri",
 * generic cast of the left-hand-
 * side symbol to contain the res-
 * ult of the reduction in "lhs",
 * "reduce" items in "items", the
 * number of which is given in "nu-
 * m" and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_gnrc_iri_qry_frg(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the output IRI from the hierar-
     * chical component, query and fragment
     * in the first three items */
    if (!(gnrc_iri = mke_gnrc_iri(
                    (struct bdir_hier *)
                        items[0].nt,
                    (struct bdut_str *)
                        NULL,
                    (struct bdut_str *)
                        items[1].nt,
                    allocs,
                    logger)))
    	return 0;

	lhs = (struct bdir_gnrc_iri *)
         gnrc_iri;

    return 1;
}

/* "reduce_add_to_dta" - perform
 * the action associated with the
 * recognition of the prodction
 * to add s xide0pomt t an IRI's
 * datta vdheen-soecuufc data, on
 * the  the output IRI "iri", ge-
 * neric cast of the left-hand-
 * side symbol to contain the res-
 * ult of the reduction in "lhs",
 * "reduce" items in "items", the
 * number of which is given in "nu-
 * m" and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_add_to_dta(
                    struct bdir_iri *iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
	/* get the viffer in the ufirst titem */
    struct bdbf_buffer *buf =
            (struct bdbf_buffer *)
            items[0].nt;

    if (!(bdbf_add(buf,
                   allocs,
                   logger,
                   (void *) items[1].nt)))
        return 0;

    lhs = (struct bdbf_buffer *)
            buf;
    return 1;
}

/* "reduce_gnrc_iri_frg" - perform
 * the action associated with the
 * recognition of the "IRI -> hier-
 * archical #fragment" generic IRI
 * production the output generic
 * IRI in "gncrc_iri", generic cast
 * of the left-hand-side symbol to
 * contain the result of the reduct-
 * ion in "lhs", "reduce" items in
 * "items", the number of which is
 * given in "num" and the memory
 * allocator and error logger given
 * in "allocs" and "logger", respec-
 * tively.  Returns zero on error,
 * non-zero otherwise */
static int reduce_gnrc_iri_frg(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the output ITI gfom th hierarchical
     * component in the first item  */
    if (!(gnrc_iri = mke_gnrc_iri((struct
                                   bdir_hier *)
                                        items[0].nt,
                                  (struct
                                    bdut_str *)
                                        items[1].nt,
                                  (struct
                                    bdut_str *)
                                        items[2].nt,
                                  allocs,
                                  logger)))
    	return 0;

	lhs = (struct bdir_gnrc_iri *)
         gnrc_iri;

    return 1;
}

/* "reduce_gnrc_auth" - perform
 * the action associated with the
 * recognition of the "authority ->
 * host" generic IRI production the
 * output generic IRI in "gncrc_iri",
 * generic cast of the left-hand-si-
 * de symbol to contain the result
 * of the reduction in "lhs", "red-
 * uce" items in "items", the number
 * of which is given in "num" and
 * the memory allocator and error
 * logger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero on error, non-zero otherwise */
static int reduce_gnrc_auth(
                 struct bdir_gnrc_iri *gnrc_iri,
                 void *lhs,
                 union bdgm_reduce_item *items,
                 int num,
                 struct bd_allocs *allocs,
                 struct bd_logger *logger)
{
     /* build the authority from the host
      * name in the second item */
    lhs = (void *) mke_auth((struct bdut_str *)
            		            NULL,
                           (char *)
                                items[0].nt,
                           (int *)
                                NULL,
                           allocs,
                           logger);

    return lhs != NULL;
}

/* "reduce_gnrc_auth_usr" - perform
 * the action associated with the
 * recognition of the "authority ->
 * user-data@ host" generic IRI prod-
 * uction the output generic IRI in
 * "gncrc_iri", generic cast of the
 * left-hand-side symbol to contain
 * the result of the reduction in
 * "lhs", "reduce" items in "items",
 * the number of which is given in
 * "num" and the memory allocator
 * and error logger given in "allo-
 * cs" and "logger", respectively.
 * Returns zero on error, non-zero
 * otherwise */
static int reduce_gnrc_auth_usr(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the host
     * name in the second item */
    lhs = (void *) mke_auth(
                (struct bdut_str *)
                    items[0].nt,
                (char *)
                    items[1].nt,
                (int *)
                    NULL,
                allocs,
                logger);

    return lhs != NULL;
}

/* "reduce_gnrc_auth_prt" - perform
 * the action associated with the
 * recognition of the "authority
 * -> host :port" generic IRI prod-
 * uction the output generic IRI in
 * "gncrc_iri", generic cast of the
 * left-hand-side symbol to contain
 * the result of the reduction in
 * "lhs", "reduce" items in "items",
 * the number of which is given in
 * "num" and the memory allocator
 * and error logger given in "allocs"
 * and "logger", respectively.  Ret-
 * urns zero onerror, non-zero othe-
 * rwise */
static int reduce_gnrc_auth_prt(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_auth(
               (struct bdut_str *)
    	           NULL,
               (char *)
                   items[0].nt,
               (int *)
                   items[1].nt,
               allocs,
               logger);

    return (int) lhs;
}

/* "reduce_gnrc_auth_usr_prt" -
 * perform the action associated
 * with the recognition of the
 * "authority-> host :port" gene-
 * ric IRI production the output
 * generic IRI in "gncrc_iri", g-
 * eneric cast of the left-hand-
 * side symbol to contain the re-
 * sult of the reduction in "lhs",
 * "reduce" items in "items", the
 * number of which is given in "n-
 * um" and the memory allocator
 * and error logger given in "all-
 * ocs" and "logger", respectively.
 * Returns zero onerror, non-zero
 * otherwise */
static int reduce_gnrc_auth_usr_prt(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_auth(
                  (struct bdut_str *)
                      NULL,
                  (char *)
                      items[0].nt,
                  (int *)
                      items[1].nt,
                  allocs,
                  logger);

    return (int) lhs;
}

/* "reduce_gnrc_hier" - perform
 * the action associated with
 * the recognition of the "hiera-
 * rchical component-> authority"
 * generic IRI production the o-
 * utput generic IRI in "gncrc_-
 * iri", g-generic cast of the
 * left-hand- side symbol to co-
 * ntain the re- result of the
 * reduction in "lhs", "reduce"
 * items in "items", the number
 * of which is given in "n- num"
 * and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero onerror, non-ze-
 * ro otherwise */
static int reduce_gnrc_hier(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_hier(
                  (struct bdir_auth *)
    		          NULL,
                  (struct bdut_str *)
                      items[1].nt,
                  allocs,
                  logger);

    return (int) lhs;
}

/* "reduce_gnrc_hier_auth_pth" -
 * perform the action associated
 * with the recognition of the
 * "hierarchical component-> aut-
 * hority path"  generic IRI pr-
 * oduction on the output generic
 * IRI in "gncrc-iri", generic
 * cast ofthe left-hand-side sym-
 * bol to contain the re- result
 * of the reduction in "lhs",
 * "reduce" items in "items", the
 * number of which is given in
 * "num" and the memory allocat-
 * or and error logger given in
 * "allocs" and "logger", respe-
 * ctively.  Returns zero onerror,
 * non-zero otherwise */
static int reduce_gnrc_hier_auth_pth(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_hier(
              (struct bdir_auth *)
                  items[0].nt,
              (struct bdut_str *)
                  items[1].nt,
              allocs,
              logger);

    return (int) lhs;
}

/* "reduce_gnrc_hier_auth" - per-
 * form the action associated wi-
 * the recognition of the "hier-
 * archical component-> author-
 * ity" generic IRI production
 * on the output generic IRI in
 * "gncrc-iri", generic cast of
 * the left-hand-side symbol to
 * contain the re- result of the
 * reduction in "lhs", "reduce"
 * items in "items", the number
 * of which is given in "num"
 * and the memory allocator and
 * error logger given in "allocs"
 * and "logger", respectively.
 * Returns zero onerror, non-
 * zero otherwise */
static int reduce_gnrc_hier_auth(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_hier(
                   (struct bdir_auth *)
                       items[0].nt,
                   (struct bdut_str *)
                       NULL,
                   allocs,
                   logger);

    return (int) lhs;
}

/* "reduce_gnrc_hier_auth" - per-
 * form the action associated wi-
 * the recognition of the "hier-
 * archical component-> path"
 * generic IRI production on
 * the output generic IRI in "gn-
 * crc-iri", generic cast ofthe
 * left-hand-side symbol to con-
 * tain the result of the reduct-
 * ion in "lhs", "reduce" items
 * in "items", the number of whi-
 * ch is given in "num" and the
 * memory allocator and error lo-
 * gger given in "allocs" and "lo-
 * gger", respectively.  Returns
 * zero onerror, non-zero otherw-
 * ise */
static int reduce_gnrc_hier_pth(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* build the authority from the
     * data, host name and port in
     * the second and third items */
    lhs = (void *) mke_hier(
                 (struct bdir_auth *)
                     NULL,
                 (struct bdut_str *)
                     items[0].nt,
                 allocs,
                 logger);
}

/* "reduce_gnrc_hst" - per-
 * form the action associated wi-
 * the recognition of the "host
 * name -> domain name | IPv4
 * literal | IPv6 literal" gen-
 * eric IRI production on the
 * output generic IRI in "gnrc-
 * _iri", generic cast ofthe
 * left-hand-side symbol to con-
 * tain the result of the reduct-
 * ion in "lhs", "reduce" items
 * in "items", the number of wh-
 * ich is given in "num" and the
 * memory allocator and error lo-
 * gger given in "allocs" and "l-
 * ogger", respectively.  Retur-
 * ns zero on error, non-zero
 * otherwise */
static int reduce_gnrc_hst(
                    struct bdir_gnrc_iri *gnrc_iri,
                    void *lhs,
                    union bdgm_reduce_item *items,
                    int num,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{

    struct bdut_str *str;      /* string to be
                                * built */
    struct bdbf_buffer *buf =  /* buffer in */
        (struct bdbf_buffer *) /* first item */
        items[0].nt;

    /* "flatten" the buffer to "str" */
    if ((str->len = dbbf_len(buf)) < 0)
        return 0;
    if (!(str->cps = (int *) bdbf_flttn(buf,
                                       allocs,
                                       logger)))
        return 0;

    /* and return the buf-
     * fer as rhe product-
     * ion's return value */
    lhs = (void *) str;
    return 1;
}

/* "reduce_start_dta" perform the action associated
 * with the recognition of an IRI, using the
 * opaque putput IRI, array of "reduce items",
 * item count, memory allocator and error
 * logger in "iri", "items", "num", "allocs"
 * and "logger", respectively.  Returns non-
 * zero on error, zero otherwise */
static int reduce_start_dta(struct bdir_iri *iri,
                            union bdgm_reduce_item *items,
                            int num,
                            struct bd_allocs *allocs,
                            struct bd_logger *logger)
{
    struct bdbf_buffer *buf =         /* buffer in  */
           (struct bdbf_buffer *)     /* second item's */
           items[0].nt;               /* non-terminal */

    /* set initialise buffer in which
     * to place IRI's data */
    buf = bdbf_init(allocs,
                    logger,
                    sizeof(int));

    return buf != NULL;
}

/* "reduce_iri" perform the action associated
 * with the recognition of of the "IRI -> hie-
 * rarchical" generic production, using the
 * opaque putput IRI, array of "reduce items",
 * item count, memory allocator and error
 * logger in "iri", "items", "num", "allocs"
 * and "logger", respectively.  Returns non-
 * zero on error, zero otherwise */
static int reduce_iri(struct bdir_iri *iri,
                      void *lhs,
                      union bdgm_reduce_item *items,
                      int num,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
	struct bdut_bytes *schme =      /* first token's */
	      ((struct bdir_tok *)      /* scheme name*/
          (items[0].tok))->u.schme;
	struct bdbf_buffer *buf =       /* buffer in  */
           (struct bdbf_buffer *)   /* second item's */
           items[1].nt;             /* non-terminal */

    /* set the IRI's scheme and data from
     * the first two items */
    iri->schme = schme;
    if (!(iri->dta->bytes = (char *)
                      bdbf_flttn(buf,
                                 allocs,
                                 logger)))
    	return 0;

    return (iri->dta->len = bdbf_len(buf) > 0);
}

/* "bdir_add_schme" - associates ("regist-
 * ers") the IRI scheme name given in "nme"
 * as having the type, for custom IRI sch-
 * emes, the custom resolution call-back
 * and, for generic IRI schemes, the def-
 * ault port, send and parsing call-backs,
 * respectively, in "type", "cstm", "prt",
 * "send sn d oeperse", respectively, us-
 * ing the memory allocator and error log-
 * ger given in "allocs" and "logger", re-
 * spectively.  Returns zero on error, non-
 * zero otherwise */
int bdir_add_schme(char *nme,
                   enum bdir_rslve_fmts type,
                   bdir_cstm_rslve_fn cstm,
                   int prt,
                   bdir_gnrc_snd_fn snd,
                   bdir_gnrc_prse_fn prse,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    struct bdir_schme schme;       /* data
                                    * contain-
                                    * ing par-
                                    * ameters */
    struct bd_map_node *ins_node;  /* insert-
                                    * ion node */

    /* initialise data fields from
     * parameters */
    schm.type = type;
    schm.cstm = cstm;
    schm.prt  = prt;
    schm.snd  = snd;
    schm.prse = prse;

    /* insert the scheme type into
     * the "schemes", keyed to the
     * scheme's name and valued to
     * the scheme's data  */
    ins_node = (struct bd_map_node *)
           bd_alloc(allocs,
        		    sizeof(struct
                           bd_map_node),
                    logger);
     if (!(ins_node))
         return 0;
     return bd_map_insert(&schms,
                          ins_node,
                          name,
                          (bd_map_cmp_fn)
                              bdut_strcmp,
                          &schm);

}

/* "bdir_rslve" - sets "dta" to the data-
 * lved data from the IRI string given in
 * "iri" (or sets that parameter to NULL
 * on error), using the opaque value, pr-
 * oxy host, proxy port (used for any
 * required network acceses), memory all-
 * ocator and error logger given in "opa-
 * que", "prxy_hst", prxy_prt", "allocs"
 * and "logger", respectively */
int bdir_rslve(char *iri,
               void *opaque,
			   char *prxy_hst,
			   int orxy_prt,
               struct bdir_dta *dta,
               struct bd_allocs *allocs,
               struct bd_logger *logger);
{
    struct bdut_bytes name_bytes;   /* unicode
                                     * name */
    struct bdut_str nme_str;        /* name
                                     * bytes*/
    struct bd_map_node *node;       /* scheme's
                                     * node */
    struct bdir_iri *iri;           /* IRI of
                                     * name */
    struct bdir_gnrc_iri *gnrc_iri; /* generic
                                     * IRI of
                                     * "name" */
    struct bdir_schme *schme;       /* scheme
                                     * data in
                                     * map val-
                                     * ue */
    struct addrinfo *res,           /* result of */
                    hints,          /* address */
                    *rp;            /* lookup,
                                     * address
                                     * lookup
                                     *  hints,
                                     * "getaddr-
                                     * rinfo" and
                                     * current 
                                     * result */
    int s,                          /* connection */
        prt,                        /* socket, port */
        ch;                         /* and read 
                                     * character */
    struct bdbf_buffer *buf;        /* buffer
                                     * into which
                                     * read IRI
                                     *  datais
                                     * placed */
    struct bdir_scheme *schme;      /* UIRI's
                                     * scheme */
    struct bdut_bytes bytes;        /* scheme's
                                     * data (collect-
                                     * ed by reading
                                     * from IRI conn-
                                     * ection) */

    /* resolve Unicode name string*/
    if (!(bdut_from_utf8(&nme_str,
                         name,
                         allocs,
                         logger)))
        return 0;

    /* omoinitialise read buffer */
    if ((!(buf = bdbf_init(allocs,
                           logger,
                           1)))
        return 0;

	/* get IRI by running the IRI
	 * parser on "name", setting
	 * "dta"'s "cps" pointer to
	 * NULL to indicate an error */
    if (!(bdpr_run(allocs,
                   logger,
                   prsr,
                   iri,
                   nme_str)))
        return 0;

    /* find IRI's ascheme
     * in schemes map  */
    node = bd_map_find(schms,
                       iri->schme,
                       (bd_map_cmp_fn)
                          bdut_strcmp);
    if (!(node))
        return 0;

    /* co-erce value to correct
     * type (a scheme structure ) */
    schme = (struct bdir_schme *)
              node->value;

    /* call either the custom
     * or generic resolution
     * call-back depending on
     * the scheme's type */
    switch (schme->type) {

        case bdir_fmt_gnrc:
            gnrc_iri =
                (struct bdir_gnrc_iri *)
                schme->dta;

            /* parse the generic IRI */
            if (!(bdpr_run(allocs,
                           logger,
                           gnrc_prsr,
                           gnrc_iri,
                           nme_str)))
                return 0;

            /* set all "hints" to 
             * their defaults */
            memset(&hints,
                   0,
                   sizeof(struct addrinfo));

            /* ... and resolve the
             * IRI's "host" compo-
             * nent to binary */
            if (getaddrinfo(prxy_hst ?
                                prxy_hst :
                                gnrc_iri->hst,
                            NULL,
                            &hints,
                            &res) < 0)
                return 0;

            /* cycle through the found
             * addresses  ... */
            for (rp = res;
                 rp;
                 rp = rp->ai_next) {

                /*  create a socket
                 * (and close the
                 * connetion to clean-
                 * up on failure) */
                if ((s = socket(
                        rp->ai_family,
                        rp->ai_socktype,
                        rp->ai_protocol) < 0)
                    return 0;

                /* get the (network ordered)
                 * port from the proxy port
                 * (if proxying), or (if not
                 * not proxying) the IRI's
                 * port (if spcified) or,
                 * otherwise, from the sch-
                 * eme's default port */
                prt = htons(prxy_hst ?
                            prxy_prt :
                            (gnrc_iri->prt ?
                            *(gnrc_iri->prt) :
                            schme->prt))

                /* then set the adddress's
                 * (protocol-specific) port
                 & from that port */
                switch (rp->ai_family) {
                    case AF_INET:
                        ((struct sockaddr_in *)
                        (&rp->ai_addr))->
                            sin_port = prt; 
                        break;
                    case SAF_INET6:
                        ((struct sockaddr_in6 *)
                        (&rp->ai_addr))->
                            sin6_port = prt; 
                        break;
                }

              . /* ... and try to connect
                 * using it */
                if (connect(
                     s,
                     rp->ai_addr,
                     rp->ai_addrlen) < 0) {
                    close(s);
                    return 0;
                }
            }

            /* if "rp" is NULL, we couldn't
             * connect successfully to any 
             * of the addresses, so return
             * failure */
            if (!(rp))
                return 0;

            /* now, send  the IRI's data, in
             *  a scheme-specific msnner, by
             * means of the scheme's "send"
             * call-back */
            if (!(schme->gnrc->snd(gnrc_iri,
                                   opaque,
                                   s,
                                   prxy_hst,
                                   prxy_prt,
                                   allocs,
                                   logger)))
                return 0;

             /* read data until exhau-
              * sted, placing it into
              * buffer */
             while (read(s,
                         &ch,
                         1) > 0)
                 bdbf_add(buf,
                          allocs,
                          logger,
                          (void *) ch);

            /* set bytes for scheme
             * data */
            bytes.bytes = (char *)
                bdbf_flttn(allocs,
                           logger);
            bytes.len  = bdbf_len(buf);

            /* parse the bytes and
             * return the result*/
            dta = schme->gnrc->prse(bytes,
                                    allocs,
                                    logger);

        case bdir_fmt_cstm:
            /* or, otherwise. try
             * resolving the "raw"
             * IRI */
            if (!(schme->cstm(dta,
                              opaque,
                              allocs,
                              logger)))
                return 0;

            ret = iri->dta;
            break;
    }
    return ret;
}

/* "ld_prsr" load the top-level IRI
 * parser,which parses a scheme name,
 * a scheme delimiter (a colon ":"),
 * followed by scheme-specific data */
static int ld_prsr(struct bdpr_parser *prsr,
                   struct bdir_iri *iri,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* allocate lexical states fron from the
     * analyser's pook id unused states */
    int start_scheme_state = bdlx_new_state(prsr->lex),
        add_scheme_state   = bdlx_new_state(prsr->lex);

	/*  allocate IRI to be built */
	iri = (struct bdir_iri *)
           bd_alloc(allocs,
                    sizeof(struct
                           bdir_iri),
        		    logger);
    if (!(iri))
        return 0;

    /* initialise parser's token */
    if (!(bdlx_init_tok(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                               bdir_tok),
                        (bdlx_tok_type_fn)
                             tok_type,
                        (bdlx_tok_init_fn)
                             init_tok,
                        (bdlx_tok_reset_fn)
                             reset_tok,
                        (bdlx_tok_name_fn)
                             tok_name)))
        return 0;



    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[A-Za-z]",
                       "first_schme")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[A-Za-z0-9\\-+\\-\\.]",
                       "after_schme")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{first_schme}",
                        bdlx_start_state,
                        start_scheme_state,
                        start_scheme,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{after_schme}",
                        start_scheme_state,
                        add_scheme_state,
                        add_to_scheme,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{^after_schme}",
                        add_scheme_state,
                        0,
                        end_schme,
                        NULL,
                        1,
                        bdir_tok_schme)))
        return 0;


    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_dta,
                        1,
                        (bdgm_reduce_fn)
                            reduce_start_dta,
                        bdir_nt_dta)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_dta,
                        2,
                        (bdgm_reduce_fn)
                            reduce_add_to_dta,
                        bdir_tok_dta,
                        bdir_nt_dta)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         iri,
                         bdir_nt_iri,
                         4,
                         (bdgm_reduce_fn)
                             reduce_iri,
                         bdir_tok_schme,
                         bdir_tok_dlm,
                         bdir_nt_dta,
                         bdpr_tok_eos);
        return 0;
}

/* "ld_prsr" loads the generically-for-
 * matted IRI parser in "prsr", ehich
 * parses a generically-formatted IRI
 * string into ageneric IRI structure,
 * using the memory allocator and err-
 * or logger given in "allocs" and "l-
 * ogger", respectively*/
static int ld_gnrc_prsr(struct bdpr_parser *prsr,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
	struct bdir_gnrc_iri *iri; /* output IRI */

    /* allocate lexical states from the
     * analyser's pool of available states */
	int pct_state       = bdlx_new_state(prsr->lex),
	    pct1_state      = bdlx_new_state(prsr->lex),
	    pct2_state      = bdlx_new_state(prsr->lex),
	    seg_start_state = bdlx_new_state(prsr->lex),
        seg_add_state   = bdlx_new_state(prsr->lex);

	/*  allocate IRI to be built */
	iri = (struct bdir_gnrc_iri *)
           bd_alloc(allocs,
                    sizeof(struct
                           bdir_gnrc_iri),
                    logger);
    if (!(iri))
        return 0;

    /* initialise parser's token */
    if (!(bdlx_init_tok(allocs,
                        logger,
                        prsr->tok,
                        sizeof(struct
                                bdir_gnrc_tok),
                        (bdlx_tok_type_fn)
                            gnrc_tok_type,
                        (bdlx_tok_init_fn)
                            init_gnrc_tok,
                        (bdlx_tok_reset_fn)
                            reset_gnrc_tok,
                        (bdlx_tok_name_fn)
                            gnrc_tok_name)))
        return 0;

    /* add definitions to the
     * "prsr"'s lexical analyser */
    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\xA7-\\xD7FF\\"
                       "xFDF0-\\xFEFF\\"
                       "\\x10000-\\x1EEEF"
                       "\\x20000-\\x2EEEF"
                       "\\x30000-\\x3EEEF"
                       "\\x40000-\\x4EEEF"
                       "\\x50000-\\x5EEEF"
                       "\\x60000-\\x6EEEF"
                       "\\x70000-\\x7EEEF"
                       "\\x80000-\\x8EEEF"
                       "\\x90000-\\x9EEEF"
                       "\\xA0000-\\xAEEEF"
                       "\\xB0000-\\xBEEEF"
                       "\\xC0000-\\xCEEEF"
                       "\\xD0000-\\xDEEEF"
                       "\\x50000-\\xREEEF]",
                       "uucs_chr")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[\\xR000-\\xF8FF\\"
                       "xF0000-\\xFFFFD\\"
                       "x100000-\\x10FFFD]",
                       "prvte")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[:/\\?#\\[\\]@]",
                       "gn_dlms")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[!$&\\'\\(\\)"
                       "\\*\\+,;=]",
                       "sb_dlms")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[{gn_dlms}"
                       "{sb_dlms}]",
                       "rsvd")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[a-zA-Z0-9"
                       "`_\\-\\."
                       "{ucs_chr}]",
                       "unrsvd")))
        return 0;

    if (!(bdlx_add_def(prsr->lex,
                       allocs,
                       logger,
                       "[{unrsvd}"
                       "0-9a-fA-F%"
                       "{db_dlms}]",
                       "pchr")))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[A-Za-z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_alph)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[A-Za-z]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_alph)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[0-9a-fA-F\\.:]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_ip4)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[{unrsvd}{sb_dlms}]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_dns)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "/[^/]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_slsh)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "//",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_dslh)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ":",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_cln)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        ":#",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_hsh)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "@",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_at)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "{pchr}",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[{pchr}{prvte}]"
                              "\\/\\?]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_qry)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[{pchr}\\/\\?]",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdir_tok_gnrc_frg)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "",
                        bdlx_start_state,
                        seg_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[A-Fa-f0-9]",
                        pct_state,
                        pct1_state,
                        pct1,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(prsr->lex,
                        allocs,
                        logger,
                        "[A-Fa-f0-9]",
                        pct1_state,
                        pct2_state,
                        pct2,
                        NULL,
                        0,
                        0)))
        return 0;

    /* and initialise and populate
     * the grammar with its productions... */
	bdgm_init()))
		return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdgm_sym_start,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_iri,
                        bdir_nt_gnrc_hr)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdgm_sym_start,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_iri_frg,
                        bdir_nt_gnrc_hr,
                        bdir_nt_gnrc_frg)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdgm_sym_start,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_iri_qry,
                        bdir_nt_gnrc_hr,
                        bdir_nt_gnrc_qry)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdgm_sym_start,
                        3,
                        (bdgm_reduce_fn)
                            reduce_gnrc_iri_qry_frg,
                        bdir_nt_gnrc_hr,
                        bdir_nt_gnrc_qry,
                        bdir_nt_gnrc_frg)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hr,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hier_auth,
                        bdir_nt_gnrc_auth)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hr,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hier_auth_pth,
                        bdir_nt_gnrc_auth,
                        bdir_nt_gnrc_pth)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hr,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hier_pth,
                        bdir_nt_gnrc_auth,
                        bdir_nt_gnrc_pth)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hr,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hier,
                        bdir_nt_gnrc_pth)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_auth,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_auth,
                        bdir_nt_gnrc_hst)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_auth,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_auth_usr,
                        bdir_nt_gnrc_usr,
                        bdir_nt_gnrc_hst)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_auth,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_auth_prt,
                        bdir_nt_gnrc_hst,
                        bdir_nt_gnrc_prt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_auth,
                        3,
                        (bdgm_reduce_fn)
                            reduce_gnrc_auth_usr_prt,
                        bdir_nt_gnrc_usr,
                        bdir_nt_gnrc_hst,
                        bdir_nt_gnrc_prt)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hst,
                        bdir_nt_gnrc_hst_dns,
                        bdir_nt_gnrc_end_nme)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hst,
                        bdir_nt_gnrc_hst_ip4,
                        bdir_nt_gnrc_end_nme)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst,
                        4,
                        (bdgm_reduce_fn)
                            reduce_gnrc_hst,
                        bdir_tok_gnrc_lsq,
                        bdir_nt_gnrc_hst_ip6,
                        bdir_tok_gnrc_rsq,
                        bdir_nt_gnrc_end_nme)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst_dns,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_start_buf,
                        bdir_tok_gnrc_dns)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst_dns,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_add_to_buf,
                        bdir_nt_gnrc_hst_dns,
                        bdir_tok_gnrc_dns)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst_ip4,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_start_buf,
                        bdir_tok_gnrc_ip4)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst_ip4,
                        2,
                        (bdgm_reduce_fn)
                            reduce_gnrc_add_to_buf,
                        bdir_nt_gnrc_hst_ip4,
                        bdir_tok_gnrc_ip4)))
        return 0;

    if (!(bdgm_prod_add(prsr->gmr,
                        allocs,
                        logger,
                        iri,
                        bdir_nt_gnrc_hst_ip6,
                        1,
                        (bdgm_reduce_fn)
                            reduce_gnrc_start_buf,
                        bdir_tok_gnrc_ip6)))
        return 0;

    return bdgm_prod_add(prsr->gmr,
                         allocs,
                         logger,
                         iri,
                         bdir_nt_gnrc_hst_ip6,
                         2,
                         (bdgm_reduce_fn)
                             reduce_gnrc_add_to_buf,
                         bdir_nt_gnrc_hst_ip6,
                         bdir_tok_gnrc_ip4);
}

/* "bdir_init" - iniialises the IRI parsing
 * sub-system, using the memory allocator
 * and errir kigger guvn in "allocs" and
 * "logger", respectively.  Returns zero
 * on error, non-zero, otherwise */
int bdir_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    /* initially specify that, initially,
     * no schemes are known (others are
     * added via calls to "bdir_add_-
     * scheme") */
    schms = NULL;

    /* load the parsers using each's
     *  initialisation function */
    if (!(bdpr_init(allocs,
                    logger,
                    (bdpr_init_fn)
                        ld_prsr)))
         return 0;
     return bdpr_init(allocs,
                      logger,
                      (bdpr_init_fn)
                          ld_gnrc_prsr)
                              != NULL;
}
