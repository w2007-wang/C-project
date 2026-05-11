#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

class ResultWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWindow(int winner, QWidget *parent = nullptr);

signals:
    void restartGame();
    void returnToStart();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int m_winner;
};

#endif