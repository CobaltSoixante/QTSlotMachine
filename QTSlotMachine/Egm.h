#pragma once

#include "EgmReelCombination.h"
#include "EgmReel.h"

class Egm
{
public:
    Egm(const EgmReelCombination& egmReelCombination, int numOfReels, int numOfRows, int secsPerGame);
    virtual ~Egm(void);

public:
    // Make it look (by faking it, a bit) as if this class is a vector of EgmReel :
    inline size_t size(void) const {return reels.size();}
    inline const EgmReel& operator[](int index) const {
        return reels[index];
    }

public:
    inline int numOfReels(void) const {return this->reels.size();}
    inline int numOfRows(void) const {return this->c_rows;}
    inline int numOfSecsPerGame(void) const {return this->secsPerGame;}
    inline void setSecsPerGame(int secsPerGame) {this->secsPerGame = secsPerGame;}
    inline const EgmReel& reel(int index) const {return reels[index];}

private:
    //const EgmReelCombination& ReelCombination;
    const int c_rows;
    int secsPerGame;
    const EgmReelCombination ReelCombination; // NOTE: We don't make this a REFERENCE to the EgmReelCombination input-parameter: we make a full copy that WE OWN, so it can't go out of scope accidentally, and the internal reels can use references to it safely.
    vector<EgmReel> reels; // collection of reels, and for each reel - it's symbols.
public:
    void play(void);
};
