/***********************************************************************
 *                                                                      *
 *               This software is part of the ast package               *
 *          Copyright (c) 1985-2011 AT&T Intellectual Property          *
 *                      and is licensed under the                       *
 *                 Eclipse Public License, Version 1.0                  *
 *                    by AT&T Intellectual Property                     *
 *                                                                      *
 *                A copy of the License is available at                 *
 *          http://www.eclipse.org/org/documents/epl-v10.html           *
 *         (with md5 checksum b35adb5213ca9657e911e9befb180842)         *
 *                                                                      *
 *              Information and Software Systems Research               *
 *                            AT&T Research                             *
 *                           Florham Park NJ                            *
 *                                                                      *
 *               Glenn Fowler <glenn.s.fowler@gmail.com>                *
 *                    David Korn <dgkorn@gmail.com>                     *
 *                     Phong Vo <phongvo@gmail.com>                     *
 *                                                                      *
 ***********************************************************************/
#include "config_ast.h"  // IWYU pragma: keep

#include <sys/types.h>

#include "sfhdr.h"  // IWYU pragma: keep
#include "sfio.h"

/*      Write out an unsigned long value in a portable format.
**
**      Written by Kiem-Phong Vo.
*/
#define N_ARRAY (2 * sizeof(Sfulong_t))

int _sfputm(Sfio_t *f, Sfulong_t v, Sfulong_t m) {
    uchar *s, *ps;
    ssize_t n, p;
    uchar c[N_ARRAY];
    SFMTXDECL(f)

    SFMTXENTER(f, -1)

    if (v > m || (f->mode != SF_WRITE && _sfmode(f, SF_WRITE, 0) < 0)) SFMTXRETURN(f, -1)
    SFLOCK(f, 0)

    /* code v as integers in base SF_UBASE */
    s = ps = &(c[N_ARRAY - 1]);
    *s = (uchar)SFBVALUE(v);
    while ((m >>= SF_BBITS) > 0) {
        v >>= SF_BBITS;
        *--s = (uchar)SFBVALUE(v);
    }
    n = (ps - s) + 1;

    if (n > 8 || SFWPEEK(f, ps, p) < n) {
        n = SFWRITE(f, (void *)s, n); /* write the hard way */
    } else {
        switch (n) {
            case 8:
                *ps++ = *s++;
            // FALLTHRU
            case 7:
                *ps++ = *s++;
            // FALLTHRU
            case 6:
                *ps++ = *s++;
            // FALLTHRU
            case 5:
                *ps++ = *s++;
            // FALLTHRU
            case 4:
                *ps++ = *s++;
            // FALLTHRU
            case 3:
                *ps++ = *s++;
            // FALLTHRU
            case 2:
                *ps++ = *s++;
            // FALLTHRU
            case 1:
                *ps++ = *s++;
            // FALLTHRU
            default:;  // EMPTY BLOCK
        }
        f->next = ps;
    }

    SFOPEN(f)
    SFMTXRETURN(f, (int)n)
}
