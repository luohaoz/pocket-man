#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    socket = new QTcpSocket();

    //连接信号槽
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::readData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socket_disconnected);

    ui->userPetPic_1->setScaledContents(true);

    IP = "127.0.0.1";
    port = 6666;
    nowPet = 0;

    setFixedSize(1200,800);
}

MainWindow::~MainWindow()
{
    delete this->socket;
    delete ui;
}

//连接断开
void MainWindow::socket_disconnected()
{
    socket->close();
}


//判断输入是否合法
bool MainWindow::hasIllegal(QString s)
{
    for(int i = 0 ; i < s.size() ; i++){
        if((s[i] >= 'a' && s[i] <= 'z')
         ||(s[i] >= 'A' && s[i] <= 'Z')
         ||(s[i] >= '0' && s[i] <= '9')){

        }
        else{
            return true;
        }
    }
    return false;
}


//登录按钮
void MainWindow::on_loginIn_clicked()
{
    socket->connectToHost(IP,port);
    if(!socket->waitForConnected(30000)){
        qDebug() << "Connection Failed !";
    }

    QByteArray sum;
    sum.clear();
    sum.append('l');

    QByteArray inName;
    QByteArray inPassword;

    inName.append(ui->name->text());
    inPassword.append(ui->password->text());

    if(hasIllegal(inName) || hasIllegal(inPassword)){
        QMessageBox::about(this,"输入非法","用户名和密码仅能由字母和数字组成！");
    }
    else{
        sum.append(inName);
        sum.append(' ');
        sum.append(inPassword);

        socket->write(sum);
    }
}

//注册按钮
void MainWindow::on_registe_clicked()
{
    socket->connectToHost(IP,port);
    if(!socket->waitForConnected(30000)){
        qDebug() << "Connection Failed !";
    }

    QByteArray sum;
    sum.clear();
    sum.append('r');

    QByteArray inName;
    QByteArray inPassword;

    inName.append(ui->name->text());
    inPassword.append(ui->password->text());

    if(hasIllegal(inName) || hasIllegal(inPassword)){
        QMessageBox::about(this,"非法输入","用户名和密码仅能由字母和数字组成！");
    }
    else{
        sum.append(inName);
        sum.append(' ');
        sum.append(inPassword);

        socket->write(sum);
    }
}


//读数据
void MainWindow::readData()
{
    QByteArray buffer;
    buffer = socket->readAll();
    //QMessageBox::about(this, QString(buffer), QString(buffer));
    if(!buffer.isEmpty()){
        int pos = 0;
        QString buf = get(buffer,pos);
        //注册时用户名已存在
        if(buf == userNameIsIn){
            qDebug() << "user name is in";
            QMessageBox::about(this,"warning","用户名已存在！");
        }
        //注册成功
        else if(buf == registeSuccessful){
            qDebug() << "registe successful";
            qDebug() << buffer;
            QMessageBox::about(this," ","注册成功！");
        }
        //登录时用户名不存在或密码错误，登录失败
        else if(buf == loginFailed){
            QMessageBox::about(this," ","用户名或密码错误！");
        }
        //登录成功
        else if(buf == loginSuccessful){
            takeCentralWidget();
            setCentralWidget(ui->users);

            user.getPlayerInfo(buffer,pos);
            get(buffer,pos);  //read a #
            int x = 0;
            ui->tableWidget->setRowCount(100);
            ui->tableWidget->setColumnCount(1);
            while(pos < buffer.size()){
                player p;
                p.getOtherPlayer(buffer,pos);
                get(buffer,pos);  //read a #
                ui->tableWidget->setItem(x,0,new QTableWidgetItem(p.toQString(),0));
                qDebug()<<p.toQString();
                x++;
            }
            showInformation();

        }

    }
    socket_disconnected();
}

