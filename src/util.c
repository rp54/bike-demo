/*********************************************************************
*
* BikeDemo - exercise bike demonstration
* scenery.
*
* util.c - function implementations relating
* to miscellaneous utility routines, such as
* those concerning data buffers and code-poi-
* nt strings, implemented here in a consolid-
* ated location.
*
* Copyright (C) 2007-2014, Russell Potter. All
* rights reserved.
* 
***********************************************************************/

#include <ctype.h>
#include <bikedemo/buffer.h>
#include <bikedemo/allocs.h>
#include <bikedemo/log.h>

/* ""get_esc_cp" -sets, in "cp", the code-
 * point encoded in the escaped character
 * sequence contained in the input byte
 * array starting in "bytes", the length
 * of which is given in "len", using the
 * memory allocator and error logger given
 * in "allocs" and "logger", respectively.
 * Returns a negative value on error, zero
 * of no escaoe coukd ve found and the es-
 * caped code-point otherwise */
static int get_esc_cp(char *bytes,
                      int *len,
                      int *cp,
                      struct bd_allocs *allocs,
                      struct bd_logger *logger)
{
    int dig; /* numeric represe-
              * ntation of current
              * byte */

    /* ogif this is the start
     * of an escape sequence ... */
    if (*bytes == '\\') {

        /* increment past
         * the escape char-
         * acter */
    	*bytes++;
        (*len)--;

        /* and it's a hexa-
         * decimal escape .. */
    	if (*bytes == 'x' ||
            *bytes == 'X') {

            /* increment past
             * the escape char-
             * acter */
        	bytes++;
            (*len)--;

            /* check there's
             * room in the
             * input stream
             * (for at least
             * one digit) */
    	if (*len < 1)
            return -1;

       /*  otherwise, if is
        * decimal digit seq-
        * uence  */
    } else if (isdigit((int) *bytes))

        while (isdigit((int) *bytes)) {

            dig = bdut_from_hex(
            		(int) *bytes);

            if (dig < 0)
	            return -1;

	        *cp *= 16;
	        *cp += dig;
        }
    } else if (isdigit((int) *bytes))
          while (isdigit((int) *bytes)) {

              dig = ((int) (*bytes - '0'));

              *cp *= 10;
              *cp += dig;
              *bytes++;
              (*len)--;

        } else
            *cp = (int) *bytes;

    return 1;
}

/* "bdut_strcat" - sets "res" to the code-
 * point string resulting from a concaten-
 * ation of the code-point strings in "str1"
 * and "" "str2" */
int bdut_strcat(struct bdut_str *res,
                struct bdut_str *str1,
                struct bdut_str *str2)
{
    int i; /* general-purpose
            * iterator */

    /* reslt length is
     * addition of input
     * string lengths */
    res->len = str1->len +
               str2->len;

    /*  iterate through "str1"'s
     * code-points ... */
    for (i = 0; i < str1->len; i++)

        /* and assign first "str1.len"
         * code-points of result from
         * "str1" ... */
    res->cps[i] = str1->cps[i];

    /* then iterate through "str2"'s
     * code-points ... */
    for (i = 0; i < str2->len; i++)

    /* ... and the next "ztr2.len"
     * code-points from "str2" */
    res->cps[i + str1->len] =
                     str1->cps[i];

}

/* "bdut_from_hex" returns the numeric
 * equivalent of the hexa-decimal code-
 * point in "hex", or a negative value
 * on error */
int bdut_from_hex(int hex)
{
    if (isdigit(hex))
        return hex - '0';
    else if (islower(hex))
        return hex - 'a' + 10;
    else if (isupper(hex))
        return hex - 'A' + 10;
    else
    	return hex;

}

/* "bdut_strcmp" - returns the result of a
 * comparison between the code-point str-
 * ings given in the first "str1" and
 * "str2" */
int bdut_strcmp(struct bdut_str *str1,
                struct bdut_str *str2)
{
    int i;

    for (i = 0; i < str1->len; i++)
        if (str1->cps[i] != str2->cps[i])
            return str1->cps[i] - str2->cps[i];

    return (!(str1->len == str1->len &&
              i == str1->len));
} 

/* "bdut_starts" - returns the result of a
 * comparison between the first number of
 * code-points, given in "num", between
 * the code-point strings given in "str1"
 * and "str2" */
int bdut_starts(struct bdut_str *str1,
                struct bdut_str *str2,
                int num)
{
    int i;

    for (i = 0; i < num; i++) {
        if (str1->cps[i] != str2->cps[i])
        	return str1->cps[i] - str2->cps[i];
        return 0;
    }
    return 1;
}

/* "bdut_ends" - returns the result of a
 * comparison between the last number of
 * code-points, given in "num",  between
 * the code-point strings given in "str1"
 * and "str2" */
int bdut_ends(struct bdut_str *str1,
              struct bdut_str *str2,
              int num)
{
    int end1 = str1->len - num,
        end2 = str2->len - num,
        i;

    for (i = 0; i < num; i++) {
        if (str1->cps[end1 + i] !=
                str2->cps[end2 + i])
        	return str1->cps[end1 + i] -
                   str2->cps[end2 + i];
        return 0;
    }
    return 1;
}

