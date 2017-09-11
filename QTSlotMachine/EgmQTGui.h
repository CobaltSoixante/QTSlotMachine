#pragma once

#include <QPushButton>
#include <QLineEdit>

#include <vector>
using namespace std;

#include "Egm.h"

class EgmQTGui : public QWidget // Must inherit from QWidget to be a QT app.
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.

public:
    // CREATE THE GUI:
    EgmQTGui(QWidget *parent, Egm& egm);
    virtual ~EgmQTGui(void);

public slots:
    // Executed when the [Play] button is clicked:
    void play();

    // Executed when a reel/thread has finished spinning (used to count-down all spinning reels, and finally reactivate the [Play] button):
    void reelSpinCompleted();

    // Helper slot to facilitate updating the GUI reel-display as the reels roll.
    // You see, updates to the GUI must be done in the context of the main QT GUI-thread;
    // this dictates that a signal be emitted, and the corresponding slot is executed by THIS object, which is "owned" by the main QT GUI-thread.
public slots:
    void advanceSymbolSlot(
        int reel_i, // Index of the reel for which the change is occuring (in this app: is identical for both GUI & EGM reel).
        int GUISymbol_i,
        int EGMSymbol_i
        );

private:
    Egm& m_egm;
    vector<vector<QLabel*>> m_reels; // collection of reels, and for each reel - it's symbols.
    QPushButton* m_pPlayButton; // keep an explicit record off the [Play] button] - so we can grey-it-out when the reels spin, and reactivate it at the end of the spins.
    QLineEdit* m_pSecsPerGameText;
    int m_numOfReelsStillSpinning;
};