//显示信息
void MainWindow::showInformation()
{
    //显示人物信息
    ui->lineEdit_userName->setText(user.getName());
    //显示win rate
    if(user.getWinNum() == 0 && user.getFailNum() == 0){
        ui->lineEdit_winRate->setText("winRate: 0%");
    }
    else{
        int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
        ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
    }
    if(user.getHighPetNUm() >= 5){
        ui->label_jiangbei->setText("金牌");
    }
    if(user.getHighPetNUm() >= 1){
        ui->label_jiangbei->setText("银牌");
    }
    else{
        ui->label_jiangbei->setText("铜牌");
    }

    //显示当前宠物信息
    ui->lineEdit_nowPet->setText(QString::number(nowPet+1,10) + "/" + QString::number(user.getPetNum(),10));
    if(user.petOfPlayer[nowPet]->getName() == petName[0]){
        ui->userPetPic_1->setPixmap(QPixmap("./pictures/smallFireDragon.jpg"));
    }
    else if(user.petOfPlayer[nowPet]->getName() == petName[1]){
        ui->userPetPic_1->setPixmap(QPixmap("./pictures/bigStone.jpg"));
    }
    else if(user.petOfPlayer[nowPet]->getName() == petName[2]){
        ui->userPetPic_1->setPixmap(QPixmap("./pictures/oldTree.jpg"));
    }
    else if(user.petOfPlayer[nowPet]->getName() == petName[3]){
        ui->userPetPic_1->setPixmap(QPixmap("./pictures/bigStone.jpg"));
    }
    switch(user.petOfPlayer[nowPet]->getGrade()){
    case GRAY:
        ui->lineEdit_name_1->setStyleSheet("color:gray");
        break;
    case GREEN:
        ui->lineEdit_name_1->setStyleSheet("color:green");
        break;
    case BLUE:
        ui->lineEdit_name_1->setStyleSheet("color:blue");
        break;
    case MAGENTA:
        ui->lineEdit_name_1->setStyleSheet("color:magenta");
        break;
    case GOLDEN:
        ui->lineEdit_name_1->setStyleSheet("color:yellow");
        break;
    default:
        break;
    }
    ui->lineEdit_name_1->setText(user.petOfPlayer[nowPet]->getName());
    ui->lineEdit_kind_1->setText("kind: " + QString::number(user.petOfPlayer[nowPet]->getKind(),10));
    ui->lineEdit_lv_1->setText("lv: " + QString::number(user.petOfPlayer[nowPet]->getLv(),10));
    ui->lineEdit_xp_1->setText("xp: " + QString::number(user.petOfPlayer[nowPet]->getXp(),10) + "/" + QString::number(user.petOfPlayer[nowPet]->getXpMax(),10));
    ui->lineEdit_hp_1->setText("hp: " + QString::number(user.petOfPlayer[nowPet]->getHp(),10));
    ui->lineEdit_atk_1->setText("atk: " + QString::number(user.petOfPlayer[nowPet]->getAtk(),10));
    ui->lineEdit_def_1->setText("def: " + QString::number(user.petOfPlayer[nowPet]->getDef(),10));
    ui->lineEdit_interval_1->setText("interval: " + QString::number(user.petOfPlayer[nowPet]->getAtkInterval(),10));
}

//选择查看玩家的下一个小精灵的按钮
void MainWindow::on_pushButton_next_1_clicked()
{
    nowPet++;
    if(nowPet >= user.petOfPlayer.size()){
        nowPet = 0;
    }

    showInformation();

}

