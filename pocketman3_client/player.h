#ifndef PLAYER_H
#define PLAYER_H

#include "pet.h"

class player{
private:
    QString name;
    QString password;
    int winNum;
    int failNum;
    int petNum;
    int highPetNum;

public:
    player();
    player(QByteArray &b);
    ~player();

    vector<pet *> petOfPlayer;

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
