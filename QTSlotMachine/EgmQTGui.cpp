#include "EgmQTGui.h"
#include "EgmQTThreadWorkerObject.h"

#include <boost/lexical_cast.hpp> // string s = boost::lexical_cast<string>( number );

#include <QHBoxLayout>
#include <QGroupBox>
#include <QMessageBox>

// CREATE THE GUI:
EgmQTGui::EgmQTGui(QWidget *parent, Egm& egm) :
    QWidget(parent), m_egm(egm), m_numOfReelsStillSpinning(0)
{
    // Create the reels.
    QHBoxLayout* pReelsLayout = new QHBoxLayout(); // Horizontal-box layout - 1 [vertical] reel in each horizontal position.
    for (int reel_i=0; reel_i < m_egm.numOfReels(); reel_i++)
    {
        QVBoxLayout *pReel = new QVBoxLayout(); // Vertical-box layout - 1 position for each row of the reel.
        this->m_reels.push_back(vector<QLabel*>()); // store a new reel in the GUI class' internal reels collection.

        // Place initial [dummy] rows into the reel.
        const EgmReel& logicalReel = this->m_egm[reel_i];
        for (int row_i=0; row_i < egm.numOfRows(); row_i++)
        {
            const EgmSymbol& egmSymbol = logicalReel[row_i];
            QLabel* pLabel = new QLabel();
            egmSymbol.putSymbol(pLabel);
            
            pReel->addWidget(pLabel, row_i);
            this->m_reels[reel_i].push_back(pLabel); // store the symbol in the current reel in the class' reels collection.
                // This symbol will be advanced continuously during a reel-spin/play session.
        } // for egm.numOfRows()

        // Put the reel into a group-box
        // (just as an effect, we don't need the group-box, but it puts a border around the entire reel, which looks noce).
        QGroupBox *pGroupBox = new QGroupBox();
        pGroupBox->setLayout(pReel);

        // Aggregate the reel into our horizontal-collection of reels.
        //pReelsLayout->addLayout(pReel); // Add the reel
        pReelsLayout->addWidget(pGroupBox); // Add the reel's "dummy" group-box. (the group-box is only for effect - to get a border around the reel).
    } // for m_egm.numOfReels()

    // Create buttons that enable user to start/exit the test.
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    this->m_pPlayButton = new QPushButton("Play"); // keep an explicit record off the [Play] button] - so we can grey-it-out when the reels spi, and reactivate ot at the end of the spins.
    pButtonsLayout->addWidget(new QLabel(tr("Game Duration [seconds]:")));
    this->m_pSecsPerGameText = new QLineEdit;
    m_pSecsPerGameText->setText(boost::lexical_cast<std::string>(m_egm.numOfSecsPerGame()).c_str());
    pButtonsLayout->addWidget(m_pSecsPerGameText);
    pButtonsLayout->addWidget(new QLabel("          ")); // unused filler, so the main button horizontal-dimension doesn't use up all the row.
    pButtonsLayout->addWidget(new QLabel("          ")); // unused filler, so the main button horizontal-dimension doesn't use up all the row.
    pButtonsLayout->addWidget(this->m_pPlayButton); // THE MAIN BUTTON.
    pButtonsLayout->addWidget(new QLabel("          ")); // unused filler, so the main button horizontal-dimension doesn't use up all the row.
    pButtonsLayout->addWidget(new QLabel("          ")); // unused filler, so the main button horizontal-dimension doesn't use up all the row.
    // Connect the "Play" button to the "play()" method:
    connect(this->m_pPlayButton, SIGNAL(clicked()), this, SLOT(play()));

    // Create main-layout to contain the reels on top, and the buttons underneath.
    QVBoxLayout* pMainLayout = new QVBoxLayout();
    // Add the reels to the main layout:
    QWidget* pReelsWidget = new QWidget();
    pReelsWidget->setLayout(pReelsLayout);
    pMainLayout->addWidget(pReelsWidget);
    // Add the buttons to the main layout:
    QWidget* pButtonsWidget = new QWidget();
    pButtonsWidget->setLayout(pButtonsLayout);
    pMainLayout->addWidget(pButtonsWidget);
    this->setLayout(pMainLayout);
}

EgmQTGui::~EgmQTGui(void)
{
}

