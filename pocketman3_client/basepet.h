#ifndef BASEPET_H
#define BASEPET_H

#include <QTextBrowser>
#include"functiondefine.h"

class pet
{
    private:
        petKind kind;
        petGrade grade;
        int lv;
        int xp;
        int xpMax;
        int hp;
        int atk;
        int def;
        int atkInterval;
        int missRate;

        //when figiting
        int hpNow;
        int atkNow;
        int defNow;
        int atkIntervalNow;
        int missRateNow;
        int attackTime;


    public:
        pet(petKind _kind, QString, int _lv);
        pet(QByteArray &b , int &pos);
        virtual ~pet();

        petKind getKind() const { return kind; };
        petGrade getGrade() const { return grade; };
        QString getName() const { return name; };
        int getLv() const { return lv; };
        int getXp() const { return xp; };
        int getXpMax() const { return xpMax; };
        int getHp() const { return hp; };
        int getAtk() const { return atk; };
        int getDef() const { return def; };
        int getAtkInterval() const { return atkInterval; };

        int getHpNow() const { return hpNow; };
        int getAtkNow() const { return atkNow; };
        int getDefNow() const { return defNow; };
        int getAtkIntervalNow() const { return atkIntervalNow; };
        int getMissRateNow() const { return missRateNow; };
        int getAttackTime() const { return attackTime; };

        void changeAtkNow(int _atk) { atkNow = _atk; };
        void changeDefNow(int _def) { defNow = _def; };
        void changeHpNow(int _hp) { hpNow = _hp; };
        void changeAtkIntervalNow(int _atkInterval) { atkIntervalNow = _atkInterval; };

        void lvUp();
        void lvInitialize();
        //void gradeUp();
        void fightStart();
        void fightEnd(int experience);
        void beAttacked(QTextBrowser *p , const pet &attacker);
        void print(QPainter &p ,int x ,int y);

        QString toQString();
        void getPetInfo(QByteArray &b , int &pos);
        //void printInFighting(QPainter &p ,int x ,int y);

        virtual void performAbilities(QTextBrowser *p, pet &attacked) = 0;

    protected:
        QString name;
};

#endif // BASEPET_H
