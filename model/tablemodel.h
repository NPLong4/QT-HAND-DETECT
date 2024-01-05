#ifndef TABLEMODEL_H
#define TABLEMODEL_H
#include "QObject"
#include "QAbstractTableModel"
#include "qqml.h"
#include "QVariant"

class TableModel: public QObject
{
    Q_OBJECT


public:
//     Q_PROPERTY(QVariantList m_model READ gname WRITE setGname NOTIFY gnameChanged)
    TableModel();
    QVariantList model() const;
    void setModel(const QVariantList &newModel);
    void resetModel();

signals:
    void modelChanged();

private:
    QVariantList m_model;
    Q_PROPERTY(QVariantList model READ model WRITE setModel RESET resetModel NOTIFY modelChanged)
};

#endif // TABLEMODEL_H
