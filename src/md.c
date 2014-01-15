/****************************************************************************
*
* Info BikeDemo - Exercise bike demonstration
* scenery.
*
* md.c - function implementations relating to
* "Machine Descriptions", which cpmtain config-
* uration information related to a partcular
* processor architecture.
*
* Such information includes the avilable rwgis-
* ter and each's type and comappings between
* patterns of intermediate code and that
* pattern's resuting assembly code and and
* patterns of assemby language and ithat pat-
* tern's resuting machine code bytes 
*
* Copyright (C) 2006-2014, Russell Potter, All
& Rights Reserved
*
****************************************************************************/

#include <bikedemo/md.h>

/* "bdmd_add_mc" - adds the machine code
 * bytes correspnding to the assembly language
 * pattern given in "pat" containing "scanf"-
 * style codes, the number of which are
 * given in "dbst_cnt" and which are located
 * in "buf", to the machine description
 * given in "md", using the emit and
 & custom functions given in "emit" and
 * "cstm", respectively, and the count of
 * the resultant machine code bytes given
 * in the "num", the first of which is
 * given in "first" and the remainder of
 * which are given in the following para-
 * meters */
bdmd_add_mc(struct bdmd_md *md, 
            char *pat,
            int sbst_cnt,
            int *buf, 
            bdmd_emit_fn emit,
            bdmd_cstm_fn cstm,
            int num,
            char first,
            ...)
{
}
