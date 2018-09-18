#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QImage>
#include <QFile>
#include "HttpRequest.h"
#include "Password.h"
#include "Reciver.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setAppkey();
    QString getInput(QString);
    bool checkAppkey();

public slots:
    void onMessage(QJsonDocument);

private slots:
    void on_ui_query_clicked();

    void on_pushButton_clicked();

    void on_pb_deploy_clicked();

    void on_te_code_textChanged();

    void on_radioButton_toggled(bool checked);

    void on_radioButton_3_toggled(bool checked);

    void on_pb_next_clicked();

    void on_pb_prev_clicked();

    void on_pushButton_2_clicked();
    void on_pb_image_clicked();

private:
    int basePort;
    Ui::MainWindow *ui;
    Password passwd;
    Reciver *rc;
    QString contractHead;
    QString contractBase;
    QString contractErc20;
};

#endif // MAINWINDOW_H
