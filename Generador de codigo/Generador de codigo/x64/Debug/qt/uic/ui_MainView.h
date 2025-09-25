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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainViewClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *mainView;
    QLabel *lbTitulo;
    QHBoxLayout *hboxTextos;
    QTextEdit *txtCargado;
    QTextEdit *txtConvertido;
    QHBoxLayout *hboxBotones;
    QPushButton *btnCargar;
    QPushButton *btnConvertir;
    QPushButton *btnGuardar;
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
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        centralWidget->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        centralWidget->setAutoFillBackground(false);
        verticalLayout_2 = new QVBoxLayout(centralWidget);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        mainView = new QVBoxLayout();
        mainView->setSpacing(6);
        mainView->setObjectName("mainView");
        mainView->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        mainView->setContentsMargins(10, 10, 10, 10);
        lbTitulo = new QLabel(centralWidget);
        lbTitulo->setObjectName("lbTitulo");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(lbTitulo->sizePolicy().hasHeightForWidth());
        lbTitulo->setSizePolicy(sizePolicy1);
        lbTitulo->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        lbTitulo->setAlignment(Qt::AlignmentFlag::AlignCenter);

        mainView->addWidget(lbTitulo);

        hboxTextos = new QHBoxLayout();
        hboxTextos->setSpacing(6);
        hboxTextos->setObjectName("hboxTextos");
        hboxTextos->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        txtCargado = new QTextEdit(centralWidget);
        txtCargado->setObjectName("txtCargado");

        hboxTextos->addWidget(txtCargado);

        txtConvertido = new QTextEdit(centralWidget);
        txtConvertido->setObjectName("txtConvertido");

        hboxTextos->addWidget(txtConvertido);

        hboxTextos->setStretch(0, 1);
        hboxTextos->setStretch(1, 1);

        mainView->addLayout(hboxTextos);

        hboxBotones = new QHBoxLayout();
        hboxBotones->setSpacing(6);
        hboxBotones->setObjectName("hboxBotones");
        btnCargar = new QPushButton(centralWidget);
        btnCargar->setObjectName("btnCargar");

        hboxBotones->addWidget(btnCargar);

        btnConvertir = new QPushButton(centralWidget);
        btnConvertir->setObjectName("btnConvertir");

        hboxBotones->addWidget(btnConvertir);

        btnGuardar = new QPushButton(centralWidget);
        btnGuardar->setObjectName("btnGuardar");

        hboxBotones->addWidget(btnGuardar);


        mainView->addLayout(hboxBotones);


        verticalLayout_2->addLayout(mainView);

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
        lbTitulo->setText(QCoreApplication::translate("MainViewClass", "Convertidor de lenguaje natural", nullptr));
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
