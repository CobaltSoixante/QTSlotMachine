#include "EgmReelCombination.h"

/*static*/ EgmReelCombination::t_symbolsRepository EgmReelCombination::s_symbolsRepository; // Repository fot symbols of ALL reels - so we avoid redundant duplicate images. // having "const" in front of the value simply doesn't work: dunno why: PROBABLY because STL collection terms must be assignable.

EgmReelCombination::EgmReelCombination(const vector<string>& combinationFileList)
{
    for (std::vector<string>::const_iterator iter = combinationFileList.begin();
        iter != combinationFileList.end();
        ++iter)
    {
        const char* fileName = (*iter).c_str();
        QPixmap picture(fileName);
        
        t_symbolsRepository::iterator symIter = EgmReelCombination::s_symbolsRepository.find(fileName);
        if (EgmReelCombination::s_symbolsRepository.end() == symIter) {
            // Create a new Label/Symbol for this file, and push it into the map/.
            EgmReelCombination::s_symbolsRepository.insert(
                EgmReelCombination::t_symbolsRepositoryPair(fileName, EgmSymbol(fileName))
                );
            symIter = EgmReelCombination::s_symbolsRepository.find(fileName);
        }
        // Add the new/existing Label/Symbol in the static-repository to this reel's instance collection of symbols.
        EgmSymbol* egmSymbol = &(*symIter).second;
        this->m_thisReelsSymbols.push_back(egmSymbol);
    }
}

EgmReelCombination::~EgmReelCombination(void)
{
}
