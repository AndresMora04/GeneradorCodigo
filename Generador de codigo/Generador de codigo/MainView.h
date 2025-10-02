#pragma once

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QStyle>
#include <iostream>
#include <string>
#include "ui_MainView.h"
#include "CodeConverter.h"
#include "LogIn.h"

using namespace std;

class LogIn;

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
    void onBackToLogin();
	void ensureProjectStructure(const QString& folderPath);

protected:
	void closeEvent(QCloseEvent* event) override;

private:
    Ui::MainViewClass ui;
    CodeConverter converter;
	LogIn* loginWindow = nullptr;
};