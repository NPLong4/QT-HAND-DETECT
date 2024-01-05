#include "modbustcp.h"

ModbusTCP::ModbusTCP(QObject *parent)
    : QObject{parent}
{
    modbusClient=new QModbusTcpClient(this);
    timer =new QTimer(this);
    connect(timer, &QTimer::timeout, this, &ModbusTCP::modbusUpdate);
    timer->setInterval(100);
}

void ModbusTCP::begin()
{
    modbusClient->setConnectionParameter(QModbusDevice::NetworkAddressParameter, "127.0.0.1");
    modbusClient->setConnectionParameter(QModbusDevice::NetworkPortParameter, 502);
    if (!modbusClient->connectDevice()) {
        qDebug() << "Failed to connect to Modbus TCP server:" << modbusClient->errorString();
    }
    else{

    }
     timer->start();
}

void ModbusTCP::modbusUpdate()
{
    qDebug("update");
    QModbusDataUnit readRequest(QModbusDataUnit::HoldingRegisters, 0, 10);
    if (auto *reply = modbusClient->sendReadRequest(readRequest, 1)) {
        if (!reply->isFinished()) {
            QObject::connect(reply, &QModbusReply::finished, [&]() {
                if (reply->error() == QModbusDevice::NoError) {
                    const QModbusDataUnit unit = reply->result();
                    for (int i = 0; i < unit.valueCount(); ++i) {
                        qDebug() << "Value" << i << ":" << unit.value(i);
                    }
                } else {
                    qDebug() << "Read request error:" << reply->errorString();
                }
                reply->deleteLater();
                modbusClient->disconnectDevice();
                QCoreApplication::quit();
            });
        } else {
            qDebug() << "Reply finished immediately";
            delete reply; // Destroy the reply object
        }
    } else {
        qDebug() << "Failed to send read request:" << modbusClient->errorString();
        modbusClient->disconnectDevice();
    }
}