//寻找对手按钮
void MainWindow::on_pushButton_next_2_clicked()
{
    int r = random(0,3);
    switch(r){
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

    //显示当前宠物信息
    if(p->getName() == petName[0]){
        ui->userPetPic_2->setPixmap(QPixmap("./pictures/smallFireDragon.jpg"));
    }
    else if(p->getName() == petName[1]){
        ui->userPetPic_2->setPixmap(QPixmap("./pictures/bigStone.jpg"));
    }
    else if(p->getName() == petName[2]){
        ui->userPetPic_2->setPixmap(QPixmap("./pictures/oldTree.jpg"));
    }
    else if(p->getName() == petName[3]){
        ui->userPetPic_2->setPixmap(QPixmap("./pictures/bigStone.jpg"));
    }
    switch(p->getGrade()){
    case GRAY:
        ui->lineEdit_name_2->setStyleSheet("color:gray");
        break;
    case GREEN:
        ui->lineEdit_name_2->setStyleSheet("color:green");
        break;
    case BLUE:
        ui->lineEdit_name_2->setStyleSheet("color:blue");
        break;
    case MAGENTA:
        ui->lineEdit_name_2->setStyleSheet("color:magenta");
        break;
    case GOLDEN:
        ui->lineEdit_name_2->setStyleSheet("color:yellow");
        break;
    default:
        break;
    }
    ui->lineEdit_name_2->setText(p->getName());
    ui->lineEdit_kind_2->setText("kind: " + QString::number(p->getKind(),10));
    ui->lineEdit_lv_2->setText("lv: " + QString::number(p->getLv(),10));
    ui->lineEdit_xp_2->setText("xp: " + QString::number(p->getXp(),10) + "/" + QString::number(user.petOfPlayer[nowPet]->getXpMax(),10));
    ui->lineEdit_hp_2->setText("hp: " + QString::number(p->getHp(),10));
    ui->lineEdit_atk_2->setText("atk: " + QString::number(p->getAtk(),10));
    ui->lineEdit_def_2->setText("def: " + QString::number(p->getDef(),10));
    ui->lineEdit_interval_2->setText("interval: " + QString::number(p->getAtkInterval(),10));

}


//退出按钮
void MainWindow::on_pushButton_loginOut_clicked()
{
    socket->connectToHost(IP,port);
    if(!socket->waitForConnected(30000)){
        qDebug() << "Connection Failed !";
    }
    nowPet = 0;
    QString temp;
    temp.clear();
    temp += "o";
    temp += user.toQString();

    socket->write(temp.toUtf8());

    //QMessageBox::about(this,"",temp);

    unsigned int x = user.petOfPlayer.size();
    for(unsigned int i = 0 ; i < x ; i++){
        user.petOfPlayer.pop_back();
    }

    takeCentralWidget();
    setCentralWidget(ui->signIn);


}


//升级赛按钮
void MainWindow::on_pushButton_lvUpBattle_clicked()
{
    if(fighting()){
        user.win();
    }
    else{
        user.fail();
    }
    ui->lineEdit_xp_1->setText("xp: " + QString::number(user.petOfPlayer[nowPet]->getXp(),10) + "/" + QString::number(user.petOfPlayer[nowPet]->getXpMax(),10));
    //显示win rate
    if(user.getWinNum() == 0 && user.getFailNum() == 0){
        ui->lineEdit_winRate->setText("winRate: 0%");
    }
    else{
        int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
        ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
    }
    //QMessageBox::about(this,"",QString::number(user.getWinNum(),10));
}

//决斗赛按钮
void MainWindow::on_pushButton_battle_clicked()
{
    //win
    if(fighting()){
        user.petOfPlayer.push_back(p);
        user.changePetNum(user.getPetNum()+1);
        user.win();

        //显示win rate
        if(user.getWinNum() == 0 && user.getFailNum() == 0){
            ui->lineEdit_winRate->setText("winRate: 0%");
        }
        else{
            int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
            ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
        }
        ui->lineEdit_nowPet->setText(QString::number(nowPet+1,10) + "/" + QString::number(user.getPetNum(),10));

    }
    //fail
    else{
        user.fail();
        takeCentralWidget();
        setCentralWidget(ui->throw_2);
        QMessageBox::about(this,"","please choose one to throw!");
        ui->pushButton_throwName1->setText(user.petOfPlayer[0]->getName());
        ui->lineEdit_throwLv1->setText(QString::number(user.petOfPlayer[0]->getLv(),10));
        ui->lineEdit_throwXp1->setText(QString::number(user.petOfPlayer[0]->getXp(),10));
        if(user.getPetNum() == 1){
            ui->pushButton_throwName2->setEnabled(false);
            ui->pushButton_throwName3->setEnabled(false);
        }
        else if(user.getPetNum() == 2){
            ui->pushButton_throwName2->setText(user.petOfPlayer[1]->getName());
            ui->lineEdit_throwLv2->setText(QString::number(user.petOfPlayer[1]->getLv(),10));
            ui->lineEdit_throwXp2->setText(QString::number(user.petOfPlayer[1]->getXp(),10));
            ui->pushButton_throwName3->setEnabled(false);
        }
        else{
            ui->pushButton_throwName2->setText(user.petOfPlayer[1]->getName());
            ui->lineEdit_throwLv2->setText(QString::number(user.petOfPlayer[1]->getLv(),10));
            ui->lineEdit_throwXp2->setText(QString::number(user.petOfPlayer[1]->getXp(),10));
            select = random(2,user.getPetNum()-1);
            ui->pushButton_throwName3->setText(user.petOfPlayer[select]->getName());
            ui->lineEdit_throwLv3->setText(QString::number(user.petOfPlayer[select]->getLv(),10));
            ui->lineEdit_throwXp3->setText(QString::number(user.petOfPlayer[select]->getXp(),10));
            ui->pushButton_throwName2->setEnabled(true);
            ui->pushButton_throwName3->setEnabled(true);
        }

    }
    //QMessageBox::about(this,"",QString::number(user.getWinNum(),10));
}


//战斗函数
bool MainWindow::fighting()
{
    user.petOfPlayer[nowPet]->fightStart();
    p->fightStart();
    int endFlag = 0;
    for(int i = user.petOfPlayer[nowPet]->getAtkIntervalNow() , j = p->getAtkIntervalNow() ; endFlag == 0 ; --i , --j){
        if(i == 0){
            i = user.petOfPlayer[nowPet]->getAtkIntervalNow();
            if(random(1,100) > performRate){
                p->beAttacked(ui->textBrowser,*user.petOfPlayer[nowPet]);
            }
            else{
                user.petOfPlayer[nowPet]->performAbilities(ui->textBrowser,*p);
            }
        }
        if(j == 0){
            j = p->getAtkIntervalNow();
            if(random(1,100) > performRate){
                user.petOfPlayer[nowPet]->beAttacked(ui->textBrowser,*p);
            }
            else{
                p->performAbilities(ui->textBrowser,*user.petOfPlayer[nowPet]);
            }
        }
        if(p->getHpNow() == 0){
            user.petOfPlayer[nowPet]->fightEnd(50);
            p->fightEnd(20);
            QMessageBox::about(this,"win","You win ! xp + 50");
            endFlag = 1;
            return true;
        }
        else if(user.petOfPlayer[nowPet]->getHpNow() == 0){
            p->fightEnd(50);
            user.petOfPlayer[nowPet]->fightEnd(20);
            QMessageBox::about(this,"fail","You fail! xp + 20");
            endFlag = 1;
            return false;
        }
    }
    return true;
}


//选择第一个小精灵送出
void MainWindow::on_pushButton_throwName1_clicked()
{
    takeCentralWidget();
    setCentralWidget(ui->users);
    //显示win rate
    if(user.getWinNum() == 0 && user.getFailNum() == 0){
        ui->lineEdit_winRate->setText("winRate: 0%");
    }
    else{
        int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
        ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
    }
    if(user.getPetNum() == 1){
        user.petOfPlayer.pop_back();
        for(int i = 0 ; i < 3 ; ++i){
            int r = random(0,3);
            pet *p;
            switch(r){
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
            user.petOfPlayer.push_back(p);
        }//end for(int i = 0 ; i < 3 ; ++i)
    }
    else{
        user.petOfPlayer.erase(user.petOfPlayer.begin(),user.petOfPlayer.begin() + 1);
    }
    user.changePetNum(user.petOfPlayer.size());
}

//选择第二个小精灵送出
void MainWindow::on_pushButton_throwName2_clicked()
{
    takeCentralWidget();
    setCentralWidget(ui->users);
    //显示win rate
    if(user.getWinNum() == 0 && user.getFailNum() == 0){
        ui->lineEdit_winRate->setText("winRate: 0%");
    }
    else{
        int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
        ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
    }
    user.petOfPlayer.erase(user.petOfPlayer.begin()+1,user.petOfPlayer.begin()+2);
    user.changePetNum(user.petOfPlayer.size());
}

//选择第三个小精灵送出
void MainWindow::on_pushButton_throwName3_clicked()
{
    takeCentralWidget();
    setCentralWidget(ui->users);
    //显示win rate
    if(user.getWinNum() == 0 && user.getFailNum() == 0){
        ui->lineEdit_winRate->setText("winRate: 0%");
    }
    else{
        int winRate = (user.getWinNum() * 100) / ( user.getFailNum() + user.getWinNum());
        ui->lineEdit_winRate->setText("winRate: " + QString::number(winRate,10) + "%");
    }
    user.petOfPlayer.erase(user.petOfPlayer.begin()+select,user.petOfPlayer.begin()+select+1);
    user.changePetNum(user.petOfPlayer.size());
}


//升级按钮
void MainWindow::on_pushButton_lvUp_clicked()
{
    user.petOfPlayer[nowPet]->lvUp();

    showInformation();

}
