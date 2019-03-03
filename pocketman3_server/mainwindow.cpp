#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    server = new QTcpServer();

    connect(server,&QTcpServer::newConnection,this,&MainWindow::newConnect);

    port = 6666;
    server->listen(QHostAddress::Any, port);

}

MainWindow::~MainWindow()
{
    server->close();
    server->deleteLater();
    delete ui;
}


//建立新连接
void MainWindow::newConnect()
{
    //获取客户端连接
    socket = server->nextPendingConnection();
    //连接QTcpSocket的信号槽，以读取新数据
    QObject::connect(socket, &QTcpSocket::readyRead, this, &MainWindow::socketReadData);
    QObject::connect(socket, &QTcpSocket::disconnected, this, &MainWindow::socketDisconnected);

    //qDebug() << "a client connect !";
}


//读数据
void MainWindow::socketReadData()
{
    QByteArray buffer;
    buffer = socket->readAll();
    if(!buffer.isEmpty()){
        qDebug() << buffer;

        //用户发送登录请求时
        if(buffer[0] == 'l'){
            QString select_all = "select * from player where name = ";
            int pos = 1;
            select_all += get(buffer,pos);
            select_all += " and password = ";
            select_all += get(buffer,pos);
            playerDatabase.query->prepare(select_all);
            if(!playerDatabase.query->exec(select_all)){
                qDebug() << "111";
                qDebug() << playerDatabase.query->lastError();
            }
            else{
                //用户名已存在且密码正确
                if(playerDatabase.query->next()){
                    QString temp;
                    temp.clear();
                    temp += loginSuccessful;
                    temp += ' ';
                    temp += playerDatabase.query->value(0).toString();  //name
                    temp += ' ';
                    temp += playerDatabase.query->value(1).toString();  //password
                    temp += ' ';
                    temp += playerDatabase.query->value(2).toString();  //win num
                    temp += ' ';
                    temp += playerDatabase.query->value(3).toString();  //fail num
                    temp += ' ';
                    temp += playerDatabase.query->value(4).toString();  //pet num
                    temp += ' ';
                    temp += playerDatabase.query->value(5).toString();  //high pet num
                    temp += ' ';
                    temp += playerDatabase.query->value(6).toString();  //pet info

                    QByteArray tempArray = temp.toUtf8();

                    player *nowPlayer = new player(tempArray);

                    temp += " #";
                    for(unsigned int i = 0 ; i < inLinePlayer.size(); ++i){
                        temp += inLinePlayer[i]->toQString();
                    }
                    socket->write(temp.toUtf8());
                    inLinePlayer.push_back(nowPlayer);
                }
                //用户名或密码错误
                else{
                    socket->write(loginFailed.toUtf8());
                }
            }
        }//end if(buffer[0] == 'l')

        //用户发送注册请求时
        else if(buffer[0] == 'r'){
            //用户名不存在，新注册用户并返回初始用户信息
            if(!nameIsInDatabase(buffer,playerDatabase)){
                QString insert_sql = "insert into player values (?, ?, ?, ?, ?, ?, ?)";
                playerDatabase.query->prepare(insert_sql);

                int pos = 1;
                playerDatabase.query->addBindValue(get(buffer,pos));  //name
                playerDatabase.query->addBindValue(get(buffer,pos));  //password
                playerDatabase.query->addBindValue(0);    //win num
                playerDatabase.query->addBindValue(0);    //fail num
                playerDatabase.query->addBindValue(3);    //pet num
                playerDatabase.query->addBindValue(0);    //high pet num

                QString temp;
                temp.clear();
                for(int i = 0 ; i < 2 ; ++i){
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
                    temp += p->toQString();
                    temp += ' ';
                }//end for(int i = 0 ; i < 2 ; ++i)
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
                temp += p->toQString();
                playerDatabase.query->addBindValue(temp);  //pet information

                if(!playerDatabase.query->exec())
                {
                    //qDebug()<<playerDatabase.query->lastError();
                }

                QString returnInfo;
                returnInfo.clear();
                returnInfo += registeSuccessful;
                returnInfo += " 0 0 3 0 ";
                returnInfo += temp;
                //qDebug() << "returnInfo: " << returnInfo;

                socket->write(returnInfo.toUtf8());

            }//end if(isIntable)
            //用户名存在
            else{
                socket->write(userNameIsIn.toUtf8());
            }
        }//end if(buffer[0] == 'r'

        //用户发出退出请求时
        else if(buffer[0] == 'o'){
            //更新用户信息
            qDebug() << buffer;
            int pos = 1;
            QString name = get(buffer,pos);
            QString win = get(buffer,pos);
            QString fail = get(buffer,pos);
            QString pNum = get(buffer,pos);
            QString hPNum = get(buffer,pos);

            QString petInfo;
            for(int i = pos; i < buffer.size()-3; i++)
            {
                petInfo+=buffer[i];
            }

            QString update_sql = QString("update player set winNum = %1, failNum = %2, petNum = %3, highPetNum = %4, pet = '%5' where name = '%6'").arg(win).arg(fail).arg(pNum).arg(hPNum).arg(petInfo).arg(name);
            playerDatabase.query->prepare(update_sql);

            if(!playerDatabase.query->exec())
            {
                qDebug() << playerDatabase.query->lastError();
            }
            else
            {
                qDebug() << "updated!";
            }
            //将此用户移除在线用户队列
            for(auto i = inLinePlayer.begin() ; i < inLinePlayer.cend(); i++)
            {
                if((*i)->getName() == name)
                {
                    i = inLinePlayer.erase(i);
                }
            }

        }

    }//end if(!buffer.isEmpty())

    socketDisconnected();
}


//断开连接
void MainWindow::socketDisconnected()
{
    socket->close();
}





