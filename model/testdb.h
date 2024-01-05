#ifndef TESTDB_H
#define TESTDB_H

#include <QObject>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
using namespace sql;
class TestDB : public QObject
{
    Q_OBJECT
public:
    explicit TestDB(QObject *parent = nullptr);
    TestDB(QString host,QString user, QString pass);
    void connectMysql();
private:
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    sql::Statement *stmt;
    QString _host;
    QString _user;
    QString _pass;
signals:
};

#endif // TESTDB_H
