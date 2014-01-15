/*****************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "js.h" - data types and function proto-
* types related to an ECMAScript (more co-
* mmonly known as JavaScript) compiler.
*
* These functions also handle accesses to
* and mutations of the properties, and
* calls on the methods of, JavaScript
* "objects", the primary means by which
* a the environment is computation is
* provided.
* 
* Copyright (C) 2006-2012, Russell Potter,
* All rights reserved
*
******************************************************************/

#ifndef __BD_JS_H
#define __BD_JS_H

#ifdef __cplusplus
extern "C" {
#endif

struct bdjs_re_assrtn_tstr;
struct bdjs_env_rec;
struct bdjs_re_state;
struct bdjs_re_mtchr;
struct bdjs_re_contn;

#include <bikedemo/js.h>
#include <bikedemo/lex.h>
#include <bikedemo/util.h>
#include <bikedemo/ic.h>
#include <bikedemo/lang.h>
#include <bikedemo/allocs.h>
#include <bikedemo/buffer.h>
#include <bikedemo/parser.h>
#include <bikedemo/log.h>

struct bdlg_lang;

/* "bdjs_types" enumeration - the
 * possible types of a JavaScript
 * object: namely, strings, numb-
 * ers, booleans, null, undefined,
 * non-primitive objects, refere-
 * nces, completions and environm-
 * ent records */
enum bdjs_types {

    bdjs_type_str,    /* string */
    bdjs_type_undf,   /* undefined */
    bdjs_type_nll,    /* null */
    bdjs_type_bln,    /* boolean */
    bdjs_type_num,    /* number */
    bdjs_type_obj,    /* non-primit-
                       * ive */
    bdjs_type_ref,    /* reference */
    bdjs_type_cmpltn, /* completion */
    bdjs_type_env_rec /* environment
                   * record */
};

/* "bdjs_ptch_types" enumeration - the
 * tyoes of resolve-able "patches" that
 * may br generated for a JavaScript
 * intermediate code stream: namely:
 * that that for a return" statement,
 * "throw" statement, thrw in a "catch"
 * clause, "break" and "continue" stat-
 * ements, respectively */
enum bdjs_ptch_types {

    bdjs_ptch_type_ret,       /* "return" */
    bdjs_ptch_type_thrw,      /* "throw" */
    bdjs_ptch_type_ctch_thrw, /* "throw" in
                               * "catch" clause */
    bdjs_ptch_type_brk,       /* "break" */
    bdjs_ptch_type_cntn       /* "continue" */
};

/* "bdjs_ops" enumeration - the possible
 * types operation types that may be
 * pushed onto the JavaScript compiler's
     * operation stack */
    enum bdjs_ops {

    bdjs_op_mod,      /* modulus ("%") */
    bdjs_op_add,      /* add ("+") */
    bdjs_op_sub,      /* subtract ("-") */
    bdjs_op_mul,      /* multiply ("*") */
    bdjs_op_div,      /* divide ("/") */
    bdjs_op_mbr,      /* member access (".")*/
    bdjs_op_new,      /* "new" <val>*/
    bdjs_op_del,      /* "delete" <prop> */
    bdjs_op_typof,    /* "typeof" <val> */
    bdjs_op_void,     /* ""void" <val> */
    bdjs_op_land,     /* logical and ("&&")  */
    bdjs_op_lor,      /* logical or ("||") */
    bdjs_op_band,     /* bit and ("&") */
    bdjs_op_is,       /* "is" */
    bdjs_op_isnt,     /* "isnt" */
    bdjs_op_bor,      /* bit or ("|") */
    bdjs_op_not,      /* logical not ("!") */
    bdjs_op_cmpl,     /* bit omplement ("~") */
    bdjs_op_lsh,      /* left shift ("<<") */
    bdjs_op_rsh,      /* right shift (">>") */
    bdjs_op_ursh,     /* unsigned right
                       * shift (">>>") */
    bdjs_op_arry,     /* array access */
    bdjs_op_eq,       /* is equal to ("==") */
    bdjs_op_seq,      /* is strictly
                       * equal to ("===") */
    bdjs_op_ne,       /* is not equal to
                       * ("!=") */
    bdjs_op_sne,      /* is not strictly
                       * equal to ("!==") */
    bdjs_op_ge,       /* is greater than or
                       * equal to (">=") */
    bdjs_op_gt,       /* is greater than
                       * (">") */
    bdjs_op_le,       /* is less than or
                       * equal to ("<=") */
    bdjs_op_lt,       /* is less than
                       * ("<") */
    bdjs_op_pos,      /* unary positive */
    bdjs_op_neg,      /* negate ("-<num>") */
    bdjs_op_xor,      /* exclusive-or
                       * ("^")*/
    bdjs_op_pstinc,   /* post-increment
                       * ("<val>++") */
    bdjs_op_pstdec,   /* post-decrement
                       * ("<val>--") */
    bdjs_op_preinc,   /* pre-increment
                       * ("++<val>") */
    bdjs_op_predec,   /* pre-decrement
                       * ("--<val>") */
    bdjs_op_lpr,      /* group in ("(")*/
    bdjs_op_rpr,      /* group out (")")*/
    bdjs_op_trnry,    /* ternary ("?:") */
    bdjs_op_asn,      /* assign ("=") */
    bdjs_op_add_asn,  /* addition assign
                       * ("+=") */
    bdjs_op_sub_asn,  /* subtraction assign
                       * ("-=") */
    bdjs_op_mul_asn,  /* multiplication
                       * assign ("*=") */
    bdjs_op_div_asn,  /* division */
    bdjs_op_mod_asn,  /* modulus assign ("%=") */
    bdjs_op_band_asn, /* bit and assign ("|=") */
    bdjs_op_bor_asn,  /* bit or assign ("&=") */
    bdjs_op_xor_asn,  /* exclusive-or assign ("^=") */
    bdjs_op_lsh_asn,  /* left shift assign ("<<=") */
    bdjs_op_rsh_asn,  /* right shift assign (">>=") */
    bdjs_op_ursh_asn, /* unsigned right shift
                       * assign (">>>=") */
    bdjs_op_instof,   /* instance of
                       * ("instanceof") */
    bdjs_op_in,       /* in ("in") */
    bdjs_op_cll,      /* function call
                       * ("nme(<prms>)") */
    bdjs_op_cma,      /* comma (",") */
    bdjs_ops_num      /* number of op-
                       * erators */
 };

/* "bdjs_flnmd_prp_flds" enumeration - the
 * fields in the descriptor of a named pro-
 * perty */
enum bdjs_nmd_prp_flds {

    bdjs_nmd_prp_val,     /* "[[Value]]" */
    bdjs_nmd_prp_get,     /* "[[Get]]" */
    bdjs_nmd_prp_set,     /* "[[Set]]" */
    bdjs_nmd_prp_wrtble,  /* "[[Writeable]]" */
    bdjs_nmd_prp_enrmble, /* "[[Enumerable]]" */
    bdjs_nmd_prp_cnfgrble /* "[[Configurable]]" */
};

/* "bdjs_precs" enumeration - the possible
 * erttyopszes precedence used by
 * the JavaScript evaluator */
enum bdjs_precs {

    bdjs_prc_cma,   /* "comma" precedence */
    bdjs_prc_asn,   /* "assignment oper-
    	             * ator" precedence */
    bdjs_prc_trnry, /* "conditional (tern-
                     * ary) operator
                     * precedence */
    bdjs_prc_lor,   /* logical "or"
                     * precedence */
    bdjs_prc_land,  /* logical "and"
                     * precedence */
    bdjs_prc_bor,   /* bit-wise "or"
                     * precedence */
    bdjs_prc_xor,   /* bit-wise "excl-
                     * usive-or" precedence */
    bdjs_prc_band,  /* bit-wise "and"
                     * group */
    bdjs_prc_eq,    /* "equality" precedence */
    bdjs_prc_rel,   /* "relational,
                     * instance of" and
                     * in" precedence */
    bdjs_prc_bshft, /* "bitwise shift"
                     * precedence */
    bdjs_prc_add,   /* "additive operator"
                     * precedence */
    bdjs_prc_mul,   /* "multiplicative
                     * operator" precedence */
    bdjs_prc_unry,  /* "unary operator"
                     * precedence */
    bdjs_prc_acc    /* "access" precedence */
};

/* "bdjs_jsn_toks" enumeration - the lexical
 * tokens returned by the JavaScript Oject
 * Notation (JSON) lexical analyser */
enum bdjs_jsn_toks {

    bdjs_jsn_tok_ws =       /* white-space */
         bdgm_sym_nres + 1,
    bdjs_jsn_tok_lsq,       /* left square
                             * bracket ("[")*/
    bdjs_jsn_tok_rsq,       /* right square
                             * bracket ("]") */
    bdjs_jsn_tok_str,       /* character string
                             * ("\"\abc\"" */
    bdjs_jsn_tok_nmrc,      /* number */
    bdjs_jsn_tok_lbr,       /* left brace ("{") */
    bdjs_jsn_tok_rbr,       /* left brace ("}") */
    bdjs_jsn_tok_cma,       /* comma (",") */
    bdjs_jsn_tok_cln,       /* colon (":") */
    bdjs_jsn_tok_kwd_true,  /* keyword "true" */
    bdjs_jsn_tok_kwd_false, /* keyword "false" */
    bdjs_jsn_tok_kwd_null,  /* keyword  "null" */
    bdjs_jsn_tok_num,       /* number of JSON
                             * tokens */
};

/* "bdjs_jsn_nts" enumeration - the JSON
 * grammar non-terminals */
enum bdjs_jsn_nts {

    bdjs_jsn_nt_val =
         bdjs_jsn_tok_num + 1,
    bdjs_jsn_nt_null_lit,
    bdjs_jsn_nt_bool_lit,
    bdjs_jsn_nt_obj,
    bdjs_jsn_nt_arry,
    bdjs_jsn_nt_str,
    bdjs_jsn_nt_num,
    bdjs_jsn_nt_mbr,
    bdjs_jsn_nt_mbr_list,
    bdjs_jsn_nt_elem,
    bdjs_jsn_nt_elem_list,
    bdjs_jsn_num_syms
};

/* "bdjs_toks" enumeration - the possible types
 * that may be returned by the JavaScript lexical
 * analyser */
enum bdjs_toks {

    bdjs_tok_id =             /* identifier */
           bdgm_sym_nres + 1,
    bdjs_tok_str,             /* string literal */
    bdjs_tok_num,             /* mumeric literal  */
    bdjs_tok_regexp,          /* regular expre-
                               * sion literal */
    bdjs_tok_not_fn_clss,     /* not "function",
                               * "{" or "class" */
    bdjs_tok_lmtr,            /* line terminator */
                              /* ("/xyz/<flags>")*/
    bdjs_tok_not,             /* logical not ("!")*/
    bdjs_tok_cmpl,            /* bit-wise comple-
                               * ment ("~")*/
    bdjs_tok_add,             /* addition ("+")*/
    bdjs_tok_sub,             /* subtraction ("-")*/
    bdjs_tok_mul,             /* multiplication ("*") */
    bdjs_tok_div,             /* division */
    bdjs_tok_lsh,             /* left shift ("<<") */
    bdjs_tok_rsh,             /* right shift (">>")*/
    bdjs_tok_ursh,            /* undigned right
                               * shift (">>>") */
    bdjs_tok_mod,             /* modulus ("%")  */
    bdjs_tok_lpr,             /* left paren-
                               * thesis ("(") */
    bdjs_tok_rpr,             /* right paren-
                               * thesis (")") */
    bdjs_tok_lbr,             /* left brace ("{") */
    bdjs_tok_rbr,             /* right brace ("}") */
    bdjs_tok_rst,              /* remainder of arg-
                               * uments ("...") */
    bdjs_tok_arrw,            /* arrow pointer
                               * ("=>") */
    bdjs_tok_band,            /* bit and ("&") */
    bdjs_tok_bor,             /* bit or ("|") */
    bdjs_tok_land,            /* logical and ("&&") */
    bdjs_tok_cma,             /* comma (",") */
    bdjs_tok_lor,             /* logical or ("||") */
    bdjs_tok_eq,              /* is equal ("==")*/
    bdjs_tok_ne,              /* is not equal ("!=") */
    bdjs_tok_seq,             /* is strictly 
                               * equal ("===") */
    bdjs_tok_sne,             /* is trictly not
                               * equal ("!==") */
    bdjs_tok_xor,             /* exclusive-or ("^") */
    bdjs_tok_qstn,            /* question mark ("?") */
    bdjs_tok_lt,              /* is less than ("<") */
    bdjs_tok_gt,              /* is greater than (">") */
    bdjs_tok_le,              /* is less than or equal
                               * to (<=") */
    bdjs_tok_ge,              /* is greater than or
                               * equal to ">=") */
    bdjs_tok_asn,             /* assign ("=")*/
    bdjs_tok_add_asn,         /* addition assign ("+=") */
    bdjs_tok_sub_asn,         /* subtraction assign ("-=") */
    bdjs_tok_mul_asn,         /* multiplication
                               * assign ("*=")*/
    bdjs_tok_div_asn,         /* division assign ("/=") */
    bdjs_tok_lsh_asn,         /* left shift assign ("<<=") */
    bdjs_tok_rsh_asn,         /* right shift assign
                               * (">>=") */
    bdjs_tok_ursh_asn,        /* unsigned right shift
                               * (">>>=")
                               * assign ("=") */
    bdjs_tok_mod_asn,         /* modulus assign ("%=") */
    bdjs_tok_xor_asn,         /* exclusive-or assign ("^=") */
    bdjs_tok_band_asn,        /* bit and assign ("&=") */
    bdjs_tok_bor_asn,         /* bit or assign (""|=)*/
    bdjs_tok_lsq,             /* left square bracket
                               * ("[")*/
    bdjs_tok_rsq,             /*  right square bracket
                               * ("]")*/
    bdjs_tok_qsi,             /* aqyuasi-literal
                               * ("id`<xxx>`") */
    bdjs_tok_inc,             /* increment ("++") */
    bdjs_tok_dec,             /* decrement ("--") */
    bdjs_tok_mbr,             /* member access (".") */
    bdjs_tok_smc,             /* semi-colon (";")*/
    bdjs_tok_cln,             /* colon (":")  */
    bdjs_tok_kwd_break,       /* keyword break" */
    bdjs_tok_kwd_case,        /* keyword case" */
    bdjs_tok_kwd_catch,       /* keyword catch" */
    bdjs_tok_kwd_class,       /* keyword class" */
    bdjs_tok_kwd_continue,    /* keyword "continue" */
    bdjs_tok_kwd_const,       /* keyword "const" */
    bdjs_tok_kwd_debugger,    /* keyword "debugger" */
    bdjs_tok_kwd_default,     /* keyword "default" */
    bdjs_tok_kwd_delete,      /* keyword "delete" */
    bdjs_tok_kwd_do,          /* keyword "do" */
    bdjs_tok_kwd_else,        /* keyword "else" */
    bdjs_tok_kwd_export,      /* keyword "export" */
    bdjs_tok_kwd_finally,     /* keyword "finally" */
    bdjs_tok_kwd_for,         /* keyword "for" */
    bdjs_tok_kwd_function,    /* keyword "function" */
    bdjs_tok_kwd_get,         /* keyword "get" */
    bdjs_tok_kwd_if,          /* keyword "if" */
    bdjs_tok_kwd_in,          /* keyword "in" */
    bdjs_tok_kwd_import,      /* keyword "import" */
    bdjs_tok_kwd_instanceof,  /* keyword "instanceof" */
    bdjs_tok_kwd_is,          /* keyword "is" */
    bdjs_tok_kwd_isnt,        /* keyword "insnt" */
    bdjs_tok_kwd_let,         /* keyword "let" */
    bdjs_tok_kwd_new,         /* keyword "new" */
    bdjs_tok_kwd_of,          /* keyword "of" */
    bdjs_tok_kwd_return,      /* keyword "return" */
    bdjs_tok_kwd_set,         /* keyword "set" */
    bdjs_tok_kwd_super,       /* keyword "super" */
    bdjs_tok_kwd_switch,      /* keyword "switch" */
    bdjs_tok_kwd_this,        /* keyword "this" */
    bdjs_tok_kwd_throw,       /* keyword "throw" */
    bdjs_tok_kwd_try,         /* keyword "try" */
    bdjs_tok_kwd_typeof,      /* keyword "typeof" */
    bdjs_tok_kwd_var,         /* keyword "var" */
    bdjs_tok_kwd_void,        /* keyword "void" */
    bdjs_tok_kwd_while,       /* keyword "while" */
    bdjs_tok_kwd_with,        /* keyword "with" */
    bdjs_num_toks             /* number of tokens */
};

/* "bdjs_prp_types" enumeration - the
 * types of properties of an object:
 * namely, either "named" or "internal" */
enum bdjs_prp_types {

    bdjs_prp_type_nmd,    /* named property */
    bdjs_prp_type_intrnl  /* internal property */
};

/* "bdjs_intrnl_prp_type" enumeration - the
 * types of internal properties of an object.
 *
 * Usually these names are surrounded by
 *  "[[" and "]]" to mark them as internal */
enum bdjs_intrnl_prp_types {

    bdjs_intrnl_prp_ptype,       /* "[[Prototype]]"         property */
    bdjs_intrnl_prp_extnsble,    /* "[[Extensible]]"        property */
    bdjs_intrnl_prp_get,         /* "[[Get]]"               property */
    bdjs_intrnl_prp_get_own_prp, /* "[[GetOwnProperty]]"    property */
    bdjs_intrnl_prp_get_prp,     /* "[[GetProperty]]"       property */
    bdjs_intrnl_prp_put,         /* "[[Put]]"               property */
    bdjs_intrnl_prp_can_put,     /* "[[CanPut]]"            property */
    bdjs_intrnl_prp_has_prp,     /* "[[HasProperty]]"       property */
    bdjs_intrnl_prp_del,         /* "[[Delete]]"            property */
    bdjs_intrnl_prp_dflt_val,    /* "[[DefaultNalue]]"      property */
    bdjs_intrnl_prp_dfn_own_prp, /* "[[DefineOwnProperty]]" property */
    bdjs_intrnl_prp_prmtve_val,  /* "[[PrimitiveValue]]"    property */
    bdjs_intrnl_prp_cnstrct,     /* "[[Conrtruct]]"         property */
    bdjs_intrnl_prp_cll,         /* "[[Call]]"              property */
    bdjs_intrnl_prp_scpe,        /* "[[Scope]]"             property */
    bdjs_intrnl_prp_fml_prms,    /* "[[FormalParameters]]"  property */
    bdjs_intrnl_prp_cde,         /* "[[Code]]"              property */
    bdjs_intrnl_prp_trgt_fn,     /* "[[TargetFunction]]"    property */
    bdjs_intrnl_prp_bnd_ths,     /* "[[BoundThis]]"         property */
    bdjs_intrnl_prp_bnd_args,    /* "[[BoundArguments]]"    property */
    bdjs_intrnl_prp_mtch,        /* "[[Match]]"             property */
    bdjs_intrnl_prp_prm_mp,      /* "[[ParameterMap]]"      property */
    bdjs_intrnl_prp_enmrte,      /* "[[Enumerate]]"         property */
    bdjs_intrnl_prp_itrte,       /* "[[Iterate]]"           property */
    bdjs_intrnl_prp_strct,       /* "[[Strict]]"            property */
    bdjs_intrnl_prp_ntve_brnd    /* "[[NativeBrand]]"       property */
};

/* "bdkjs_ntve_brnds" tenumeration - the possible
 * "brands" of native objects, ejich includes var-
 * ious primitive type wrappers and native run-time
 functions */

enum bdjs_ntve_brnds {

    bdjs_ntve_brnd_fn,        /* native Function */
    bdjs_ntve_brnd_arry,      /* native Array */
    bdjs_ntve_brnd_mth,       /* native Math */
    bdjs_ntve_brnd_re,        /* native RegExp */
    bdjs_ntve_brnd_err,       /* native Error */
    bdjs_ntve_brnd_args,      /* native Arguments */
    bdjs_ntve_brnd_jsn,       /* native JSON */
    bdjs_ntve_brnd_str_wrppr, /* String wrapper */
    bdjs_ntve_brnd_bln_wrppr, /* Boolean wrapper */
    bdjs_ntve_brnd_num_wrppr, /* Number wrapper */
    bdjs_ntve_brnd_prvte_nme  /* private name */
};

/* "bdjs_re_toks" enumeration - the possible
 * tlexical tokens returned by JavaScript
 * regular expression lexical analyser */
enum bdjs_re_toks {

    bdjs_re_tok_crt =          /* caret ("^") */
            bdgm_sym_nres + 1,
    bdjs_re_tok_ast,           /* asterisk ("*") */
    bdjs_re_tok_qstn,          /* question mark ("?") */
    bdjs_re_tok_dot,           /* period (".") */
    bdjs_re_tok_pls,           /* plus sign ("+") */
    bdjs_re_tok_dllr,          /* dollar sign ("$") */
    bdjs_re_tok_bkslsh,        /* back-slash ("\") */
    bdjs_re_tok_cma,           /* comma (",") */
    bdjs_re_tok_dsh,           /* dash ("-") */
    bdjs_re_tok_bar,           /* vertical bar ("|") */
    bdjs_re_tok_wd_strt,       /* word start ("\B") */
    bdjs_re_tok_wd_end,        /* eord end ("\b") */
    bdjs_re_tok_pct,           /* percent-sign ("%") */
    bdjs_re_tok_pos_assrtn,    /* positive assertion
                                * ("?=") */
    bdjs_re_tok_neg_assrtn,    /* negative assertion
                                * ("?!") */
    bdjs_re_tok_qc,            /* question-mark-colon
                                * sequence ("?:") */
    bdjs_re_tok_lpr,           /* left parenthesis */
                               /* ("(") */
    bdjs_re_tok_rpr,           /* right parenthesis */
                               /* (")") */
    bdjs_re_tok_lbr,           /* left brace ("{") */
    bdjs_re_tok_rbr,           /* right brace ("}") */
    bdjs_re_tok_lsq,           /* left square bracket
                                * ("[") */
    bdjs_re_tok_rsq,           /* right square bracket
                                * (*]") */
    bdjs_re_tok_pat_chr,       /* pattern char
                                * ("[^*+?()[]{}\|]") */
    bdjs_re_tok_id_esc,        /* ID escaape
                                * ("[^*+?{^id_start}]") */
    bdjs_re_tok_ctl_esc,       /* control escape
                                * ("[fnrtv]") */
    bdjs_re_tok_dec_esc,       /* decimal escape
                                * ("{id}^0-9]") */
    bdjs_re_tok_chr_clss_esc,  /* char. class escape
                                * ("[^\]") */
    bdjs_re_tok_clss_atm_ndsh, /* class atom
                                * no dash ("[^\]-]") */
    bdjs_re_tok_ctl_ltr,       /* control letter
                                * ("c[a-zA-Z]") */
    bdjs_re_tok_digs,          /* decimal digit
                                * sequence ("[0-9]+") */
    bdjs_re_tok_b,             /* letter "b" */
    bdjs_re_tok_c,             /* letter "c" */
    bdjs_re_toks_num           /* number of tokens */
};

/* "bdjs_re_nts" enumeration - the possible
 * types of regular expression non-terminals */
enum bdjs_re_nts {

    bdjs_re_nt_pttrn = bdjs_re_toks_num,
    bdjs_re_nt_pttrn_chr,
    bdjs_re_nt_dsjnctn,
    bdjs_re_nt_alt,
    bdjs_re_nt_assrtn,
    bdjs_re_nt_trm,
    bdjs_re_nt_atm,
    bdjs_re_nt_clss_atm,
    bdjs_re_nt_clss_atm_ndsh,
    bdjs_re_nt_atm_esc,
    bdjs_re_nt_clss_esc,
    bdjs_re_nt_chr_clss_esc,
    bdjs_re_nt_clss_rnges,
    bdjs_re_nt_nmpty_clss_rnges,
    bdjs_re_nt_nmpty_clss_rnges_ndsh,
    bdjs_re_nt_qtfr,
    bdjs_re_nt_qtfr_prfx,
    bdjs_re_nt_dec_dgts,
    bdjs_re_nt_chr_clss,
    bdjs_re_nt_ctl_esc,
    bdjs_re_nt_ctl_ltr,
    bdjs_re_nt_hex_esc_seq,
    bdjs_re_nt_id_esc,
    bdjs_re_nt_chr_esc,
    bdjs_re_nt_uc_esc_seq

};

/* "bdjs_nts" enumeration - the possible non-
 * terminals of the JavaScript grammar */
enum bdjs_nts {

    bdjs_nt_src_elem = bdjs_num_toks
                       + 1,
    bdjs_nt_src_elems,
    bdjs_nt_eplge,
    bdjs_nt_expr,
    bdjs_nt_expr_noin,
    bdjs_nt_arry_cmprhnsn,
    bdjs_nt_gnrtr_cmprhnsn,
    bdjs_nt_gnrtr_expr,
    bdjs_nt_asgn_pttrn,
    bdjs_nt_arry_asgn_pttrn,
    bdjs_nt_obj_asgn_pttrn,
    bdjs_nt_asgn_rst_elem,
    bdjs_nt_asgn_elem,
    bdjs_nt_asgn_elem_lst,
    bdjs_nt_asgn_prp,
    bdjs_nt_asgn_prp_lst,
    bdjs_nt_cmprhnsn_fr,
    bdjs_nt_cmprhnsn_fr_lst,
    bdjs_nt_fr_bndng,
    bdjs_nt_let_or_cnst,
    bdjs_nt_fn_expr,
    bdjs_nt_clss_expr,
    bdjs_nt_bndng_id,
    bdjs_nt_bndng_expr,
    bdjs_nt_bndng_lst,
    bdjs_nt_bndng_pttrn,
    bdjs_nt_bndng_prp,
    bdjs_nt_sngle_nme_bndng,
    bdjs_nt_obj_bndng_pttrn,
    bdjs_nt_arry_bndng_pttrn,
    bdjs_nt_bndng_rst_elem,
    bdjs_nt_bndng_elem,
    bdjs_nt_bndng_elem_lst,
    bdjs_nt_sprd,
    bdjs_nt_prp_nme,
    bdjs_nt_prp_df,
    bdjs_nt_prp_df_list,
    bdjs_nt_mthd_df,
    bdjs_nt_sprd_elem,
    bdjs_nt_asgn_expr,
    bdjs_nt_prp_asgn,
    bdjs_nt_prp_dfn,
    bdjs_nt_prp_dfn_list,
    bdjs_nt_lit,
    bdjs_nt_id_nme,
    bdjs_nt_arry_lit,
    bdjs_nt_obj_lit,
    bdjs_nt_elsn,
    bdjs_nt_elem_list,
    bdjs_nt_prp_set_prm_list,
    bdjs_nt_frml_prm_list,
    bdjs_nt_fn_bdy,
    bdjs_nt_fn_dcl,
    bdjs_nt_elem_sprd,
    bdjs_nt_clss_dcl,
    bdjs_nt_lxcl_dcl,
    bdjs_nt_dcl,
    bdjs_nt_asn_expr,
    bdjs_nt_asn_expr_noin,
    bdjs_nt_args,
    bdjs_nt_arg_list,
    bdjs_nt_blk,
    bdjs_nt_stmt,
    bdjs_nt_stmt_lst_itm,
    bdjs_nt_blk_stmt,
    bdjs_nt_stmt_list,
    bdjs_nt_var_stmt,
    bdjs_nt_empty_stmt,
    bdjs_nt_expr_stmt,
    bdjs_nt_if_stmt,
    bdjs_nt_it_stmt,
    bdjs_nt_it_start,
    bdjs_nt_it_end,
    bdjs_nt_cont_stmt,
    bdjs_nt_brk_stmt,
    bdjs_nt_brkble_stmt,
    bdjs_nt_ret_stmt,
    bdjs_nt_wth_stmt,
    bdjs_nt_lbld_stmt,
    bdjs_nt_swtch_stmt,
    bdjs_nt_thrw_stmt,
    bdjs_nt_try_stmt,
    bdjs_nt_dbggr_stmt,
    bdjs_nt_for_in,
    bdjs_nt_for_out,
    bdjs_nt_var_dcl,
    bdjs_nt_var_dcl_list,
    bdjs_nt_var_dcl_noin,
    bdjs_nt_var_dcl_list_noin,
    bdjs_nt_cse_blk,
    bdjs_nt_cse_clse,
    bdjs_nt_cse_clses,
    bdjs_nt_dflt_clses,
    bdjs_nt_initlsr,
    bdjs_nt_initlsr_noin,
    bdjs_nt_ctch,
    bdjs_nt_fnlly,
    bdjs_num_syms
};

/* "bdjs_cmpltn_types" enumeration   possible
 * ways in which a JavaScript execution may
 * complete, namely: "normally", by thown
 * exception, function return or by execyution
 * transfer via "break" or "continue" statem-
 * ents   -*/
 enum bdjs_cmpltn_types {
 
     bdjs_cmpltn_nrml, /* normal */
     bdjs_cmpltn_thrw, /* thrown exception */
     bdjs_cmpltn_ret,  /* function return  */
     bdjs_cmpltn_brk,  /* "break" statement */
     bdjs_cmpltn_cont  /* "continue" statement */
 };

/* "bdjs_sv_toks" enumeration = possible
 * types of tokens returned by the lexical
 * analyser of the "string value" (SV) pa-
 * rseer, ised to rio get the numeric val-
 * ue of the string reprepresentation of
 * a number */
enum bdjs_sv_toks {

    bdjs_sv_tok_,
};

/* "bdjs_sv_nts" enumeration = the non-ter-
 * minals of the "" grammar */
enum bdjs_sv_nts {

    bdjs_sv_nt_,
};

/* "bdjs_cv_toks" enumeration = possible
 * types of tokens returned by the lexical
 * analyser of the "code unit value" (CV)
 * parseer, ised to rio get the value of
 * the code value of rthe string repres-
 * entation */
enum bdjs_cv_toks {

    bdjs_cv_tok = bdgm_sym_nres,
    bdjs_cv_toks_num,
};

/* "bdjs_mv_nts" enumeration = the non-ter-
 * minals of the "mathmatical value" grammar */
enum bdjs_mv_nts {

    bdjs_mv_nt_lit,
    bdjs_mv_nt_dec_lit,
    bdjs_mv_nt_us_dec_lit,
    bdjs_mv_nt_digs,
    bdjs_mv_nt_exp_prt,
    bdjs_mv_num_syms
};

/* "_bdjs_mv_toks" enumeration - the poaassible token
 * types returned ny the mathematical value (MV) par-
 * ser's lexical analyser */

enum bdjs_mv_toks {

    bdjs_mv_tok_pt =       /* decimal point (".") */
        bdgm_sym_nres + 1,
    bdjs_mv_tok_dig,       /* decimal digit ("[0-9]") */
    bdjs_mv_tok_exp,       /* exponent indicator
                            * ("[eE]") */
    bdjs_mv_tok_ws,        /* white-space */
    bdjs_mv_tok_kwd_inf,   /* ketyword "Infinity" */
    bdjs_mv_tok_pz,        /* positive zero ("+0")*/
    bdjs_mv_tok_nz,        /* negative zero ("-0") */
    bdjs_mv_num_toks       /* number of tokens */
};

/* "bdjs_qv_toks" enumeration - possible
 * types of tokens returned by the lexical
 * analyser of the "code quasi-string
 * value" (QV) parseer, ised to rio get
 * the of quas=value (QV) of a quasi-lit-
 * eral */
enum bdjs_qv_toks {

    bdjs_qv_tok_sbst_strt =  /* substitution
                              * start ("${")*/
        bdgm_sym_nres + 1,
    bdjs_qv_tok_sbst_end,    /* substitution-
                              * end ("}")*/
    bdjs_qv_tok_chr,         /* non-substi-
                              * tution char 
                              * ("[^$\`]")*/
    bdjs_qv_num_toks,        /* number of
                              * tokens */
};

/* "bdjs_qv_nts" enumeration = the non-ter-
 * minals of the "quasi-value" grammar */
enum bdjs_qv_nts {

    bdjs_qv_nt_fll =
        bdjs_qv_num_toks + 1,
    bdjs_qv_nt_chrs,
    bdjs_qv_nt_hd,
    bdjs_qv_nt_mddle,
    bdjs_qv_nt_tl,
    bdjs_qv_num_syms

};

/* "bdjs_numrc_types" enumeration - possible
 * numeric types: either NaN, positive zero
 * negative zero, positive infinity, negative
 * infinity or any other (normal) number */
enum bdjs_nmrc_types {

    bdjs_nmrc_nan,    /* not-a-number */
    bdjs_nmrc_ps_inf, /* positive infinity */
    bdjs_nmrc_ng_inf, /* negative infinity */
    bdjs_nmrc_ps_zro, /* positive zero */
    bdjs_nmrc_ng_zro, /* negative zero */
    bdjs_nmrc_num     /* any other (normal)
                        * number */
};

/* "bdjs_re_flgs" enumeration - possible
 * regular expression flags: either "global",
 * "milti-line" or "ignore case" */
enum bdjs_re_flgs {

    bdjs_re_flg_glbl,  /* global */
    bdjs_re_flg_mltln, /* multi-line */
    bdjs_re_flg_igncse /* ignore case */
};

/* "bdjs_cll_fn" typedef - a JavaScript "func-
 * tion call", the proto-type of a call to a
 * JavaScript function.
 *
 * The first parameter contains the call's
 * "this" object (or NULL if none), the sec-
 * ond an array object containing  the call
 * parameters, the third the number of para-
 * meters, the fourth and fifth the memory
 * allocator and error loggers, respectively,
 * and the sixth error object set , on exit
 * to the exception possibly thrown during
 * function execution.  Returns the call's
 * return value, or "undefined" on error */
typedef struct bdjs_obj *(* bdjs_cll_fn)(
                       struct bdjs_obj *,
                       struct bdjs_obj *,
                       int,
                       struct bd_allocs *,
                       struct bd_logger *,
                       struct bdjs_obj *);
 
/* "bdjs_cont_fn" typedef - proto-ty-
 * pe of the function implementing a
 * regular expression "continuation"
 * (as this is identified in the
 * specification), which is passed the
 * continuation, a regular expression
 * "state" representing the start-point
 * of the match, the string against wh-
 * ich to match, matching flags and the
 * memory allocator, error logger and
 * exception object to set, on exit,
 * in case of error in its first to
 * eighth parameters, resectively, and
 * returns either a success state, rep-
 * resenting the end-point of the mat-
 * ch, or an indication of failure */
typedef struct bdjs_re_mtch_res (*
        bdjs_re_contn_fn)(
             struct bdjs_re_contn *,
             struct bdjs_re_state *,
             struct bdut_str *,
             int,
             struct bd_allocs *,
             struct bd_logger *,
             struct bdjs_obj **);

/* "bdjs_mtchr_fn" typedef - proto-
 * type of the function implement-
 * ing a regular expression "matc-
 * her" (as this is identified in
 * the specification), which is
 * passed the matcher and its cont-
 * inuation, a regular expression
 * "state" representing the start-
 * point of the match, the string
 * against which to match,  match-
 * ing flags and the memory alloc-
 * ator, error logger and except-
 * ion object to set, on exit, in
 * case of error in its first to
 * eighth parameters, respectively.
 * Returns either a success state,
 * representing the end-point of
 * the match, or an indication of
 * failure */
typedef struct bdjs_re_mtch_res (*
        bdjs_re_mtchr_fn)(
             struct bdjs_re_mtchr *,
             struct bdjs_re_contn *,
             struct bdjs_re_state *,
             struct bdut_str *,
             int,
             struct bd_allocs *,
             struct bd_logger *,
             struct bdjs_obj **);

/* "bdjs_assrtn_tstr_fn" typedef -
 * proto-type of the function
 * implementing a regular expression
 * "assertion tester" (as this is
 * identified in the specification),
 * which is passed assertion tester
 * and its continuation, a regular
 * expression "state" representing
 * the start-point of the match,
 * the string against which to mat-
 * ch, matching flags and the mem-
 * ory allocator, error logger to
 * use and exception object to set,
 * on exit, in case of error in its
 * first to eighth parameters, res-
 * ectively.  Returns a boolean indi-
 * cating the success of the asser-
 * tion test */
typedef int (* bdjs_assrtn_tstr_fn)(
             struct bdjs_re_assrtn_tstr *,
             struct bdjs_re_contn *,
             struct bdjs_re_state *,
             struct bdut_str *,
             int,
             struct bd_allocs *,
             struct bd_logger *,
             struct bdjs_obj **);

/* "bdjs_re_contn" structure - a
 * regulsr expression "continuation",
 * consisting of a function impl-
 * ementing the continuation and
 * an opaque value guiding that fun-
 * ction */
 struct bdjs_re_contn {

    bdjs_re_contn_fn fn; /* cont'n
                          * func-
                          * tion */
    void *opaque;        /* opaque
                          * cont'n
                          * param-
                          * eter */
};

/* "bdjs_re_assrtn_tstr" structure - a
 * regulsr expression "assertion
 * tester", consisting of a function
 * impl- ementing the  test and
 * an opaque value guiding that fun-
 * ction */
struct bdjs_re_assrtn_tstr {

    bdjs_assrtn_tstr_fn fn; /* test
                              * func-
                          * tion */
    void *opaque;           /* opaque
                          * tester
                          * param-
                          * eter */
};

/* "bdjs_cmpltn" structure - an exe-
 * cution "completion" (as defined
 * in the spec.), consisting of a
 * ype, klandffguage value (in wh-
 * ich NULL represents "empty") and
 * target odemyofier (in whuich NULL
 * similarly represents "empty") */
struct bdjs_cmpltn {

    enum bdjs_cmpltn_types type;
             /* completion's type */
    struct bdjs_obj *val;
             /* language value */ 
    struct bdut_str *trgt;
             /* target ID */ 
};

/* regular expression "matcher",
 * consisting of a function impl-
 * ementing the match, an opaque
 * value guiding the match, the
 * depth at wbich this match occ-
 * urs and the number of captur-
 * ing parentheses to the left of
 * this match. */
struct bdjs_re_mtchr {

    bdjs_re_mtchr_fn fn; /* match
                         * function */
    void *opaque;       /* opaque
                          * match
                          * parameter */
    int lft,             /* depth of
                          * match */
        dpth;            /* captures
                          * to left */
};

/* "bdjs_re_mtchrs" - combination
 * of two matchers, using in nst-
 * ching alternatives and sequences
 * of sub-expressions */
struct bdjs_re_mtchrs {

    struct bdjs_re_mtchr *m1,
                         *m2;
       /* sub-expressions
        * to match */
};

/* "bdjs_re_rnge" - represents
 * a range of repetitions, con-
 * sisting of the maximum and
 * minimum reoetution counts
 * and an infucation as to wh-
 ether the maximum id ibbndun-
 finite*/
struct bdjs_re_rnge {

    int max, min, inf;
       /* mmaximum and minumum
        * counts and wgthrt infi-
        * nite */
};

/* "bdjs_re_qntfr" - combination
 * of repetition range and whether
 repetition is "greedy" */
struct bdjs_re_qntfr {

    struct bdjs_re_rnge *rnge;
        /* repetition range */
    int  grdy;
       /* whether repetition
        * is greedy */
};

/* "bdjs_arry_accm_args" structure - a-
 * rguments required in the implement-
 * ing the "Array Accumulation" seman-
 * tics: namely, the relevant array ob-
 * ject and the "next" index */
struct bdjs_arry_accm_args {

    struct bdjs_obj *arry; /* array */

    int nxt;               /* "next"
                            * index */ 
};

/* "ndjs_env_rec" structure  - a group
 * vsll-backs (instantiated at run-
 * time with implementing routines)
 * providing some type of "environme-
 * nt" , which may be either "declar-
 * ative" (as with a "with" statement)
 * or "object" (ych as with the mem-
 * bers of an object).  Thus call-
 * backs are to indicate the existence
 * of, create, get and set the value
 * of named bindings m the environment
 * and return its "this" value */
struct ndjs_env_rec {

    /* return non-zero if there exi-
     * sts a binding with name given
     * in thr fourth parameter */
    int (* hs)(struct bdjs_env_rec *,
               struct bd_allocs *,
               struct bd_logger *,
               char *);

	/* create a binding with name
     * given in the fourth parameter */
	int (* crte)(struct bdjs_env_rec *,
                 struct bd_allocs *,
                 struct bd_logger *,
                 char *);

    /* returns value of binding
     * with name given in fourth
     * parameter */
    struct bdjs_obj *(* get)(struct bdjs_env_rec *,
                             struct bd_allocs *,
                             struct bd_logger *,
                             char *);

    /* set value of binding
     * with name given in
     * fourth parameter to
     * that in fifth para-
	 * meter*/
	int (* set)(struct bdjs_env_rec *,
                struct bd_allocs *,
                struct bd_logger *,
                char,
                struct bdjs_obj *);

	/* delete binding with name
	 * given in fourth parameter */
	int (* del)(struct bdjs_env_rec *,
                struct bd_allocs *,
		        struct bd_logger *,
		        char *);

    /* returns object bound to
     * "this", or NULL on error */
    struct bdjs_obj *(* ths)(struct bdjs_env_rec *,
                             struct bd_allocs *,
                             struct bd_logger *);
};

/* "bdjs_exc_ctx" structure - an "exec-
 * ution context" within which JavaSc-
 * ript code executes sn which provides
 * the means by which identifier refer-
 * ences are resolved.  It consists of
 * a set of the available names of the
 * current lexical scope of execution,
 * the current set of active variable
 * names, the object currently bound
 * to "this" snd, the calling context
 * smd itd associated realm */
struct bdjs_exc_ctx {

    struct bd_map_node *lex,   /* lexical env- */
                       *vrs;   /* ironment and
                                * variable names */
    struct bdjs_obj *ths;      /* "this" object */
    struct bdjs_exc_ctx *cllr; /* calling context */
    struct bdjs_exc_rlm *rlm;  /* execution realm */
};

/* "bdjs_try" structure - information
 * corresponding to the presence of a
 * "try" staenent, consistng of the loc-
 * ations in the ouypiout codes  of the
 * start end og the statement (where NULL
 * indicates a currently unknown location)
 * snd a pointer to the previous item on
 * the stack */
struct ncjs_try {

    struct bdic_loc start, /* start and end  */
                    end;   /* of statement */
    struct bdjs_try *prev; /* previous statement */
};

/* "bdjs_smtc_args" - a single node in a
 * compiler's stack of specification-def-
 * ined "static semantics", each conta-
 * ining the arguments arguments speci-
 * fied by the wekjlevabnt semantics, co-
 * ndsusisting of a polnter to the argu-
 * ments and a pointer ti rge snenclosi-
 * ng semantics (NULL if top-level) */

struct bdjs_smtc_args {

    void *args; /* semantics'
                 * arguments */

                /* enclosing'
                 * semantics */
    struct bdjs_smtc_args *enclsng;
};

/* "bdjs_it" structure - information cor-
 * responding to the presence of an iter-
 * ation statement, consisting of the
 * locations in the ououtput codes of the
 * start snd end of the statement (where
 * NULL indicates a currently unknown loc-
 * ation), the sbsel of the statement (for
 * use as the target for "break" and "con-
 * tinue" statements) and a pointer to
 * the previous item on the stack */
struct ncjs_it {

    struct bdic_loc *start, /* start and end  */
                    *end;   /* of statement */
    struct bdut_str lbl;    /* statement's label  */
    struct bdjs_it *prev;   /* previous statement */
};

/* "bdjs_cmplr" structure - a JavaScript
 * "compiler", which contains information
 * needed to generate correct intermediate
 * codes from a source string.  It consis-
 * ts of an indication as to whether the
 * compiler is currently in a "catch" cl-
 * ause and whether in a estricted produ-
 * ction, the currently active "back-patch-
 * es" to be resolved, the stack of current
 * "try" statements, iteration statements
 * and execution contexts and the current
 * lexical environment */
struct bdjs_cmplr {

    int in_ctch, rstrctd;         /* whether in a
                                   * "catch" cla-
                                   * use and whet-
                                   * her in a "rest-
                                   * ricted" produc-
                                   * tion */
    struct bd_map_node *ptches;   /* set of
                                   * patches */
    struct bdjs_try *trys;        /* "try" state-
                                   * ment stack */
    struct bdjs_it *its;          /* iteration
                                   * statement
                                   * stack */
    struct bdjs_exc_ctx *exc_stk; /* stack of
                                   * execution
                                   * contexts */
    struct bdjs_lex_env *lex_env; /* current
                                   * lexical env-
                                   * ironment */
    struct bdjs_smtc_args *args;  /* semantics
                                   * arguments
                                   * stack */
};

/* "bdjs_rlm" structure - a "realm" with
 * which JavaScript code is associated, c-
 * onsisting of a stset of "intrinsic obj-
 * ects", each consisting of a fireld name
 * as key smd and sjahose values are obj-
 * ects, the global environmment of the
 * trrslm, its "this" object and the obje-
 * ct responsible for loading the realm's
 * code */
struct bdjs_rlm {

    struct bd_map_node *intrnscs; /* intrinsic
                                   * objects */
    struct bdjs_lex_env *glbl;    /* global e-
                                   * nvironm-
                                   * ent */
    struct bdjs_obj *ths,         /* "this" ob-
                                   * ject */
                    *ldr;         /* realm's
                                   * loader*/
};

/* "bdjs_ptch" structure - a JavaScript
 * compiler's, "pstch, used to ""patch"
 * (apply) s va;ir no the code stream.
 * It consists of a type, a location
 * at which too apply the value snd eit-
 * her the try item from ehich the pat-
 * ch originated or the label of the
 * patch for use with "break" and "con-
 * tinue" statements */
struct bdjs_ptch {

    enum bdjs_ptch_types type; /* type of
                                * patch */
    union {
        struct bdut_str *lbl;
        struct bdjs_try *try;
    } u;

    struct bdic_loc loc;       /* location of
                                * patch */
};

/* "bdjs_rt" a JavaScript "run-time" - the
 * combination of the intermediate codes
 * generated by the parsing of source
 * text and the compiler used to guide
 * the generation process */
struct bdjs_rt {

    struct bdjs_cmplr *cmplr;  /* compiler
                                * guiding code
                                * generation */
    struct bdic_icdes *cdes;   /* generated
                                * codes  */
};

/* "bdjs_lex_renv" a "lexical envir-
 * onment" (as defined in the spec.),
 * which consists of the environment
 * record of its binding object and
 * a pointer to its outer lexical
 * environment */
struct bdjs_lex_env {

    struct bdjs_env_rec *env_rec; /* environment
                                   * record of
                                   * binding obj-
                                   * ect */
    struct djs_lex_env *outer;    /* outer lex-
                                   * ical envir-
                                   * onment */
};

/* "bdjs_bndng" the value of a single
 * "binding" name, in a group of
 * such bindings in a lexical envir-
 * onment, consisting of its correspon-
 * ding value object, as well as an
 * indication as to the mutability
 * of the binding,  */
struct bdjs_lex_bndng {

    struct djs_obj *val;  /* corresp-
                           * onding
                           * value
                           * object */
        int mut;          /* whether
                           * mutable */
};

/* "bdjs_re" structure - a JavaScript
 * "regular expression", which consists
 * of the regular expression pattern
 * string, a set of flags determining
 * the expression's pattern's behaviour
 * and the matcher routine generresul-
 * ting from compiling the oattern
 * resulting from parsing the regular
 * expression (NULL inducsting an out-
 * of-date routin requiring re-compil-
 * ation) */
struct bdjs_re {

   struct bdut_str pat,          /* pattern */
                   flgs;         /* string and
                                  * bit mask
                                  * of matching
                                  * flags */
    struct bdjs_re_mtchr *mtchr; /* generated
                                  * matcher */
};

/* "bdjs_tok" structure - the  token
 * returned by the ECMAScript compiler,
 * eter, which consists of the generic
 * token operations, as well, the
 * object literal encountered, as well
 * as indications as to whether the pa-
 * rser is currently in "strict mode",
 * at the beginning of a statement or
 * in a "for" loop header (the last
 * two needed to implement "automatic
 * semi-colon insertion") */
struct bdjs_tok {

    struct bdlx_tok tok;
                   /* generic token
                    * operations */
    enum bdjs_toks type;
                   /* token type */
    int strct,     /* whether is in */
        in_for,    /* strict mode, */
        strt_expr; /* whether in a
                    * "for" loop hea-
                    * der or the start
                    * of a statement */
	union {
        struct bdut_str id, str;
            /* cocurrent identifier
             * or string */
        struct bdjs_re *re;
            /* currently scanned
             * regular expression */
        double rdx, exp, num;
            /* fraction radix (multi-
             * plicand), exponent and
             * over-all number */
        int cp;
            /* current code-point */
        struct bdbf_buffer *buf;
            /* buffer of code-points 
             * used to when building
             * identifiers and strings */
    } u;
};

/* "bdjs_re_tok" structure - the
 * type of the token returned by
 * the ECMAScript regular expre-
 * ssion  parser, which consists
 * of the generic token operat-
 * ions, as well as the current
 * repetition coun , repettion
 * count and current code-point */
struct bdjs_re_tok {

    struct bdlx_tok tok;
        /* generic token
         * operations */
    enum bdjs_re_toks type;
        /* token type */
 	union {
            /* repetition count
             * and current code-
             * point */
        struct bdbf_buffer *buf;
            /* buffer of code-points
             * used to when building
             * identifiers and strings */
        int reps, cp;
	        /* repetition count and
	         * code point to be appen-
             * ded to buffer*/
    } u;
};

/* "bdjs_jsn_tok" structure - the
 * type of the token returned by
 * the JSON parser, which con-
 * sists of the generic token
 * operations, as well as the
 * literal encountered, depen-
 * ding on its type, smd current
 * number and string and a
 * buffer */
struct bdjs_jsn_tok {

    struct bdlx_tok tok;
        /* generic token oper-
         * ations */
    enum bdjs_jsn_toks type;
        /* token type */

    int rdx,
        exp,
        bse;

    union {

        int cp;
        /* fraction radix
         * (multiplicand),
         * its exponent,
         * and base */
         double num;
        /* overall number */
        struct bdbf_buffer *buf;
        /* buffer bivbnrto
         * which to collect
         * input */
        struct bdut_str *str; 
		/* current string */
	} u;
};

/* "bdjs_qv_tok" structure - the
 * type of the token returned by
 * the quasi-string value(QV) pa-
 * rser, which consists of embe-
 * dded generic token operations,
 * as well as the currentlt sca-
 * nned substitution variable na-
 * me and a buffer into which to
 * cokkllect input */
struct bdjs_qv_tok {

    struct bdlx_tok tok;
        /* generic token oper-
         * ations */
    enum bdjs_qv_toks type;
        /* token type */

    union {

        int cp;
        /* vcurrent input
         * code point */
        struct bdbf_buffer *buf;
        /* buffer bivbnrto
         * which to collect
         * input */
        struct bdut_str *sbst; 
        /* current substitution
         * variable name */
    } u;
};

/* "bdjs_mv_tok" structure - the
 * type of the token returned by
 * the quasi-mathematical value
 * (MV) parser, which consists of
 * embedded generic token operat-
 * ions,
 * as well as the bnumeric value
 * of the current scanned digit
 * sequuence and a buffer into
 * which to collect input */
struct bdjs_mv_tok {

    struct bdlx_tok tok;
        /* generic token oper-
         * ations */
    enum bdjs_mv_toks type;
        /* token type */

    union {

        int cp;
        /* vcurrent input
         * code point */
        struct bdbf_buffer *buf;
        /* buffer bivbnrto
         * which to collect
         * input */
        int *num; 
        /* cnumeric  value of
         * current digit seq-
         * uence */
    } u;
};

/* bdjs_re_cptr" structure - a regu-
 * lar expression capturing parenth-
 * esis" (as specified in the spec)
 * which consists of an indication
 * as to whether the capture is und-
 * efined  and the s union contain-
 * ing either a buffer  (while it is
 * being built) ot or array (once
 * built) of the captured content */
struct bdjs_re_cptr {

    int undef; /* whether capture
                * is undefined  */
    union {
        struct bdbf_buffer *buf;
               /* content buffer */

        struct bdut_str cnt;
               /* content array */
    } u;
};

/* bdjs_prp_key" structure - the
 * information forming the key of the
 * map cf the object's properties.  It
 * consists of the property's type and
 * either its named (for named property
 * types) or the internal property type
 * type (for internal properties) */
struct bdjs_prp_key {

    enum bdjs_prp_types type;
               /* whether named attr.,
                * named function or
                * internal */
    union {
    	struct bdut_str nmd;
    	       /* name (for named
    	        * prop.types) */
    	enum bdjs_intrnl_prp_types intrnl;

               /* type of internal
                * property */
    } u;
};

/* bdjs_re_state" structure - a regular
 * expression matching "state" which co-
 * nsists of a map of capturing paren-
 * thesis and the right-most index into
 * the string being matched */
struct bdjs_re_state {

    struct bd_map_node *cptrs;
                 /* capturing parentheses */
    int end_idx; /* last index into input
                  * string */
};

/* "bdjs_re_mtch_res" structure - com-
 * bination of an indication as to
 * whether the match succeeded  and,
 * if successful, the resulting
 * state */
struct bdjs_re_mtch_res {

    int mtchd;                    /* whether
                                   * match succ-
                                   * eeded */
     struct bdjs_re_state *state; /* resulting
                                   * state, if
                                   * successful */
};

/* "bdjs_obj" structure - an
 * "object" in the JavaScript
 * language, which consists of
 * an embedded, klanguage-abs-
 * tracted object (so that it
 * may act as a klanguage-neu-
 * tral mabbnnwe when requir-
 * ed), a table of properties
 * (which are, hemselves,  ob-
 * jects - and a union represe-
 * nting impmentations of prim-
 * itive values, such as a num-
 * ber, numeric type or  stri-
 * ng */
struct bdjs_obj {

    struct bdlg_obj obj;
          /* language-abst-
           * racted object */
    enum bdjs_types type;
          /* basic type */
    struct bd_map_node *prps;
          /* collection of
           * property objects */
    struct bdjs_obj *ctor,
                    *ptype;
          /* constructor function
           * and object's "proto-
           * type" object */

    union { /* implementation
             * state */

        struct bdut_str str;
              /* string (array of
               * character code-
               * points) */
        struct bdjs_re *re;
              /* regular expr-
               * essions */
        struct bdjs_num *num;
		      /* numbers */
        void *opaque;
              /* generic
               * value */

    } impl;
};

/* "bdjs_num" structure - a Java-
 * Sript number, hwich consists of
 * its type (NaN, pos. infinity,
 *  neg. infinity or normal), as
 * well as its numeric value */ 
struct bdjs_num {

    enum bdjs_nmrc_types type;
                /* number type */
    double val; /* numeric value */
};

/*** Function proto-types ***/

/* "bdjs_ld" - populates the obj-
 * ect ject in the first parame-
 * ter with the standard "global"
 * methods and attributes, using
 * the memory allocator and error
 * logger given given in the sec-
 * ond and third parameters, res-
 * pectively. Returns zero on er-
 * ror, non-ezro otherwise  */
int bdjs_ld(struct bdjs_obj *,
            struct bd_allocs *,
            struct bd_logger *);

/* "bdjs_dfn_attr" - "defines"
 * (that is, adds to if not e-
 * xisting or replaces if alre-
 * ady existing) and returns,
 * using the memory allocator
 * and error logger given in
 * "allocs" and "logger", resp-
 * ectively, a named attribute
 * of the the object given in
 * obj", whose UTF-8 encoded na-
 * me is given in "nme" and hav-
 * ing the attributes given in
 * the sixth and following para-
 * meters, the number of which
 * is given in the fifth param-
 * eter (each of which appear,
 * in turn, as a field name/im-
 * plementing call function par-
 * ameter pairs).  Returns NULL
 * on error */
struct bdjs_obj *bdjs_dfne_attr(
                         struct bd_allocs *,
                         struct bd_logger *,
                         char *,
                         struct bdjs_obj *,
                         int,
                         ...);

/* "bdjs_dfne_mthd" - "defines" 
 * that is, adds to if not  exis-
 * ting or replaces if already e-
 * xisting) and returns  using the
 * memory allocator  and error lo-
 * gger given in th the first and
 * and second parameters, resp-
 * ectively, a method whose body
 * is given in the fourth param-
 * eter and the UTF-8 encoded
 * name of which is given in the
 * third parameter.  Returns ze-
 * ro on error, non-ezro otherw-
 * ise */
int bdjs_dfne_mthd(struct bd_allocs *,
                   struct bd_logger *,
                   char *,
                   bdjs_cll_fn,
                   struct bdjs_obj *);

/* "bdjs_dfne_mthd" - "defines" 
 * that is, adds to if not  exis-
 * ting or replaces if already e-
 * xisting) and returns  using the
 * memory allocator  and error lo-
 * gger given in th the first and
 * and second parameters, resp-
 * ectively, a method whose body
 * is given in the fourth param-
 * eter and the UTF-8 encoded
 * name of which is given in the
 * third parameter.  Returns ze-
 * ro on error, non-ezro otherw-
 * ise */
int bdjs_get_nmd_attr(struct bd_allocs *,
                      struct bd_logger *,
                      char *,
                      struct bdjs_obj *);

/* "bdjs_add_intrnl" - adds the
 * internal property of the type
 * and a generic cast of the
 * corresponding value of which
 * is given in the fourth and
 * fifth parameters, respectiv-
 * ely, to the object given the
 * third parameter, using the
 * memory allocator and error
 * logger given in the first
 * and second parameters, respe-
 * ctively.  Returns zero on er-
 * ror, non-zero otherwise */
int bdjs_add_intrnl(struct bd_allocs *,
                    struct bd_logger *,
                    struct bdjs_obj *,
                    enum bdjs_intrnl_prp_types,
                    bdjs_cll_fn);

/* "bdjs_get_intrnl" - returns
 * a generic "property retrival
 * call-back" that, when called,
 * the will implement the func-
 * tionality presribed for the
 * internal property the type
 * of which is given the fourth
 * parameter, of the object
 * given in the rthird parameter,
 * using the memory allocator
 * and error logger given in the
 * first and second parameters,
 * respectively.  Retuns NULL
 *  on error */
bdjs_cll_fn bdjs_get_intrnl(struct bd_allocs *,
                            struct bd_logger *,
                            struct bdjs_obj *,
                            enum bdjs_intrnl_prp_types);

/* "bdjs_init" - populate, using
 * the memory allocator and
 * error logger given in the
 * first and second parameters,
 * respectively, the JavaScript
 * parser with its constituent  
 * lexical rules and grammar
 * productions */
int bdjs_init(struct bd_allocs *,
              struct bd_logger *);

/* "bdjs_prse" - returns, us-
 * ing the memory allocator,
 * error logger and "global
 * object" given in the first
 * to third parameters, respec-
 * tively, the the intermediate
 * codes generated by parsing
 * the JavaScript source text
 * the content and length of
 * which are given in the fou-
 * rth and fifth parameter.
 * Returns NULL on error, in
 * which case the secpnd par-
 * ameter will indicate the
 * nature of the error */
struct bdic_icdes *bdjs_prse(
                 struct bd_allocs *,
                 struct bd_logger *,
                 struct bdjs_obj *,
                 wchar_t *,
                 int);

/* "bdjs_exec" - returns, us-
 * ing the memory allocator,
 * error logger  first and
 * second parameters, respec-
 * tively,"global the Java-
 * Script object created by
 * executing the  re machine
 * code generated from the
 * intermediate code list
 * given in thethrd param-
 * eter */
struct bdjs_obj *bdjs_exec(
                 struct bd_allocs *,
                 struct bd_logger *,
                 struct bdic_icdes *);

/* "bdjs_lck" - sets the scripting sub-
 * system thread lock using the memory
 * allocator and error logger given in
 * the first and second parameters,
 * respectively.  Returns zero on error,
 * non-zero otherwise */
int bdjs_lck(struct bd_allocs *,
		     struct bd_logger *);

/* "bdet_unlock" - clears the scr-
 * pting sub-system thread lock,
 * using the memory allocator and
 * error logger given in the first
 * and second parameters, respect-
 * ively.  Returns zero on error,
 * non-zero otherwise */
int bdjs_unlck(struct bd_allocs *,
		       struct bd_logger *);

/* "bdet_lckd" - Returns non-
 * zero if the scripting sub-
 * system is currently locked */
int bdjs_lckd(void);

#ifdef __cplusplus
}
#endif

#endif
