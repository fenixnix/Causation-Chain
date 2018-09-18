#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QFile>

#include "onnrequest.h"
#include "nhttprequest.h"
#include "necc.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void GetInfo(QString addr);

private slots:
    void on_actionRecharge_triggered();

    void on_pushButton_clicked();

    void OnRcvInfo(QString msg);

private:
    Ui::MainWindow *ui;
    NEcc ecc;
    OnnRequest onn;
    NHttpRequest* http;
};

#endif // MAINWINDOW_H
