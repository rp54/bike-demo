/*****************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery 
* 
* ic.h - data structures and function
* proto-types related to compiler inter-
* mediate codes
*
* Intermediate codes are the "raw" output
* of of a compiler designed to be as arch-
* itecture-neutral as possible, generated
* just tbefore the last phase of the comp-
* iler - object code generation - transforms
* them into architecture-specific machine
code, which may then be directly executed. 
*
* intermedate codes perform fumctions such
* as: (among others) pushing finction
* parameters, calling functions (both with
* and without return values), returning
* from functions and procedures, perform-
* ing arithmatic, logical operations,
* and branching (both unconditionally  and
* based on the result of value comparisons)
*
* This implementation uses the triples
* form of intermediate codes (implemented
* using quadruples).
*
* Copyright (C) 2006-2012, Russell Potter,
* All Rights Reserved
*
******************************************************************/

#ifndef __BD_IC_H
#define __BD_IC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/buffer.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/map.h>
#include <bikedemo/ic.h>

/* "bdic_types" enumeration - the possible types of
 * intermediate code types, which incude parameter
 * passing, function calls, arithmatic, bit shifting
 * and rotating and returning from function calls */
enum bdic_types {

    bdic_type_param, /* set call param-
                      * eter */
    bdic_type_call,  /* excecute procedure
                      * call (no return
                      * value)*/
    bdic_type_fcall, /* call function
                      * call (with return
                      * value)*/
    bdic_type_add,   /* addition */
    bdic_type_sub,   /* subtraction */
    bdic_type_mul,   /* multiplication */
    bdic_type_div,   /* division */
    bdic_type_neg,   /* negate */
    bdic_type_land,  /* logical and */
    bdic_type_lor,   /* logical or */
    bdic_type_not,   /* logical not */
    bdic_type_band,  /* bit-wise and */
    bdic_type_bor,   /* bit-wise or */
    bdic_type_rol,   /* bit-wise rotate
                      * left */
    bdic_type_ror,   /* bit-wise rotate */
                     /* right */
    bdic_type_cpy,   /* copy */
    bdic_type_shr,   /* bit shift right */
    bdic_type_shl,   /* bit shift left */
    bdic_type_ret,   /* return from procedure */
    bdic_type_jmp,   /* unconditional jump */
    bdic_type_jlt,   /* jump if less than */
    bdic_type_jle,   /* jump if less than or
                      * equal to */
    bdic_type_jgt,   /* jump if greater than */
    bdic_type_jge,   /* jump if greater than or
                      * equal to */
    bdic_type_jeq,   /* jump if is equal to */
    bdic_type_jne    /* jump if is not equal to */
};


/* "bdic_cnst_types" enumeration - the
 * possible types a constant value may
 * have within an intermediate code,
 * namely: byte, short, integer, long,
 * float or double  */
enum bdic_cnst_types {

    bdic_cnst_type_byte, /* byte */
    bdic_cnst_type_shrt, /* short */
    bdic_cnst_type_int,  /* integer */
    bdic_cnst_type_lng,  /* long */
    bdic_cnst_type_flt,  /* long */
    bdic_cnst_type_dbl   /* double */
};

/* "bdic_mdes" enumeration - the possible
 * "modes"that an intermediate code value may
 * take, namely, an immediate constant value,
 * zymbol reference, intermediate code offset,
 * which the valuor an absolute address */
enum bdic_mdes {

    bdic_mde_cnst, /* constant value */
    bdic_mde_sym,  /* symbol table entry */
    bdic_mde_indr, /* indirect reference
                    * to instruction (as
                    * offset from beginn-
                    * ing of code stream) */
    bdic_mde_addr  /* absolute address */
};

/* "bdic_cnst" structure - representation
 * of a constnr value*/

/* "bdop_cnst" structure = - san intermed-
 * iate code constant, consisting of oits
 * type smmsnd s union containing its val-
 * ue */
struct bdic_cnst {

    enum bdic_cnst_types type; /* type */

    union { /* constant's
             * value */

        char b;   /* char */
        short s;  /* short */
        int i;    /* int */
        long l;   /* long */
        float f;  /* float */
        double d; /* double */
    } u;
};

/* "bdic_val" structure - a single
 * "value" (either in operands or
 * result) containing either a
 * literal value, symbol table in-
 * dex or instruction address or
 * intermediate code offset, dep-
 * ending on the mode */
struct bdic_val {

    enum bdic_mdes mode; /* type of
                          * operand */

    union {

        void *lit,             /* literal */
             *addr;            /* value or
                                * absolute
                                * address */
        int sym;               /* index
                                * into sym-
                                * bol table  */
        int indr;              /* indirect
                                * offset
                                * into int-
                                * ermediate
                                * code list */
        struct bdic_cnst cnst; /* oonstant 
                                * value */
    } val;
};

/* "bdic_icde" structure - a single
 * intermediate code, which contains a
 * result symbol, an internedyate code
 * type, and at miost sat three struct-
 * ures representing
 * the code's operands */
