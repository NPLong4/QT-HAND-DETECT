#ifndef MODBUSTCP_H
#define MODBUSTCP_H

#include <QObject>

class ModbusTCP : public QObject
{
    Q_OBJECT
public:
    explicit ModbusTCP(QObject *parent = nullptr);

signals:

};

#endif // MODBUSTCP_H
