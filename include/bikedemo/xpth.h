/*************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery
*
* "xpth.h" - data types and function proto-
* types related to the W3C's XML Path Lan-
* guage (X-PATH) 3.0, a mechanis by which
* an XML node may be located via a "path
* expression" selative to another node
*
* Copyright (C) 2007-2013, Russell Potter,
* All rights reserved
*
**************************************************************/

#ifndef __BD_XPATH
#define __BD_XPATH

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/xml.h>

/* "bdxp_op_precs" enumeration -
 * the possible operator precede-
 * nces that mapy bre found in an
 * X-PATH expression */
enum bdxp_op_precs {

    bdxp_op_prec_cma,     /* "," */
    bdxp_op_prec_for,     /* "for","let",
                           * "some", "every",
                           * "if" */
    bdxp_op_prec_lor,     /* "logical or" */
    bdxp_op_prec_to,      /* "to" */
    bdxp_op_prec_rel,     /* "ne", "lt, "le",
                           * "gt" ge", "=",
                           * "!=", "<", "<=",
                           * ">" ">=", "is",
                           * "<<", ">>" */
    bdxp_op_prec_lor,     /* "||" */
    bdxp_op_prec_to,      /* "to" */
    bdxp_op_prec_add,     /* "+", "=" (binary) */
    bdxp_op_prec_div,     /* "div", "idiv",
                           * "mod" */
    bdxp_op_prec_unn,     /* "union", "|" */
    bdxp_op_prec_isct,    /* "intersect",
                           * "except" */
    bdxp_op_prec_isct,    /* "intersect" */
    bdxp_op_prec_instnce, /* "instance of" */
    bdxp_op_prec_trt,     /* "treat as" */
    bdxp_op_prec_cstble,  /* "castable as" */
    bdxp_op_prec_cst,     /* "cast as" */
    bdxp_op_prec_pos,     /* "+" (unary) */
    bdxp_op_prec_map,     /* "!" (map) */
    bdxp_op_prec_pth,     /* "/", "//" */
    bdxp_op_prec_arry     /* "[]" */
};

/* "bdxp_ops" enumeration - the pos-
 *sible operators that mapy be found
 * in an X-PATH expression */
enum bdxp_ops {

    bdxp_op_cma,
    bdxp_op_for,
    bdxp_op_let,
    bdxp_op_some,
    bdxp_op_every,
    bdxp_op_if,
    bdxp_op_or,
    bdxp_op_and,
    bdxp_op_seq,
    bdxp_op_sne,
    bdxp_op_slt,
    bdxp_op_sle,
    bdxp_op_sgt,
    bdxp_op_sge,
    bdxp_op_is,
    bdxp_op_lsh,
    bdxp_op_rsh,
    bdxp_op_lor,
    bdxp_op_to,
    bdxp_op_add,
    bdxp_op_mul,
    bdxp_op_div,
    bdxp_op_idiv,
    bdxp_op_mod,
    bdxp_op_unn,
    bdxp_op_sunn,
    bdxp_op_isct,
    bdxp_op_excpt,
    bdxp_op_instnce,
    bdxp_op_trt,
    bdxp_op_cstble,
    bdxp_op_cst,
    bdxp_op_pos,
    bdxp_op_mp,
    bdxp_op_pth,
    bdxp_op_arry
};

/* bdxp_nts" the non-terminals
 * of the X-PATH fgrammar */
enum bdxp_nts {

