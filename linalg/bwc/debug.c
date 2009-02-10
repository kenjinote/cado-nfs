#define _POSIX_C_SOURCE 200112L
#define _GNU_SOURCE

#include <stdio.h>
#include <stdarg.h>
#include "debug.h"

void debug_write(abobj_ptr abase, const abt * v,
        unsigned int n, const char * fmt, ...)
{
    char * tmp;
    va_list ap;

    va_start(ap, fmt);
    int rc = vasprintf(&tmp, fmt, ap);
    FILE * f = fopen(tmp, "w");
    rc = fwrite(v, sizeof(abt), aboffset(abase, n), f);
    fclose(f);
    va_end(ap);
}

