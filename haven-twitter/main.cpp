#include <QApplication>
#include <QCoreApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName("haven-twitter");
    QCoreApplication::setApplicationVersion("0.1");

    QApplication a(argc, argv);

    MainWindow w;
    w.showSplash();
    w.show();

    return a.exec();
}
