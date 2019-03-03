#include "database.h"
#include <QDebug>

Database::Database()
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("petDatabase.db");
        database.setUserName("luohaozhong");
        database.setPassword("123456");
    }

    if (!database.open())
    {
        //qDebug() << "Failed to connect the database.";
    }
    else
    {
        query = new QSqlQuery(database);
        QString createSql = "CREATE TABLE player (name TEXT primary key, password TEXT, winNum INT, failNum INT, petNum INT, highPetNum INT, pet TEXT);";
        query->prepare(createSql);
        if(!query->exec()){
            //qDebug() << "Error in create sql." << query->lastError();
        }
        else{
            //qDebug() << "Table create !";
        }
    }
}

Database::~Database()
{
    database.close();
}
