#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "mainnetserver.h"
//#include "netsync.h"

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
  void on_sendBtn_clicked();
  void on_RefreshMemberList(QStringList list);
  void on_RcvMessage(QString msg);

  void on_actionRequire_NAT_triggered();

  void on_actionOnnRequire_triggered();

  void OnRcvOnnRequire(QString contractID, QString addr, QString cmd, QString data);

  void on_actionStart_triggered();

  void on_actionCheck_triggered();

private:
  Ui::MainWindow *ui;
  MainNetServer node;
  //NetSync sync;
};

#endif // MAINWINDOW_H
