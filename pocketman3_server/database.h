#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database
{

public:
    Database();
    ~Database();

    QSqlDatabase database;
    QSqlQuery *query;
};

#endif // DATABASE_H
