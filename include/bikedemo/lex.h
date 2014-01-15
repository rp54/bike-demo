/*****************************************************************
*
* BikeDemo - Exercise bike demonstration
* scenery
*
* lex.h - data types and function proto-types relat-
* ed to lexical analysis (that is, conversion of a
* character stream into a sequence of "tokens" such
* as, for example, a number, keyword or string lite-
* ral).
*
* The analyser is able to recognise, at its most
* basic level, "rules", each of which contain regu-
* lar expression input patterns associated with an
* action on the resulting input token that, coll-
* ectively, set the token to its appropriate value,
* and "definitions" that associate a name with a
* regular expression, so that such an expression may
* be called by name from a rule.
*
* Copyright (C) 2006-2014, Russell Potter, All
* rights reserved
*
******************************************************************/

#ifndef __BD_LEX_H
#define __BD_LEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/grammar.h>  
#include <bikedemo/eval.h>
#include <bikedemo/allocs.h>  
#include <bikedemo/log.h>

struct bdlx_tok;

/* "bdlx_re_op_types" enumeration - possible
 * types of regular expression operators
 * (such as optional repetition or altern-
 * ation) that may be pushed onto the reg-
 * ular expression evaluation stack */
enum bdlx_re_op_types {

    bdlx_re_op_rep,       /* repeat loop */
    bdlx_re_op_alt,       /* alternate
                           * choices  */
    bdlx_re_op_seq,       /* etual seguence */
    bdlx_re_op_lpr,       /* open grouping */
    bdlx_re_op_rpr,       /* close grouping */
    bdlx_re_op_num        /* number of operators */
}; 

/* "bdlx_re_cdes" enumeration - possible
 * "regular expression codes" that will,
 * when "executed" by regular expression
 * "virtual machine", generate an NFA able,
 * to, when converted to a DFA, to accept
 * input matching the regular expression */
enum bdlx_re_cdes {

    bdlx_re_cde_rep =      /* repeat an */
        bdev_res_cdes_num, /* NFA one or more
                            * times */
    bdlx_re_cde_chce,      /* alternate betw-
    		                * een choice of
    		                * NFAs */
    bdlx_re_cde_seq,       /* append to seque-
                            * nce of NFAs */
    bdlx_re_cde_dfn_ref,   /* reference a seque-
                            * nce of NFAs */
    bdlx_re_cde_neg,       /* negate stack top */
    bdlx_re_cde_end,       /* cend of expression */
    bdlx_re_cdes_num       /* number of operators */
};

/* "bdlx_re_toks" enumeration - possible
 * tokens returned by the regular expre-
 * ssion lexical analyser */
enum bdlx_re_toks {

    bdlx_re_tok_ast,    /* zero or more reps */
    bdlx_re_tok_qstn,  /* zero or mone rep */
    bdlx_re_tok_sym,   /* immediate symbol */
    bdlx_re_tok_lcrly, /* right curly bracket */
    bdlx_re_tok_rcrly, /* one or more reps */
    bdlx_re_tok_bksl,  /* backslash ("\") */
    bdlx_re_tok_plus,  /* zero or one rep */
    bdlx_re_tok_lpar,  /* open grouping */
    bdlx_re_tok_rpar,  /* close grouping */
    bdlx_re_tok_bar,   /* choice */
    bdlx_re_tok_dsh,   /* range dash */
    bdlx_re_tok_lsq,   /* open square bracket  */
    bdlx_re_tok_rsq,   /* close squsre bracket */
    bdlx_re_tok_lcly,  /* open curly bracket */
    bdlx_re_tok_rcly,  /* close curly bracket */
    bdlx_re_tok_chr,   /* any other character or
                        * escape sequence (either
                        * decimal or hexa-decimal) */
    bdlx_re_tok_ltr,   /* letter ([a-zA-Z]) */
    bdlx_re_tok_cma,   /* comma (",") */
    bdlx_re_tok_crt,   /* caret ("^") */
    bdlx_re_tok_int,   /* integral number
                        * ([0-9]+) */
    bdlx_re_tok_any,   /* any character (".") */
    bdlx_re_tok_defn,  /* definition name: a
                        * sequence of letters
                        * ("[a-zA-Z]+") */
    bdlx_re_tok_num    /* number of tokens */
}; 

