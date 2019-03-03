#include "mainwindow.h"
#include <QApplication>

int timeCount = 0;
int main(int argc, char *argv[])
{
    srand((unsigned)time(NULL));

    HANDLE h;
    CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)countTime,NULL,1,0);
    ResumeThread(h);
    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}



//记录服务端运行时长的线程
void countTime(){
    while(true){
        timeCount++;
        qDebug() << timeCount;
        Sleep(1000);
    }
}


//分词函数
QString get(const QByteArray &b , int &pos)
{
    QString str;
    str.clear();
    for( ; pos < b.size() ; pos++){
        if(b[pos] == ' '){
            ++pos;
            return str;
        }
        else{
            str += b[pos];
        }
    }
    return str;
}


//随机数函数
int random(int left, int right)
{
    return ((rand() % (right - left + 1)) + left);
}


//判断该用户名是否在数据库中
bool nameIsInDatabase(QByteArray &b , Database &playerDatabase)
{
    QString select = "select * from player where name = ";
    int pos = 1;
    select += get(b,pos);

    if(!playerDatabase.query->exec(select)){
        qDebug() << playerDatabase.query->lastError();
    }
    else{
        //用户名已存在
        if(playerDatabase.query->next()){
            qDebug() << "user name is in the table";
            qDebug() << b;
            return true;
        }
        //用户名不存在
        else{
            qDebug() << "user name is not in table";
            qDebug() << b;
            return false;
        }
    }
    return true;
}

