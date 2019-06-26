#pragma once



#include "mana.h"



#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <ctype.h>

#include <vec.h>
#include <fileu.h>
#include <upool.h>




#ifdef ARYLEN
# undef ARYLEN
#endif
#define ARYLEN(a) (sizeof(a) / sizeof((a)[0]))




#ifdef max
# undef max
#endif
#ifdef min
# undef min
#endif
#define max(a,b) ((a) > (b) ? (a) : (b))
#define min(a,b) ((a) < (b) ? (a) : (b))




#define zalloc(sz) calloc(1, sz)



static char* stzncpy(char* dst, char const* src, u32 len)
{
    assert(len > 0);
#ifdef _WIN32
    char* p = _memccpy(dst, src, 0, len - 1);
#else
    char* p = memccpy(dst, src, 0, len - 1);
#endif
    if (p) --p;
    else
    {
        p = dst + len - 1;
        *p = 0;
    }
    return p;
}





static u32 align(u32 x, u32 a)
{
    return (x + a - 1) / a * a;
}

















































