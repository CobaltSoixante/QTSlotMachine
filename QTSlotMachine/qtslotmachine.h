#ifndef QTSLOTMACHINE_H
#define QTSLOTMACHINE_H

#include "ui_qtslotmachine.h"

class QTSlotMachine : public QMainWindow
{
    Q_OBJECT

public:
    QTSlotMachine(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~QTSlotMachine();

private:
    Ui::QTSlotMachineClass ui;
};

#endif // QTSLOTMACHINE_H
