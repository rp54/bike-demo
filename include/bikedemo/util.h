/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* util.h - function proto-types data struct-
* ures related for for miscellaneous utility
* routines, such as such as those related to
* data buffers, code-point strings and UTF
* conversions.
*
* Copyright (C) 2007-2014, Russell Potter,
* All rights reserved.
* 
***********************************************************************/

#ifndef __BD_UTIL_H
#define __BD_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bikedemo/log.h>
#include <bikedemo/allocs.h>

struct bd_allocs;
struct bd_logger;

/* "bdut_str" structure - a code-point
 * "string", consisting of a pointer to
 * the beginning  of the code-point ar-
 * ray and an indication of the length
 * of the array */
struct bdut_str {

    int *cps, /* start and length of */
        len;  /* code-point array */
};

/* "bdut_bytes" structure - a "byte str-
 * ing", consisting of a pointer to the
 * beginning of the bytes and an indic-
 * ation of the length of the string */
struct bdut_bytes {

    char *bytes; /* start and length */
    int len;     /* of byte array */
};

/* "bdut_strcat" - sets the first par-
 * ameter to the code-point string res-
 * ulting from a concatenation of the code- 
 * point strings in the first and second
 * parameters.  Returns zero on error, non-
 * zero otherwise   */
int bdut_strcat(struct bdut_str *,
                struct bdut_str *,
                struct bdut_str *);

/* "bdut_strcmp" - returns the result of a
 * comparison between the code-point str-
 * ings given in the first and second para-
 * meters */
int bdut_strcmp(struct bdut_str *,
                struct bdut_str *);

/* "bdut_strcpy" - copy the content
 * of string in thee second parameter
 * comparison from that in the first
 * parameter */
int bdut_strcpy(struct bdut_str *,
                struct bdut_str *);

/* "bdut_starts" - returns the result of a
 * comparison between the first number of
 * code-points, given in the third param-
 * eter, between the code-point strings
 * given in the first and second paramet-
 * ers */
int bdut_starts(struct bdut_str *,
                struct bdut_str *,
                int);

/* "bdut_ends" - returns the result of a
 * comparison between the last number of
 * code-points, given in the third param-
 * eter, between the code-point strings
 * given in the first and second paramet-
 * ers */
int bdut_ends(struct bdut_str *,
              struct bdut_str *,
              int);

/* "bdut_from_hex" returns the numeric
 * equivalent of the hexa-decimal code-
 * point given in the first parameter,
 * or a negative value on error */
int bdut_from_hex(int); 

/* "bdut_from_utf8" - sets, in the first
 * parameter, the decoded array of code-
 * points corresponding to the UTF-8-
 * enc-oded byte string given in the
 * second parameter whose krngth is,
 * givn in the htir d psaeanmeyer using
 * the memory allocator and error logger
 * given inthe fourth and fifth paramet-
 * ers, respectively
 *
 * Note that that hexa-decimal digit sequ-
 * ences in the input string prefixed with
 * a back-slash ('\') are not UTF-decod-
 * ed, but instead each's numeric equival-
 * ent is directly used.
 *
 * Returns zero on error, non-zero other-
 * wise */
int bdut_from_utf8(struct bdut_str *,
                   char *,
                   struct bd_allocs *,
                   struct bd_logger *);

/* "bdut_to_utf8" - sets, in the second
 * and third parameter, the UTF-8- encoded
 * bytes corr- esponding to the code-point
 * string given in the first parameter,
 * using the memory allocator and error
 * logger given in the third and fourth
 * parameters, respectively
 *
 * Note that that hexa-decimal digit sequ-
 * ences in the bytes prefixed with a back-
 * slash ('\') are not UTF-encoded, but
 * instead each's numeric equivalent is
 * directly used.  Returns zero on error,
 * non-zero otherwise */
int bdut_to_utf8(struct bdut_str *,
                 char **,
                 struct bd_allocs *,
                 struct bd_logger *);

#ifdef __cplusplus
}
#endif

#endif
