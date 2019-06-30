#include "mana_a.h"







typedef vec_t(MANA_TokSrcInfo) MANA_TokSrcInfoVec;


typedef struct MANA_SpaceSrcInfo
{
    vec_u32 fileBases[1];
    MANA_TokSrcInfoVec toks[1];
} MANA_SpaceSrcInfo;

MANA_SpaceSrcInfo* MANA_spaceSrcInfoNew(void)
{
    MANA_SpaceSrcInfo* srcInfo = (MANA_SpaceSrcInfo*)zalloc(sizeof(*srcInfo));
    return srcInfo;
}

void MANA_spaceSrcInfoFree(MANA_SpaceSrcInfo* srcInfo)
{
    vec_free(srcInfo->toks);
    vec_free(srcInfo->fileBases);
    free(srcInfo);
}

const MANA_TokSrcInfo* MANA_tokSrcInfo(const MANA_SpaceSrcInfo* srcInfo, u32 tokIdx)
{
    return srcInfo->toks->data + tokIdx;
}









typedef struct MANA_Lexing
{
    MANA_Space* space;
    u32 srcLen;
    const char* src;
    u32 cur;
    u32 curLine;
    const MANA_LexingOpt* opt;
    MANA_SpaceSrcInfo* srcInfo;
    vec_char tmpStrBuf[1];
} MANA_Lexing;

static MANA_Lexing MANA_lexingNew
(
    MANA_Space* space, u32 srcLen, const char* src, const MANA_LexingOpt* opt, MANA_SpaceSrcInfo* srcInfo
)
{
    if (srcInfo)
    {
        vec_push(srcInfo->fileBases, MANA_spaceToksTotal(space));
    }
    MANA_Lexing ctx[1] =
    {
        { space, srcLen, src, 0, 1, opt, srcInfo }
    };
    return *ctx;
}

static void MANA_lexingFree(MANA_Lexing* ctx)
{
    vec_free(ctx->tmpStrBuf);
}







static bool MANA_skipSapce(MANA_Lexing* ctx)
{
    const char* src = ctx->src;
    for (;;)
    {
        if (ctx->cur >= ctx->srcLen)
        {
            return false;
        }
        else if (' ' >= src[ctx->cur])
        {
            if ('\n' == src[ctx->cur])
            {
                ++ctx->curLine;
            }
            ++ctx->cur;
            continue;
        }
        else if (ctx->cur + 1 < ctx->srcLen)
        {
            if (0 == strncmp("//", src + ctx->cur, 2))
            {
                ctx->cur += 2;
                for (;;)
                {
                    if (ctx->cur >= ctx->srcLen)
                    {
                        return false;
                    }
                    else if ('\n' == src[ctx->cur])
                    {
                        ++ctx->cur;
                        ++ctx->curLine;
                        break;
                    }
                    ++ctx->cur;
                }
                continue;
            }
            else if (0 == strncmp("/*", src + ctx->cur, 2))
            {
                ctx->cur += 2;
                u32 n = 1;
                for (;;)
                {
                    if (ctx->cur >= ctx->srcLen)
                    {
                        return false;
                    }
                    else if (ctx->cur + 1 < ctx->srcLen)
                    {
                        if (0 == strncmp("/*", src + ctx->cur, 2))
                        {
                            ++n;
                            ctx->cur += 2;
                            continue;
                        }
                        else if (0 == strncmp("*/", src + ctx->cur, 2))
                        {
                            --n;
                            if (0 == n)
                            {
                                ctx->cur += 2;
                                break;
                            }
                            ctx->cur += 2;
                            continue;
                        }
                    }
                    else if ('\n' == src[ctx->cur])
                    {
                        ++ctx->curLine;
                    }
                    ++ctx->cur;
                }
                continue;
            }
        }
        break;
    }
    return true;
}









