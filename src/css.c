/****************************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* css.c - function implementations relating to the
* parsing of a data stream formatted in according
* to the W3C's Cascading Style-sheets (CSS) specif-
* ication (2007 snapshot).
*
* Copyright (C) 2006-2013, Russell Potter, All
* Rights Reserved
*
****************************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/css.h>
#include <bikedemo/parser.h>
#include <bikedemo/buffer.h>
#include <bikedemo/lex.h>
#include <bikedemo/grammar.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

/* map of colour keywords */
static struct bd_map_node *clr_kwds;

/* "prsr, gnrc_prsr" - parser to
 parse style-sheet */
struct bdpr_parser *prsr;

/* cmp_clrs return a comparison of the
 * colours in clr1 and cir2, forming
 *  a map of colours comparison function */
 static int cmp_clrs(struct bdcs_clr *clr1,
                     struct bdcs_clr *clr2)
{
	/* return a component-by-component
	 * comparison of the colours */
	return clr1->r - clr2->r &&
           clr1->g - clr2->g &&
           clr1->b - clr2->b &&
	       clr1->b - clr2->a;
}

/* ld_clr - loads the colour named
 * in "name" and composed of the
 * red, green, blue and alpha  col-
 * our components from the colour
 * components and alpha channel (re-
 * presented as integers out of a
 * possible 255) in "r", "g", "b"
 * and "a", respectively, using the
 * memory allocator and error logg-
 * ergiven in "allocs" and "logger",
 * respectively
 *
 * Returns zero on failure, non-zero
 * otherwise*/
