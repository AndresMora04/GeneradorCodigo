#include <QApplication>
#include "LogIn.h"
#include <QFile>
#include <QDebug>

static void loadAppStyleSheet() {
    Q_INIT_RESOURCE(resources);

    QFile f(":/styles/theme.qss");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    qApp->setStyle("Fusion");
    qApp->setStyleSheet(QString::fromUtf8(f.readAll()));
    f.close();
}


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    loadAppStyleSheet();

    LogIn w;
    w.setObjectName("LogInView");
    w.show();
    return a.exec();
}