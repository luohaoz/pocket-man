#include "mainwindow.h"

//produce a wild pet randomly
pet::pet(petKind _kind, QString , int _lv)
{
    kind = _kind;
    //name = _name;

    //choose grade randomly
    int gradeDecide = random(1, 100);
    if(gradeDecide == 100)
        grade = GOLDEN;
    else if(gradeDecide >= 95)
        grade = MAGENTA;
    else if(gradeDecide >= 70)
        grade = BLUE;
    else if(gradeDecide >= 40)
        grade = GREEN;
    else
        grade = GRAY;

    lv = _lv;
    xp = 0;
    xpMax = xpMaxInitial;

    atk = lv * atkRate_lv + grade * lv * atkRate_grade;
    def = lv * defRate_lv + grade * lv * defRate_grade;
    hp = lv * hpRate_lv + grade * lv * hpRate_grade;
    atkInterval = 1000 - lv * intervalRate_lv - grade * lv * intervalRate_grade;
    missRate = 10 + grade;

    //strengthen it's advantage according to kind
    switch(kind){
    case ATK:
        atk += lv * atkRate_lv;
        break;
    case DEF:
        def += lv * defRate_lv;
        break;
    case HP:
        hp += lv * hpRate_lv;
        break;
    default:
        atkInterval = atkInterval - lv * intervalRate_lv;
        break;
    }  //end of switch(kind)
}

pet::pet(QByteArray &b, int &pos)
{
    get(b,pos);
    kind = petKind(get(b,pos).toInt());
    grade = petGrade(get(b,pos).toInt());
    lv = get(b,pos).toInt();
    xp = get(b,pos).toInt();

    xpMax = xpMaxInitial;

    atk = lv * atkRate_lv + grade * lv * atkRate_grade;
    def = lv * defRate_lv + grade * lv * defRate_grade;
    hp = lv * hpRate_lv + grade * lv * hpRate_grade;
    atkInterval = 1000 - lv * intervalRate_lv - grade * lv * intervalRate_grade;
    missRate = 10 + grade;

    //strengthen it's advantage according to kind
    switch(kind){
    case ATK:
        atk += lv * atkRate_lv;
        break;
    case DEF:
        def += lv * defRate_lv;
        break;
    case HP:
        hp += lv * hpRate_lv;
        break;
    default:
        atkInterval = atkInterval - lv * intervalRate_lv;
        break;
    }  //end of switch(kind)
}

pet::~pet()
{

}


void pet::lvUp()
{
    if(xp >= xpMax){
        if(lv < lvUpperBound){
            lv++;
            xp = xp - xpMax;
            xpMax = xpMax * xpRate;

            atk = lv * atkRate_lv + grade * lv * atkRate_grade;
            def = lv * defRate_lv + grade * lv * defRate_grade;
            hp = lv * hpRate_lv + grade * lv * hpRate_grade;
            atkInterval = 1000 - lv * intervalRate_lv - grade * lv * intervalRate_grade;
            missRate = 10 + grade;

            switch(kind){
            case ATK:
                atk += lv * atkRate_lv;
                break;
            case DEF:
                def += lv * defRate_lv;
                break;
            case HP:
                hp += lv * hpRate_lv;
                break;
            default:
                atkInterval = atkInterval - lv * intervalRate_lv;
                break;
            }  //end of switch(kind)
        }
    }
}

//when a wild pet is caught, use this to make it's lv = 1
void pet::lvInitialize()
{
    lv = lvInitial;
    xp = 0;
    xpMax = xpMaxInitial;

    atk = lv * atkRate_lv + grade * lv * atkRate_grade;
    def = lv * defRate_lv + grade * lv * defRate_grade;
    hp = lv * hpRate_lv + grade * lv * hpRate_grade;
    atkInterval = 1000 - lv * intervalRate_lv - grade * lv * intervalRate_grade;
    missRate = 10 + grade;

    switch(kind){
    case ATK:
        atk += lv * atkRate_lv;
        break;
    case DEF:
        def += lv * defRate_lv;
        break;
    case HP:
        hp += lv * hpRate_lv;
        break;
    default:
        atkInterval = atkInterval - lv * intervalRate_lv;
        break;
    }  //end of switch(kind)
}

