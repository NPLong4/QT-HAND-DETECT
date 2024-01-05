#include "mainwindow.h"
#include <QApplication>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QModbusReply>
#include <QUrl>
#include <QDebug>
class ModbusHandler : public QObject {
    Q_OBJECT
public slots:
    void onDataRead(int data) {
        // Xử lý dữ liệu đọc được từ Modbus ở đây
        qDebug() << "Received data from Modbus:" << data;
    }
};
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    w.setWindowTitle("Nguyễn Phước Long - 62131035");

    // Tạo một đối tượng QModbusTcpClient
//    QModbusTcpClient modbusClient;

//    // Tạo đối tượng ModbusHandler để xử lý tín hiệu từ Modbus
//    //ModbusHandler modbusHandler;

//    // Kết nối tín hiệu "dataReady" của QModbusTcpClient với slot "onDataRead" của ModbusHandler
//    //QObject::connect(&modbusClient, &QModbusTcpClient::dataReady, &modbusHandler, &ModbusHandler::onDataRead);

//    // Đặt địa chỉ IP và cổng của Modbus TCP Server
//    QUrl url("tcp://192.168.1.193:502");
//    modbusClient.setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
//    modbusClient.setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

//    // Kết nối đến thiết bị Modbus
//    if (!modbusClient.connectDevice()) {
//        qDebug() << "Failed to connect to Modbus device!";
//        return 1;
//    }

//    // Định cấu trúc dữ liệu để đọc
//    QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, 0x0001, 1); // Đọc từ thanh ghi Modbus có địa chỉ 0x0001

//    // Gửi yêu cầu đọc đến thiết bị Modbus
//    auto reply = modbusClient.sendReadRequest(readUnit, 1);

//    // Đợi phản hồi từ thiết bị Modbus
//    if (reply->isFinished()) {
//        if (reply->error() == QModbusDevice::NoError) {
//            qDebug() << "Data read from Modbus:" << reply->result().value(0);
//        } else {
//            qDebug() << "Error reading from Modbus:" << reply->errorString();
//        }
//    } else {
//        qDebug() << "Read request not finished yet. Waiting for response...";
//        QObject::connect(reply, &QModbusReply::finished, [&]() {
//            if (reply->error() == QModbusDevice::NoError) {
//                qDebug() << "Data read from Modbus:" << reply->result().value(0);
//            } else {
//                qDebug() << "Error reading from Modbus:" << reply->errorString();
//            }
//            a.quit();
//        });
//    }
    return a.exec();
}
