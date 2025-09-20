#pragma once

#include <QMainWindow>
#include "ui_MainView.h"

class MainView : public QMainWindow
{
	Q_OBJECT

public:
	MainView(QWidget *parent = nullptr);
	~MainView();

private:
	Ui::MainViewClass ui;
};

