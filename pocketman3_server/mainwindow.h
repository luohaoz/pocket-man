#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <windows.h>
#include "database.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void newConnect();
    void socketReadData();
    void socketDisconnected();

    Database playerDatabase;
    vector<player*> inLinePlayer;

private:
    Ui::MainWindow *ui;

    QTcpServer *server;
    QTcpSocket *socket;

    int port;
};

QString get(const QByteArray &b , int &pos);
bool nameIsInDatabase(QByteArray &b , Database &playerDatabase);
bool infoIsInDatabase(QByteArray &b , Database &playerDatabase);

#endif // MAINWINDOW_H
