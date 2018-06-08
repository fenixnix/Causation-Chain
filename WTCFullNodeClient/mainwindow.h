#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "np2pnode.h"

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
  void on_actionSelfTest_triggered();

private:
  Ui::MainWindow *ui;
  NP2PNode node;
};

#endif // MAINWINDOW_H