static int ld_clr(char *name,
                  int r,
                  int g,
                  int b,
                  int a,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{
    struct bdcs_clr clr;
   struct bd_map_node *ins_node; /* insertion
                                  * node */
         ins_node = (struct bd_map_node *)
              bd_alloc(allocs,
                       sizeof(struct
                              bd_map_node),
                       logger);
    if (!(ins_node))
       return 0;

	/* set the alpha channel to 1
     * for colour keywords */
    clr.a = 1.0;

     /* components are conv-
      * erted from integers
      *(out of 255)*/
    clr.r = (double) r / 255;
    clr.g = (double) g / 255;
    clr.b = (double) b / 255;
    clr.a = (double) a / 255;

    /* insert the colour of that
     * name into the map of colour
     * keyords, returning an
     * error if insertion fails */
    if (!(bd_map_insert(&clr_kwds,
                        ins_node,
                        name,
                        (bd_map_cmp_fn)
                             cmp_clrs,
                        &clr)))
        return 0;

    /* if we get to here, indicate
     * success */
    return 1;
}

/* ld_clr_kwds - load the 16 pre-defi-
 * ned CSS colour keyword names - and
 * each's associated colour - into the
 * colour keyword map.  return zero on
 * error, non-zero otherwise */
static int ld_clr_kwds(struct bd_allocs *allocs,
                       struct bd_logger *logger)
{
    /* initialise colour keyword map
     * to empty */
    clr_kwds = NULL;

    /* load colour keywords, dicating an error
     * if a load fails  */
    if (!(ld_clr("transparent",
                255, 255, 255, 0,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("aliceblue",
                240, 248, 255, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("antiquewhite",
                 250, 235, 215, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("aqua",
                 0, 255, 255, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("aquamarine", 1,
                 127, 255, 212,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("azure",
                 140, 255, 255, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("beige",
                 245, 245, 220, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("bisque",
                 255, 228, 196, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("black",
                 0, 0, 0, 1,
                 allocs,
                 logger)))
         return 0;

    if (!(ld_clr("blanchedalmond",
                 255, 235, 205, 1,
                 allocs,
                 logger)))
         return 0;

	/* if we get here, just return
     * the last call's status */
    return ld_clr("yellowgreen",
                  154,  205,  50, 1,
                  allocs,
                  logger);
}

/* id_start - CSS lexical analyser transition
 * callback to set the first code-point of
 * tok's identifier to the code-point given
 * in cp  Note, here, that opaque is unused.
 * Returns zero on an error, non-zero oth-
 * erwise */
static int id_start(struct bdlx_tok *tok,
                    int cp,
		            void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    /* get opaque as index into
     * string */

    /* initialise buffer holding identifier,
     * returning status of initialisation
     * routine */
	cs_tok->buf = bdbf_init(allocs,
                            logger,
                            sizeof(int));
	return cs_tok->buf != NULL; 
}


/* str_uc_esc_add - CSS lexical analyser trans-
 * ition callback to sset the first code-point of
 * tok's string to the code-point given in cp,
 * as ab dadittional part of a Unicode escape
 * sequence.  Returns zero on an error, non-zero
 * otherwise */
static int str_uc_esc_add(struct bdlx_tok *tok,
                          int dig,
		                  void *opaque,
                          struct bd_allocs *allocs,
                          struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* multiply tok's cpode-point by sm order of ma-
     * gnitude (to make space for the added dugut),
     * then add cp's value */
    cs_tok->u.cp *= 16;
	cs_tok->u.cp += bdut_from_hex(dig);

	return 1;
}

/* id_uc_esc_start - CSS lexical analyser trans-
 * ition callback to sset the first code-point of
 * tok's identifier to the code-point given in cp,
 * as the first part of a Unicode escape sequence
 * Returns zero on an error, non-zero otherwise */
static int id_uc_esc_start(struct bdlx_tok *tok,
                           int dig,
		                   void *opaque,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* set current code-point to dig */
     cs_tok->u.cp = dig;

	 return 1;
}

/* id_uc_esc_add - CSS lexical analyser trans-
 * ition callback to sset the first code-point of
 * tok's identifier to the code-point given in
 * cp, as an dadittional part of a Unicode escape
 * sequence.  Returns zero on an error, non-zero
 * otherwise */
static int id_uc_esc_add(struct bdlx_tok *tok,
                         int dig,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* set current index to cp */
    cs_tok->u.cp *= 16;
    cs_tok->u.cp += bdut_from_hex(dig);

    return 1;
}

/* str_add - CSS lexical analyser transition
 * callback to append the code-point in cp
 * to tok's string.  opaque contains the
 * index into string at which to set cp.
 * Returns zero on an error, non-zero other-
 * wise */
static int str_add(struct bdlx_tok *tok,
                   int cp,
		           void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* set tok's code-point to cp */
	cs_tok->u.cp = cp;

    return 1;
}

/* id_add - CSS lexical analyser transition
 * callback to append the code-point in cp
 * to tok's identifier.  opaque contains
 * the index into string at which to set cp.
 * Returns zero on an error, non-zero other-
 * wise  */
static int id_add(struct bdlx_tok *tok,
                  int cp,
		          void *opaque,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /*add cp to the token's buffer */
	return bdbf_add(cs_tok->buf,
	                allocs,
                    logger,
                    (void *) &cp);
}

/* uc_start - CSS lexical analyser transition
 * callback to start a unicode range sequence,
 * setting the tok's range field's upper and
 * lower bouds to the code-point in cp. Note 
 * Notr that the opaque value is unused here.
 * Returns zero on an error, non-zero other-
 * wise */
static int uc_start(struct bdlx_tok *tok,
                    int cp,
		            void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* start seqywbce vy setting both the
     * upper and lower bouds to cp */
	cs_tok->u.range.uppr =
    cs_tok->u.range.lowr = cp;

	return 1;
}

/* uc_add - CSS lexical analyser
 * transition callback to add a hexa-
 * decimal digit to  unicode range,
 * increasing both the range's upper
 * and lower bouds by he sane amount.
 * Note that the opaque value is
 * unused here.  Returns zero on an
 * error, non-zero otherwise */
static int uc_add(struct bdlx_tok *tok,
                  int cp,
		          void *opaque,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* multiply both bounds by an order
     * of magnitude (to make for the
	 * added digit) */
    cs_tok->u.range.uppr *= 16;
	cs_tok->u.range.uppr += bdut_from_hex(cp);
    cs_tok->u.range.lowr *= 16;
	cs_tok->u.range.lowr += bdut_from_hex(cp);

	return 1;
}

/* uc_add_wc - CSS lexical analyser
 * transition callback to add a wild-
 * card character to a unicode range,
 * so that the range's upper bound is
 * increased hexadecimal order of magn-
 * itude plud the naxmimum hexadecimal
 * character and lower boud is increas-
 * ed by only by an order of nagnitude.
 * Note that the opaque value is unused
 * here.  Returns zero on an error, non-
 * zero otherwise */
static int uc_add_wc(struct bdlx_tok *tok,
                     int cp,
		             void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* multiply both bounds by an order
	 * of magnitude (to make space for
	 * an added digit), but only add
	 * 0xF (the maximum mexa-decimal
     * digit) to the upper bound to
     * create a range to account for
     * the added wild-card */
    cs_tok->u.range.lowr *= 16;
    cs_tok->u.range.uppr *= 16;
    cs_tok->u.range.uppr += 0xF;

	return 1;
}

/* "uc_2_start_2" - CSS lexical anal-
 * yser transition callback to start
 * the upper bound of a dash-separated
 * unicode range.  Returns zero on an
 * error, non-zero otherwise */
static int uc_start_2(struct bdlx_tok *tok,
                      int cp,
		              void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* set the first digit of only
     * the upper bound : the lower
     * bound remains the same */
    cs_tok->u.range.uppr = cp;

	return 1;
}

/* "uc_2_add_2" - CSS lexical analyser
 * transition callback to add a hexadec-
 * imal digit to the upper bound of a
 * dash-separated Unicode range.  Ret-
 * urns zero on an error, non-zero
 * otherwise */
static int uc_add_2(struct bdlx_tok *tok,
                    int cp,
		            void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* multiply only the upper bound by
     * an order of magnitude (to make
     * spacefir he added digit): the lower
     * bound remains the same */
    cs_tok->u.range.uppr *= 16;
    cs_tok->u.range.uppr += bdut_from_hex(cp);

    return 1; 

}

/* "id_end" - CSS lexical analyser transition
 * callback to NULL-terminate tok's ident-
 * fier.  an integer cast of opaque contains
 * the length of the identifier after termi-
 * nation.  Returns zero on an error, non-
 * zero otherwise  */
static int id_end(struct bdlx_tok *tok,
                  int cp,
		          void *opaque,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	int len; /* length of identifer or
              * string */
    
    /* return status of NULL-termin-
     * ation string */
	if (!(bdbf_add(cs_tok->buf,
                   allocs,
                   logger,
			       0)))
        return 0;

    /* flatten and NULL-terminate
     * token's identifier to correct
     * length */
	cs_tok->u.id.cps = (int *) bdbf_flttn(
                                cs_tok->buf,
                                allocs,
                                logger);
	if (!(cs_tok->u.id.cps))
        return 0;
    len = bdbf_len(cs_tok->buf);
    if (len < 0)
        return 0;

    /* NULL-terminate "id"
     * string */
    cs_tok->u.id.cps[len] = 0;

    /* indicate success if
     * we get to here */
    return 1;
}

/* "get_clr_kwd" - CSS lexical analyser
 * transition callback to set "tok"'s
 * colour field from its name, a void
 * cast of which is given in opaque.
 * Returns zero on an error, non-zero
 * otherwise */
static int get_clr_kwd(struct bdlx_tok *tok,
                       int unused,
		               void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	struct bd_map_node *clr_node; /* node
                                   * of colour */
    
    /* get the colour keyword name from
     * opaque */
	char *name = (char *) opaque;

    /* ... find the name's node
     * in the colour keywords */
    clr_node = bd_map_find(clr_kwds,
	                      name,
                          (bd_map_find_fn)
                              strcmp);
    if (!(clr_node))
        return 0; 

    /* snd set the token's colour from
     * the node's key ( a colour) */
	cs_tok->u.clr = (struct bdcs_clr *)
                     (clr_node->value);

    /* if we get here,
     * we've succeeded */
    return 1;
}

/* "start_buffer" - CSS lexical analyser
 * transition cal-back to startcollect-
 * ing input into "tok"'s buffer, using
 * the memroy allocator snd error logger
 * givb in "allocs" and "logger", respe-
 * ctively.  Returns zero on an error,
 * non-zero otherwise */
static int start_buffer(struct bdlx_tok *tok,
                        int cp,
		                void *opaque,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* initialise "tok"'s buffer, then
     * add"cp" as its first code-point  */
    if (!(cs_tok->buf = bdbf_init(allocs,
                                  logger,
                                  sizeof(int))))
    	return 0;
    return bdbf_add(cs_tok->buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "add_to_buffer" - CSS lexical analyser
 * transition cal-back to sadd the code-
 * point "cp" to "tok"'s buffer, using
 * the memroy allocator snd error logger
 * givb in "allocs" and "logger", respe-
 * ctively.  Returns zero on an error,
 * non-zero otherwise */
static int add_to_buffer(struct bdlx_tok *tok,
                         int cp,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* initialise "tok"'s buffer, then
     * add"cp" as its first code-point  */
    if (!(cs_tok->buf = bdbf_init(allocs,
                                  logger,
                                  sizeof(int))))
    	return 0;
    return bdbf_add(cs_tok->buf,
                    allocs,
                    logger,
                    (void *) &cp);
}

/* "uc_esc_end" - CSS lexical analyser
 * transition callback to end of a Unicode
 * escape sequence in either tok's string or
 * identifier.  Returns zero on an error,
 * non-zero otherwise */
static int uc_esc_end(struct bdlx_tok *tok,
                      int cp,
		              void *opaque,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
    
    /* add the code-point built by the
     * escape sequence (atored in tok's
     * cp foeld) to tok's buffer,
     * returning the routine's status */
    return bdbf_add(cs_tok->buf,
                    allocs,
                    logger,
                    (void *) &cs_tok->u.cp);
}

/* "clr_hex_start" - CSS lexical anal-
 * yser transition callback to start a
 * hexa-decimal colour specification
 * sequence.  Note here "opaque" is
 * unused.  Returns zero on an error,
 * non-zero otherwise */
static int clr_hex_start(struct bdlx_tok *tok,
                         int cp,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* initialise index to zero for use by next
	 * state (i.e: adding further hexadecimal
     * digits) */
    cs_tok->idx = 0;

    return 1;
}

/* "clr_hex_add" - CSS lexical analyser
 * transition callback to add a digit
 * (giveb its cide-point roresentation
 * in cp) to the hexidecimal colour
 * specification sequence in tok's
 * colour field.  Note here opaque
 * is unused.  Returns zero on an error,
 * non-zero otherwise */
static int clr_hex_add(struct bdlx_tok *tok,
                       int cp,
		               void *opaque,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

	switch (cs_tok->idx++) {

        /* initialially assume thus us a 4-bit colour
		 * specification, in ehich case the colour is
         * actually an 8-bit colour eith a reprated
         * second digit (for eample #ABC ie #AABBCC) */
        case 0:
            cs_tok->u.clr->r = (double)((bdut_from_hex(cp) >> 4)
                                      + bdut_from_hex(cp) / 256);
            break;
        case 1:
            cs_tok->u.clr->g = (double)((bdut_from_hex(cp) >> 4)
                                      + bdut_from_hex(cp) / 256);
            break;
        case 2:
            cs_tok->u.clr->b = (double)((bdut_from_hex(cp) >> 4)
                                      + bdut_from_hex(cp) / 256);
            break;

        /* the fourth hexadecimal digit being parsed
         * means this must be an 8 bit colour spec, so
         * the actual 8 bit colour needs to be corr-
         * ected from the 4-bit repeated digit assump-
         * tion */
        case 3:
            cs_tok->u.clr->r += (double)((cs_tok->u.clr->r
                    - bdut_from_hex(cp)) / 256);
            break;
        case 4:
            cs_tok->u.clr->g += (double)((cs_tok->u.clr->g
                  - bdut_from_hex(cp)) / 256);
            break;
        case 5:
            cs_tok->u.clr->b += (double)((cs_tok->u.clr->b
                  - bdut_from_hex(cp)) / 256);
            break;

        default:
			return 0;

    }
    return 1;
}

/* "clr_hex_add" - CSS lexical analyser
 * transition callback to set the EGB
 * literal value of the CSS token in
 * "tok" eithout an alpha channel,
 * which defaults to 1 */
static int set_rgb(struct bdlx_tok *tok,
                   int cp,
		           void *unused,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* default the missing alpha
     * channel to one */
    cs_tok->u.clr->a = 1.0;
	return 1;
}

/* "start_dig" - CSS lexical analyser
 * transition callback to set tok's
 * number field to the code-point repre-
 * senting a numeric digit dig.
 * Returns zero on an error, non-zero
 * otherwise */
static int start_dig(struct bdlx_tok *tok,
                     int dig,
		             void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* and set the string's index
	 * to the modified value snd initi-
     * alise radix */
	 cs_tok->u.num = (int)(dig - '0');

    return 1;
}

/* "add_dig" - JavaScript lexical
 * analyser transition callback to
 * svrevck rgat a scanned identifier
 * the od is not a "future reserved
 * word",  (the list of which depends
 * of whether the code is running in
 * "strict mode")  */
static int add_dig(struct bdlx_tok *tok,
                   int unused,
                   void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{ 
    struct bdjs_rt *rt;
    /* "strict mode" future
     * reserved words*/
    char *strct_wds[] = {
       "enum",
       "extends",
        (char *) NULL,
    };
    /* "non-strict mode"
     * future reserved
     words*/
    char *nonstrct_wds[] = {
        "package",
        "private",
        "protected",
        "public",
        "static",
        "yield",
        (char *) NULL,
    };
    /* token coerced to correct type */
    struct bdcs_tok *js_tok =
              (struct bdcs_tok *) tok;


    /* get the parser run-time located
     * in the opaque value */
    rt = (struct bdjs_rt *)
           opaque;
    return 1;
}

/* "rgb_set_r" - CSS lexical analyser
 * transition callback to set the red
 * component of an RGB triple, as
 * parsed into the num field of
 * tok.  Returns zero on an error,
 * non-zero otherwise */
static int rgb_set_r(struct bdlx_tok *tok,
                     int dig,
		             void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	
    /* set the red component of
     * tok's colour from the
     * num field from which it
     * has parsed  */
    cs_tok->u.clr->r = cs_tok->u.num;

    return 1;
}

/* "rgb_to_pcnt_r" - CSS lexical analyser
 * transition callback to convert the red
 * component of an RGB triple, from a
 * range of (0 - 255) to a percentage.
 * Returns zero on an error, non-zero
 * otherwise */
static int rgb_to_pcnt_r(struct bdlx_tok *tok,
                         int dig,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	
    /* convert red component of tok's
     * colour to percentage */
    cs_tok->u.clr->r *= 255 / 100;

    return 1; 
}

/* "rgb_to_pcnt_g" - CSS lexical analyser
 * transition callback to convert the green
 * component of an RGB triple, from a
 * range of (0 - 255) to a percentage.
 * Returns zero on an error, non-zero
 * otherwise */
static int rgb_to_pcnt_g(struct bdlx_tok *tok,
                         int dig,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	
    /* convert green component of tok's
     * colour to percentage */
    cs_tok->u.clr->g *= 255 / 100;

    return 1;
}

/* "rgb_to_pcnt_b" - CSS lexical analyser
 * transition callback to convert the blue
 * component of an RGB triple, from a
 * range of (0 - 255) to a percentage.
 * Returns zero on an error, non-zero
 * otherwise */
static int rgb_to_pcnt_b(struct bdlx_tok *tok,
                         int dig,
		                 void *opaque,
                         struct bd_allocs *allocs,
                         struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	
    /* convert blue component of tok's
     * colour to percentage */
    cs_tok->u.clr->b *= 255 / 100;

    return 1;
}

/* "at_dot" - CSS lexical analyser
 * transition callback to handle a
 * a decimal pont in the currently
 * scanned number.  Returns zero on
 * an error, non-zero otherwise */
static int at_dot(struct bdlx_tok *tok,
                   int dig,
		           void *opaque,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;
	
    /* set radix to reflect integral
	 * nature of number (prior to
     * addition of fraction digits) */
    cs_tok->rdx = 1;

    return 1;
}

/* "end_units" - CSS lexical analyser
 * transition callback to end the
 * units of the currently scanned
 * measurement.  Returns zero on
 * an error, non-zero otherwise */
static int end_units(struct bdlx_tok *tok,
                     int dig,
		             void *opaque,
                     struct bd_allocs *allocs,
                     struct bd_logger *logger)
{
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* set he units string of "tok"'s
     * measurement field by "flattening"
     * its buffer, and set uts length
     * from the buffer's length */
    cs_tok->u.msrmnt.units.cps = (int *)
                 bdbf_flttn(cs_tok->buf,
                            allocs,
                            logger);
    cs_tok->u.msrmnt.units.len =
                 bdbf_len(cs_tok->buf);
    return 1;
}

/* "add_frac" - CSS lexical analyser
 * transition callback to append the
 * fractional figit in dig to tok's
 * number field.  Returns zero on an
 * error, non-zero otherwise */
static int add_frac(struct bdlx_tok *tok,
                    int dig,
		            void *opaque,
                    struct bd_allocs *allocs,
                    struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

	/* decrease the fraction radix to
     * reflect added fractional digit */
    cs_tok->rdx /= 10;

    /* increase the number accordingly  */
	cs_tok->u.num += cs_tok->rdx *
                     (int)(dig - '0');

    return 1;
}

/* to_pct - CSS lexical analyser
 * transition callback to convert tok's
 * number field to a percentage value.
 * Returns zero on an error, non-zero
 * otherwise */
static int to_pct(struct bdlx_tok *tok,
                  int unused,
		          void *opaque,
                  struct bd_allocs *allocs,
                  struct bd_logger *logger)
{ 
    /* coerce token to correct type */
	struct bdcs_tok *cs_tok =
              (struct bdcs_tok *) tok;

    /* and convert tok's number field
	 * field to a percentage */
	cs_tok->u.num /= 100;

    return 1;
}

/* reduce_chrst - perform the action
 * associated with the recognition of a
 * regular style-sheet "@charset" dec-
 * laration, using the output style-
 * sheet given in "sheet", left-hand-
 * side symbol in "lhs", reduce items
 * in "items", the number of which is
 * given in "num", and the memory all-
 * ocator and error logger given in
 * "allocs" and "logger", respectively.
 * Returns zero on an error, non-zero
 * otherwise  */
static int reduce_chrst(struct bdcs_sheet *sheet,
                        void *lhs,
                        union bdgm_reduce_item *items,
                        int num,
                        struct bd_allocs *allocs,
                        struct bd_logger *logger)
{
	return 1;
}

/* reduce_no_chrst - perform the action
 * associated with the recognition of
 * a style-sheet without a "@charset"
 * declaration, using the output style-
 * sheet given in "sheet", left-hand-
 * side symbol in "lhs", reduce items
 * in "items", the number of which is
 * given in "num", and the memory all-
 * ocator and error logger given in
 * "allocs" and "logger", respectively.
 * Returns zero on an error, non-zero
 * otherwise  */
static int reduce_no_chrst(struct bdcs_sheet *sheet,
                           void *lhs,
                           union bdgm_reduce_item *items,
                           int num,
                           struct bd_allocs *allocs,
                           struct bd_logger *logger)
{
    return 1;
}

/* load_parser - the parser initalisation
 * function used to load the grammar product-
 * ions and lexical rules constituting the
 * CSS parser given in css , using the
 * opaque parameter, memory allocator and
 * error logger given in, opaque, allocs
 * and logger, respectively */
static int load_parser(struct bdpr_parser *css,
                       struct bd_allocs *allocs,
                       struct bd_logger *logger)

{
    /* allocate a block of sequentially
     * increasing states, between which
     * the analyser wil transition, from
     * the analyeser's pool if available
     * states */
    int start_state               = bdlx_new_state(css->lex),
        at_state                  = bdlx_new_state(css->lex),
        cmt_state                 = bdlx_new_state(css->lex),
        cmt_slsh_state            = bdlx_new_state(css->lex),
        cmt_ast_state             = bdlx_new_state(css->lex),
        id_start_state            = bdlx_new_state(css->lex),
        id_add_state              = bdlx_new_state(css->lex),
        id_esc_start_state        = bdlx_new_state(css->lex),
        id_uc_esc_start_state     = bdlx_new_state(css->lex),
        id_uc_esc_add_state       = bdlx_new_state(css->lex),
        id_uc_esc_end_state       = bdlx_new_state(css->lex),
        at_start_state            = bdlx_new_state(css->lex),
        at_add_state              = bdlx_new_state(css->lex),
        at_esc_start_state        = bdlx_new_state(css->lex),
        at_uc_esc_start_state     = bdlx_new_state(css->lex),
        at_uc_esc_add_state       = bdlx_new_state(css->lex),
        at_uc_esc_end_state       = bdlx_new_state(css->lex),
        sstr_start_state          = bdlx_new_state(css->lex),
        sstr_add_state            = bdlx_new_state(css->lex),
        sstr_esc_start_state      = bdlx_new_state(css->lex),
        sstr_uc_esc_start_state   = bdlx_new_state(css->lex),
        sstr_uc_esc_add_state     = bdlx_new_state(css->lex),
        sstr_uc_esc_end_state     = bdlx_new_state(css->lex),
        sstr_esc_add_state        = bdlx_new_state(css->lex),
        sstr_esc_state            = bdlx_new_state(css->lex),
        dstr_add_state            = bdlx_new_state(css->lex),
        dstr_esc_start_state      = bdlx_new_state(css->lex),
        dstr_uc_esc_start_state   = bdlx_new_state(css->lex),
        dstr_uc_esc_add_state     = bdlx_new_state(css->lex),
        dstr_uc_esc_end_state     = bdlx_new_state(css->lex),
        url_start_state           = bdlx_new_state(css->lex),
        url_add_state             = bdlx_new_state(css->lex),
        url_esc_start_state       = bdlx_new_state(css->lex),
        url_uc_esc_start_state    = bdlx_new_state(css->lex),
        url_uc_esc_add_state      = bdlx_new_state(css->lex),
        url_dq_start_state        = bdlx_new_state(css->lex),
        url_dq_add_state          = bdlx_new_state(css->lex),
        url_dq_esc_start_state    = bdlx_new_state(css->lex),
        url_dq_uc_esc_start_state = bdlx_new_state(css->lex),
        url_dq_uc_esc_add_state   = bdlx_new_state(css->lex),
        url_dq_uc_esc_end_state   = bdlx_new_state(css->lex),
        start_dig_state           = bdlx_new_state(css->lex),
        add_dig_state             = bdlx_new_state(css->lex),
        at_dot_state              = bdlx_new_state(css->lex),
        start_frac_state          = bdlx_new_state(css->lex),
        add_frac_state            = bdlx_new_state(css->lex),
        end_num_state             = bdlx_new_state(css->lex),
        start_hex_chr_state       = bdlx_new_state(css->lex),
        add_hex_chr_state         = bdlx_new_state(css->lex),
        hex_clr_start_state       = bdlx_new_state(css->lex),
        hex_clr_add_state         = bdlx_new_state(css->lex),
        rgb_start_state           = bdlx_new_state(css->lex),
        rgb_start_r_state         = bdlx_new_state(css->lex),
        rgb_add_r_state           = bdlx_new_state(css->lex),
        rgb_dot_r_state           = bdlx_new_state(css->lex),
        rgb_add_frac_r_state      = bdlx_new_state(css->lex),
        rgb_pcnt_r_state          = bdlx_new_state(css->lex),
        rgb_end_r_state           = bdlx_new_state(css->lex),
        rgb_start_g_state         = bdlx_new_state(css->lex),
        rgb_add_g_state           = bdlx_new_state(css->lex),
        rgb_dot_g_state           = bdlx_new_state(css->lex),
        rgb_add_frac_g_state      = bdlx_new_state(css->lex),
        rgb_pcnt_g_state          = bdlx_new_state(css->lex),
        rgb_end_g_state           = bdlx_new_state(css->lex),
        rgb_start_b_state         = bdlx_new_state(css->lex),
        rgb_add_b_state           = bdlx_new_state(css->lex),
        rgb_dot_b_state           = bdlx_new_state(css->lex),
        rgb_add_frac_b_state      = bdlx_new_state(css->lex),
        rgb_pcnt_b_state          = bdlx_new_state(css->lex),
        rgb_end_b_state           = bdlx_new_state(css->lex),
        rgb_add_frac_a_state      = bdlx_new_state(css->lex),
        rgb_pcnt_a_state          = bdlx_new_state(css->lex),
        rgb_end_a_state           = bdlx_new_state(css->lex),
        uc_start_state            = bdlx_new_state(css->lex),
        uc_add_state              = bdlx_new_state(css->lex),
        uc_add_wc_state           = bdlx_new_state(css->lex),
        uc_start_2_state          = bdlx_new_state(css->lex),
        uc_add_2_state            = bdlx_new_state(css->lex),
        start_unit_state          = bdlx_new_state(css->lex),
        add_unit_state            = bdlx_new_state(css->lex),
        endnum_state              = bdlx_new_state(css->lex),
        clr_kwd_start_state       = bdlx_new_state(css->lex),
        clr_kwd_add_state         = bdlx_new_state(css->lex);

    /* add its constituent rules
     * to the CSS lexical analyser */
    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "hex",
                       "[a-fA-F0-9]")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "nasc",
                       "[\\x200-\\x377]")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "nmstrt",
                       "[a-zA-Z]|{nasc}")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "nmchr",
                       "[0-9a-zA-Z\\-]|{nasc}")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "urlchr",
                       "[!#$%&\\*\\-~]|{nasc}")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "nl",
                       "\\n|\\r\\n|\\r|\\f")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "ws",
                       "[ \\t\\r\\n\\f]+")))
        return 0;

    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "sstrchr",
                       "[\t\\ !#$%&\\(\\-~]|{nl}"
                             "|{nasc}]")))
        return 0;
    
    if (!(bdlx_add_def(css->lex,
                       allocs,
                       logger,
                       "dstrchr",
                      "[\t !#$%&\\(\\-~]|{nl}"
                             "|\'{nasc}]")))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "/\\*",
                        bdlx_start_state,
                        start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^\\*]",
                        cmt_state,
                        cmt_state,
                        NULL,
                        NULL,
                        0,
                        0)))
	        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\*",
                        cmt_state,
                        cmt_ast_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "/",
                        cmt_ast_state,
                        start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmstrt}",
                        start_state,
                        id_start_state,
                        id_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmchr}",
                        id_add_state,
                        id_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{^nmchr}",
                        id_add_state,
				        0,
                        id_end,
                        NULL,
                        1,
                        bdcs_tok_id)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\\\",
                        id_add_state,
                        id_esc_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        id_uc_esc_start_state,
                        id_uc_esc_add_state,
                        id_uc_esc_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        id_uc_esc_add_state,
                        id_uc_esc_add_state,
                        id_uc_esc_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\'",
                        start_state,
                        sstr_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\'",
                        bdlx_start_state,
                        sstr_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\",
                        sstr_start_state,
                        sstr_esc_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\"",
                        dstr_add_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_str)))
        return 0;
  
    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{sstrchr}",
                        sstr_add_state,
				        sstr_add_state,
                        str_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{dstrchr}",
                        dstr_add_state,
                        dstr_add_state,
                        str_add,
                        NULL,
				        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\{",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_lbr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\}",
                        start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_lbr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
		                "\\{",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
		                1,
                        bdcs_tok_rbr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{ws}",
                        bdlx_start_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_ws)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                       "{ws}",
                        dstr_add_state,
                        sstr_esc_start_state,
                        str_add,
                        NULL,
                        1,
                        bdcs_tok_ws)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        sstr_esc_add_state,
                        sstr_uc_esc_start_state,
                        id_uc_esc_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        sstr_esc_add_state,
                        sstr_uc_esc_add_state,
                        str_uc_esc_add,
	                    NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{^hex}",
                        sstr_esc_add_state,
                        sstr_add_state,
                        str_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "@",
                        bdlx_start_state,
                        at_start_state,
                        NULL,
                        NULL,
				        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
				        "import[^a-zA-Z]",
                        at_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_kwd_imprt)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmstrt}",
                        at_state,
                        at_start_state,
                        id_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmchr}",
                        at_start_state,
                        at_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmchr}",
                        at_add_state,
                        at_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{^nmchr}",
                        at_add_state,
                        0,
                        NULL,
                        id_add,
                        1,
                        bdcs_tok_at_kwd)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\\\",
                        at_start_state,
                        at_esc_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        at_uc_esc_start_state,
                        at_uc_esc_add_state,
                        id_uc_esc_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{^hex}",
                        at_uc_esc_add_state,
                        at_uc_esc_end_state,
                        uc_esc_end,
	                    NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        bdlx_start_state,
                        start_dig_state,
                        start_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        start_dig_state,
                        add_dig_state,
                        add_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if ((bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\.",
                        add_dig_state,
                        at_dot_state,
                        at_dot,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        add_frac_state,
                        add_frac_state,
                        add_frac,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^0-9]",
                        add_frac_state,
                        end_num_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        end_num_state,
                        start_unit_state,
                        start_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        start_unit_state,
                        add_unit_state,
                        add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        add_unit_state,
                        add_unit_state,
                        add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^a-zA-Z]",
                        add_unit_state,
                        0,
                        end_units,
                        NULL,
                        1,
                        bdcs_tok_msrmnt)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "%",
                        end_num_state,
                        0,
                        to_pct,
                        NULL,
                        1,
                        bdcs_tok_pcnt)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^0-9]",
                        add_frac_state,
                        end_num_state,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_num)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[uU][rR][lL]\\(",
                         start_state,
                         url_start_state,
                         NULL,
                         NULL,
                         0,
                         0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
	                        "url(\"",
                        start_state,
                        url_dq_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{urlchr}",
                        url_dq_start_state,
                        url_dq_add_state,
                        id_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{urlchr}",
                        url_dq_add_state,
                        url_dq_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\)",
                        url_dq_add_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_url)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmstrt}",
                        at_state,
                        at_start_state,
                        id_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmchr}",
                        at_start_state,
                        at_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{nmchr}",
                        at_add_state,
                        at_add_state,
                        id_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{^nmchr}",
                        at_add_state,
                        0,
                        id_add,
                        NULL,
                        1,
                        bdcs_tok_at_kwd)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{\\\\",
                        at_add_state,
                        at_esc_start_state,
                        id_uc_esc_start,
                        NULL,
                        0,
                        0)))
        return 0;

   if (!(bdlx_add_rule(css->lex,
                       allocs,
                       logger,
                       "{hex}",
                       at_esc_start_state,
                       at_uc_esc_start_state,
                       id_uc_esc_add,
                       NULL,
                       0,
                       0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                         logger,
                        "#",
                        start_state,
                        hex_clr_start_state,
                        clr_hex_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        hex_clr_start_state,
                        hex_clr_add_state,
                        clr_hex_add,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[rR][gG][bB]\\(",
                        start_state,
                        rgb_start_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_start_state,
                        rgb_add_r_state,
                        start_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_add_r_state,
                        rgb_add_r_state,
                        add_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_dot_r_state,
                        rgb_add_frac_r_state,
                        add_frac,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\.",
                        rgb_add_r_state,
                        rgb_dot_r_state,
                        at_dot,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "%",
                        rgb_add_frac_r_state,
                        rgb_pcnt_r_state,
                        rgb_to_pcnt_r,
                        NULL,
	                    0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        ",",
                        rgb_add_frac_r_state,
                        rgb_end_r_state,
                        rgb_set_r,
                        NULL,
				        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{ws}",
                        rgb_end_r_state,
                        rgb_end_r_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_end_r_state,
                        rgb_start_g_state,
                        start_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_add_g_state,
                        rgb_add_g_state,
                        add_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_dot_g_state,
                        rgb_add_frac_g_state,
                        add_frac,
				        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\.",
                        rgb_add_g_state,
                        rgb_dot_g_state,
                        at_dot,
				        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "%",
                        rgb_add_frac_g_state,
                        rgb_pcnt_g_state,
                        rgb_to_pcnt_g,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{ws}",
                        rgb_add_frac_g_state,
                        rgb_end_g_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        ",",
                        rgb_add_frac_g_state,
                        rgb_end_g_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{ws}",
                        rgb_end_g_state,
                        rgb_end_g_state,
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_end_g_state,
                        rgb_start_b_state,
                        start_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_start_b_state,
                        rgb_add_b_state,
                        add_dig,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[0-9]",
                        rgb_dot_b_state,
                        rgb_add_frac_b_state,
                        add_frac,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\.",
                        rgb_add_b_state,
                        rgb_dot_b_state,
                        at_dot,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "%",
                        rgb_add_frac_b_state,
                        rgb_pcnt_b_state,
                        rgb_to_pcnt_b,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\)",
                        rgb_add_frac_b_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_clr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\)",
                        rgb_pcnt_b_state,
                        0,
                        set_rgb,
                        NULL,
                        1,
                        bdcs_tok_clr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        bdlx_start_state,
                        clr_kwd_start_state,
                        start_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        clr_kwd_start_state,
                        clr_kwd_add_state,
                        add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[a-zA-Z]",
                        clr_kwd_add_state,
                        clr_kwd_add_state,
                       add_to_buffer,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^a-zA-Z]",
                        clr_kwd_add_state,
                        0,
                        (bdlx_trans_fn)
                             get_clr_kwd,
                        NULL,
                        1,
                        bdcs_tok_clr)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
	                    "U\\+",
                        start_state,
                        uc_start_state, 
                        NULL,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
				        "{hex}",
                        uc_start_state,
                        uc_add_state,
                        uc_start,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        uc_add_state,
                        uc_add_state,
                        uc_add,
                        NULL,
                        0,
                        0)))
        return 0;
				  
    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\?",
                        uc_add_state,
                        uc_add_wc_state,
                        uc_add_wc,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "[^?]",
                        uc_add_wc_state,
                        0,
                        NULL,
                        NULL,
                        1,
                        bdcs_tok_uc_rnge)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "\\-",
                        uc_add_state,
                        uc_start_2_state,
                        uc_start_2,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
		                "{hex}",
                        uc_add_2_state,
				        uc_start_2_state,
                        uc_start_2,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
                        "{hex}",
                        uc_add_2_state,
				        uc_add_2_state,
                        uc_add_2,
                        NULL,
                        0,
                        0)))
        return 0;

    if (!(bdlx_add_rule(css->lex,
                        allocs,
                        logger,
				        "{^hex}",
                        uc_add_2_state,
                        0,
	                    NULL,
                        NULL,
                        1,
                        bdcs_tok_uc_rnge)))
        return 0;

    /* ... and populate the CSS
     * parser with its grammar
     * productions */
    if (!(bdgm_prod_add(css->gmr,
                        allocs,
                        logger,
                        NULL,
                        bdcs_nt_sheet,
                        3,
                        (bdgm_reduce_fn)
                              NULL,
                        bdcs_nt_chrst,
                        bdcs_nt_imprts,
                        bdcs_nt_at_drctv)))
        return 0;

    if (!(bdgm_prod_add(css->gmr,
                        allocs,
                        logger,
                        NULL,
                        bdcs_nt_chrst,
                        0,
				        (bdgm_reduce_fn)
				             reduce_no_chrst)))
        return 0;

    /* if we get here, just return
     * the call's status  */
    return (int) bdgm_prod_add(css->gmr,
                               allocs,
                               logger,
                               NULL,
                               bdcs_nt_chrst,
                               2,
                               (bdgm_reduce_fn)
                                   reduce_chrst,
                                bdcs_tok_kwd_chrst,
				                bdcs_tok_str);
}

/* "bdcs_init" - initialises the CSS sub-system,
 * using the memory allocator and error logger
 * given in "allocs" and "logger", respectively.
 * Returns on error, non-zero otherwise */
int bdcs_init(struct bd_allocs *allocs,
              struct bd_logger *logger)
{
	if (!(ld_clr_kwds(allocs,
                      logger)))
	    return 0;
    bdpr_init(prsr,
              allocs,
              logger,
              load_parser);

    return prsr != NULL;
}