struct bdic_icde {

    enum bdic_types type;      /* code type */
    int rslt;                  /* result symbol */
    struct bdic_val oprnds[3]; /* max. three
                                * operands */
};

/* "bdic_loc" structure - am intermediate
 * code "location" , consysting of its off-
 * set within a code list and the kocation
 * within the code */
struct bdic_loc {

    int off, /* offset of code */
        arg; /* and argument number */
};

/* "bdic_icdes" a list of intermediate codes,
 * cpmsustinf og a inffer of the iccdes (used
 * when idcodes are being built), a "flatt-
 * ened" array of those codes (used sfter code
 * generation is complete) snd a nap containing
 * the codes, ordered by position, so that the
 * codes nay be accessed in that order */
struct bdic_icdes {

    struct bd_map_node *map;     /* map conta-
                                  * ining codes
                                  * in code list */
    union {
        struct bdbf_buffer *buf; /* buffer of
                                  * code list */
        struct bdic_icde *arry;  /* array of
                                  * generated
                                  * codes */ 
    } u;
};

/*** function proto-types ***/

/* "bdic_init_cdes" returns a pointer to
 * an (initially empty) intermediate code
 * list, using the memory allocator and
 * error logger given in the first and
 * second parameters, respectively */
struct bdic_icdes *bdic_init_cdes(
                    struct bd_allocs *,
                    struct bd_logger *);

/* "bdic_idx" returns the intermediate
 * code located at the index  ,given in
 * the first parameter, into the inter-
 * mediate code list given in the second
 * parameter, using the memory allocator
 * and ror logger given in the third
 * and fourth parameters, respectively */
struct bdic_icde *bdic_idx(int,
		                   struct bdic_icdes *,
                           struct bd_allocs *,
                           struct bd_logger *);

/* "bdic_mark" - returns the location
 * given the first parameter within
 * the current end of the intermediate
 * code list given in the second para-
 * meter, using the memory allocator
 * and error logger given in the third
 * and fourth parameters, respectively.
 * Returns NULL on error */
struct bdic_loc *bdic_mark(enum bdic_locs,
                           struct bdic_icdes *,
                           struct bd_allocs *,
                           struct bd_logger *);

/* "bdic_ptch" - "patches" (writes over)
 * with the value given in the second
 * parameter at the location given in
 * thr first parameter within the
 * intermediate code list given in the
 * third parameter, using the memory
 * allocator and error logger given in
 * thr fourth and fifth parameters esp-
 * ectively.  Returns zero on error,
 * non-zero otherwise */
int bdic_ptch(struct bdic_loc *,
              void *,
              struct bdic_icdes *,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdic_emit" - "emits" (appends)
 * the intermediate code code ewose
 * type and result symbol number is
 * givren in the second and third
 * parameters , respectively , op-
 * erand modes given in the fourth,
 * sixth and eighth parameters,
 * respectively, and the operand
 * data given in the fifth. seventh
 * snd ninth, second pstsarameters,
 * respectively, and operand data
 * given in the fifth, seventh and
 * ninth parameters, respectively,
 * from the intermediate code list
 * given in the first parameter,
 * using the memory allocator and
 * error logger given in the tenth
 * and eleventh parameters, respe-
 * ctively.  Returns zero on error,
 * non-zero otherwise */
int bdic_emit(struct bdic_icdes *,
              enum bdic_types,
              int,
              enum bdic_mdes,
              void *,
              enum bdic_mdes,
              void *,
              enum bdic_mdes,
              void *,
              struct bd_allocs *,
              struct bd_logger *);

/* "bdic_emit_cde" - "emits" (outputs)
 * the intermediate code given in the
 * second parameter fron thr intermed-
 * iate code list given in the first
 * parameter, using the memory allocat-
 * or and error logger given in the
 * third and fourth parameters, resp-
 * ectively.  Returns  zero on error,
 * non-zero otherwise */
int bdic_emit_cde(struct bdic_icdes *,
                  struct bdic_icde *,
                  struct bd_allocs *,
                  struct bd_logger *);

/* "bdic_len" - returns the length of the
 * intemediate code list given in the first
 * parameter */
int bdic_len(struct bdic_icdes *);

/* "bdic_cat" -  returns the concatenation of
 * the intermediate codes given in the first
 * parameter to those in the second parameter
 * using the memory allocator and error logger
 * given in the second and third parameters,
 * respectively.  Returns NULL on error */
struct bdic_icdes *bdic_cat(struct bdic_icdes *,
                            struct bdic_icdes *,
                            struct bd_allocs *,
                            struct bd_logger *);

/* "bdic_end" -  completes the generation
 * of intermediate codes from the code list
 * given in the first parameter and memory
 * allocator and error logger given in the
 * second and third parameters, respectively.
 * Returns a pointer to the beginning an arr-
 * ay xonsisting of its emitted codes */
struct bdic_icde *bdic_end(struct bdic_icdes *,
                           struct bd_allocs *,
                           struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
