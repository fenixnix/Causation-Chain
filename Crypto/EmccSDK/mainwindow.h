#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include "nemcc.h"

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
  void on_btnGenKeyPair_clicked();

  void on_btnSign_clicked();

  void on_btnVerify_clicked();

private:
  Ui::MainWindow *ui;
  NEmcc emcc;
};

#endif // MAINWINDOW_H
