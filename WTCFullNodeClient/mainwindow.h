#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
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

  void on_actionDefault_File_triggered();

  void on_RefreshMemberList(QStringList list);

private:
  Ui::MainWindow *ui;
  NP2PNode node;
};

#endif // MAINWINDOW_H
