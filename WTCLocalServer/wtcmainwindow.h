#ifndef WTCMAINWINDOW_H
#define WTCMAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "ntimesync.h"
#include "nclientinterface.h"

namespace Ui {
class WTCMainWindow;
}

class WTCMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit WTCMainWindow(QWidget *parent = 0);
    ~WTCMainWindow();

private slots:
    void on_actionSend_Causue_triggered();

    void on_actionSend_Result_triggered();

    void on_actionRefresh_triggered();

    void onP2pMsg(QString addr, QString msg);

    void on_actionInit_Game_triggered();

    void on_tickBtn_clicked();

    void on_tstTickBtn_clicked();

    void on_actionJoin_Game_triggered();

    void on_actionClose_Game_triggered();

    void on_actionLoad_Game_triggered();

private:
    Ui::WTCMainWindow *ui;
    NClientInterface interface;
};

#endif // WTCMAINWINDOW_H
