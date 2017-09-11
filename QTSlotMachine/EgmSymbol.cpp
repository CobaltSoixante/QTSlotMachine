#include "EgmSymbol.h"

#include <string>
#include <fstream> // ifstream
#include <exception>
using namespace std;

/*static*/ map<string, QPixmap> EgmSymbol::s_pixMap;

EgmSymbol::EgmSymbol(const char* imageFileName)
: m_imageFileName(imageFileName) // kept only for diagnostic purposes
{
    // If a filename doesn't exist - QT remains silent - I NEVER want this to happen:
    // it cost me significant development time tracking this bug:
    // my reels were simply "empty" of images, and I had no idea why -
    // I was sure it was my fault.
    // I now add an exception to expose this problem immediately in the future.
    if ( !ifstream(imageFileName) ) {
        throw exception((string() + "EgmSymbol::EgmSymbol(const char* imageFileName) - file '" + imageFileName + "' - doesn't exist").c_str());
    }

    // If we don't already store this image in our internal map - store it now!
    const map<string, QPixmap>::iterator iter = s_pixMap.find(imageFileName);
    if ( s_pixMap.end() == iter ) {
        s_pixMap[imageFileName] = QPixmap(imageFileName);
    }
    m_pPixmap = &s_pixMap[imageFileName];
}

void EgmSymbol::putSymbol(QLabel* i_pLabel) const {
    /*
    // Test something:
    static int sym_i = -1;;
    static QPixmap picture(QPixmap("unitTest.bmp"));
    ++sym_i;
    if (sym_i>=16) {
        i_pLabel->setPixmap(picture.scaled(100,100,Qt::KeepAspectRatio)); //pLabel->setPixmap(picture);
        return;
    }
    */

    // Resize the QPixmap according to:
    // http://stackoverflow.com/questions/8211982/qt-resizing-a-qlabel-containing-a-qpixmap-while-keeping-its-aspect-ratio
    // get label dimensions
    int w = 100; // pLabel->width();
    int h = 100; // pLabel->height();
    i_pLabel->setPixmap(m_pPixmap->scaled(w,h,Qt::KeepAspectRatio));

    return;
};

EgmSymbol::~EgmSymbol(void)
{
}