/*
void pet::gradeUp()
{
    grade = (enum petGrade) (grade + 1);
    atk += lv * atkRate_grade;
    def += lv * defRate_grade;
    hp += lv * hpRate_grade;
    atkInterval -= lv * intervalRate_grade;
    missRate += grade;
}
*/

void pet::fightStart()
{
    hpNow = hp;
    //mpNow = mp;
    atkNow = atk;
    defNow = def;
    atkIntervalNow = atkInterval;
    missRateNow = missRate;
    attackTime = 0;
}

void pet::fightEnd(int experience)
{
    xp += experience;
}

void pet::beAttacked(QTextBrowser *p , const pet &attacker)
{
    int miss = random(1, 100);
    if(miss > missRateNow){
        int damage = attacker.atkNow - defNow;
        if(hpNow > damage){
            hpNow -= damage;
            p->append(name + " is attacked by " + attacker.name + " , hp - " + QString::number(damage,10));
            //cout<<name<<"is attacked by "<<attacker.name<<", hp - "<<damage<<endl;
        }
        else{
            hpNow = 0;
            p->append(name + " is attacked by " + attacker.name + " , hp - " + QString::number(damage,10));
            p->append(name + " is dead !");
            //cout<<name<<"is attacked by "<<attacker.name<<", "<<name<<" is dead !"<<endl;
        }
    }
    else{
        p->append(name + "is attacked by " + attacker.name + " , Miss !");
        //cout<<name<<"is attacked by "<<attacker.name<<", Miss !"<<endl;
    }
}

QString pet::toQString()
{
    QString temp;
    temp += name;
    temp += ' ';
    temp += QString::number(kind,10);
    temp += ' ';
    temp += QString::number(grade,10);
    temp += ' ';
    temp += QString::number(lv,10);
    temp += ' ';
    temp += QString::number(xp,10);
    return temp;
}

void pet::getPetInfo(QByteArray &b, int &pos)
{
    name = get(b,pos);
    kind = petKind(get(b,pos).toInt());
    grade = petGrade(get(b,pos).toInt());
    lv = get(b,pos).toInt();
    xp = get(b,pos).toInt();

    xpMax = xpMaxInitial;
    atk = lv * atkRate_lv + grade * lv * atkRate_grade;
    def = lv * defRate_lv + grade * lv * defRate_grade;
    hp = lv * hpRate_lv + grade * lv * hpRate_grade;
    atkInterval = 1000 - lv * intervalRate_lv - grade * lv * intervalRate_grade;
    missRate = 10 + grade;

    //strengthen it's advantage according to kind
    switch(kind){
    case ATK:
        atk += lv * atkRate_lv;
        break;
    case DEF:
        def += lv * defRate_lv;
        break;
    case HP:
        hp += lv * hpRate_lv;
        break;
    default:
        atkInterval = atkInterval - lv * intervalRate_lv;
        break;
    }  //end of switch(kind)

}


