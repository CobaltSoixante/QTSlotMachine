#include "UnitTests.h"

#include "EgmQTGui.h" // for UnitTests::testEgmQTGui()

#include <boost/lexical_cast.hpp> // string s = boost::lexical_cast<string>( number );
#include <boost/date_time/posix_time/posix_time.hpp> // boost::posix_time::ptime etc.
// Threading model: I use QThread in lieu of boost::thread (the latter doesn't work well with QT).
#include <boost/thread.hpp> // boost::thread

#include <QGraphicsScene>
#include <QMessageBox>
#include <QGridLayout>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QThread>

/*
NOTE: these tests are a memory-leak waiting to happen:
I apologize in advance.
*/

UnitTests::UnitTests(void)
{
}

UnitTests::~UnitTests(void)
{
}

//======================================================

void UnitTests::testQGraphicsPixmapItem(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication a(argc, argv);
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item(QPixmap("unitTest.bmp"));
    scene.addItem(&item);
    view.setWindowTitle("UnitTests::testQGraphicsPixmapItem");
    view.show();
    a.exec();
    return;
}

//======================================================

// Make sure message-boxes in QT work as expected.
void UnitTests::testMessageBox(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);
    // You'd think a message-box doesn't need all the above preliminary crap - but it does.
    QMessageBox msgBox;
    // Title of the box:
    msgBox.setText("Unit Test: testMessageBox");
    // A bit of narrative in the box:
    msgBox.setInformativeText(
        "You should see this message-box with an [OK] button. "
        "Just press the [OK] button to successfully conclude this test."
        );
    msgBox.exec();
    app.quit();
    return;
}

//======================================================

// This is the classic recipe for creating a QGridLayout:
UnitTests::TestQGridLayout::TestQGridLayout(QWidget *parent)
    : QWidget(parent)
{
    //QPixmap picture(QPixmap("unitTest.bmp"));
    QPixmap picture("unitTest.bmp");
    QGridLayout *pGrid = new QGridLayout(this);

    const int Rows = 2;
    const int Cols = 2;
    for (int r=0; r<Rows; r++) {
        for (int c=0; c<Cols; c++) {
            // Note: you MUST create a new object/widget for each cell!
            QLabel* pLabel;
            pLabel = new QLabel();

            // Resize the QPixmap according to:
            // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
            // get label dimensions
            int w = 100; // pLabel->width();
            int h = 100; // pLabel->height();
            // set a scaled pixmap to a w x h window keeping its aspect ratio 
            pLabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio));
            //pLabel->setPixmap(picture);

            pGrid->addWidget(pLabel, r, c);
        }
    }

    this->setLayout(pGrid);
}
void UnitTests::testQGridLayout(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    UnitTests::TestQGridLayout gridWindow;
    gridWindow.setWindowTitle("UnitTests::testQGridLayout");
    gridWindow.show();

    app.exec();
    return;
}

//======================================================

