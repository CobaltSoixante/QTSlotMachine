#pragma once
#include <QtGui>
#include "f:\qt\4.8.1\src\corelib\kernel\qobject.h"

class MyCustomWidget :
    public QWidget
{
public:
    MyCustomWidget(QWidget* parent=NULL);
    virtual ~MyCustomWidget(void);
private:
    // Delete these QObject's (basic QWidget's, higher QGridLayout's, etc) at destruction.
    QObject qObjectsToDeleteAtDestruction;
};
