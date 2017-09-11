#pragma once

#include "EgmReel.h"

#include <QtGui>

#include <vector>
using namespace std;

class EgmQTThreadWorkerObject : public QObject
{
    Q_OBJECT;
public:
    EgmQTThreadWorkerObject(
        size_t reelIndex, // Index of the reel in the GUI: we need to SIGNAL this info to the SLOT in the GUI.
        vector<QLabel*>& GUIReel, // The GUI display to be modified.
        const EgmReel& EGMReel // The logical EGM reel we are moving on the display.
        , unsigned int milliSecs // the number of seconds the reel spins.
    );
    virtual ~EgmQTThreadWorkerObject(void);

    // Connected to the thread-start() signal, this gets a single game going (IE 1 spin of all the reels):
public slots:
    void doWork();


    // Helper signal-&-slot to facilitate updating the GUI reel-display as the reels roll.
    // You see, updates to the GUI must be done in the context of the main QT GUI-thread;
    // this dictates that a signal be emitted, so the corresponding slot is executed by the main QT GUI-thread.
signals:
    void advanceSymbolSignal(int reel_i, int GUISymbol_i, int EGMSymbol_i);

    // Connected to thread wrap-up slots, as per:
    // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
signals:
    void finished();

private:
    const size_t m_reelIndex;
    vector<QLabel*>& m_GUIReel; // can't make this a const: will be changed by the SIGNAL we emit to the SLOT in the GUI.
    const EgmReel& m_EGMReel;
    const unsigned int m_milliSecs;
};
