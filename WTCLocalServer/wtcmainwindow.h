#ifndef WTCMAINWINDOW_H
#define WTCMAINWINDOW_H

#include <QMainWindow>
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

private:
    Ui::WTCMainWindow *ui;
    NClientInterface interface;
};

#endif // WTCMAINWINDOW_H