    bdxp_nt_xpth,
    bdxp_nt_expr,
    bdxp_nt_expr_list,
    bdxp_nt_for_expr,
    bdxp_nt_var_nme,
    bdxp_nt_qntfd_expr,
    bdxp_nt_let_expr,
    bdxp_nt_smple_for_clse,
    bdxp_nt_smple_for_bndng,
    bdxp_nt_smple_let_clse,
    bdxp_nt_smple_let_bndng,
    bdxp_nt_if_expr,
    bdxp_nt_or_expr,
    bdxp_nt_and_expr,
    bdxp_nt_expr_sngle,
    bdxp_nt_prm,
    bdxp_nt_cma_prm,
    bdxp_nt_cma_prm_list,
    bdxp_nt_gnrl_comp,
    bdxp_nt_val_comp,
    bdxp_nt_nde_comp,
    bdxp_nt_smple_map_expr,
    bdxp_nt_pth_expr,
    bdxp_nt_rel_pth_expr,
    bdxp_nt_step_expr,
    bdxp_nt_rel_step_expr,
    bdxp_nt_axis_step,
    bdxp_nt_fwd_axis,
    bdxp_nt_fwd_step,
    bdxp_nt_abbrv_fwd_step,
    bdxp_nt_rvse_step,
    bdxp_nt_rvse_axis,
    bdxp_nt_abbrv_rvse_step,
    bdxp_nt_nde_tst,
    bdxp_nt_nme_tst,
    bdxp_nt_wldcrd,
    bdxp_nt_arg,
    bdxp_nt_arg_list,
    bdxp_nt_arg_plcehldr,
    bdxp_nt_pred,
    bdxp_nt_pred_list,
    bdxp_nt_lit,
    bdxp_nt_nmrc_lit,
    bdxp_nt_vr_nme,
    bdxp_nt_vr_rf,
    bdxp_nt_fn_itm_expr,
    bdxp_nt_fn_rf,
    bdxp_nt_inln_fn_expr,
    bdxp_nt_sngle_type,
    bdxp_nt_type_dcl,
    bdxp_nt_seq_type,
    bdxp_nt_ocrrnce_indctr,
    bdxp_nt_itm_type,
    bdxp_nt_atmc_or_unn_type,
    bdxp_nt_knd_tst,
    bdxp_nt_any_knd_tst,
    bdxp_nt_doc_tst,
    bdxp_nt_elem_tst,
    bdxp_nt_txt_tst,
    bdxp_nt_cmmt_tst,
    bdxp_nt_ns_tst,
    bdxp_nt_pi_tst,
    bdxp_nt_attr_tst,
    bdxp_nt_attr_nme_or_wc,
    bdxp_nt_schma_attr_tst,
    bdxp_nt_elem_dcl,
    bdxp_nt_elem_nme,
    bdxp_nt_attr_nme,
    bdxp_nt_smple_type_nme,
    bdxp_nt_type_nme,
    bdxp_nt_fn_tst,
    bdxp_nt_any_fn_tst,
    bdxp_nt_typd_fn_tst,
    bdxp_nt_prn_itm_type
};

