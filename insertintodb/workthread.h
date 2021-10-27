#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QSqlDatabase>
#include <QQueue>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();
    void addToTask(const QVector<QVector<QVariant>> &vecData);

protected:
    void run() override;

private:
    bool createTable(QString &strErr);
    bool execQuery(QString &strErr);
    bool insertToDB(QString &strErr);
private:
    QSqlDatabase m_db;
    QString m_strTableName;
    QQueue<QVector<QVector<QVariant>>> m_queData;
    QVector<QVector<QVariant>> m_vecData;
};

#endif // WORKTHREAD_H
