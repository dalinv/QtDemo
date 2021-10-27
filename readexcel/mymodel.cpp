#include "mymodel.h"

MyModel::MyModel(QObject *parent)
    : QStandardItemModel(parent)
    ,m_maxValue(0)
    ,m_minValue(0)
{

}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (m_vecData.empty() || index.row()+1 > m_vecData.size() || index.column() > m_vecData[0].size())//判断边界条件
        {
            return QVariant();
        }
        return m_vecData[index.row()][index.column()];
    }

    if (role == Qt::BackgroundRole)
    {
        if ( index.column() != m_iColumnMoney)
        {
            return QVariant();
        }
        double qValue = this->data(this->index(index.row(),m_iColumnMoney),Qt::DisplayRole).toDouble();
        int colorValue = static_cast<int>(ceil(255.0 * (qValue - m_minValue) / (m_maxValue - m_minValue)));
        QColor color(colorValue, 0, 0);
        return color;
    }

    if (role == Qt::ForegroundRole)
    {
        if (index.column() != m_iColumnMoney)
        {
            return QVariant();
        }
        return QColor(Qt::white);
    }
    return QStandardItemModel::data(index, role);
}


void MyModel::slotGetRowData(const QStringList &lsData)
{
    if(lsData.isEmpty())
    {
        return;
    }
    m_vecData.push_back(lsData);
}

void MyModel::slotGetMaxMiniValue(const int & iCol,const double &maxValue,const double &minValue)
{
    m_iColumnMoney=iCol;
    m_maxValue=maxValue;
    m_minValue=minValue;
}
