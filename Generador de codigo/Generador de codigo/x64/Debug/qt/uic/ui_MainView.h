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
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainViewClass
{
public:
    QWidget *centralWidget;
    QPushButton *btnCargar;
    QPushButton *btnConvertir;
    QPushButton *btnGuardar;
    QTextEdit *txtCargado;
    QTextEdit *txtConvertido;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainViewClass)
    {
        if (MainViewClass->objectName().isEmpty())
            MainViewClass->setObjectName("MainViewClass");
        MainViewClass->resize(600, 400);
        centralWidget = new QWidget(MainViewClass);
        centralWidget->setObjectName("centralWidget");
        btnCargar = new QPushButton(centralWidget);
        btnCargar->setObjectName("btnCargar");
        btnCargar->setGeometry(QRect(120, 220, 80, 24));
        btnConvertir = new QPushButton(centralWidget);
        btnConvertir->setObjectName("btnConvertir");
        btnConvertir->setGeometry(QRect(250, 220, 80, 24));
        btnGuardar = new QPushButton(centralWidget);
        btnGuardar->setObjectName("btnGuardar");
        btnGuardar->setGeometry(QRect(380, 220, 80, 24));
        txtCargado = new QTextEdit(centralWidget);
        txtCargado->setObjectName("txtCargado");
        txtCargado->setGeometry(QRect(120, 50, 101, 111));
        txtConvertido = new QTextEdit(centralWidget);
        txtConvertido->setObjectName("txtConvertido");
        txtConvertido->setGeometry(QRect(340, 50, 101, 111));
        MainViewClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainViewClass);
        menuBar->setObjectName("menuBar");
        menuBar->setGeometry(QRect(0, 0, 600, 21));
        MainViewClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainViewClass);
        mainToolBar->setObjectName("mainToolBar");
        MainViewClass->addToolBar(Qt::ToolBarArea::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainViewClass);
        statusBar->setObjectName("statusBar");
        MainViewClass->setStatusBar(statusBar);

        retranslateUi(MainViewClass);

        QMetaObject::connectSlotsByName(MainViewClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainViewClass)
    {
        MainViewClass->setWindowTitle(QCoreApplication::translate("MainViewClass", "MainView", nullptr));
        btnCargar->setText(QCoreApplication::translate("MainViewClass", "Cargar", nullptr));
        btnConvertir->setText(QCoreApplication::translate("MainViewClass", "Convertir", nullptr));
        btnGuardar->setText(QCoreApplication::translate("MainViewClass", "Guardar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainViewClass: public Ui_MainViewClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINVIEW_H
