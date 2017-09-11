/********************************************************************************
** Form generated from reading UI file 'qtslotmachine.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTSLOTMACHINE_H
#define UI_QTSLOTMACHINE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QTSlotMachineClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QTSlotMachineClass)
    {
        if (QTSlotMachineClass->objectName().isEmpty())
            QTSlotMachineClass->setObjectName(QStringLiteral("QTSlotMachineClass"));
        QTSlotMachineClass->resize(600, 400);
        menuBar = new QMenuBar(QTSlotMachineClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        QTSlotMachineClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QTSlotMachineClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QTSlotMachineClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(QTSlotMachineClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        QTSlotMachineClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QTSlotMachineClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QTSlotMachineClass->setStatusBar(statusBar);

        retranslateUi(QTSlotMachineClass);

        QMetaObject::connectSlotsByName(QTSlotMachineClass);
    } // setupUi

    void retranslateUi(QMainWindow *QTSlotMachineClass)
    {
        QTSlotMachineClass->setWindowTitle(QApplication::translate("QTSlotMachineClass", "QTSlotMachine", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class QTSlotMachineClass: public Ui_QTSlotMachineClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTSLOTMACHINE_H
