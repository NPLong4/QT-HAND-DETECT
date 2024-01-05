#ifndef MODBUS_H
#define MODBUS_H


#include <QString>

#include <QGuiApplication>

#include <QDateTime>
#include <QDebug>
#include <QModbusClient>
#include <QModbusDataUnit>
#include <QModbusReply>
//
#include <QModbusTcpClient>
#include <QObject>
#include <QTimer>
#include <QtSerialPort/QSerialPort>

class Modbus : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool q_connectionState READ getState WRITE setState NOTIFY varChanged)
    //
    Q_PROPERTY(bool q_isBusy READ getIsBusy NOTIFY varChanged)

signals:
    void varChanged();

public:
    enum ModbusConnection
    {
        Serial,
        Tcp
    };
    ~Modbus();

public:
    Modbus();
    Modbus(QString Ip, int Port,QString nameSer);
    void setState(bool state) { connection_state = state; }
    bool getState() { return connection_state; }
    bool getIsBusy() { return isBusy; }
    void setIp(QString Ip);
    void setPort(int Port);
    bool getBusState();



    Q_INVOKABLE bool startConnection();
    Q_INVOKABLE bool reconnectIfDisconnectedFor(int seconds);
    Q_INVOKABLE void stopConnection();
    Q_INVOKABLE void writeSingleHoldingRegister(int add, int value, int server);
    void writeSingleHoldingRegisterCompleted();

    Q_INVOKABLE void writeSingleCoil(int add, bool value, int server);
    void writeSingleCoilComleted();

    Q_INVOKABLE void readSingleHoldingRegister(int add, int ID, int *data);
    void readSingleHoldingRegisterRecieved();

    Q_INVOKABLE void readHoldingRegister(int ID, int start_add, int number_register);
    void readHoldingRegisterCompleted();
    void readCoilsCompleted();
    void readMultiCoils(int server, int start_add, int number_coils, bool *data);

    void readSingleCoil(int server, int add, bool *data);
    void readSingleCoilCompleted();
    void readMultiDiscrete(int server, int start_add, int number_coils, bool *data);
    void readDiscreteCompleted();

    void myReadHoldingRegister(int ID, int start_add, int number_register, int *data);
    void myReadHoldingRegisterCompleted();

    void readMultiInputRegister(int server, int start_add, int number_register, int *data);
    void readMultiInputRegisterCompleted();
    //
    int holding_register_store[30]={0};//holding_register_store[0]
    int nNumAddress = 0;
    //
signals:
    // void readSingleHoldingRegisterCompleted(int value);
    void readSingleHoldingRegisterCompleted();
    void readHoldingRegisterCompletedSignal();
    void readCoilsCompletedSignal();
    void readDiscreteCompletedSignal();
    void readInputCompletedSignal();
    void readHoldingRegisterCompletedOK() const;
    void readHoldingRegister0x47CompletedOK() const;
    void readSingleCoilCompletedSignal();
    void writeSingleHoldingRegisterCompletedOK();

private slots:
    void onModbusStateChanged(QModbusDevice::State state);

private:
    ModbusConnection m_type;
    QModbusTcpClient *modbusDevice = nullptr;
    QTimer *serialTimer;
    void saveSettings();
    int nBytes;
    int nHoldingByte;
    int nBytesInputResgister;
    int ID;
    int start_address;
    bool connection_state = false;
    bool *discrete_result;
    bool *coil_result;
    bool *single_coil_result;
    int *holding_register_result;
    int *input_register_result;
    int nDiscrete;
    //
    bool isBusy;
    QDateTime utcTimeDisconnect;
    QString ip;
    int port;

    QString deviceName="";
};

#endif // MODBUS_H
