#include "mysql.h"
#include "qdebug.h"

MySQL::MySQL(QObject *parent)
    : QObject{parent}
{

}
MySQL::MySQL(sql::SQLString hostName, sql::SQLString user, sql::SQLString pass)
{
    _host=hostName;
    _user=user;
    _pass=pass;
}
void MySQL::connectDatatbase()
{
    bool stateConnectToHost=true;
    try {
        qDebug() << "connecting to mysql server....";
        driver = get_driver_instance();
        //        con = driver->connect("tcp://127.0.0.1:3306", "root", "01041998");
        con=driver->connect(_host,_user,_pass);
        qDebug() << "connected" ;
        con->setSchema("PCCC");
        stmt=con->createStatement();

    } catch (sql::SQLException &e) {
        stateConnectToHost=false;
        if(e.getErrorCode()==1049){
            qDebug()<<"No databases";
        }
        qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
        qDebug()<< ", SQLState: " << &e.getSQLState() << " )" ;
    }
    stateConnect=stateConnectToHost;
}
void MySQL::readDatabase()
{
    res= stmt->executeQuery("select *from history");
    while (res->next()) {
        static int a=0;
        qDebug()<<"id: "<<res->getInt(1);
        qDebug()<<"label: "<<res->getInt(3);
        qDebug()<<"label: "<<res->getString(2).c_str();
        qDebug()<< "num: "<<a++;
    }
}
void MySQL::reconnectDatabases()
{
    con->reconnect();
}
void MySQL::deleteTale(SQLString tableName)
{
    try{
        stmt->executeQuery("DROP TABLE "+tableName);
    }
    catch (sql::SQLException &e) {
        qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
        qDebug() << ", SQLState: " << &e.getSQLState() << " )";
    }
}
void MySQL::getHistory()
{
//    qDebug()<<"read history";
    if(getStateConnect()){
    QVariantList temp;
    try {
        res=  stmt->executeQuery("SELECT * FROM history ORDER BY id DESC LIMIT 100;");
        while (res->next()) {
            QVariantMap m_p;
            m_p.insert("STT",res->getInt("id"));
            m_p.insert("Time",res->getString("date_time").c_str());
            m_p.insert("name",res->getString("name").c_str());
            temp.push_back(m_p);
        }
        setModel(temp);

    } catch (sql::SQLException &e) {
        qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
        qDebug() << ", SQLState: " << &e.getSQLState() << " )";
    }
//    qDebug()<<"lasted ID: "<< getLastedID();
    }

}
void MySQL::sqlExcute(SQLString query)
{
    try{
        res=stmt->executeQuery(query);
        while (res->next()) {
        }
    }
    catch(sql::SQLException &e){
        qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
        qDebug() << ", SQLState: " << &e.getSQLState() << " )";
    }
}
QHash<int, QByteArray> MySQL::roleName()
{
    static QHash <int, QByteArray> mapping;
    mapping[STT]="1";
    mapping[DATE]="11:20:30";
    mapping[NAME]="Bien doi nguon cap";
    return mapping;
}
QVariantList MySQL::model() const
{
    return m_model;
}
void MySQL::setModel(const QVariantList &newModel)
{
    if (m_model == newModel)
        return;
    m_model = newModel;
//    qDebug()<<newModel;
    emit modelChanged();
}
bool MySQL::getStateConnect() const
{
    return stateConnect;
}

int MySQL::getLastedID()
{
    try{
        res=stmt->executeQuery("SELECT id FROM history ORDER BY id DESC LIMIT 1");
        while (res->next()) {
        lastedID= res->getInt("id");
        }
    }
    catch(sql::SQLException &e){
        qDebug() << "# ERR: " << e.what();
        qDebug() << " (MySQL error code: " << e.getErrorCode();
        qDebug() << ", SQLState: " << &e.getSQLState() << " )";
    }
    return lastedID;
}
