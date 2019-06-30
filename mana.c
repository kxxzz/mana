#include "mana_a.h"





MANA_Space* MANA_spaceNew(void)
{
    MANA_Space* space = zalloc(sizeof(*space));
    space->dataPool = upool_new(256);
    return space;
}

void MANA_spaceFree(MANA_Space* space)
{
    vec_free(space->tmpBuf);
    upool_free(space->dataPool);
    vec_free(space->toks);
    free(space);
}



u32 MANA_spaceToksTotal(const MANA_Space* space)
{
    return space->toks->length;
}


void MANA_spaceToksReset(MANA_Space* space)
{
    vec_resize(space->toks, 0);
}







const char* MANA_spaceDataPtrByCstr(MANA_Space* space, const char* str)
{
    u32 len = (u32)strlen(str);
    u32 off = upool_elm(space->dataPool, str, len + 1, NULL);
    return upool_elmData(space->dataPool, off);
}

const char* MANA_spaceDataPtrByBuf(MANA_Space* space, const char* ptr, u32 len)
{
    vec_resize(space->tmpBuf, len + 1);
    memcpy(space->tmpBuf->data, ptr, len);
    space->tmpBuf->data[len] = 0;
    u32 off = upool_elm(space->dataPool, space->tmpBuf->data, len + 1, NULL);
    return upool_elmData(space->dataPool, off);
}



u32 MANA_tokNewByCstr(MANA_Space* space, const char* str, u32 flags)
{
    u32 len = (u32)strlen(str);
    u32 offset = upool_elm(space->dataPool, str, len + 1, NULL);
    MANA_TokInfo info = { offset, len, flags };
    u32 tokId = space->toks->length;
    vec_push(space->toks, info);
    return tokId;
}

u32 MANA_tokNewByBuf(MANA_Space* space, const char* ptr, u32 len, u32 flags)
{
    vec_resize(space->tmpBuf, len + 1);
    memcpy(space->tmpBuf->data, ptr, len);
    space->tmpBuf->data[len] = 0;
    u32 offset = upool_elm(space->dataPool, space->tmpBuf->data, len + 1, NULL);
    MANA_TokInfo info = { offset, len, flags };
    u32 tokId = space->toks->length;
    vec_push(space->toks, info);
    return tokId;
}

const char* MANA_tokDataPtr(const MANA_Space* space, u32 tokIdx)
{
    assert(tokIdx < space->toks->length);
    MANA_TokInfo* info = space->toks->data + tokIdx;
    return upool_elmData(space->dataPool, info->offset);
}

u32 MANA_tokDataSize(const MANA_Space* space, u32 tokIdx)
{
    assert(tokIdx < space->toks->length);
    MANA_TokInfo* info = space->toks->data + tokIdx;
    return info->length;
}

u32 MANA_tokFlags(const MANA_Space* space, u32 tokIdx)
{
    assert(tokIdx < space->toks->length);
    MANA_TokInfo* info = space->toks->data + tokIdx;
    return info->flags;
}









































































