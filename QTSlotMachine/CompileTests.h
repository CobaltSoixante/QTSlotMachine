#pragma once

#include "EgmSymbol.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

class CompileTests
{
public:
    CompileTests(void);
    virtual ~CompileTests(void);

private:
    static map<string, /*const*/EgmSymbol> s_commonReelsMap; // having "const" in front of the value simply doesn't work: dunno why.
    //typedef vector<const EgmSymbol/*&*/> t_myMap; // Having &(reference) for the terms simply doesn't work - dunno why.
    //t_myMap m_thisReelsSymbols; // Having &(reference) for the terms simply doesn't work - dunno why.
    //vector<const EgmSymbol/*&*/> m_thisReelsSymbols; // Having &(reference) for the terms simply doesn't work - dunno why.
};
