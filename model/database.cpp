#include "database.h"

Database::Database(QObject *parent)
    : QObject{parent}
{
db=new QSqlDatabase();
}

void Database::connectDatabase()
{
    db->addDatabase("QMYSQL");
db->setHostName("127.0.0.1");
db->setDatabaseName("PCCC");
db->setUserName("root");
db->setPassword("01041998");
connectState=db->open();
if(connectState){
    qDebug()<<"connected";
}
else{
    qDebug()<<"disconnected";
}
}
