#include "mana_a.h"







typedef vec_t(MANA_TokSrcInfo) MANA_TokSrcInfoVec;


typedef struct MANA_SpaceSrcInfo
{
    vec_u32 fileBases[1];
    MANA_TokSrcInfoVec toks[1];
} MANA_SpaceSrcInfo;

static MANA_SpaceSrcInfo* MANA_spaceSrcInfoNew(void)
{
    MANA_SpaceSrcInfo* srcInfo = (MANA_SpaceSrcInfo*)zalloc(sizeof(*srcInfo));
    return srcInfo;
}

static void MANA_spaceSrcInfoFree(MANA_SpaceSrcInfo* srcInfo)
{
    vec_free(srcInfo->toks);
    vec_free(srcInfo->fileBases);
}

static const MANA_TokSrcInfo* MANA_tokSrcInfo(MANA_SpaceSrcInfo* srcInfo, u32 tokIdx)
{
    return srcInfo->toks->data + tokIdx;
}












void MANA_lexing(const MANA_Space* space, const char* src, const MANA_LexingOpt* opt, MANA_SpaceSrcInfo* srcInfo)
{

}





























































