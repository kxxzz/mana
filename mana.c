#include "mana_a.h"




typedef vec_t(MANA_TokSrcInfo) MANA_TokSrcInfoVec;



typedef struct MANA_SpaceSrcInfo
{
    vec_u32 fileBases[1];
    MANA_TokSrcInfoVec toks[1];
} MANA_SpaceSrcInfo;

static void MANA_spaceSrcInfoFree(MANA_SpaceSrcInfo* srcInfo)
{
    vec_free(srcInfo->toks);
    vec_free(srcInfo->fileBases);
}











const MANA_TokSrcInfo* MANA_tokSrcInfo(const MANA_Space* space, u32 tokId)
{
    return NULL;
}





















































































