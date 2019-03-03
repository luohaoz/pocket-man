#ifndef CONSTDEFINE_H
#define CONSTDEFINE_H

#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include<time.h>
#include<QString>
#include<QPainter>

using namespace std;

enum petKind
{
    ATK, DEF, HP, LOW_INTERVAL
};

enum petGrade
{
    GRAY, GREEN, BLUE, MAGENTA, GOLDEN
};

const int lvInitial = 1;
const int xpMaxInitial = 100;
const int lvUpperBound = 15;

const int atkRate_lv = 30;
const int atkRate_grade = 6;
const int defRate_lv = 10;
const int defRate_grade = 2;
const int hpRate_lv = 100;
const int hpRate_grade = 20;
const int intervalRate_lv = 20;
const int intervalRate_grade = 5;
const int xpRate = 1.1;
const int performRate = 10;

const int petSumNumber = 4;
const int petOfEveryKind = 1;
const QString petName[petSumNumber] =
{
    "smallFireDragon", "bigStone", "oldTree", "pikachu"
};

const QString userNameIsIn = "usernameisin";
const QString registeSuccessful = "registesuccessful";
const QString loginFailed = "logininfailed";
const QString loginSuccessful = "logininsuccessful";

#endif // CONSTDEFINE_H
