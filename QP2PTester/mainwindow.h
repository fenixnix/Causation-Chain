#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private Q_SLOTS:
    void OnCUDP();

    void on_pushButton_clicked();

    void on_btnP2PSend_clicked();

private:
    Ui::MainWindow *ui;
    QUdpSocket *udp;
    QString rcvBuffer;
};

#endif // MAINWINDOW_H
