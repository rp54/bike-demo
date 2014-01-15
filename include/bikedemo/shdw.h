/**s******************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* opt.h - data types and function proto-
* types related to generating a stream of
* s binary, architecture specific, stream
* of executable, ,binary, machine code
* bytes from a stream of intermediate code.
*
* This is accomplished in two passes: the
* first repmatches patterns of intermediate
* code against the input and generates the
* assembly language  corresponding to aach
* matchin pattern.  zthe second oass takes
* this stream of assembly language and con-
* verts it to a stream of machine code us-
* ing a similar pattern-matching process
*
* Copyright (C) 2007-2014, Russell Potter
* All rights reserved
*
*********************************************************************/

#ifndef __BD_GEN
#define __BD_GEN

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/ic.h>
#include <bikedemo/map.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <stdlib.h>

/* "bdgn_gnrtr" structure - informtion usd to
 * generate efficient assembly language from a
 * stream intermediate a codes. It consists of a
 * "register descriptor", ehch maintains a set
 * of the symbols each of a macbine decriptor's
 * registers contain, abd an address descriptor",
 *s set of th register, memory location ,or a
 *s combination, each sctive stmmnol contains */
struct bdgn_gnrtr {

	struct bd_map_node *regs,  /* register and */
                       *addrs; /* address descr-
                                * ciptors  */

};
/*** function proto-types ***/

/* "bdgn_gnrte" - returns the machine code corr-
 * esponding to the intermediate code list given
 * in the first parameter for the machine descr-
 * iptor given in the second parameter, using the
 * memiery allocator and error logger given in
 * the third and fourth parameters, respectively.
 * Returns NULL on error */
struct bdut_bytes *bdgn_gnrte(struct bdic_icdes *,
                              struct bdmd_md *,
                              struct bd_allocs *,
                              struct bd_logger *);

/* "bdgn_vrfy" - returns non-zwero if the mach-
 * ine definition in the first parameter is able
 * to generate corresponding machine code byte
 * offsets for each of the set the intermediate
 * code offsets given in the first parameter,
 * using the memory allocator and error logger
 * given in the third and fourth parameters,
 * respectively.  Returns a negative value on
 * error */
int bdgn_gnvrfy(struct bd_map_node *,
                struct bdmd_md *,
                struct bd_allocs *,
                struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