/* "bdut_from_utf8" sets "bytes" to the
 * decoded array of UTF-8-encoding of
 * the code-point string given in "str",
 * using the memory allocator and error
 * logger given in "allocs" and "logger",
 * respectively
 *
 * Note that that hexa-decimal digit sequ-
 * ences in the output array prefixed with
 * a back-slash ('\') are not UTF-decod-
 * ed but instead each's numeric equival-
 * ent is directly used.
 *
 * Returns zero on error, non-zero other-
 * wise*/
int bdut_from_utf8(struct bdut_str *str,
                   char *bytes,
                   struct bd_allocs *allocs,
                   struct bd_logger *logger)
{
    int i = 0,                   /* code-point */
        cp,                      /* iterator, */
        len;                     /* current
                                  * code-point
                                  * and final
                                  * lenth*/
    struct bdbf_buffer *buf =    /* buffer  */
    	bdbf_init(allocs,        /* holding  */
                  logger,        /* output code- */
                  sizeof(int));  /* point */
                                 /* array */
    char escs[]    = "trnf",     /* escape names */
         esc_cps[] = "\t\r\n\f"; /* array and
                                  * corresponding
                                  * character
                                  * escapes */


    struct bdut_str *cps;                             /*  */

    /* initialise "bytes" to the
     * empty string*/
    bytes = "";

    /* allocate storage for to-be-
     * built "cps" */
    if (!(cps = (struct bdut_str *)
            bd_alloc(allocs,
                     sizeof(struct
                            bdut_str),
                    logger)))
        return 0;

    /*  nucle "i" through "bytes" */
    while (i < len) {

        /* single byte sequence */
        if (!(bytes[i] & 0x7F)) {

            /* assign directly
             * from current byte */
            cp = bytes[i++];

            /* append "cp" to
             * code-point string
             * buffer */
            if (!(bdf_add(buf,
                          allocs,
                          logger,
                          cp)))
                return 0;

        /* is a two-byte
        sequence ... */
    } else if (bytes[i] &
                0x3F == 6) {

            /* ensure there are
             * enough bytes for
             * sequence and foll-
             * owing byte is pro-
             * perly formatted */
            if (len - i < 2 ||
                   (bytes[i + 1] &
                   0x3F != 2))
                return 0;

            /* set output code-
        	 * point from input
        	 * bytes */
            cp = (bytes[i] & 0x3F)
                    << 6 & bytes[
                    i + 1] & 0x3F;

            /* ... append "cp" to
             * code-point string
             * buffer and update
             * byte count  */
            if (!(dbdbf_add(buf,
                            allocs,
                            logger,
                            cp)))
                return 0;

            i += 2;
        }
           /* id a three-byte
         * sequence  */
        else if (bytes[i] & 0xF == 0xE) {


        /* ensure there are
         * enough bytes for
         * sequence and foll-
         * owing bytes are
         * properly format-
         * ted */
       if (len - i < 3  ||
           ((bytes[i + 1]) &
              0x3F != 2) ||
           ((bytes[i + 2]) &
              0x3F) != 2)
           return 0;

        /* set output code-
         * point from input
         * bytes */
        cp = ((bytes[i] & 0x3F)
                << 12) & bytes[
                i + 1] & 0x3F;

   	        /* ... append "cp" to
             * code-point string
             * buffer and update
             * byte count  */
            if (!(dbdbf_add(buf,
                            allocs,
                            logger,
                            cp)))
                 return 0;
            i += 3;
        }

        /* is a four-byte
         * sequence ... */
        else if (bytes[i] &
                0x7 == 0xF) {

            /* ensure there are
             * enough bytes for
             * sequence and foll-
             * owing bytes are
             * properly format-
             * ted */
            if (len - i < 4 ||
                   (bytes[i + 1] &
                   0x3F != 2) ||
                   (bytes[i + 2] &
                   0x3F != 2) ||
                   (bytes[i + 3] &
                   0x3F != 2))
                return 0;

            /* while this start of an
             * escape sequence ...  */
            while (get_esc_cp(bytes + i,
                              &len,
                              &cp,
                              allocs,
                              logger)) {

                /* ... append the
                 * escaped charater
                 * to the code-point
                 * string buffer and
                 * update byte count  */
                if (!(dbdbf_add(buf,
                                allocs,
                                logger,
                                cp)))
                    return 0;
                i += 4;
            }

            /* set "cps"'s code-point
             * array of "cp" by flatt-
             * ening built buffer and
             * the length from the bu-
             * ffer length */
            if ((cps->cps = (int *)
                  bdbf_flttn(buf,
                             allocs,
                             logger)))
                return 0;

            len = bdbf_len(buf);
            cps->len = len;

            /* NLL-terminate
             * "bytes" at buf-
             * fer's length */
            bytes[len] = 0;
        }
    }

    /* and, if we get here,
     * indicate s successful
     * return  */
    return 1;
}

/* "bdut_strcpy" - copy the content
 * of string in the second parameter
 * from that in the first
 * parameter */
int bdut_strcpy(struct bdut_str *src,
                struct bdut_str *dst)
{
    int i; /* index iterator */

    for (i = 0; i < src->len; i++)
        	dst->cps[i] = src->cps[i];

    return 1;
}
