#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <iostream>
#include <string>
#include "ui_MainView.h"
#include "CodeConverter.h"

using namespace std;

class MainView : public QMainWindow
{
    Q_OBJECT

public:
    MainView(QWidget* parent = nullptr);
    ~MainView();

private slots:
    void onLoadFile();
    void onConvert();
    void onSaveFile();

private:
    Ui::MainViewClass ui;
    CodeConverter converter;
};