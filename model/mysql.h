#ifndef MYSQL_H
#define MYSQL_H
#include <QObject>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <QVector>
#include <QObject>
#include <QQueue>
#include <qqml.h>
#include <QAbstractTableModel>
#include "QVariant"
using namespace sql;
struct history_db
{
    int id;
    QString date_time;
    QString name;
};
class MySQL : public QObject
{
    Q_OBJECT
public:
    explicit MySQL(QObject *parent = nullptr);
    MySQL(sql::SQLString, sql::SQLString, sql::SQLString);
    void connectDatatbase();
    void readDatabase();
    void reconnectDatabases();
    void deleteTale(sql::SQLString tableName);
    void getHistory();
    void deleteTable(sql::SQLString tableName);
    void createTable(sql::SQLString tableName);
    void sqlExcute(sql::SQLString query);
//    void insertValueTinhTaiLap(int id, QString dateTime,QString listChart, float ton, float toff );
    enum tableHistory{
        STT=Qt::UserRole,
        DATE,
        NAME
    };
        QHash<int ,QByteArray> roleName();
    //    void createTable(sql::SQLString tableName);
    QVariantList model() const;
    void setModel(const QVariantList &newModel);
    bool getStateConnect() const;
    int getLastedID();
    sql::Driver *driver;
    sql::Connection *con;
    sql::PreparedStatement *prep_stmt;
    sql::ResultSet *res;
    sql::Statement *stmt;
private:
    sql::SQLString _host="tcp://127.0.0.1:3306";
    sql::SQLString _user="root";
    sql::SQLString _pass="root";
    QQueue <history_db> history_queue;
    QVariantList m_model;
    Q_PROPERTY(QVariantList model READ model WRITE setModel NOTIFY modelChanged)
    bool stateConnect=false;
    int lastedID=0;

signals:
    void modelChanged();
};
#endif // MYSQL_Hs