/* "bdlx_re_nts" enumeration - the non-terminals
 * tokens returned by the regular expression
 * grammar */
enum bdlx_re_nts {

    bdlx_re_nt_re = bdlx_re_tok_num + 1,
    bdlx_re_nt_re_list,
    bdlx_re_nt_esc,
    bdlx_re_nt_expr,
    bdlx_re_nt_chr,
    bdlx_re_nt_clss_list,
    bdlx_re_nt_clsses,
    bdlx_re_nt_clss,
    bdlx_re_nt_clss_sym,
    bdlx_re_nt_clss_rnge,
    bdlx_re_nt_rep,
    bdlx_re_nt_to_rep,
    bdlx_re_nt_from_rep,
    bdlx_re_nt_rnge_rep,
    bdlx_re_nt_clist,
    bdlx_re_nt_neg_clist,
    bdlx_re_nt_def_ref,
    bdlx_re_nt_neg_def_ref,
    bdlx_re_nt_neg_clss,
    bdlx_re_nt_clss_chr,
    bdlx_re_nt_dgts_on,
    bdlx_re_nt_dgts_off,
    bdlx_re_nt_dfn_on,
    bdlx_re_nt_dfn_off
}; 

/* "bdlx_sym_types" enumeration - lists the 
 * transition symbol types, which may be
 * either empty or non-empty */
enum bdlx_sym_types {
    
    bdlx_sym_type_empty, /* no input
                          * symbol read
                          * required for
                          * transition */
    bdlx_sym_type_nrml   /* non-empty,
                          * "normal"
                          * symbol */
};

/* "bdlx_re_evl_stk_types" enumeration -
 * the possible types the nay be placed on
 * the regular expression evaluator's run-
 * time stack: namely, NFAs, strings or
 * integers */
enum bdlx_re_stk_types {

    bdlx_re_stk_type_nfa, /* NFA */
    bdlx_re_stk_type_str, /* string */
    bdlx_re_stk_type_int  /* integer */
};

/* "bdlx_res_states" enumeration - list 
 * of "special" reserved analyser states,
 * consisting of the initial ("start")
 * state */
enum bdlx_res_states {
    
    bdlx_start_state,   /* initial state */
	bdlx_states_num_res /* number of reserved
	                     * states */
};

/* "bdlx_trans_fn" typedef - proto-type of the
 * "transition function" called when a trans-
 * ition is made from one state to another.
 * when used in an NFA, usually This callback
 * should implement the change in the token
 * this transition represents.
 *
 * the first parameter contains the relevant
 * token, the second parameter the current
 * code-point while the third parameter is
 * the parameter to pass with the function
 * call, and thr fourth and fifth parameters
 * contain the memory allocator and error
 * logger, respectively (used for any necess-
 * ary memory allocation).  Returns zero on
 * an error, non-zero otherwise */
typedef int ( *bdlx_trans_fn)(struct bdlx_tok *,
                              int,
                              void *,
                              struct bd_allocs *,
                              struct bd_logger *);


/* "bdlx_dfa_entry" structure - informa-
 * tion hwikd fir each state and each next
 * input character in a DFA, (kniwn as its
 * "entry") ehich consists of a transition
 * function to call when this entry's state/
 * input symbol combination is encoutered,
 * that fuction opaque parameter and a state
 * number to which to transfer */
struct bdlx_dfa_entry {

    bdlx_trans_fn fn; /* transition callback */
    void *opaque;     /* callback's opaque para-
                       * meter */
    int next;         /* DFA state to which to
                       * transfer */
};

/* "bdlx_nfa_trans" structure - represents a
 * single transiton in a non-deterministic
 * finite automaton, which consists of its
 * type (i.e. empty etc), source and dest-
 * ination states, whether the destination
 * state is accepting, its transition func-
 * tion and the function's opaque parameter */
struct bdlx_nfa_trans {

    enum bdlx_sym_types type; /* whether normal
                               * or empty symbol */
     int mn, mx;              /* min. and max
                               * symbols */
    int from, to, acc;        /* source and
                               * destination
                               * states, trans-
                               * ition and
                               * whether is
                               * accepting */
     bdlx_trans_fn fn;	      /* transition
                               * function */
    void *opaque;             /* function's
                               * opaque
                               * parameter */

};

