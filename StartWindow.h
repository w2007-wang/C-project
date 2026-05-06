#ifndef STARTWINDOW_H
#define STARTWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
<<<<<<< HEAD
#include <QPixmap>
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc

class StartWindow : public QWidget
{
    Q_OBJECT
public:
    explicit StartWindow(QWidget *parent = nullptr);

<<<<<<< HEAD
protected:
    void paintEvent(QPaintEvent *event) override;

=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
signals:
    void startGameClicked();

private slots:
    void onStartClicked();
    void onExitClicked();
<<<<<<< HEAD

private:
    QPixmap m_background;
=======
>>>>>>> 668665c50b6398872617da10f73688d7bf3592fc
};

#endif
