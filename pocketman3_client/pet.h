#ifndef PET_H
#define PET_H

#include"basepet.h"

class smallFireDragon: public pet
{
    public:
        smallFireDragon(petKind , QString , int);
        smallFireDragon(QByteArray &b , int &pos);
        virtual void performAbilities(QTextBrowser *p , pet &);
};


class pikachu: public pet
{
    public:
        pikachu(petKind , QString , int);
        pikachu(QByteArray &b , int &pos);
        virtual void performAbilities(QTextBrowser *p, pet &attacked);
};

class bigStone: public pet
{
    public:
        bigStone(petKind , QString , int);
        bigStone(QByteArray &b , int &pos);
        virtual void performAbilities(QTextBrowser *p , pet &);
};

class oldTree: public pet
{
    public:
        oldTree(petKind , QString , int);
        oldTree(QByteArray &b , int &pos);
        virtual void performAbilities(QTextBrowser *p , pet &);
};

#endif // PET_H







