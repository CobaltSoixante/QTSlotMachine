#pragma once

#include "EgmSymbol.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

/*
    A reel-combination is a collection of symbols that can appear in a given reel of the EGM.
*/
class EgmReelCombination
{
public:
    EgmReelCombination(const vector<string>& combinationFileList);
    virtual ~EgmReelCombination(void);

public:
    // Make it look (by faking it, a bit) as if this class is a vector of EgmSymbol :
    inline size_t size(void) const {return m_thisReelsSymbols.size();}
    inline const EgmSymbol& operator[](int index) const {
        return *m_thisReelsSymbols[index];
    }

private:
    typedef pair<string, EgmSymbol> t_symbolsRepositoryPair;
    typedef map<string, /*const*/ EgmSymbol> t_symbolsRepository; // Repository fot symbols of ALL reels - so we avoid redundant duplicate images. // having "const" in front of the value simply doesn't work: dunno why: PROBABLY because STL collection terms must be assignable.
    static t_symbolsRepository s_symbolsRepository;

private:
    vector<const EgmSymbol*> m_thisReelsSymbols; // All terms here are pointers to terms already present in "s_commonReelsMap".  // I wanted them to be references, but that doesn't work/compile: explanation in http://stackoverflow.com/questions/922360/why-cant-i-make-a-vector-of-references .
};