UnitTests::TestQHBoxLayout::TestQHBoxLayout(QWidget *parent)
    : QWidget(parent)
{
    QHBoxLayout* pQHBoxLayout = new QHBoxLayout(this);

    // Note: you MUST create a new object/widget for each cell!
     const int NumElements = 3;
     for (int i = 0; i < NumElements; ++i) {
         pQHBoxLayout->addWidget(new QLabel("Label"));
         pQHBoxLayout->addWidget(new QPushButton("Button"));
     }

    this->setLayout(pQHBoxLayout);
}
void UnitTests::testQHBoxLayout(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    UnitTests::TestQHBoxLayout testWindow;
    testWindow.setWindowTitle("UnitTests::testQHBoxLayout");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

UnitTests::TestQVBoxLayout::TestQVBoxLayout(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *pQVBoxLayout = new QVBoxLayout(this);

    // Note: you MUST create a new object/widget for each cell!
    pQVBoxLayout->addWidget(new UnitTests::TestQGridLayout());
    pQVBoxLayout->addWidget(new UnitTests::TestQHBoxLayout());

    this->setLayout(pQVBoxLayout);
}
void UnitTests::testQVBoxLayout(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    UnitTests::TestQVBoxLayout testWindow;
    testWindow.setWindowTitle("UnitTests::testQVBoxLayout");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

// Test REPLACING a widget in a QVBox
/*UnitTests::*/TestQVBoxLayoutReplaceWidget::TestQVBoxLayoutReplaceWidget(QWidget *parent)
    : QWidget(parent), Rows(2)
{
    QPixmap picture(QPixmap("unitTest.bmp"));
    QVBoxLayout *pQVBoxLayout = new QVBoxLayout(this);

    for (int r=0; r<this->Rows; r++) {
        // Note: you MUST create a new object/widget for each cell!
        QLabel* pLabel;
        pLabel = new QLabel();

        // Resize the QPixmap according to:
        // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
        // get label dimensions
        int w = 100; // pLabel->width();
        int h = 100; // pLabel->height();
        // set a scaled pixmap to a w x h window keeping its aspect ratio 
        pLabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);

        pQVBoxLayout->addWidget(pLabel, r, 0/*col*/);
        this->row.push_back(pLabel);
    }

    this->setLayout(pQVBoxLayout);
}
QWidget* TestQVBoxLayoutReplaceWidget::operator [](const int index) const
{
    return this->row[index];
}

// private:
void /*UnitTests::*/TestQVBoxLayoutReplaceWidget::replaceWidget(const int index, QWidget* pNewWidget)
{
    QWidget* pWidgetToReplace = (*this)[index];
    QVBoxLayout* pLayout = (QVBoxLayout*) this->layout();
    pLayout->removeWidget(pWidgetToReplace);
    pLayout->insertWidget(index, pNewWidget);

    // Don't ask... QWidget disabled it's assignment operator.
    //(*this)[index] = pNewWidget;
    for (vector<QWidget*>::iterator iter = this->row.begin();
        this->row.end() != iter;
        ++iter)
    {
        if (*iter == pWidgetToReplace)
            *iter = pNewWidget;
    }
}
void /*UnitTests::*/TestQVBoxLayoutReplaceWidget::removeWidget(const int index)
{
    QWidget* pWidgetToRemove = (*this)[index];
    QVBoxLayout* pLayout = (QVBoxLayout*) this->layout();
    pWidgetToRemove->hide(); // looks slightly better with this (but still a bit off).
    pLayout->removeWidget(pWidgetToRemove);
}

// public slots:
void /*UnitTests::*/TestQVBoxLayoutReplaceWidget::replaceWidget(QAbstractButton* abstractButton)
{
    QPixmap picture(QPixmap("unitTest2.bmp"));

    // Note: you MUST create a new object/widget for each cell!
    QLabel* pLabel;
    pLabel = new QLabel();

    // Resize the QPixmap according to:
    // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
    // get label dimensions
    int w = 100; // pLabel->width();
    int h = 100; // pLabel->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio 
    pLabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);

    this->replaceWidget(1/*row*/, pLabel);
}
void /*UnitTests::*/TestQVBoxLayoutReplaceWidget::removeWidget(QAbstractButton* abstractButton)
{
    this->removeWidget(0/*row*/);
}

void UnitTests::testQVBoxLayoutReplaceWidget(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    /*UnitTests::*/TestQVBoxLayoutReplaceWidget testWindow;
    testWindow.setWindowTitle("UnitTests::testQVBoxLayoutReplaceWidget");
    testWindow.show();

    QMessageBox msgBoxRep;
    msgBoxRep.setText("Test Replacing (2nd) Widget");
    msgBoxRep.setInformativeText(
        "Press me, and you should see the 2nd widget/row replaced."
        );
    QObject::connect(&msgBoxRep, SIGNAL(buttonClicked(QAbstractButton*)), &testWindow, SLOT(replaceWidget(QAbstractButton*)));
    msgBoxRep.setModal(false); // Make this "modeless" so it exists in-paralel/asynchronously with the above "main"-window.
    msgBoxRep.exec(); // You still need this line - it won't work without it.

    QMessageBox msgBoxRem;
    msgBoxRem.setText("Test Removing (1st) Widget");
    msgBoxRem.setInformativeText(
        "Press me, and you should see the 1st widget/row removed."
        );
    QObject::connect(&msgBoxRem, SIGNAL(buttonClicked(QAbstractButton*)), &testWindow, SLOT(removeWidget(QAbstractButton*)));
    msgBoxRem.setModal(false); // Make this "modeless" so it exists in-paralel/asynchronously with the above "main"-window.
    msgBoxRem.exec(); // You still need this line - it won't work without it.

    app.exec();
    return;
}

