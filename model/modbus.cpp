#include "modbus.h"
#include <QObject>
#include <QModbusRtuSerialMaster>
#include <QThread>

Modbus::Modbus()
{
  modbusDevice = new QModbusTcpClient();
  nBytes = 0;
}

Modbus::Modbus(QString Ip, int Port,QString nameSer) : ip(Ip), port(Port),deviceName(nameSer), utcTimeDisconnect(QDateTime::currentDateTimeUtc())
{
  modbusDevice = new QModbusTcpClient();
  nBytes = 0;
}

void Modbus::setIp(QString Ip)
{
  ip = Ip;
}

void Modbus::setPort(int Port)
{
  port = Port;
}

bool Modbus::getBusState()
{
    if (modbusDevice->state() == QModbusDevice::ConnectedState)
    {
        return 1;
    }
    else if (modbusDevice->state() == QModbusDevice::ConnectingState)
    {
        return 0;
    }
    else if (modbusDevice->state() == QModbusDevice::UnconnectedState )
    {
        return 0;
    }
    else if (modbusDevice->state() == QModbusDevice::ClosingState)
    {
        return 0;
    }
}


bool Modbus::startConnection()
{
  connect(modbusDevice, &QModbusTcpClient::stateChanged, this, &Modbus::onModbusStateChanged);
  modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, port);
  modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, ip);
  connect(modbusDevice, &QModbusClient::errorOccurred, [this](QModbusDevice::Error)
          { qDebug() << modbusDevice->errorString(); });
  if (!modbusDevice)
  {
    qDebug() << "Modbus-create fail "+ deviceName;
    return false;
  }

  if (!modbusDevice->connectDevice())
  {
    qDebug() << "Modbus-cannot connect "+ deviceName;
    connection_state = false;
    emit varChanged();
  }
  else
  {
    qDebug() << "Modbus-connected "+deviceName+": "<< modbusDevice->state();
    connection_state = true;
    emit varChanged();
  }
  isBusy = false;
  return true;
}

bool Modbus::reconnectIfDisconnectedFor(int seconds)
{
  if (!connection_state && ((QDateTime::currentDateTimeUtc().toMSecsSinceEpoch() - utcTimeDisconnect.toMSecsSinceEpoch()) > (seconds * 1000)))
  {
    stopConnection();
    return (startConnection());
  }
  return true;
}

// FC06
void Modbus::writeSingleHoldingRegister(int add, int value, int server)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  QModbusDataUnit reg(QModbusDataUnit::HoldingRegisters, add, 1);
  reg.setValue(0, value);
  QModbusReply *reply;
  reply = modbusDevice->sendWriteRequest(reg, server);
  if (reply)
  {
    if (!reply->isFinished())
    {
      connect(reply, &QModbusReply::finished, this, &Modbus::writeSingleHoldingRegisterCompleted);
    }
    else
    {
      Modbus::writeSingleHoldingRegisterCompleted();
    }
  }
  else
    qDebug() << "Write Single Holding Resister Error!";
}

void Modbus::writeSingleHoldingRegisterCompleted()
{
  //    qDebug() << "Write Single Holding Resister Completed!";
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  if (reply)
  {
    reply->deleteLater();
  }
  emit writeSingleHoldingRegisterCompletedOK();
  isBusy = false;
}

//
void Modbus::readSingleHoldingRegister(int add, int ID, int *data)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  // startConnection();
  QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, add, 1);
  if (auto *reply = modbusDevice->sendReadRequest(readUnit, ID))
  {
    //  qDebug()<<"Reading single holding register ...";
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::readSingleHoldingRegisterRecieved);
    else
    {
      Modbus::readSingleHoldingRegisterRecieved();
    }
  }
  else
    qDebug() << "request error";
  holding_register_result = data;
}

void Modbus::readSingleHoldingRegisterRecieved()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();
  //    qDebug() << "read ";
  //    qDebug() << "";
  //    qDebug() << QString("The value is %1").arg(result.value(0));
  *holding_register_result = result.value(0);
  reply->deleteLater();
  emit readSingleHoldingRegisterCompleted();
  isBusy = false;
}

void Modbus::writeSingleCoil(int add, bool value, int server)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  // startConnection();
  QModbusDataUnit reg(QModbusDataUnit::Coils, add, 1);
  reg.setValue(0, value);
  QModbusReply *reply;
  reply = modbusDevice->sendWriteRequest(reg, server);
  if (reply)
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::writeSingleCoilComleted);
    else
      Modbus::writeSingleCoilComleted();
  }
  else
    qDebug() << "Write Single Coil Error!";
}

void Modbus::writeSingleCoilComleted()
{
  //    qDebug() << "Write Single Coil Completed!";
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  reply->deleteLater();
  isBusy = false;
}
// FC03 - 4x
void Modbus::readHoldingRegister(int server, int start_add, int number_register)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  nBytes = number_register;
  start_address = start_add;
  ID = server;
  nNumAddress = ID;
  QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, start_address,
                           static_cast<unsigned short>(nBytes));

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, ID))
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::readHoldingRegisterCompleted);
    else
      Modbus::readHoldingRegisterCompleted();
  }
  else
    qDebug() << deviceName+" request error";
}

void Modbus::readHoldingRegisterCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();

  for (int j = 0; j < nBytes; j++)
  {
    holding_register_store[j] = result.value(j);
    // qDebug() <<"start_address:"<<nNumAddress<< QString("The value of %1 is %2").arg(j).arg(result.value(j));
    // qDebug() <<"ModbusTCP, ID: "<<nNumAddress<<QString("The value of %1 is %2").arg(j).arg(result.value(j));
  }

  reply->deleteLater();
  emit readHoldingRegisterCompletedOK();
  isBusy = false;
}

