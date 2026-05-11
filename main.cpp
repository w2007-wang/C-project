#include "mainwindow.h"
#include "StartWindow.h"
#include "ResultWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow s;
    MainWindow w;

    QObject::connect(&s, &StartWindow::startGameClicked, [&](){
        if (s.isMaximized()) {
            w.showMaximized();
        } else {
            w.show();
        }
    });

    QObject::connect(&w, &MainWindow::returnToStart, [&](){
        s.show();
    });

    QObject::connect(&w, &MainWindow::showResult, [&](int winner){
        w.hide();
        ResultWindow *r = new ResultWindow(winner);
        if (w.isMaximized()) {
            r->showMaximized();
        } else {
            r->show();
        }
        QObject::connect(r, &ResultWindow::restartGame, [&, r](){
            w.restartGame();
            if (r->isMaximized()) {
                w.showMaximized();
            } else {
                w.show();
            }
            r->deleteLater();
        });
        QObject::connect(r, &ResultWindow::returnToStart, [&, r](){
            s.show();
            r->deleteLater();
        });
    });

    s.show();
    return a.exec();
}
