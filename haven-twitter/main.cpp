#include "mainwindow.h"

#include <QApplication>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    MainWindow w;

    w.showSplash();
    w.show();

    return a.exec();
}
