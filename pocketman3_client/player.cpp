#include "player.h"

player::player() {}

player::player(QByteArray &b)
{
    int pos = 0;
    get(b,pos);
    name = get(b,pos);
    password = get(b,pos);
    winNum = get(b,pos).toInt();
    failNum = get(b,pos).toInt();
    petNum = get(b,pos).toInt();
    highPetNum = get(b,pos).toInt();
    while(pos < b.size()){
        int n = pos;
        QString temp = get(b,n);
        pet *p;
        if(temp == petName[0]){
            p = new smallFireDragon(b,pos);
        }
        else if(temp == petName[1]){
            p = new bigStone(b,pos);
        }
        else if(temp == petName[2]){
            p = new oldTree(b,pos);
        }
        else if(temp == petName[1]){
            p = new pikachu(b,pos);
        }
        petOfPlayer.push_back(p);
    }

}

player::~player()
{
    for(unsigned int i = 0 ; i < petOfPlayer.size() ; i++){
        delete petOfPlayer[i];
    }
}

void player::addPet(pet *p)
{
    petOfPlayer.push_back(p);
}

void player::getPlayerInfo(QByteArray &b, int &pos)
{
    name = get(b,pos);
    password = get(b,pos);
    winNum = get(b,pos).toInt();
    failNum = get(b,pos).toInt();
    petNum = get(b,pos).toInt();
    highPetNum = get(b,pos).toInt();

    pet *p;
    while(b[pos] != '#'){
        int n = pos;
        get(b,n);    //get name
        //kind
        switch(get(b,n).toInt()){
        case 0:
            p = new smallFireDragon(ATK,"smallFireDragon",lvInitial);
            break;
        case 1:
            p = new bigStone(DEF,"bigStone",lvInitial);
            break;
        case 2:
            p = new oldTree(HP,"oldTree",lvInitial);
            break;
        case 3:
            p = new pikachu(LOW_INTERVAL,"pikachu",lvInitial);
            break;
        default:
            break;
        }
        p->getPetInfo(b,pos);
        petOfPlayer.push_back(p);
    }
}

void player::getOtherPlayer(QByteArray &b , int &pos)
{

    name = get(b,pos);

    winNum = get(b,pos).toInt();
    failNum = get(b,pos).toInt();
    petNum = get(b,pos).toInt();
    highPetNum = get(b,pos).toInt();

    //QString temp;
    pet *p;
    while(b[pos] != '#'){
        int n = pos;
        get(b,n);    //get name
        //kind
        switch(get(b,n).toInt()){
        case 0:
            p = new smallFireDragon(ATK,"smallFireDragon",lvInitial);
            break;
        case 1:
            p = new bigStone(DEF,"bigStone",lvInitial);
            break;
        case 2:
            p = new oldTree(HP,"oldTree",lvInitial);
            break;
        case 3:
            p = new pikachu(LOW_INTERVAL,"pikachu",lvInitial);
            break;
        default:
            break;
        }
        p->getPetInfo(b,pos);
        petOfPlayer.push_back(p);
    }
}

QString player::toQString()
{
    QString temp;
    temp.clear();

    temp += name;
    temp += ' ';
    temp += QString::number(winNum,10);
    temp += ' ';
    temp += QString::number(failNum,10);
    temp += ' ';
    temp += QString::number(petNum,10);
    temp += ' ';
    temp += QString::number(highPetNum,10);
    temp += ' ';
    for(unsigned int i = 0 ; i < petOfPlayer.size() ; ++i){
        temp += petOfPlayer[i]->toQString();
        temp += ' ';
    }
    temp += "# ";
    return temp;
}









