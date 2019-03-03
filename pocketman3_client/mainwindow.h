#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <pet.h>
#include <QMessageBox>
#include <QDebug>
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
    bool fighting();
    void showInformation();
    bool hasIllegal(QString s);

private slots:
    void socket_disconnected();

    void on_loginIn_clicked();
    void on_registe_clicked();

    void readData();

    void on_pushButton_next_1_clicked();

    void on_pushButton_next_2_clicked();

    void on_pushButton_loginOut_clicked();


    void on_pushButton_lvUpBattle_clicked();

    void on_pushButton_battle_clicked();

    void on_pushButton_throwName1_clicked();

    void on_pushButton_throwName2_clicked();

    void on_pushButton_throwName3_clicked();


    void on_pushButton_lvUp_clicked();


private:
    Ui::MainWindow *ui;
    QTcpSocket *socket;

    QString IP;
    int port;

    int select;
    unsigned int nowPet;
    player user;
    pet *p;

};


#endif // MAINWINDOW_H
