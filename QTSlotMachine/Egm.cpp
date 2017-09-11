#include "Egm.h"

#include <time.h> // for seeding srand()

Egm::Egm(const EgmReelCombination& egmReelCombination, int numOfReels, int numOfRows, int secsPerGame)
    :
    ReelCombination(egmReelCombination),
    c_rows(numOfRows),
    secsPerGame(secsPerGame)
{
    for (int reel_i=0; reel_i < numOfReels; reel_i++)
    {
        EgmReel egmReel(ReelCombination);
        this->reels.push_back(egmReel); // store a new reel in the class' internal reels collection.
    }

    /* initialize random seed: */
    srand (time(NULL));
}

Egm::~Egm(void)
{
}

void Egm::play(void)
{
    for (size_t reel_i=0; reel_i<this->reels.size(); reel_i++) {
        EgmReel& reel = this->reels[reel_i];
        reel.setCurrentPosition( rand() % reel.size() ); // is in the range 0 to reel.size()-1 .
    }
}
