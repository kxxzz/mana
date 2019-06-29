#include <stdbool.h>
#include <stdint.h>
#include <assert.h>

#include <vec.h>


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

u32 MANA_tokNewByCstr(MANA_Space* space, const char* str, u32 flags);
u32 MANA_tokNewByBuf(MANA_Space* space, const char* ptr, u32 len, u32 flags);

u32 MANA_tokDataId(const MANA_Space* space, u32 tokIdx);
const char* MANA_tokData(const MANA_Space* space, u32 tokIdx);
u32 MANA_tokDataSize(const MANA_Space* space, u32 tokIdx);
u32 MANA_tokFlags(const MANA_Space* space, u32 tokIdx);





enum
{
    MANA_TokFlag_Quoted = 1U << 0,
};



typedef struct MANA_TokSrcInfo
{
    u32 file;
    u32 offset;
    u32 line;
    u32 column;
} MANA_TokSrcInfo;

typedef struct MANA_SpaceSrcInfo MANA_SpaceSrcInfo;

MANA_SpaceSrcInfo* MANA_spaceSrcInfoNew(void);
void MANA_spaceSrcInfoFree(MANA_SpaceSrcInfo* srcInfo);
const MANA_TokSrcInfo* MANA_tokSrcInfo(MANA_SpaceSrcInfo* srcInfo, u32 tokIdx);


typedef struct MANA_LexingOpt
{
    const char* aloneChars;
} MANA_LexingOpt;

void MANA_lexing(MANA_Space* space, const char* src, const MANA_LexingOpt* opt, MANA_SpaceSrcInfo* srcInfo);































































