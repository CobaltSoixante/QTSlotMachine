#pragma once

#include <map>
using namespace std;

#include <QLabel>
#include <QPixmap>

class EgmSymbol
{
public:
    EgmSymbol(const char* imageFileName);
    virtual ~EgmSymbol(void);
    void putSymbol(QLabel* i_pLabel) const;

private:
    static map<string, QPixmap> s_pixMap;
    QPixmap* m_pPixmap;
    const string m_imageFileName; // kept for diagnostic/debug purposes only.
};
