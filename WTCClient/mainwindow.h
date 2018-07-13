#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include "np2pnode.h"
#include "ncryptop2p.h"
#include "netsync.h"
#include "ncausationconsensus.h"

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

  void on_actionRequire_All_triggered();

  void on_actionRequire_NAT_triggered();

private:
  Ui::MainWindow *ui;
  NP2PNode node;
  //NetSync sync;
  NCausationConsensus consensus;
};

#endif // MAINWINDOW_H
