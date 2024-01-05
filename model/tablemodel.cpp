#include "tablemodel.h"
TableModel::TableModel()
{
    QVariantList q;
    for(int i=0;i<10;i++){
        QVariantMap p;
        p.insert("STT",i);
        p.insert("name","teen la gif");
        p.insert("Time","time");
        q.push_back(p);
    }
    setModel(q);
}

QVariantList TableModel::model() const
{
    return m_model;
}
void TableModel::setModel(const QVariantList &newModel)
{
    if (m_model == newModel)
        return;
    m_model = newModel;
    emit modelChanged();
}

void TableModel::resetModel()
{
    setModel({}); // TODO: Adapt to use your actual default value
}