void pet::print(QPainter &p, int x ,int y){
    QFont font("Arial",16,QFont::Bold,true);
    font.setUnderline(false);
    font.setOverline(false);
    font.setCapitalization(QFont::SmallCaps);
    font.setLetterSpacing(QFont::AbsoluteSpacing,3);
    p.setFont(font);

    int length = 200;
    int width = 50;
    QString temp;

    //print name with grade
    temp.clear();
    temp += "名称:";
    temp += name;
    QRectF ff1(x,y,length,width);
    p.drawRect(ff1);

    switch(grade){
    case GRAY:
        p.setPen(QColor(Qt::gray));
        break;
    case GREEN:
        p.setPen(QColor(Qt::green));
        break;
    case BLUE:
        p.setPen(QColor(Qt::blue));
        break;
    case MAGENTA:
        p.setPen(QColor(Qt::magenta));
        break;
    case GOLDEN:
        p.setPen(QColor(Qt::yellow));
        break;
    default:
        break;
    }
    p.drawText(ff1,Qt::AlignCenter,temp);
    p.setPen(QColor(Qt::black));

    //print kind
    temp.clear();
    temp += "种类:";
    QRectF ff2(x,y + width,length,width);
    p.drawRect(ff2);
    switch(kind){
    case ATK:
        temp += "高攻击";
        break;
    case DEF:
        temp += "高防御";
        break;
    case HP:
        temp += "高血量";
        break;
    case LOW_INTERVAL:
        temp += "高敏捷";
        break;
    default:
        temp += "error";
        break;
    }
    p.drawText(ff2,Qt::AlignCenter,temp);


    //print lv
    temp.clear();
    temp += "等级:";
    temp += QString::number(lv,10);
    QRectF ff3(x,y + width * 2,length,width);
    p.drawRect(ff3);
    p.drawText(ff3,Qt::AlignCenter,temp);

    //print xp / xpMax
    temp.clear();
    temp += "经验:";
    temp += QString::number(xp,10);
    temp += "/";
    temp += QString::number(xpMax,10);
    QRectF ff4(x,y + width * 3,length,width);
    p.drawRect(ff4);
    p.drawText(ff4,Qt::AlignCenter,temp);

    //print hp
    temp.clear();
    temp += "hp:";
    temp += QString::number(hp,10);
    QRectF ff5(x,y + width * 4,length,width);
    p.drawRect(ff5);
    p.drawText(ff5,Qt::AlignCenter,temp);

    //print atk
    temp.clear();
    temp += "攻击力:";
    temp += QString::number(atk,10);
    QRectF ff6(x,y + width * 5,length,width);
    p.drawRect(ff6);
    p.drawText(ff6,Qt::AlignCenter,temp);

    //print def
    temp.clear();
    temp += "防御力:";
    temp += QString::number(def,10);
    QRectF ff7(x,y + width * 6,length,width);
    p.drawRect(ff7);
    p.drawText(ff7,Qt::AlignCenter,temp);

    //print atk interval
    temp.clear();
    temp += "攻击间隔:";
    temp += QString::number(atkInterval,10);
    QRectF ff8(x,y + width * 7,length,width);
    p.drawRect(ff8);
    p.drawText(ff8,Qt::AlignCenter,temp);

}


smallFireDragon::smallFireDragon(petKind _kind, QString _name, int _lv):pet(_kind, _name, _lv) {
    name = petName[0];
}
smallFireDragon::smallFireDragon(QByteArray &b , int &pos):pet(b,pos) {
    name = petName[0];
}
void smallFireDragon::performAbilities(QTextBrowser *p, pet &)
{
    changeAtkNow(getAtkNow() + atkRate_lv);
    p->append("smallFireDragon uses [atk improve] , atk + " + QString::number(atkRate_lv,10));
}

pikachu::pikachu(petKind _kind , QString _name , int _lv):pet(_kind , _name , _lv) {
    name = petName[3];
}
pikachu::pikachu(QByteArray &b , int &pos):pet(b,pos) {
    name = petName[3];
}
void pikachu::performAbilities(QTextBrowser *p , pet &attacked)
{
    p->append("pikachu uses [lighten] , causes " + QString::number(getAtk() * 2,10) + " damage");
    if(attacked.getHpNow() > getAtk() * 2){
        attacked.changeHpNow(attacked.getHpNow() - getAtk() * 2);
    }
    else{
        attacked.changeHpNow(0);
        p->append(attacked.getName() + " is dead !");
    }

}

bigStone::bigStone(petKind _kind , QString _name , int _lv):pet(_kind , _name , _lv) {
    name = petName[1];
}
bigStone::bigStone(QByteArray &b , int &pos):pet(b,pos) {
    name = petName[1];
}
void bigStone::performAbilities(QTextBrowser *p, pet &)
{
    changeDefNow(getDefNow() + defRate_lv);
    p->append("bigStone uses def improve , def + " + QString::number(defRate_lv,10));
}

oldTree::oldTree(petKind _kind , QString _name , int _lv):pet(_kind , _name , _lv) {
    name = petName[2];
}
oldTree::oldTree(QByteArray &b , int &pos):pet(b,pos) {
    name = petName[2];
}
void oldTree::performAbilities(QTextBrowser *p, pet &)
{
    changeHpNow(getHpNow() + 20);
    p->append("oldTree uses hp recover , hp + 20!");
}
