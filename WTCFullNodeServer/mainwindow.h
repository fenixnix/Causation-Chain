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

private:
  Ui::MainWindow *ui;
  P2PFullNodeNetwork fullNetwork;
};

#endif // MAINWINDOW_H
