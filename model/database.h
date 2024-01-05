#ifndef DATABASE_H
#define DATABASE_H
#include <QtSql>
#include <QDebug>
#include <QSqlDatabase>

#include <QObject>

class Database : public QObject
{
    Q_OBJECT

private:
QSqlDatabase *db;
bool connectState=false;

public:
    explicit Database(QObject *parent = nullptr);

void connectDatabase();
signals:

};

#endif // DATABASE_H
