#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Board.h"
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void chooseModel();
    void slot_endGameBox();
    void endGameBox(QString msg);
    void undo();
    void gamePlayTimeout();
    void userChanged();
    void reset();
private:
    Ui::MainWindow *ui;
    Board* m_board{};
    QTimer gamePlayTimer;
    int user1Count = 10;
    int user2Count = 10;
};



#endif // MAINWINDOW_H
