#include "mainwindow.h"
#include "StartWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StartWindow s;
    MainWindow w;

    // 点击开始 → 显示游戏窗口
    QObject::connect(&s, &StartWindow::startGameClicked, [&](){
        w.show();
    });

    s.show();     // 先显示开始界面
    return a.exec();
}
