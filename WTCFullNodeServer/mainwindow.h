#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "p2pfullnodenetwork.h"

namespace Ui {
  class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:
  void OnNewConnect();

  void on_actionSelfTest_triggered();

  void on_bindBtn_clicked();

private:
  Ui::MainWindow *ui;
  P2PFullNodeNetwork fullNetwork;
};

#endif // MAINWINDOW_H