void EgmQTGui::advanceSymbolSlot(
    int reel_i, // Index of the reel for which the change is occuring (in this app: is identical for both GUI & EGM reel).
    int GUISymbol_i,
    int EGMSymbol_i
    )
{
    // replace the current symbol with the next one.
    const EgmReel& egmReel = this->m_egm[reel_i];
    vector<QLabel*> guiReel = this->m_reels[reel_i];
    egmReel[EGMSymbol_i].putSymbol(guiReel[GUISymbol_i]);
    // causes me to crash: reel[symbolIndex]->repaint(); // Force refresh of the label (otherwise: not always refreshed).
    // I don't think I need this: this->m_GUIReel[GUISymbol_i]->update(); // Better, but still unreliable.
}

// When the [Play] button of the GUI is clicked - this function that gets called.
void EgmQTGui::play()
{
    // Get the duration of the game.
    int duration = atoi(this->m_pSecsPerGameText->text().toStdString().c_str());
    if (0 == duration) {
        // You'd think a message-box doesn't need all the above preliminary crap - but it does.
        QMessageBox msgBox;
        // Title of the box:
        msgBox.setText("BAD DURATION!");
        // A bit of narrative in the box:
        msgBox.setInformativeText("Enter a valid duration in seconds for the game.");
        msgBox.exec();
        return;
    }
    m_egm.setSecsPerGame(duration);


    // Grey-out the play-button [until all reels finish spinning].
    this->m_pPlayButton->setEnabled(false);
    if (0 != this->m_numOfReelsStillSpinning) // sanity-check
        throw exception("EgmQTGui::play() - sanity check: reels still spinning when [Play] clicked.");
    this->m_numOfReelsStillSpinning = m_egm.numOfReels();

    // Spin the reels on the "logical" egm (IE the actual game - not the displayed GUI stuff).
    this->m_egm.play();

    // Create a thread for each reel [and start each];
    // based much on a combination of QT formal doco (the suggestion to use worker-objects), but mainly:
    // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    for (
        int reel_i=0;
        reel_i<this->m_egm.numOfReels();
        ++reel_i)
    {
        QThread *thread = new QThread;
        const int milliSecsForThisReelToSpin = this->m_egm.numOfSecsPerGame()*1000/(this->m_egm.numOfReels() - reel_i);
        EgmQTThreadWorkerObject *worker = new EgmQTThreadWorkerObject(
            reel_i, // 20120807: I have to add this too, it's messy & can be neater, am just learning the "niceties" of QT and having to go through the GUI thread when updating the GUI from multiple threads.
            this->m_reels[reel_i], // The physical GUI reel that is displayed
            this->m_egm.reel(reel_i), // The logical EGM reel
            milliSecsForThisReelToSpin
            );
        worker->moveToThread(thread);

        // This slot/signal is specific to my application:
        // I "grey-out" my [Play] button while the reels spin (otherwise all hell/access-violation cal occur if user clicks button in mid-spin);
        // When the reels have all finished spinning - I need the [Play] buttun to know, so it can re-activate itself.
        connect (thread, SIGNAL(finished()), this, SLOT(reelSpinCompleted()));

        // This is specific to my application: the thread emits a 'advanceSymbolSignal',
        // which causes a single 'advanceSymbolSlot' call to be executed in the context of the QT event-loop
        // (very important to update the QT display in the context of the QT event-loop: avoids nasty crashes).
        // NOTE: we need the worker-signal to connect to a slot from THIS THREAD: the main GUI thread! (won't work otherwise).
        connect(
            worker, SIGNAL(advanceSymbolSignal(int, int, int)),
            this, SLOT(advanceSymbolSlot(int, int, int))
            );

        // The rest of this is pretty much copied from the suggested recipe in:
        // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
        connect(thread, SIGNAL(started()), worker, SLOT(doWork()));
        connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
        connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
        connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
        thread->start();
    }
}

// When all reels have stopped spinning - enable the [Play] button again.
// Note: because this slot is executed in the QT main event-loop context -
// no multi-threaded synchronization mechanisms are required to protect member/instance variables used here.
void EgmQTGui::reelSpinCompleted() {
    // Sanity check:
    if (0==this->m_numOfReelsStillSpinning)
        throw exception("EgmQTGui::reelSpinCompleted() - sanity check failed.");

    if (0 == --this->m_numOfReelsStillSpinning)
        this->m_pPlayButton->setEnabled(true);
}
