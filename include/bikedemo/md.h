/*****************************************************************
*
* BikeDemo - exercise bike demonstration scenery
*
* md.h - data structures and function proto-types
* for lexical "machine descriptions", which describe
* how abstract, processor-neutral intermediate code
* instructions are translated into the processor's
* binary machine code, and, to accomplish this,
* contains a high-level description of the processor's
* characteristics (registers, etc), its assembly
* language, and the machine code corresponding to
* that assembly language.
*
* a machine decription also contains a map of
* pairings between a "pattern" of intermediate
* codes and the equivalent assembly language
* to be emitted. Note that this optimisation phase
* is different to the achitecture-independant
* phases such as common sub-expression and
* induction variable eliminations 
*
* Copyright (C) 2006-2014, Russell Potter,
* All rights reserved
*
******************************************************************/

#ifndef __BD_MD_H
#define __BD_MD_H

#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <bikedemo/ic.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bdmd_md;

/* "bdmd_types" enumeration - the types of
 * registers possible in a machine definition:
 * that is, either floating-point or integer */
enum bdmd_types {

	bdmd_reg_type_flt, /* floating-point type */
	bdmd_reg_type_int, /* integer type */

};
/* "bdmd_holds" enumeration - the type(s)
 * of data a register can hold: that is,
  * either address or data */
enum bdmd_holds {

	bdmd_reg_holds_addr = 1,  /* can h
                                * addresses */
	bdmd_reg_holds_data = 2,  /* can hold
                                *  data */
	bdmd_reg_holds_either = 4 /* can hold
                                * either data
                                * or addresses */

};

/* "bdmd_reg_span" structure - the part of
 * a larger ("parent") register that a register
 * may represent, for processor types in which
 * register values may overlap in thsu way, such
 * as the Intel i386 family.
 *
 * This information consists of the offset of
 * the start offset (within the parent) and the
 * number of bits (also in the parent) of the
 * span */
struct bdmd_reg_span {

    int start, len; /* number of the register
				     * is which this span
                     * is a a part, and
                     * the bit start posi-
                     * tion and length of
                     * the span */
};

/* "bdmd_reg" structure - information relating
 * to a single register of a processor, including
 * its size, type, the other regsiter of its
 * pair (for paired registers) and the type of
 * data it can hold etc */
struct bdmd_reg {

    char *name;                 /* string name of
                                 * register */
    enum bdmd_types type;       /* type of
				                 * register */
    int sz;                     /* bit size */
    enum bdmd_holds holds;      /* data type
				                 * register can
				                 * hold */
    struct bdmd_reg *parent;    /* parent register
                                 * number o this
                                 * register (NULL
                                 * if no parent*/
    struct bdmd_reg_span *span; /* span of this
                                 * register (within
                                 * its osrent) */
};

/* "bdmd_init_fn" - proto-type of the function called
 * to populate a macgine dcruptor given in the first
 * parameter with its constituting intermediate code
 * to assembly language and assembly language to
 * machine code translations */
typedef int (* bdmd_init_fn)(struct bdmd_md *);

/* "bdmd_emit_fn" - proto-type of the function called
 * to output the byte (given in the second parameter)
 * of machine code of the machine dcruptor given in
 * thefirst parameter, passingthe opaque value in the
 * third parameter */
typedef int (* bdmd_emit_fn)(struct bdmd_md *,
                             char,
                             void *);

/* "bdmd_cstm_fn" - proto-type of the custom"
 * function that sets the mchine code bytes,
 * the number of which is set in the second
 * parameter, corresponding to the assembly
 * language instruction read from the buffer
 * given in the third parameter.  The fourth
 * parameter contains the byte offset at
 * which the resulting machine code starts
 * (before the the instruction is output) */
typedef int (* bdmd_cstm_fn)(struct bdmd_md *,
							 int *,
							 char *,
							 int);

/* "bdmd_md" structure - a "machine definition" for
 * representing  a particular type of processsor, the
 * the constututing information includes reguster desc-
 * riptions, tranlations of patterns of imtermediate
 * language to assenbly laguage instructions and trans-
 * lation from assembly smgugae instructions   to
 * machine codes */
struct bdmd_md {

	struct bd_map_node *regs; /* the registers
                               * of in deccri-
                               * ptor  */
	struct bd_map_node *ics;  /* translations
                               * from patterns of
                               * intermediate
                               * codes to this
                               * architecture's
                               * assembly language */
    struct bd_map_node *mcs;  /* translations from
                               * assembly language
                               *  to each's corr-
                               * esponding machine
                               * code */
};

/*** function proto-types ***/

/* "bdmd_init" - returns a machine decriptior
 * by populated with machine code and assem-
 * bly language translation patterns specified
 * in th nitialisation routinr givrn n he third
 * parameter,  using the memory allocator and
 * error logger given in the first and second
 * parameters, respectively */
int bdmd_init(struct bd_allocs *,
              struct bd_logger *,
              bdmd_init_fn);

/* "bdmd_add_ic_pat" - adds the intermediate
 * code-to-ssembly language translation pat-
 * tern whose intermediate code pattern and
 * corresponding assembly language pattern
 * are given in the second and third paramet-
 * ers, respectively */
 int bdmd_add_ic_pat(struct bdmd_md *,
                    char *,
                    char *);

/* "bdmd_add_mc_pat" - adds the assembly lan-
 * guage-to-machine code translation pattern
 * whose assembly language pattern and corr-
 * sponding machine code pattern are given
 * in the second and third parameters, resp-
 * ectively */
int bdmd_add_mc_pat(struct bdmd_md *,
                    char *,
                    char *);
                

#ifdef __cplusplus
}
#endif

#endif
