#include "MyUtils.h"

#include <conio.h> // bool kbhit()
#include <windows.h> // void Sleep(int millisecs)

#include <sstream>
using namespace std;

#if (defined(QT_LARGEFILE_SUPPORT) || defined(QT_CORE_LIB) || defined(QT_GUI_LIB) )
#include <QMessageBox>
#include <QApplication>
#endif

static bool s_isQT =
#if (defined(QT_LARGEFILE_SUPPORT) || defined(QT_CORE_LIB) || defined(QT_GUI_LIB) )
        true
#else
        false
#endif
    ;
static bool s_isConsole = !s_isQT;

MyUtils::MyUtils(void)
{
}

MyUtils::~MyUtils(void)
{
}

// Wait indefinitely until user strikes any key
///*static*/ void waitForKeystroke(const stringstream &ss, const char* i_title)
//{
//    waitForKeystroke(ss.str().c_str(), i_title);
//}
/*static*/ void MyUtils::waitForKeystroke(const char* i_msg, const char* i_title)
{
    if (s_isConsole) {
        char* msg = NULL!=i_msg ? i_msg : "Press any key...";
        cout << msg << endl;
        while (!kbhit())
            Sleep(1000);
        ::getch(); // gobble up the keystroke
    }
    else {
        int argc=1;
        char *argv[] = {"MyUtils::waitForKeystroke"};
        QApplication app(argc, argv);
        QMessageBox msgBox;
        // Title of the box:
        if (NULL!=i_title)
            msgBox.setText(i_title);
        // A bit of narrative in the box:
        msgBox.setInformativeText(
            (
                string()
                + (NULL!=i_msg ? i_msg : "")
                + "Press [OK] to continue."
            ).c_str()
        );
        msgBox.exec();
        app.quit();
        return;
    }
}
