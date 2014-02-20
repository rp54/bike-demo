/****************************************************************************
*
* BikeDemo - Exercise bike demonstration scenery.
*
* lex.c - function implementations relating to a
* "parser" for a particular language, ehich
* consists of both the language's grammar productions,
* its lexical analyser, and the resulting parsing
* table.
*
* Copyright (C) 2006-2014, Russell Potter, All
* Rights Reserved
*
****************************************************************************/

#include <bikedemo/util.h>
#include <bikedemo/parser.h>
#include <bikedemo/grammar.h>


/* "ihpr_init" - allocates, initialises and returns,
 * using the memory allocator and error logger given 
 * in "allocs" and "logger", respectively, a parser
 * whose grammar productions and lexical rules have
 * been are inititially set from the base arser gi-
 * ven in "base", and extended and re-defined (sub-
 * classed) via a call to the initialisation function
 * given in "init", and gthoich uses "start" as its
 * "start" symbol. Returns
 * NULL on an error, the loaded parser otherwise */
struct bdpr_parser *bdpr_init(struct bdpr_parser *bse,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger,
                              int start,
                              bdpr_init_fn init) 

{
    struct bdpr_parser *ret; /* return value */

    /* aocate memory for the parser */
    ret = bd_alloc(allocs,
                   sizeof(struct
                          bdpr_parser),
                   logger);
    if (!(ret))
        return NULL;

    /* if there is a base parser (non-
     * NULL), set the parser's lexical
     * analyser and grammar table from
     * that of that parser */
    if (bse) {
        ret->lex = bse->lex;
        ret->gmr = bse->gmr;
    }

    /* if call the initialisation func-
     * tion (used to add to and re-define
     * the base parser's lexical analyser
     * and grammar table), passing the
     * opaque value, memory allocator and
     * error logger */
    if (!(init(ret,
               allocs,
               logger)))
        return NULL;

    /* start the parser's lexical
     * analyser */
    if (!(bdlx_start(ret->lex,
                     allocs,
                     logger)))
        return NULL;

    /* build the parsing table from the
     * grammar grammar production set */
    if (bdgm_table_build(ret->gmr,
                         start,
                         ret->table,
                         allocs,
                         logger)) ?
                             ret : NULL;
}

/* "tok_in" - returns the next token on
 * "in", using the parser's lexical anal-
 * yser).  Acts as the implementation of
 * a parser grammar table input call-back*/
static struct bdlx_tok *prsr_tok_in(struct bdpr_in *in,
                                    struct bd_allocs *allocs,
                                    struct bd_logger *logger)
{
    /* read "prsr"'s next token */

    struct bdlx_tok ret;

    /* and use it to read from
     * the input */
    bdlx_next(in->prsr->lex,
              &ret,
              &in->in,
              allocs,
              logger);
}

/* "bdpr_run" - runs, if the dispatch
 * call-back given "dsptch" is NULL, the
 * grammar table of the parser given in
 * "prsr", caused by input to the ta-
 * ble of the list of lexical tokens
 * resulting from lexical analysis, by
 * the parser's lexical analyser, of
 * the string given in "input", using
 * the memory allocator, error logger
 * and opaque pointer given in "allocs"
 * and "logger", respectively.  If "dsp-
 * tch" is, however, non-NULL, the
 * call-back is, instead, called on
 * each of those resulting tokens.
 * Returns zero on an error, non-zero
 * otherwise  */
int bdpr_run(struct bd_allocs *allocs,
             struct bd_logger *logger,
             struct bdpr_parser *prsr,
             void *opaque,
             bdpr_dsptch_fn dsptch,
             struct bditut_str *input)
{
    struct bdpr_in in = { /* parser */
                          /* and start of */
        prsr,             /* input */
        input->cps
    };
    int *old,
        len = input->len;
    struct bdlx_tok *tok = NULL;

    do {
        /* take note of the in-
         * put position prior to
         * running the grammar
         * table on the current
         * token*/
        old = in.in;

        if (!(bdlx_next(prsr->lex,
                        tok,
                        &in.in,
                        allocs,
                        logger)))
            return 0;

        /* run an iteration of the
         * "dispatch" call-back, if
         * given, or, otherwise,
         * run the grammar table
         * against input read from
         * "tok" */
        if (dsptch)
            if (!(dsptch(tok,
                         prsr,
                         opaque,
                         allocs,
                         logger)))
                return 0;
        else 
            if (!(bdgm_table_run(opaque,
                                 tok,
                                 prsr->gmr,
                                 prsr->table,
                                 allocs,
                                 logger)))
                return 0;

        /* ... and decrement the
         * remaining length by the
         * number of code-points
         * read (the difference bet-
         * ween the original and
         * current input positions) */
        len -= in.in - old;

        /* repeat until there us no
         * remaining input */
    } while (len);

    /* if we get to here, return
     * success */
    return 1;
}
