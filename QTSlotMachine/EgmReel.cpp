#include "EgmReel.h"

EgmReel::EgmReel(const EgmReelCombination& reelCombination)
: m_pCombination(&reelCombination), m_currentPosition(0)
{
}

EgmReel::~EgmReel(void)
{
}
