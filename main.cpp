#include "mainwindow.h"
#include "StartWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow s;
    MainWindow w;

    QObject::connect(&s, &StartWindow::startGameClicked, [&](){
        w.show();
    });

    QObject::connect(&w, &MainWindow::returnToStart, [&](){
        s.show();
    });

    s.show();
    return a.exec();
}
