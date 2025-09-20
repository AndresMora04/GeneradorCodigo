/********************************************************************************
** Form generated from reading UI file 'MainView.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINVIEW_H
#define UI_MAINVIEW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainViewClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainViewClass)
    {
        if (MainViewClass->objectName().isEmpty())
            MainViewClass->setObjectName("MainViewClass");
        MainViewClass->resize(600, 400);
        menuBar = new QMenuBar(MainViewClass);
        menuBar->setObjectName("menuBar");
        MainViewClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainViewClass);
        mainToolBar->setObjectName("mainToolBar");
        MainViewClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(MainViewClass);
        centralWidget->setObjectName("centralWidget");
        MainViewClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainViewClass);
        statusBar->setObjectName("statusBar");
        MainViewClass->setStatusBar(statusBar);

        retranslateUi(MainViewClass);

        QMetaObject::connectSlotsByName(MainViewClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainViewClass)
    {
        MainViewClass->setWindowTitle(QCoreApplication::translate("MainViewClass", "MainView", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainViewClass: public Ui_MainViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEW_H
