/********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* opt.h - function lmplementations- rela-
* ted to generating s stream of s binary,
* architecture specific, stream of execu-
* table, binary, machine code bytes from
* a stream of intermediate code.
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
* Copyright (C) 2007-2013, Russell Potter
* All rights reserved
*
*********************************************************************/

#include <bikedemo/ic.h>
#include <bikedemo/map.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>
#include <stdlib.h>

struct bdmd_md;

/* "bdgn_gnrte" - returns the machine code corr-
 * esponding to the intermediate code list given
 * in "cdes" for the machine descriptor given
 * in "md", using the memory allocator and error
 * logger given in "allocs" and "loggger", resp-
 * ectively.  Returns NULL on error */
struct bdut_bytes *bdgn_gnrte(struct bdic_icdes *cdes,
                              struct bdmd_md *md,
                              struct bd_allocs *allocs,
                              struct bd_logger *logger)
{
    return NULL;
}

/* "bdgn_vrfy" - returns non-zero if the mach-
 * ine definition in "md" is able to generate
 * the machine code byte offsets corresponding
 * to each of the set the intermediate code
 * offsets given in "offs" using the memory all-
 * ocator and error logger given in "allocs"
 * and "logger", respectively.  Returns a neg-
 * ative value on error */
int bdgn_vrfy(struct bd_map_node *offs,
              struct bdmd_md *md,
              struct bd_allocs *allocs,
              struct bd_logger *logger)
{
    return 0;

}
