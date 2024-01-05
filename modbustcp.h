#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include <QObject>
#include <QtSerialBus>
#include <QTimer>

class ModbusTCP : public QObject
{
    Q_OBJECT
private:
    QModbusTcpClient *modbusClient ;
    QTimer *timer;
public:
    explicit ModbusTCP(QObject *parent = nullptr);
    void begin();
private slots:
    void modbusUpdate();
signals:

};

#endif // MODBUSTCP_H