static MANA_TokSrcInfo MANA_lexingTokSrcInfo(MANA_Lexing* ctx, u32 begin)
{
    assert(ctx->srcInfo);
    assert(ctx->srcInfo->fileBases->length > 0);
    MANA_TokSrcInfo tokInfo[1] = { 0 };
    tokInfo->file = ctx->srcInfo->fileBases->length - 1;
    tokInfo->offset = begin;
    tokInfo->line = ctx->curLine;
    tokInfo->column = 1;
    for (u32 i = 0; i < tokInfo->offset; ++i)
    {
        char c = ctx->src[tokInfo->offset - i];
        if (strchr("\n\r", c))
        {
            tokInfo->column = i;
            break;
        }
    }
    return *tokInfo;
}










static u32 MANA_lexingNextToken(MANA_Lexing* ctx)
{
    MANA_Space* space = ctx->space;
    const MANA_LexingOpt* opt = ctx->opt;
    MANA_SpaceSrcInfo* srcInfo = ctx->srcInfo;
    vec_char* tmpStrBuf = ctx->tmpStrBuf;

    const char* src = ctx->src;
    if (ctx->cur >= ctx->srcLen)
    {
        return -1;
    }
    if (!MANA_skipSapce(ctx))
    {
        return -1;
    }

    u32 begin = ctx->cur;
    if (('"' == src[ctx->cur]) || ('\'' == src[ctx->cur]))
    {
        char endCh = src[begin];
        ++ctx->cur;
        for (;;)
        {
            if (ctx->cur >= ctx->srcLen)
            {
                return false;
            }
            else if (endCh == src[ctx->cur])
            {
                break;
            }
            else if ('\\' == src[ctx->cur])
            {
                ctx->cur += 2;
                continue;
            }
            else
            {
                if ('\n' == src[ctx->cur])
                {
                    ++ctx->curLine;
                }
                ++ctx->cur;
            }
        }
        ++ctx->cur;
        {
            vec_resize(tmpStrBuf, 0);
            const char* src = ctx->src + begin + 1;
            for (u32 i = 0; i < ctx->cur - begin - 2; ++i)
            {
                if ('\\' == src[i])
                {
                    ++i;
                    vec_push(tmpStrBuf, src[i]);
                    continue;
                }
                vec_push(tmpStrBuf, src[i]);
            }
        }
        if (srcInfo)
        {
            MANA_TokSrcInfo tokSrcInfo = MANA_lexingTokSrcInfo(ctx, begin);
            vec_push(srcInfo->toks, tokSrcInfo);
        }
        return MANA_tokNewByCstr(space, tmpStrBuf->data, MANA_TokFlag_Quoted);
    }
    else
    {
        for (;;)
        {
            if (ctx->cur >= ctx->srcLen)
            {
                break;
            }
            else if (strchr(opt->aloneChars, src[ctx->cur]))
            {
                if (0 == (ctx->cur - begin))
                {
                    ++ctx->cur;
                    break;
                }
                else
                {
                    break;
                }
            }
            else if (strchr("\"' \t\n\r\b\f", src[ctx->cur]))
            {
                break;
            }
            else
            {
                ++ctx->cur;
            }
        }
        if (srcInfo)
        {
            MANA_TokSrcInfo tokSrcInfo = MANA_lexingTokSrcInfo(ctx, begin);
            vec_push(srcInfo->toks, tokSrcInfo);
        }
        return MANA_tokNewByBuf(space, src + begin, ctx->cur - begin, 0);
    }
}












void MANA_lexing(MANA_Space* space, const char* src, const MANA_LexingOpt* opt, MANA_SpaceSrcInfo* srcInfo)
{
    MANA_Lexing ctx[1] = { MANA_lexingNew(space, (u32)strlen(src), src, opt, srcInfo) };
    while (MANA_skipSapce(ctx))
    {
        u32 tokId = MANA_lexingNextToken(ctx);
        if (-1 == tokId)
        {
            break;
        }
    }
    MANA_lexingFree(ctx);
}





























































