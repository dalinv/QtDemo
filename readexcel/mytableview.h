#ifndef MYTABLEVIEW_H
#define MYTABLEVIEW_H

#include <QMainWindow>
#include <QStandardItemModel>

QT_BEGIN_NAMESPACE
namespace Ui { class MyTableView; }
QT_END_NAMESPACE

class MyModel;
class MyTableView : public QMainWindow
{
    Q_OBJECT

public:
    MyTableView(QWidget *parent = nullptr);
    ~MyTableView();
    void loadExcel(const QString& strFileName);

signals:
    void sigGetRowData(const QStringList &lsData);
    void sigGetMaxMiniValue(const int & iCol,const double &maxValue,const double &minValue);

private slots:
    void on_pushButton_clicked();

private:
    Ui::MyTableView *ui;
    MyModel *m_pModel;
    int m_iRows;
    int m_iCols;
    int m_iColumnMoney;
    double m_maxValue;
    double m_minValue;
    //QList<QStringList> m_lsData;
    std::vector<QStringList> m_vecData;
    std::vector<int> m_vecRedValue;

};
#endif // MYTABLEVIEW_H
