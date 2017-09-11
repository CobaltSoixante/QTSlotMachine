#include "CompileTests.h"

/*static*/ map<string, /*const*/EgmSymbol> CompileTests::s_commonReelsMap; // Repository for symbols of ALL reels - so we avoid redundant duplicate images.
CompileTests::CompileTests(void)
{
    string("");
    string();
    string()+"";
    //CompileTests::s_commonReelsMap[""] = EgmSymbol("fileName"); // req's default constructor: can't use.
    typedef pair <string, EgmSymbol> EgmSymbolMapPair;
    CompileTests::s_commonReelsMap.insert(EgmSymbolMapPair("fileName", EgmSymbol("fileName")));
    map<string, /*const*/EgmSymbol>::iterator symIter = CompileTests::s_commonReelsMap.find("fileName");
    EgmSymbol egmSymbol = (*symIter).second;
    //egmSymbol = CompileTests::s_commonReelsMap[""];  // req's default constructor: can't use.
}

CompileTests::~CompileTests(void)
{
}