/* "bdlx_tok_type_fn" typedef- proto-type of the fun-
 * ction called to return the general type of the
 * token passed in the first parameter, using the
 * memory mmemory allocator and error logger given
 * in the second and third oarameters, respectively.
 * Returns zero on an error, non-zero otherwise */
typedef int (* bdlx_tok_type_fn)(struct bdlx_tok *,
                                 struct bd_allocs *,
                                 struct bd_logger *);

/* "bdlx_tok_init_fn" typedef - proto-type of the
 * function called to perform one-time initialis=
 * ation of the token passed in the first parameter,
 * using the memory allocator and error logger given
 * in the second snd third oarameters, respectively.
 * Returns zero on an error, non-zero otherwise */
typedef void (* bdlx_tok_init_fn)(struct bdlx_tok *,
                                  struct bd_allocs *,
                                  struct bd_logger *);

/* "bdlx_tok_reset_fn" typedef - prototype of the
 * function called to reset the fields of the token
 * passed in the first parameter before each use,
 * using the memory allocator and error logger given
 * in the second and third parameters, respectively,
 * Returns zero on an error, non-zero otherwise */
typedef void (*bdlx_tok_reset_fn)(struct bdlx_tok *,
                                  struct bd_allocs *,
                                  struct bd_logger *);

/* "bdlx_tok_name_fn" typedef  - proto-type of the
 * function called to etrurn the message table index
 * of the name of the token passed in the first par-
 * ameter, using the memory allocator and error logger
 * given in the third and fourth oarameters, respect-
 * ively.  The second oarameter giccates wgether the
 & name should indicate ht exzct content if the token,
 * rather than its general name.  Returns a negative
  * on an error */
typedef int (*bdlx_tok_name_fn)(struct bdlx_tok *,
                                int,
                                struct bd_allocs *,
                                struct bd_logger *);

/* "bdlx_sym_fn" typedef  - proto-type of the
 * function called by "process_dfa" to indicate,
 * by returning non-zero, that some proessing
 * should be performed when the current input
 * symbol matches the symbol given in the sec-
 * ond parameter and the NFA from which the
 * DFA being processed was created contains
 * the transition given in the first parameter,
 * using the memory allocator and error logger
 * given in the third and fourth parameters,
 * respectively.  Returns zero on an error, non-
 * zero otherwise */
typedef int (*bdlx_sym_fn)(struct bdlx_nfa_trans *,
                           int,
                           struct bd_allocs *,
                           struct bd_logger *);

/* "bdlx_val_fn" typedef  - proto-type of the
 * function called by "process_dfa" to second
 * parameter to the the value from the NFA
 * transition given in the first parameter
 * that should be then be assigned into a
 * DFA corresponding DFA en-
 * try, using the memory allocator and error
 * logger given in the second and third param-
 * eters, respectively.  Returns zero on an
 * error, non-zero otherwise */
typedef int (*bdlx_val_fn)(struct bdlx_nfa_trans *,
                           void **,
                           struct bd_allocs *,
                           struct bd_logger *);

/* "bdlx_cond_fn" typedef  - proto-type of the
 * function called by "process_dfa" to indicate,
 * by returning non-zero, the that the NFA tran-
 * sition given in the first parameter is suit-
 * able for further processing, using the memory
 * alloca-tor and error logger given in the
 * second and third parameters, respectively.
 * Returns zero on an error, non-zero otherwise */
