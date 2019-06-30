#pragma warning(disable: 4101)

#include "mana.h"



#include <stdlib.h>
#ifdef _WIN32
# include <crtdbg.h>
#endif

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include <fileu.h>




static int mainReturn(int r)
{
#if !defined(NDEBUG) && defined(_WIN32)
    system("pause");
#endif
    return r;
}






static void tests(void)
{
    char* text;

    u32 textSize = FILEU_readFile("../1.mana", &text);
    assert(textSize != -1);

    MANA_Space* space = MANA_spaceNew();
    MANA_SpaceSrcInfo* srcInfo = MANA_spaceSrcInfoNew();

    MANA_LexingOpt opt[1] = { { "()[]{};," } };
    MANA_lexing(space, text, opt, srcInfo);

    for (u32 i = 0; i < MANA_spaceToksTotal(space); ++i)
    {
        const char* str = MANA_tokDataPtr(space, i);
        printf("%s\n", str);
    }

    free(text);
    MANA_spaceSrcInfoFree(srcInfo);
    MANA_spaceFree(space);
}







int main(int argc, char* argv[])
{
#if !defined(NDEBUG) && defined(_WIN32)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    tests();
    return mainReturn(EXIT_SUCCESS);
}






























































