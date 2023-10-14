#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    int r =  app.exec();

    GlobalProcess::deleteInstance();

    return r;
}
