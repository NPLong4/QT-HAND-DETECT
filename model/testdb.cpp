#include "testdb.h"
#include "cppconn/exception.h"
#include "qdebug.h"
TestDB::TestDB(QObject *parent)
    : QObject{parent}
{

}

TestDB::TestDB(QString host, QString user, QString pass)
{
    _host=host;
    _user=user;
    _pass=pass;
}
void TestDB::connectMysql()
{
    try {
        qDebug() << "connecting to mysql server....";
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "01041998");
         qDebug() << "connected" ;
    } catch (sql::SQLException &e) {
         qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
         qDebug()<< ", SQLState: " << &e.getSQLState() << " )" ;
    }
}