//======================================================
// Note: "TestBoostThread" is a failure;
// It will statistically work the 1st time it is executed, I am keeping it as a curio item;
// BUT - for use in conjunction with QT - I am betting my fortunes on QThread.

/*UnitTests::*/TestBoostThread::TestBoostThread(QWidget *parent)
    : QWidget(parent), Reels(5), Rows(3), SecsPerGame(5)
{
    // Create the reels.
    QHBoxLayout* pReelsLayout = new QHBoxLayout();
    for (int reel=0; reel < this->Reels; reel++)
    {
        QVBoxLayout *pReel = new QVBoxLayout();
        this->reels.push_back(vector<QLabel*>()); // store a new reel in the class' internal reels collection.

        // Place initial rows into the reel.
        for (int row=0; row < this->Rows; row++)
        {
            QLabel* pSymbol = new QLabel();
            string s = boost::lexical_cast<string>( row );
            pSymbol->setText(s.c_str());
            pReel->addWidget(pSymbol, row, 0/*col*/);
            this->reels[reel].push_back(pSymbol); // store the symbol in the current reel in the class' reels collection.
        }

        // Aggregate the reel into our horizontal-collection of reels.
        QWidget* pWidget = new QWidget();
        pWidget->setLayout(pReel);
        pReelsLayout->addWidget(pWidget);
    }

    // Create buttons that enable user to start/exit the test.
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    QPushButton* pPlayButton = new QPushButton("Play");
    pButtonsLayout->addWidget(pPlayButton);
    // Connect the "Play" button to the "play()" method:
    connect(pPlayButton, SIGNAL(clicked()), this, SLOT(play()));

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
// Spin the reel.
/*static*/ void /*UnitTests::*/TestBoostThread::reelThread(
    vector<QLabel*>& reel // The reel we are moving
    , const int milliSecs // the number of seconds we are moving it
    )
{
    ////static boost::mutex reelThreadmutex; // static var to synchronize multiple threads.

    const boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time(); 
    const boost::posix_time::ptime time_end = time_start + boost::posix_time::millisec(milliSecs);
    for (
        boost::posix_time::ptime time_current = time_start;
        time_current < time_end;
        time_current = boost::posix_time::microsec_clock::local_time()
        )
    {
        const int rSize = reel.size();
        // "lastSymbolValue" works even if 1==rSize .
        QString lastSymbolValue = reel[0]->text();
        // Display the 1st "rSize-1" symbols on the reel.
        for (int symbol_i = 0; symbol_i < rSize-1; symbol_i++)
        {
            // Replace the label text - AND protect it with a mutex (seems 2B very vulnerable in QT).
            ////reelThreadmutex.lock();
            reel[symbol_i]->setText(reel[symbol_i+1]->text()); // Get the next symbol into the current reel symbol.
            lastSymbolValue = reel[symbol_i]->text(); // Get the "new" last-symbol-value.
            ////reelThreadmutex.unlock();
            // Pause a bit, let other threads execute.
            boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        }
        // Display the final [new] symbol on the reel.
        const int nextSymbolValueInt = 1 + atoi(lastSymbolValue.toStdString().c_str());
        const string nextSymbolValueStr = boost::lexical_cast<string>(nextSymbolValueInt);
        // Replace the label text - AND protect it with a mutex (seems 2B very vulnerable in QT).
        ////reelThreadmutex.lock();
        reel[rSize-1]->setText(nextSymbolValueStr.c_str()); 
        ////reelThreadmutex.unlock();
        // Pause a bit, let other threads execute.
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    }
}

void /*UnitTests::*/TestBoostThread::play()
{
    //this->reels[0][0]->setText("haha");
    //return;

    // Create a thread for each reel [and start each].
    boost::thread* threads = new boost::thread[this->reels.size()];
    for (int reel = 0; reel < this->reels.size(); reel++)
    {
        const int secsForThisReelToSpin = this->SecsPerGame*1000/(this->Reels-reel);
        threads[reel] = boost::thread(
            reelThread, this->reels[reel]
            , secsForThisReelToSpin
            );
    }
    /*M*/return;

    // Wait for the threads/reel-spins to ALL complete.
    for (int reel = 0; reel < this->reels.size(); reel++)
    {
        threads[reel].join();
    }

    // Delete internal array.
    delete [] threads;
}
void UnitTests::testBoostThread(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    /*UnitTests::*/TestBoostThread testWindow;
    testWindow.setWindowTitle("UnitTests::testBoostThread");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

/*UnitTests::*/TestQThreadMetaInvoke::TestQThreadMetaInvoke(QWidget *parent)
    : QWidget(parent), Reels(5), Rows(3), SecsPerGame(5)
{
    // Create the reels.
    QHBoxLayout* pReelsLayout = new QHBoxLayout();
    for (int reel=0; reel < this->Reels; reel++)
    {
        QVBoxLayout *pReel = new QVBoxLayout();
        this->reels.push_back(vector<QLabel*>()); // store a new reel in the class' internal reels collection.

        // Place initial rows into the reel.
        for (int row=0; row < this->Rows; row++)
        {
            QLabel* pSymbol = new QLabel();
            string s = boost::lexical_cast<string>( row );
            pSymbol->setText(s.c_str());
            pReel->addWidget(pSymbol, row, 0/*col*/);
            this->reels[reel].push_back(pSymbol); // store the symbol in the current reel in the class' reels collection.
        }

        // Aggregate the reel into our horizontal-collection of reels.
        QWidget* pWidget = new QWidget();
        pWidget->setLayout(pReel);
        pReelsLayout->addWidget(pWidget);
    }

    // Create buttons that enable user to start/exit the test.
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    QPushButton* pPlayButton = new QPushButton("Play");
    pButtonsLayout->addWidget(pPlayButton);
    // Connect the "Play" button to the "play()" method:
    connect(pPlayButton, SIGNAL(clicked()), this, SLOT(play()));

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
// Spin the reel.
void /*UnitTests::*/TestQThreadMetaInvoke::reelThread(
    vector<QLabel*>& reel // The reel we are moving
    , const int milliSecs // the number of seconds we are moving it
    )
{
    const boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time(); 
    const boost::posix_time::ptime time_end = time_start + boost::posix_time::millisec(milliSecs);
    for (
        boost::posix_time::ptime time_current = time_start;
        time_current < time_end;
        time_current = boost::posix_time::microsec_clock::local_time()
        )
    {
        const int rSize = reel.size();
        // "lastSymbolValue" works even if 1==rSize .
        QString lastSymbolValue = reel[0]->text();
        // Display the 1st "rSize-1" symbols on the reel.
        for (int symbol_i = 0; symbol_i < rSize-1; symbol_i++)
        {
            // Replace the label text.
            reel[symbol_i]->setText(reel[symbol_i+1]->text()); // Get the next symbol into the current reel symbol.
            lastSymbolValue = reel[symbol_i]->text(); // Get the "new" last-symbol-value.
            // Pause a bit, let other threads execute.
            boost::this_thread::sleep(boost::posix_time::milliseconds(5));
        }
        // Display the final [new] symbol on the reel.
        const int nextSymbolValueInt = 1 + atoi(lastSymbolValue.toStdString().c_str());
        const string nextSymbolValueStr = boost::lexical_cast<string>(nextSymbolValueInt);
        // Replace the label text.
        reel[rSize-1]->setText(nextSymbolValueStr.c_str()); 
        // Pause a bit, let other threads execute.
        boost::this_thread::sleep(boost::posix_time::milliseconds(5));
    }
}

void /*UnitTests::*/TestQThreadMetaInvoke::play()
{
    // Create a thread for each reel [and start each].
    for (int reel = 0; reel < this->reels.size(); reel++)
    {
        const int secsForThisReelToSpin = this->SecsPerGame*1000/(this->Reels-reel);
        QMetaObject::invokeMethod( this, "reelThread", Qt::QueuedConnection
                           , Q_ARG( vector<QLabel*>&, this->reels[reel] )
                           , Q_ARG( const int, secsForThisReelToSpin )
                           );
    }
    return;
}
void UnitTests::testQThreadMetaInvoke(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    /*UnitTests::*/TestQThreadMetaInvoke testWindow;
    testWindow.setWindowTitle("UnitTests::TestQThreadMetaInvoke");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

// CREATE THE GUI.
/*UnitTests::*/TestQThreadWorkerObject::TestQThreadWorkerObject(QWidget *parent)
    : QWidget(parent), Reels(5), Rows(3), SecsPerGame(5), m_reelsStillSpinning(0)
{
    // Create the reels.
    QHBoxLayout* pReelsLayout = new QHBoxLayout();
    for (int reel=0; reel < this->Reels; reel++)
    {
        QVBoxLayout *pReel = new QVBoxLayout();
        this->reels.push_back(vector<QLabel*>()); // store a new reel in the class' internal reels collection.

        // Place initial rows into the reel.
        for (int row=0; row < this->Rows; row++)
        {
            QLabel* pSymbol = new QLabel();
            string s = boost::lexical_cast<string>( row );
            pSymbol->setText(s.c_str());
            pReel->addWidget(pSymbol, row, 0/*col*/);
            this->reels[reel].push_back(pSymbol); // store the symbol in the current reel in the class' reels collection.
        }

        // Aggregate the reel into our horizontal-collection of reels.
        QWidget* pWidget = new QWidget();
        pWidget->setLayout(pReel);
        pReelsLayout->addWidget(pWidget);
    }

    // Create buttons that enable user to start/exit the test.
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    this->m_pPlayButton = new QPushButton("Play"); // keep an explicit record off the [Play] button] - so we can grey-it-out when the reels spi, and reactivate ot at the end of the spins.
    pButtonsLayout->addWidget(this->m_pPlayButton);
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

// When the [Play] button of the GUI is clicked - this function that gets called.
void /*UnitTests::*/TestQThreadWorkerObject::play()
{
    // Grey-out the play-button [until all reels finish spinning].
    this->m_pPlayButton->setEnabled(false);
    if (0 != this->m_reelsStillSpinning) // sanity-check
        throw exception("TestQThreadWorkerObject::play() - sanity check: reels still spinning when [Play] clicked.");
    this->m_reelsStillSpinning = this->Reels;

    // Create a thread for each reel [and start each];
    // based much on a combination of QT formal doco (the suggestion to use worker-objects), but mainly:
    // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
    for (int reel=0; reel<this->Reels; ++reel) {
        QThread *thread = new QThread;
        const int secsForThisReelToSpin = this->SecsPerGame*1000/(this->Reels-reel);
        MyQThreadWorkerObject *worker = new MyQThreadWorkerObject(this->reels[reel], secsForThisReelToSpin);
        worker->moveToThread(thread);

        // This slot/signal is specific to my application:
        // I "grey-out" my [Play] button while the reels spin (otherwise all hell/access-violation cal occur if user clicks button in mid-spin);
        // When the reels have all finished spinning - I need the [Play] buttun to know, so it can re-activate itself.
        connect (thread, SIGNAL(finished()), this, SLOT(reelSpinCompleted()));

        // This is specific to my application: the thread emits a 'advanceSymbolSignal',
        // which causes a single 'advanceSymbolSlot' call to be executed in the context of the QT event-loop
        // (very important to update the QT display in the context of the QT event-loop: avoids nasty crashes).
        connect(
            worker, SIGNAL(advanceSymbolSignal(vector<QLabel*>, int)),
            worker, SLOT(advanceSymbolSlot(vector<QLabel*>, int))
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
void TestQThreadWorkerObject::reelSpinCompleted() {
    // Sanity check:
    if (0==this->m_reelsStillSpinning)
        throw exception("TestQThreadWorkerObject::reelSpinCompleted() - sanity check failed.");

    if (0 == --this->m_reelsStillSpinning)
        this->m_pPlayButton->setEnabled(true);
}

MyQThreadWorkerObject::MyQThreadWorkerObject(
        vector<QLabel*>& reel // The reel we are moving
        , const int milliSecs // the number of seconds we are moving it
        ) : reel(reel), milliSecs(milliSecs)
{
}

void MyQThreadWorkerObject::doWork(void)
{
    // Spin the reel.
    vector<QLabel*>& reel =  this->reel; // The reel we are moving
    const int milliSecs = this->milliSecs; // the number of seconds we are moving it
    const boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time(); 
    const boost::posix_time::ptime time_end = time_start + boost::posix_time::millisec(milliSecs);
    for (
        boost::posix_time::ptime time_current = time_start;
        time_current < time_end;
        time_current = boost::posix_time::microsec_clock::local_time()
        )
    {
        for (int symbol_i = 0; symbol_i < reel.size(); symbol_i++)
        {
            // Replace the label text.
            emit advanceSymbolSignal(reel, symbol_i);
            // Pause a bit (let other threads execute, etc).
            boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        }
    }

    // Signal (in standard fashion) that this thread is completed.
    emit finished();
}

void MyQThreadWorkerObject::advanceSymbolSlot(vector<QLabel*> reel, int symbolIndex)
{
    if (symbolIndex < reel.size()-1)
    {
        // replace the current symbol with the next one.
        reel[symbolIndex]->setText(reel[symbolIndex+1]->text()); // Get the next symbol into the current reel symbol.
    }
    else // symbolIndex == reel.size()-1
    {
        // Display the final [new] symbol on the reel.
        const int nextSymbolValueInt = 1 + atoi(reel[symbolIndex]->text().toStdString().c_str());
        const string nextSymbolValueStr = boost::lexical_cast<string>(nextSymbolValueInt);
        // Replace the label text.
        reel[symbolIndex]->setText(nextSymbolValueStr.c_str()); 
    }
    // causes me to crash: reel[symbolIndex]->repaint(); // Force refresh of the label (otherwise: not always refreshed).
    reel[symbolIndex]->update(); // Better, but still unreliable.
}

void UnitTests::testQThreadWorkerObject(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    /*UnitTests::*/TestQThreadWorkerObject testWindow;
    testWindow.setWindowTitle("UnitTests::testQThreadWorkerObject");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

void UnitTests::testEgmQTGui(const vector<string>& combinationFileList, size_t numOfReels, size_t numOfRows, size_t secsPerGame)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    // Create the symbol combination (this game has only one, shared by all reels).
    EgmReelCombination egmReelCombination(combinationFileList);
    Egm egm(egmReelCombination, numOfReels, numOfRows, secsPerGame);
    EgmQTGui testWindow(NULL/*QWidget *parent*/, egm);
    testWindow.setWindowTitle("EgmQTGui");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

UnitTests::TestEgmQTGuiInterimLayout::TestEgmQTGuiInterimLayout(QWidget *parent)
    : QWidget(parent)
{
    QPixmap picture("unitTest.bmp");

    const size_t NumOfReels = 5;
    // Create the reels.
    QHBoxLayout* pIntrmHBoxLayout = new QHBoxLayout(); // Horizontal-box layout - 1 [vertical] reel in each horizontal position.
    for (int reel_i=0; reel_i < NumOfReels; reel_i++)
    {
        QVBoxLayout *pIntrmVBoxLayout = new QVBoxLayout(); // Vertical-box layout - 1 position for each row of the reel.

        // Place initial [dummy] rows into the reel.
        const size_t NumOfRows = 3;
        for (int row_i=0; row_i < NumOfRows; row_i++)
        {
            // Note: you MUST create a new object/widget for each cell!
            QLabel* pLabel;
            pLabel = new QLabel();

            // Resize the QPixmap according to:
            // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
            // get label dimensions
            int w = 100; // pLabel->width();
            int h = 100; // pLabel->height();
            // set a scaled pixmap to a w x h window keeping its aspect ratio 
            pLabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio));

            pIntrmVBoxLayout->addWidget(pLabel, row_i);

        } // for...numOfRows

        pIntrmHBoxLayout->addLayout(pIntrmVBoxLayout); // Add the reel

    } // for...numOfReels

    this->setLayout(pIntrmHBoxLayout); // Add all the reels,
}

/*static*/ void UnitTests::testEgmQTGuiInterim(void) {
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    UnitTests::TestEgmQTGuiInterimLayout testWindow;
    testWindow.setWindowTitle("UnitTests::testEgmQTGuiInterim");
    testWindow.show();

    app.exec();
    return;
}

//======================================================

/*UnitTests::*/TestReplacePixmap::TestReplacePixmap(QWidget *parent)
    : QWidget(parent)
{
    QPixmap picture(QPixmap("unitTest.bmp"));

    // I need a "layout" type object to contain the label - QT won't let me do anything otherwise:
    QVBoxLayout *pMainLayout = new QVBoxLayout(this);

    this->m_qlabel = new QLabel();
    pMainLayout->addWidget(m_qlabel);

    // Resize the QPixmap according to:
    // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
    // get label dimensions
    int w = 100; // pLabel->width();
    int h = 100; // pLabel->height();
    // set a scaled pixmap to a w x h window keeping its aspect ratio 
    m_qlabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);

    // Add buttons-layout that enable user to start/exit the test.
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    QWidget* pButtonsWidget = new QWidget();
    pButtonsWidget->setLayout(pButtonsLayout);
    pMainLayout->addWidget(pButtonsWidget);

    // I'll also add a button to facilitate in SWITCHING the image.
    // Button#1:
    QPushButton* pButton = new QPushButton(":-( SwitchImageINEfficient");
    pButtonsLayout->addWidget(pButton);
    connect(pButton, SIGNAL(clicked()), this, SLOT(replacePixmapSlot()));
    // Button#2:
    QPushButton* pButtonEfficient = new QPushButton(":-) SwitchImageEfficient");
    pButtonsLayout->addWidget(pButtonEfficient);
    connect(pButtonEfficient, SIGNAL(clicked()), this, SLOT(replacePixmapEfficientSlot()));
    // Button#3:
    this->m_pButtonEfficientTestEmit = new QPushButton(";-) SwitchImageEfficientTestEmit");
    pButtonsLayout->addWidget(m_pButtonEfficientTestEmit);
    connect(m_pButtonEfficientTestEmit, SIGNAL(clicked()), this, SLOT(replacePixmapEfficientTestEmitSlot()));
    connect(
        this, SIGNAL(testSignal()),
        this, SLOT(testSlot())
        );

    this->setLayout(pMainLayout);
}

// private:
void /*UnitTests::*/TestReplacePixmap::replacePixmap(const char* i_fileName)
{
    QString fileName = i_fileName;
    QPixmap picture(fileName);
    int w = 100; // pLabel->width();
    int h = 100; // pLabel->height();
    m_qlabel->setPixmap(picture.scaled(w,h,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);
}
void /*UnitTests::*/TestReplacePixmap::replacePixmapEfficient(const QPixmap& pixmap)
{
    int w = 100; // pLabel->width();
    int h = 100; // pLabel->height();
    m_qlabel->setPixmap(pixmap.scaled(w,h,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);
}

// public slots:
void /*UnitTests::*/TestReplacePixmap::replacePixmapSlot()
{
    static const char* imageFiles[] = {"unitTest2.bmp", "unitTest.bmp"};
    const size_t numOfImageFiles = sizeof imageFiles / sizeof imageFiles[0];
    static size_t index = 0;
    this->replacePixmap(imageFiles[ index++ % numOfImageFiles ]);
}

void /*UnitTests::*/TestReplacePixmap::replacePixmapEfficientSlot()
{
    static const char* imageFiles[] = {"unitTest2.bmp", "unitTest.bmp"};
    const size_t numOfImageFiles = sizeof imageFiles / sizeof imageFiles[0];
    static size_t index = 0;
    static QPixmap pixmaps[] = { QPixmap(imageFiles[0]), QPixmap(imageFiles[1]) };
    this->replacePixmapEfficient(pixmaps[ index++ % numOfImageFiles ]);
}

void /*UnitTests::*/TestReplacePixmap::replacePixmapEfficientTestEmitSlot()
{
    this->m_pButtonEfficientTestEmit->setEnabled(false); // Grey-out the button [until all reels finish spinning].
    emit testSignal();
}
void TestReplacePixmap::testSlot() {
    static const char* imageFiles[] = {"unitTest2.bmp", "unitTest.bmp"};
    const size_t numOfImageFiles = sizeof imageFiles / sizeof imageFiles[0];
    static size_t index = 0;
    static QPixmap pixmaps[] = { QPixmap(imageFiles[0]), QPixmap(imageFiles[1]) };
    this->replacePixmapEfficient(pixmaps[ index++ % numOfImageFiles ]);

    this->m_pButtonEfficientTestEmit->setEnabled(true); // UNGrey-out the button
}

void UnitTests::testReplacePixmap(void)
{
    int argc=1;
    char *argv[] = {"testFunc"};
    QApplication app(argc, argv);

    /*UnitTests::*/TestReplacePixmap testWindow;
    testWindow.setWindowTitle("UnitTests::testQVBoxLayoutReplaceWidget");
    testWindow.show();

    app.exec();
    return;
}
