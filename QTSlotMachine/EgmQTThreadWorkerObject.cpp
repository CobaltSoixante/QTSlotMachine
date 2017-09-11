#include "EgmQTThreadWorkerObject.h"

#include <boost/date_time/posix_time/posix_time.hpp> // boost::posix_time::ptime etc.
// Threading model: I use QThread in lieu of boost::thread (the latter doesn't work well with QT).
#include <boost/thread.hpp> // But: I use boost::thread / boost::this_thread::sleep() - becuase QT doesn't have a "sleep()".

EgmQTThreadWorkerObject::EgmQTThreadWorkerObject(
    size_t reelIndex, // Index of the reel in the GUI: we need to SIGNAL this info to the SLOT in the GUI.
    vector<QLabel*>& GUIReel, // The GUI display to be modified.
    const EgmReel& EGMReel // The logical EGM reel we are moving on the display.
    , unsigned int milliSecs // the number of seconds the reel spins.
    ) : m_reelIndex(reelIndex), m_GUIReel(GUIReel), m_EGMReel(EGMReel), m_milliSecs(milliSecs)
{
}

EgmQTThreadWorkerObject::~EgmQTThreadWorkerObject(void)
{
}

void EgmQTThreadWorkerObject::doWork(void)
{
    // Spin the reel.
    const int milliSecs = this->m_milliSecs; // the number of seconds we are moving it
    const boost::posix_time::ptime time_start = boost::posix_time::microsec_clock::local_time(); 
    const boost::posix_time::ptime time_end = time_start + boost::posix_time::millisec(milliSecs);
    size_t EGMSymbol_i = (m_EGMReel.getPreviousPosition() + 1) % m_EGMReel.size();
    for (
        boost::posix_time::ptime time_current = time_start;
        (time_current<time_end) ? true : (EGMSymbol_i!=m_EGMReel.getCurrentPosition()) ? true : false;
        time_current = boost::posix_time::microsec_clock::local_time()
        )
    {
        for (
            size_t GUISymbol_i = 0;
            GUISymbol_i < this->m_GUIReel.size();
            ++GUISymbol_i, ++EGMSymbol_i, EGMSymbol_i%=m_EGMReel.size()
            )
        {
            // Replace the label symbol.
            emit advanceSymbolSignal(this->m_reelIndex, GUISymbol_i, EGMSymbol_i);

            // Pause a bit (let other threads execute, etc).
            boost::this_thread::sleep(boost::posix_time::milliseconds(50));
        }
    }

    // Signal (in standard fashion) that this thread is completed.
    emit finished();
}