void Modbus::myReadHoldingRegister(int server, int start_add, int number_register, int *data)
{
  if (connection_state == false)
  {
    qDebug() << "Modbus not connected!";
    return;
  }

  //
  isBusy = true;
  //
  nHoldingByte = number_register;
  start_address = start_add;
  ID = server;
  nNumAddress = ID;

  QModbusDataUnit readUnit(QModbusDataUnit::HoldingRegisters, start_address,
                           static_cast<unsigned short>(nHoldingByte));

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, ID))
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::myReadHoldingRegisterCompleted);
    else
      Modbus::myReadHoldingRegisterCompleted();
  }
  else
    qDebug() << "request error";
  holding_register_result = data;
}

void Modbus::myReadHoldingRegisterCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();

  for (int j = 0; j < nHoldingByte; j++)
  {
    holding_register_result[j] = result.value(j);
    //      qDebug() <<"ModbusRTU, ID: "<<nHoldingByte<<QString("The value of %1 is %2").arg(j).arg(result.value(j));
  }
  //
  reply->deleteLater();
  emit readHoldingRegisterCompletedSignal();
  isBusy = false;
}

void Modbus::onModbusStateChanged(QModbusDevice::State state)
{
  qDebug() << deviceName+" State changed: " << state;
  connection_state = (state == QModbusDevice::ConnectedState);

  if (state != QModbusDevice::ConnectedState)
    utcTimeDisconnect = QDateTime::currentDateTimeUtc();
}

void Modbus::readMultiCoils(int server, int start_add, int number_coils, bool *data)
{
  if (modbusDevice->state() != QModbusDevice::ConnectedState)
  {
    // qDebug () << "Read Single coil not complete! Modbus connection state: " << modbusDevice->state();
    return;
  }
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  nBytes = number_coils;
  start_address = start_add;

  QModbusDataUnit readUnit(QModbusDataUnit::Coils, start_address,
                           static_cast<unsigned short>(nBytes));

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, server))
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::readCoilsCompleted);
  }
  else
    qDebug() <<" request error";

  coil_result = data;
}

void Modbus::readSingleCoil(int server, int add, bool *data)
{
  if (modbusDevice->state() != QModbusDevice::ConnectedState)
  {
    // qDebug () << "Read Single coil not complete! Modbus connection state: " << modbusDevice->state();
    return;
  }
  isBusy = true;
  QModbusDataUnit readUnit(QModbusDataUnit::Coils, add, 1);

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, server))
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::readSingleCoilCompleted);
    else
      Modbus::readSingleCoilCompleted();
  }
  else
    qDebug() << "request error";

  single_coil_result = data;
}

void Modbus::readSingleCoilCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();
  *single_coil_result = result.value(0);
  reply->deleteLater();
  emit readSingleCoilCompletedSignal();
  isBusy = false;
}

void Modbus::readCoilsCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();

  for (int j = 0; j < nBytes; j++)
  {
    coil_result[j] = result.value(j);
  }
  reply->deleteLater();
  emit readCoilsCompletedSignal();
  //
  isBusy = false;
}

void Modbus::readMultiDiscrete(int server, int start_add, int number_coils, bool *data)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  nDiscrete = number_coils;
  QModbusDataUnit readUnit(QModbusDataUnit::DiscreteInputs, start_add,
                           static_cast<unsigned short>(number_coils));

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, server))
  {
    if (!reply->isFinished())
      //      connect the finished signal of the request to your read slot
      connect(reply, &QModbusReply::finished, this, &Modbus::readDiscreteCompleted);
    else
      Modbus::readDiscreteCompleted(); // broadcast replies return immediately
  }
  else
    qDebug() << "request error";

  discrete_result = data;
}

void Modbus::readDiscreteCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();

  for (int j = 0; j < nDiscrete; j++)
  {
    discrete_result[j] = result.value(j);
    // qDebug() << QString("The Discrete Input value of %1 is %2").arg(j).arg(result.value(j));
  }
  reply->deleteLater();
  emit readDiscreteCompletedSignal();
  isBusy = false;
}

void Modbus::readMultiInputRegister(int server, int start_add, int number_register, int *data)
{
  if (connection_state == false)
    return;
  //
  isBusy = true;
  //
  nBytesInputResgister = number_register;
  QModbusDataUnit readUnit(QModbusDataUnit::InputRegisters, start_add,
                           static_cast<unsigned short>(nBytesInputResgister));

  if (auto *reply = modbusDevice->sendReadRequest(readUnit, ID))
  {
    if (!reply->isFinished())
      connect(reply, &QModbusReply::finished, this, &Modbus::readMultiInputRegisterCompleted);
    else
      Modbus::readMultiInputRegisterCompleted();
  }
  else
    qDebug() << "request error";
  input_register_result = data;
}

void Modbus::readMultiInputRegisterCompleted()
{
  QModbusReply *reply = qobject_cast<QModbusReply *>(sender());
  const QModbusDataUnit result = reply->result();

  for (int j = 0; j < nBytesInputResgister; j++)
  {
    input_register_result[j] = result.value(j);
    // qDebug() << QString("The Discrete Input value of %1 is %2").arg(j).arg(result.value(j));
  }
  reply->deleteLater();
  emit readInputCompletedSignal();
  isBusy = false;
}

void Modbus::stopConnection()
{
  modbusDevice->disconnectDevice();
  connection_state = false;
  qDebug() << "ModbusTCP-Ngat ket noi!";
  emit varChanged();
}

Modbus::~Modbus()
{
  modbusDevice->disconnectDevice();
  delete modbusDevice;
  modbusDevice = nullptr;
}
