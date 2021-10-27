#ifndef MYMODEL_H
#define MYMODEL_H

#include <QObject>
#include <QStandardItemModel>
#include <vector>

class MyModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit MyModel(QObject *parent=nullptr);

public slots:
    void slotGetRowData(const QStringList &lsData);
    void slotGetMaxMiniValue(const int & iCol,const double &maxValue,const double &minValue);

protected:
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    double m_maxValue;
    double m_minValue;
    int m_iColumnMoney;
    std::vector<QStringList> m_vecData;
};

#endif // MYMODEL_H
