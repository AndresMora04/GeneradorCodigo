
//#include <QApplication>
//#include "LogIn.h"
//
//int main(int argc, char* argv[])
//{
//    QApplication a(argc, argv);
//
//    // Abrir primero la pantalla de LogIn
//    LogIn login;
//    login.show();
//
//    return a.exec();
//}




#include <iostream>
using namespace std;
#include "MainView.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MainView w;
	w.show();
	return a.exec();
}
