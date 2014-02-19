/*************`****************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* parser.h - data structures and function proto-
* types concerning programming language parsers.
*
* Such parsers each consist of a grammar produc-
* tion set, a grammar table created by he produ-
* ction set, a lexical analyser to tokenise the
* grammar table's input and the parser's build-
* time structure used to generate correct run-
* time environment.
* 
* Copyright (C) 2006-2014, Russell Potter,
* All rights reserved
*
******************************************************************/

#ifndef __BD_PARSER_H
#define __BD_PARSER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/lex.h>
#include <bikedemo/grammar.h>
#include <bikedemo/allocs.h> 
#include <bikedemo/log.h> 

/* "bdlx_res_toks" enumeration - list 
 * of "special" reserved parser tokens,
 * consisting of the "end of stream" token */
enum bdpr_res_toks {
    
    bdpr_tok_eos,    /* end of stream */
    bdpr_tok_num_res /* number of reserved
                      * tokens */
};
 
/* "bdpr_in" - structure - combination of a parser,
 * the current position in an input stream of
 * code-points: used by a parser to pack these
 * fields into the parser's grammar table token
 * input call-back opaque value */
struct bdpr_in {

    struct bdpr_parser *prsr; /* relevant parser */

    int *in;                  /* pointer to current
                               * code-point in parser's
                               * input stream */
};

/* "bdpr_init_fn" typedef - proto-type of the
 * function  to load a parser with its constit-
 * uent lexical rules and grammar productions,
 * and to populate its token with its call-backs.
 *
 * athe first parameter is the parser to load,
 * the second and third parameters are the memory
 * allocator and error logger to use in the load,
 * respectively.  Returns zero on error, non-zero
 * otherwise */
typedef int (* bdpr_init_fn)(struct bdpr_parser *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdpr_dsptch_fn" typedef - proto-type of the
 * function  to receive a lexical token from a
 * parser's lexical analyser
 *
 * the first parameter is the received lexical
 * eoken, the second is the relevant parser,
 * and the third and fourth is the the memory
 * allocator and error logger to
 * use.  Returns zero on error, non-zero oth-
 * erwise */
typedef int (* bdpr_dsptch_fn)(struct bdlx_tok *,
                               struct bdpr_parser *,
                               struct bd_allocs *,
                               struct bd_logger *);

/* "ihpr_parser" structure - represents a combin-
 * ation of the grammar table, lexical analyser
 * and the lexical token used by a language, tog-
 * ether with the grammar productions and resul-
 * ting parser table built from the grammar pro-
 * ductions, the parser's initialisation funct-
 * ion and the parser's run-time info. */
struct bdpr_parser {

    struct bdgm_grmr *gmr;    /* language's grammar */

    struct bdlx_lex *lex;     /* its lexical analyser */

    struct bdlx_tok *tok;     /* analyser's token */

    struct bdgm_table *table; /* parser table (as gene-
                               * rated from grammar)*/

    bdpr_init_fn init;        /* initialisation function */

    void *rt;                 /* run-time data pointer */
};

/** Function proto-types ***/

/* "ihpr_init" - allocates, initialises and returns,
 * using the memory allocator and error logger given 
 * in the first and second and third parameters, re-
 * spectively, a parser whose grammar productions
 * and lexical rules have been are inititially set
 * from the base parser given on the first parameter,
 * and extended and re-defined (sub-classed) via a
 * call to the initialisation function given in the
 * fourth parameter.  Returns NULL on an error, the
 * loaded parser otherwise */
struct bdpr_parser *bdpr_init(struct bdpr_parser *,
                              struct bd_allocs *,
                              struct bd_logger *,
                              bdpr_init_fn); 

/* "ihpr_insert" - "insert" (that is, synthes-
 * ise as if next on an input stream) the token
 * type given in the fourth parameter into
 * the current position of the stream, using
 * the memory allocator and error logger given
 * in the first and second parameters, respect-
 * ively.  Returns zero on an error, non-zero
 * otherwise  */
int bdpr_insert(struct bd_allocs *,
                struct bd_logger *,
                struct bdpr_parser *,
                int);


/* "bdpr_run" - runs, if the dispatch
 * call-back given in the fifth para-
 * meter is NULL, the grammar table of
 * the parser given in the third para-
 * meter, caused by input to the ta-
 * ble of the list of lexical tokens
 * resulting from lexical analysis, by
 * the parser's lexical analyser, of
 * the string given in the sixth para-
 * meter, using the memory allocator,
 * error logger and opaque pointer
 * given in the first, second and
 * fourth paarameters, respectively.
 * If the fifth parameter is, however,
 * non-NULL, the call-back is, instead,
 * called on each of those resulting
 * tokens.  Returns zero on an error,
 * non-zero otherwise  */
int bdpr_run(struct bd_allocs *,
             struct bd_logger *,
             struct bdpr_parser *,
             void *,
             bdpr_dsptch_fn,
             struct bdut_str *);

/* "bdpr_cleanup" - reclaims the resources used
 * by the parser given in the first parameter,
 * usually just before it is deleted */
void bdpr_cleanup(struct bdpr_parser *); 

#ifdef __cplusplus
}
#endif

#endif