/* "bdxp_toks" - the tokens ret-
 * urned from the XPATH lexical
 * analyser */
 enum bdxp_toks {

    bdxp_tok_id,                         /* identifier "$id" */
    bdxp_tok_str,                        /* string "'abc'" */
    bdxp_tok_num,                        /* number "[0-9]+" */
    bdxp_tok_dble,                       /* floating point
                                          * number "[0-9]+[eE].
	                                      * [0-9]+[0-9]+" */
    bdxp_tok_fsl,                        /* forward slash "/" */
    bdxp_tok_dfsl,                       /* double
                                          * forward slash "//" */
    bdxp_tok_prnt,                       /* parent ".." */
    bdxp_tok_ctx,                        /* context node "." */
    bdxp_tok_ast,                        /* asterisk "*" */
    bdxp_tok_cln,                        /* colon ":" */
    bdxp_tok_qstn,                       /* question mark "?" */
    bdxp_tok_not,                        /* not "!" */
    bdxp_tok_hsh,                        /* hash "#" */
    bdxp_tok_eq,                         /* equal to "=" */
    bdxp_tok_ne,                         /* not equal to "!=" */
    bdxp_tok_lt,                         /* less than "<" */
    bdxp_tok_le,                         /* less than or
                                          * equal to "<=" */
    bdxp_tok_gt,                         /* greater than "<" */
    bdxp_tok_ge,                         /* greater than
                                          * or equal to "<=" */
    bdxp_tok_axis,                       /* axis "::" */
    bdxp_tok_lsh,                        /* left shift "<<" */
    bdxp_tok_rsh,                        /* right shift ">>" */
    bdxp_tok_cma,                        /* comma "," */
    bdxp_tok_add,                        /* add "+" */
    bdxp_tok_sub,                        /* subtract "-" */
    bdxp_tok_scat,                       /* string
                                          * concatenate "||" */
    bdxp_tok_unn,                        /* union "|" */
    bdxp_tok_dllr,                       /* dollar sign "$" */
    bdxp_tok_let,                        /* let ":=" */
    bdxp_tok_lbr,                        /* left brace "{" */
    bdxp_tok_rbr,                        /* right brace "}" */
    bdxp_tok_lpr,                        /* left
                                          * parenthesis "(" */
    bdxp_tok_rpr,                        /* right
                                          * parenthesis  ")" */
    bdxp_tok_lsq,                        /* left square
                                          * bracket "[" */
    bdxp_tok_rsq,                        /* right square
                                          * bracket "]" */
    bdxp_tok_kwd_for,                    /* keyword "for" */
    bdxp_tok_kwd_in,                     /* keyword "in" */
    bdxp_tok_kwd_let,                    /* keyword "let" */
    bdxp_tok_kwd_return,                 /* keyword "return" */
    bdxp_tok_kwd_if,                     /* keyword "if" */
    bdxp_tok_kwd_then,                   /* keyword "then" */
    bdxp_tok_kwd_else,                   /* keyword "else" */
    bdxp_tok_kwd_revery,                 /* keyword "every" */
    bdxp_tok_kwd_satisfies,              /* keyword "satisfies" */
    bdxp_tok_kwd_or,                     /* keyword "or" */
    bdxp_tok_kwd_and,                    /* keyword "and" */
    bdxp_tok_kwd_div,                    /* keyword "div" */
    bdxp_tok_kwd_mod,                    /* keyword "mod" */
    bdxp_tok_kwd_intersect,              /* keyword "intersect" */
    bdxp_tok_kwd_except,                 /* keyword "except" */
    bdxp_tok_kwd_instance,               /* keyword "instance" */
    bdxp_tok_kwd_of,                     /* keyword "of" */
    bdxp_tok_kwd_treat,                  /* keyword "treat" */
    bdxp_tok_kwd_as,                     /* keyword "as" */
    bdxp_tok_kwd_castable,               /* keyword "castable" */
    bdxp_tok_kwd_cast,                   /* keyword "cast" */
    bdxp_tok_kwd_child,                  /* keyword "child" */
    bdxp_tok_kwd_decendant,              /* keyword "descendant" */
    bdxp_tok_kwd_decendant_or_self,      /* keyword "descendant-or-self" */
    bdxp_tok_kwd_ansestor,               /* keyword "ancestor" */
    bdxp_tok_kwd_ansestor_or_self,       /* keyword "ancestor-or-self" */
    bdxp_tok_kwd_attribute,              /* keyword "attribute" */
    bdxp_tok_kwd_self,                   /* keyword "self" */
    bdxp_tok_kwd_following,              /* keyword "following" */
    bdxp_tok_kwd_following_sibling,      /* keyword "following-sibling" */
    bdxp_tok_kwd_preceding,              /* keyword "preceding" */
    bdxp_tok_kwd_preceding_sibling,      /* keyword "preceding-sibling" */
    bdxp_tok_kwd_namespace,              /* keyword "namespace" */
    bdxp_tok_kwd_parent,                 /* keyword "parent" */
    bdxp_tok_kwd_item,                   /* keyword "item" */
    bdxp_tok_kwd_is,                     /* keyword "is" */
    bdxp_tok_kwd_eq,                     /* keyword "eq" */
    bdxp_tok_kwd_ne,                     /* keyword "ne" */
    bdxp_tok_kwd_lt,                     /* keyword "lt" */
    bdxp_tok_kwd_le,                     /* keyword "le" */
    bdxp_tok_kwd_gt,                     /* keyword "gt" */
    bdxp_tok_kwd_ge,                     /* keyword "ge" */
    bdxp_tok_kwd_function,               /* keyword "function" */
    bdxp_tok_kwd_empty_sequence,         /* keyword "empty-sequence" */
    bdxp_tok_kwd_node,                   /* keyword "node" */
    bdxp_tok_kwd_text,                   /* keyword "text" */
    bdxp_tok_kwd_comment,                /* keyword "comment" */
    bdxp_tok_kwd_namespace_node,         /* keyword "namespace-node" */
    bdxp_tok_kwd_element,                /* keyword "element" */
    bdxp_tok_kwd_attribute,              /* keyword "attribute" */
    bdxp_tok_kwd_processing_instruction, /* keyword "processing-instruction" */
    bdxp_tok_kwd_schema_element,         /* keyword "schema-element" */
    bdxp_tok_kwd_schema_attribute        /* keyword "schema-attribute" */
};

/*** function declarations ***/

 /* "bdxp_slct" - sets the third
 * parameter to the (possibly empty)
 * node set by the X-PATH expression
 * given in the first parameter,
 * from the node given in the second
 * parameter, using the memory
 * allocator and error logger given
 * in the fourth and fifth parameters,
 * respectively */
int bdxp_slct(struct bdut_str *,
              struct bdxl_node *,
              struct bd_map_node **,
              struct bd_allocs *,
              struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
