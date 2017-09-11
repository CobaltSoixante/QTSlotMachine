#pragma once

#include <QWidget>
#include <QAbstractButton>
#include <QLabel>
#include <QPushButton>
#include <QWidget>
#include <QWidget>
#include <QWidget>
#include <QWidget>
#include <QWidget>
#include <QWidget>
#include <QWidget>

#include <vector>
using namespace std;

class UnitTests
{
public:
    UnitTests(void);
    ~UnitTests(void);
    static void testQGraphicsPixmapItem(void);
    // Make sure message-boxes in QT work as expected.
    static void testMessageBox(void);
    // This is the classic recipe for creating a QGridLayout:
    static void testQGridLayout(void);
    // test run the most important class of this application:
    static void testEgmQTGui(const vector<string>& combinationFileList, size_t numOfReels, size_t numOfRows, size_t secsPerGame);
    // EgmQTGui() is not panning out, do some interim tests / scenario-emulations:
    static void testEgmQTGuiInterim(void);

private:
    // This is the classic recipe for creating a QGridLayout:
    class TestQGridLayout : public QWidget
    {
    public:
        TestQGridLayout(QWidget *parent = NULL);
    };

    class TestQHBoxLayout : public QWidget
    {
    public:
        TestQHBoxLayout(QWidget *parent = NULL);
    };

    class TestQVBoxLayout : public QWidget
    {
    public:
        TestQVBoxLayout(QWidget *parent = NULL);
    };

    class TestEgmQTGuiInterimLayout : public QWidget
    {
    public:
        TestEgmQTGuiInterimLayout(QWidget *parent = NULL);
    };

public:
   static void testQHBoxLayout(void);
   static void testQVBoxLayout(void);
   // Test REPLACING a widget in a QVBox
   static void testQVBoxLayoutReplaceWidget(void);
   // Test boost multi-threading & join
   static void testBoostThread(void);
   // Test QThread using method "QMetaObject::invokeMethod".
   static void testQThreadMetaInvoke(void);
   // Test QThread using worker-objects (IE the newer "correct" way that doesn't derive from "QThread").
   static void testQThreadWorkerObject(void);

   static void testReplacePixmap(void);
};

//======================================================

// Don't ask! I was gonna make this a nested class in UnitTests:: -
// and got the darndest build-error:
// Error	1	error PRJ0019: A tool returned an error code from "Moc'ing UnitTests.h..."	QTSlotMachine	QTSlotMachine
// Well, I looked at the full compile-log on:
// file://F:\Documents and Settings\adam\My Documents\Visual Studio 2008\Projects\QTSlotMachine\Debug\BuildLog.htm
// and here's what it said:
// f:\Documents and Settings\adam\My Documents\Visual Studio 2008\Projects\QTSlotMachine\QTSlotMachine\UnitTests.h(36): Error: Meta object features not supported for nested classes
// (line#36 is where I had my Q_OBJECT macro).
// So: what can I do? - This feeble test-class is now an entity in it's own right!
class TestQVBoxLayoutReplaceWidget : public QWidget
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.
public:
    TestQVBoxLayoutReplaceWidget(QWidget* parent = NULL);
    // Enable easily replacing a widget in the box:
    QWidget* operator[] (const int index) const;
public slots:
    void replaceWidget(QAbstractButton* abstractButton);
    void removeWidget(QAbstractButton* abstractButton);
private:
    void replaceWidget(const int index, QWidget* widget);
    void removeWidget(const int index);
    void removeWidget(const int index, QWidget* widget);
    const int Rows;
    vector<QWidget*> row;
};

//======================================================

class TestBoostThread : public QWidget
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.
public:
    TestBoostThread(QWidget *parent = NULL);

public slots:
    void play();

private:
    const int Reels;
    const int Rows;
    const int SecsPerGame;
    vector<vector<QLabel*>> reels; // collection of reels, and for each reel - it's symbols.
    static void reelThread(
        vector<QLabel*>& reel // The reel we are moving
        , const int milliSecs // the number of seconds we are moving it
        );
};

//======================================================

class TestQThreadMetaInvoke : public QWidget
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.
public:
    TestQThreadMetaInvoke(QWidget *parent = NULL);
public slots:
    void play();
private:
    const int Reels;
    const int Rows;
    const int SecsPerGame;
    void reelThread(
        vector<QLabel*>& reel // The reel we are moving
        , const int milliSecs // the number of seconds we are moving it
        );
    vector<vector<QLabel*>> reels; // collection of reels, and for each reel - it's symbols.
};

//======================================================

// Worker thread, along the lines suggested by (toward the end):
// http://qt-project.org/doc/qt-4.8/QThread.html 

// The framework GUI class:
class TestQThreadWorkerObject : public QWidget
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.
public:
    // CREATE THE GUI.
    TestQThreadWorkerObject(QWidget *parent = 0);

public slots:
    // Executed when the [Play] button is clicked:
    void play();

    // Executed when a reel/thread has finished spinning (used to count-down all spinning reels, and finally reactivate the [Play] button):
    void reelSpinCompleted();

private:
    const int Reels;
    const int Rows;
    const int SecsPerGame;
    vector<vector<QLabel*>> reels; // collection of reels, and for each reel - it's symbols.
    QPushButton* m_pPlayButton; // keep an explicit record off the [Play] button] - so we can grey-it-out when the reels spi, and reactivate ot at the end of the spins.
    int m_reelsStillSpinning;
};

// The worker object where work gets done:
// Represents a object/thread for a single reel.
class MyQThreadWorkerObject: public QObject
{
    Q_OBJECT;
public:
    MyQThreadWorkerObject(
        vector<QLabel*>& reel // The reel we are moving
        , const int milliSecs // the number of seconds we are moving it
    );

    // Connected to the thread-start() signal, this gets a single game going (IE 1 spin of all the reels):
public slots:
    void doWork();

    // Helper signal-&-slot to facilitate updating the GUI reel-display as the reels roll.
    // You see, updates to the GUI must be done in the context of the main QT GUI-thread;
    // this dictates that a signal be emitted, so the corresponding slot is executed by the main QT GUI-thread.
signals:
    void advanceSymbolSignal(vector<QLabel*> reel, int symbolIndex);
public slots:
    void advanceSymbolSlot(vector<QLabel*> reel, int symbolIndex);

    // Connected to thread wrap-up slots, as per:
    // http://mayaposch.wordpress.com/2011/11/01/how-to-really-truly-use-qthreads-the-full-explanation/
signals:
    void finished();

private:
    vector<QLabel*>& reel;
    const int milliSecs;
};

//======================================================

class TestReplacePixmap : public QWidget
{
    Q_OBJECT // This macro must be included in all classes that contain signals and/or slots.
public:
    TestReplacePixmap(QWidget* parent = NULL);

// For replacePixmapEfficientTestEmitSlot() to emit a signal:
signals:
    void testSignal();
public slots:
    void testSlot();

public slots:
    void replacePixmapSlot();
    void replacePixmapEfficientSlot();
    void replacePixmapEfficientTestEmitSlot();
private:
    void replacePixmap(const char* imageName);
    void replacePixmapEfficient(const QPixmap& pixmap);
    QLabel* m_qlabel;
    QPushButton* m_pButtonEfficientTestEmit;
};
