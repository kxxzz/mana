#include <stdbool.h>
#include <stdint.h>
#include <assert.h>



typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef float f32;
typedef double f64;




typedef struct MANA_Space MANA_Space;

MANA_Space* MANA_spaceNew(void);
void MANA_spaceFree(MANA_Space* space);


u32 MANA_spaceToksTotal(const MANA_Space* space);


typedef struct MANA_Tok { u32 id; } MANA_Tok;

static MANA_Tok MANA_Tok_Invalid = { (u32)-1 };


MANA_Tok MANA_tokFromCstr(MANA_Space* space, const char* str, u32 flags);
MANA_Tok MANA_tokFromBuf(MANA_Space* space, const char* ptr, u32 len, u32 flags);


u32 MANA_tokSize(const MANA_Space* space, MANA_Tok tok);
u32 MANA_tokDataId(const MANA_Space* space, MANA_Tok tok);
const char* MANA_tokData(const MANA_Space* space, MANA_Tok tok);
u32 MANA_tokFlags(const MANA_Space* space, MANA_Tok tok);




typedef struct MANA_TokSrcInfo
{
    u32 file;
    u32 offset;
    u32 line;
    u32 column;
} MANA_TokSrcInfo;

const MANA_TokSrcInfo* MANA_tokSrcInfo(const MANA_Space* space, MANA_Tok tok);




































































