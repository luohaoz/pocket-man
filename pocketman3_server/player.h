#ifndef PLAYER_H
#define PLAYER_H

#include "pet.h"
#include <QDebug>

class player{
private:
    QString name;
    QString password;
    int winNum;
    int failNum;
    int petNum;
    int highPetNum;
    vector<pet *> petOfPlayer;

public:
    player(QByteArray &b);
    ~player();

    void addPet(pet *);
    void getPlayerInfo(QByteArray &b , int &pos);
    void getOtherPlayer(QByteArray &b , int &pos);
    QString toQString();

    void win() { winNum++; } ;
    void fail() { failNum++; } ;
    void changePetNum(int num) { petNum = num; } ;
    QString getName() const { return name; } ;
    QString getPassword() const { return password; } ;
    int getWinNum() const { return winNum; } ;
    int getFailNum() const { return failNum; } ;
    int getPetNum() const { return petNum; } ;
    int getHighPetNUm() const { return highPetNum; } ;


};

#endif // PLAYER_H