typedef void *(*bdlx_cond_fn)(struct bdlx_nfa_trans *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdlx_entry_fn" typedef  - proto-type of the
 * function called by "process_dfa" to use the
 * value given in the third paramete to modify the
 * entry given in the first parameter, using the
 * transition of the NfA from ehuch the DFA being
 * processed ewas created, and using the memory
 * alloca-tor and error logger given in the third
 * and fourth parameters, respectively. Returns
 * NULL on an error.  Returns zero on an error,
 * non-zero otherwise */
typedef int (*bdlx_entry_fn)(struct bdlx_dfa_entry *,
                             struct bdlx_nfa_trans *,
                             void *,
                             struct bd_allocs *,
                             struct bd_logger *);

/* "bdlx_tok" structure - the operations of a
 * generic lexical token, containing one field
 * and three callbacks: the size of the token
 * (to calculate allocation requirements), to
 * return its type, to perform one0time initi-
 * alisation, to reset its fields and return
 * a string index into a descriptive name of
 * its content, respectively.  Actual tokens
 * will instantiate these callbacks and possi-
 * bly contain whatever additional fields the
 * language (for which this token is used) req-
 * uires */
struct bdlx_tok {

    int sz; /* size of token */
        /* return type of token */
    bdlx_tok_type_fn type;
        /* reset token fields */
    bdlx_tok_init_fn init;
        /* return name of token */
    bdlx_tok_reset_fn reset;
        /* return name of token */
    bdlx_tok_name_fn name;

    /* ... and, optionally, additional
     * language-specific fields */
};

/* "bdlx_re_tok" structure - the regular
 * expression parser's lexical token and
 * containing the generic callbacks, the
 * token's type, indications as to wheth-
 * er digits are processed numerically
 * and wgtyjrt definition names are capt-
 * ured, followed by a union either cont-
 * aining the currently scanned definit-
 * ion name, symbol code-point or number */
struct bdlx_re_tok {

    struct bdlx_tok tok;
                   /* generic
                    * call-backs */

	enum bdlx_re_toks type;
	               /* token type */

	int dfns,      /* whrther numer- */
	    dgts;      /* ical processing
	                * and definition
	                * capturing is on */


	union {

        char *defn;              /* definition
                                  * name */
        int cp,                  /* code-point of
                                  * symbol */
            num;                 /* number */
        struct bdbf_buffer *buf; /* buffer to
                                  * collect
                                  * input */
    } u;
};

/* "bdlx_range" structure - repres-
 * ents a upper smd ower limit
 * betwween which a value should
 * lie */
struct bdlx_range {

    int upper, lower; /* uppper and
					   * lower lonit */
}; 

/* "bdlx_lex" structure - contains a reg-
 * ular expression parser, the evaluat-
 * or of the current regular expression
 * and the rules and definitions consti-
 * tuting the lexical analyser */
struct bdlx_lex {

    struct bdpr_parser *re;   /* regular expression parser */
    struct bdlx_nfa *nfa;     /* the NFA of entire lex-
                               * ical analyser, as gener-
                               * ated by the addition of
                               * rules and definitions */
    struct bdlx_dfa *dfa;     /* the DFA generated from
                               * the NFA */
    struct bd_map_node *defs; /* the set of definitions
                               * in the analyser */
};

/* "bdlx_defn" structure - a dingle "def-
 * inition in a lexical nalyser, which
 * esrves to link between a name and an
 * NFA, in order to re-use commnon funct-
 * ionality using that name.   It cons-
 * ists of the name, the corresponding NFA
 * and the analyser in whichthe defin-
 * ition is located and the definition's
 * owning lexical analyser */
struct bdlx_defn {

    char *name;             /* definition
                             * name */

    struct bdlx_nfa *nfa;   /* corresp-
                             * onding
                             * NFA */ 
    
    struct bdlx_lex *owner; /* owning
                             * analyser */
};

/* "bdlx_nfa" structure - information
 * representing a non-deterministic finite
 * automata - NFA - which may contain empty
 * transitions to more than one state from
 * any given state.  An NFA consists of the
 * number of states and a msp of its const-
 * ituent transitions */
struct bdlx_nfa {

    int num; /* number of states in NFA */

    struct bd_map_node *trns;
             /* map of transitions in NFA */
};

/* "bdlx_dfa" structure - information
 * representing a "deterministic finite
 * automata", which is a sub-type of a "state
 * machine" in which there is a transition
 * to exactly one state from all states, in
 * which there are no "empty" transitions.
 * Such a state contains a pointer to an array
 * of DFA states */
struct bdlx_dfa {

    int num;                       /* number of
                                    * states in NFA */
    struct bdlx_dfa_state *states; /* pointer to
                                    * array od
                                    * next states
                                    * for each next
                                    * input chara-
                                    * cter  */
};

/* "bdlx_dfa_state" structure - a single satate
 * of a DFA, which xonstss of the an indication
 * as to whether the state is accpting and a poi-
 * nter to an array state symbols, one fir each
 * possible next input character for that
 * state */
struct bdlx_dfa_state {

    int acc;                    /* whether state
                                 * is accepting */
    struct bdlx_dfa_entry *ins; /* pointer to
                                 * array of
                                 * symbol states
                                 * for each next
                                 * input chara-
                                 * cter  */
};

/*** function proto-types ***/

/* "bdlx_init" - using the allocator and error
 * logger given in the third and fourth parame-
 * ters, respectively, sets the second para-
 * meter to a lexical analyser initially cont-
 * aining no rules or definitions.  Returns
 * zero on error, non-zero otherwise */
int bdlx_init(struct bdlx_tok *,
		      struct bdlx_lex **,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdlx_tok_init" - using the memeory allocator
 * and error logger given in the second and third 
 * parameters, respectively, and sets the third
 * parameter to a lexical token of the size giv-
 * en in the fourth parameter and having the in-
 * itialise, type, reset and name callbcks given
 * in the fifth to eighth parameters, respectively.
 * Returns zero on error, non-zero otherwise */
int bdlx_tok_init(struct bd_allocs *,
                  struct bd_logger *,
                  struct bdlx_tok *,
                  int,
                  bdlx_tok_init_fn,
	              bdlx_tok_type_fn,
                  bdlx_tok_reset_fn,
                  bdlx_tok_name_fn);

/* "bdlx_add_rule" - using the memory allocator
 * and error logger given in the second and third
 * parameters, respectively, add a "rule", which is
 * an association between the "pattern" of input
 * characters given in the fourth parameter and
 * a renstiion between the states giveb in the fifth
 * and suxth parameters, respectively, along with
 * the execution of the "recognition callback"
 * function given in the seventh parameter.  The
 * tenth parameter indicates the token type to
 * accept (otherwise NULL if not accepting).  The
 * eighth parameter specifies the opaque parameter
 * to pass with the call to the rule's callback.
 * Returns zero on error, non-zero otherwise */
int bdlx_add_rule(struct bdlx_lex *,  
                  struct bd_allocs *, /* memory allocator */
                  struct bd_logger *, /* error logger */
                  char *,             /* regular expression */ 
                  int,                /* preceeding rule */
                  int,                /* resulting rule */
                  bdlx_trans_fn,      /* transition callback */
                  void *,             /* callback's opaque
                                       * parameter */
                  int,                /* whether rule is
                                       * accepting */
                  int);               /* resulting token type
                                       * of accepting token */

/* "bdlx_add_def" - using the memory allocator
 * and error logger given in the dsecond and third
 * parameters, respectively, add a "definition",
 * which is an association between the regular
 * expression of the input characters and name
 * the fouth anndfifth parameters, respectively,
 * to the analyser given in the first parameter.
 * Returns zero on error, non-zero otherwise */
int bdlx_add_def(struct bdlx_lex *,
                 struct bd_allocs *,
                 struct bd_logger *,
                 char *,
                 char *);


/* "bdlx_start" - prepares, using the allocator
 * and error logger given in the second and third
 * parameters, respectively, the lexical analyser
 * given in the first parameter to start resding
 * input using its constittuent rules and defin-
 * itions.  Returns zero on an error, non-zero
 & * otherwise */
int bdlx_start(struct bdlx_lex *,
               struct bd_allocs *,
               struct bd_logger *); 

/* "bdlx_next - sets the second parameter to
 * the next token found by performing lexical
 * analysis using the analyser in the first
 * parameter on the input stream the start of
 * which is pointed to by the third parameter,
 * in the process updating the third parameter
 * so as to point just after the location at
 * which the scanned token was found, using
 * the memory allocator and error logger in
 * the fifth and sixth parameters, respectiv-
 * ely.  Returns zero on error, non-zero oth-
 * erwise */
int bdlx_next(struct bdlx_lex *,
              struct bdlx_tok *,
              int **,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdlx_new_state" - returns the next unalloc-
 * ated state of the analyser given in the
 & first parameter.  Returns a negative value
 * on an error */
int bdlx_new_state(struct bdlx_lex *);

/* "bdlx_cleanup" - using the allocator and error
 * logger given in the second and third parameters,
 * respectuvely, reclaims the resources used by
 * the lexical analyser given in the first para-
 * ameter, usually just before it is to be
 * deleted */
void bdlx_cleanup(struct bdlx_lex *,
                  struct bd_allocs *,
                  struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
