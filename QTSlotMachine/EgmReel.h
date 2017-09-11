#pragma once

#include "EgmReelCombination.h"
class EgmReel
{
    friend class Egm; // So the EGm class can use the private setCurrentPosition() function.
public:
    EgmReel(const EgmReelCombination& reelCombination);
    virtual ~EgmReel(void);

public:
    // Make it look (by faking it, a bit) as if this class is a vector of EgmSymbol :
    inline size_t size(void) const {return m_pCombination->size();}
    inline const EgmSymbol& operator[](int index) const {
        return (*m_pCombination)[index];
    }
    inline size_t getCurrentPosition() const { return m_currentPosition; }
    inline size_t getPreviousPosition() const { return m_previousPosition; }

private:
    inline void setCurrentPosition(size_t newPosition) {
        m_previousPosition = m_currentPosition;
        m_currentPosition = newPosition;
    }

private:
    const EgmReelCombination* m_pCombination; // Make this a pointer (not reference)
        // for a mundane/technical reason: so it is assignable
        // (and a convenient default 'operator=' is generated) -
        // so that it can be used as a term in a STL-vector.
    size_t m_currentPosition; // current position of the reel. Will typically be the 1st (top-most) row on an EGM that displays several rows for each reel (usu. 3).
    size_t m_previousPosition; // this is a courtesy service, we keep this value for a GUI that may need it.
};
