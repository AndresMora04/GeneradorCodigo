#pragma once

#include <QMainWindow>
#include "ui_LogIn.h"
#include "MainView.h"  

class LogIn : public QMainWindow
{
    Q_OBJECT

public:
    LogIn(QWidget* parent = nullptr);
    ~LogIn();

private slots:
    void onIngresar();   

private:
    Ui::LogInView ui;
};