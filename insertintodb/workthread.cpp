#include "workthread.h"
#include <QSqlQuery>
#include <QDateTime>
#include <QSqlError>
#include <QUuid>
#include <QDebug>

#define STR_DB_NAME "event_record.db"
#define STR_DB_TABLE_NAME "event"

WorkThread::WorkThread()
{
}


void WorkThread::run()
{
    while (m_queData.size() >= 1)
    {
        m_vecData.clear();
        m_vecData = m_queData.dequeue();

        QString strErr;
        if (!insertToDB(strErr))
        {
            qDebug() << QString("insert into db failed, the reason: %1!").arg(strErr);
        }
    }
    if (m_db.isOpen())
    {
        m_db.close();
    }
}

bool WorkThread::insertToDB(QString &strErr)
{
    //初始化数据库
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(QString("D:/project1/%1").arg(STR_DB_NAME));

    if (!m_db.open())
    {
        strErr = m_db.lastError().text();
        return false;
    }

    //创建表
    if (!createTable(strErr))
    {
        return false;
    }

    //插入数据库
    if (!execQuery(strErr))
    {
        return false;
    }
    return true;
}

bool WorkThread::createTable(QString &strErr)
{    
    //表格按日期分类
    m_strTableName = STR_DB_TABLE_NAME + QDateTime::currentDateTime().toString("yyyyMMdd");
    
    //创建表
    QString strSql = QString("CREATE TABLE IF NOT EXISTS %1("
        "id INT PRIMARY KEY NOT NULL,"
        "posX INT NOT NULL,"
        "posY INT NOT NULL,"
        "event TEXT NOT NULL,"
        "time TEXT NOT NULL)").arg(m_strTableName);
    QSqlQuery query(m_db);
    if (!query.exec(strSql))
    {
        strErr = strSql;
        return false;
    }
    return true;
}

bool WorkThread::execQuery(QString &strErr)
{
    if (m_vecData.isEmpty())
    {
        return false;
    }
    QSqlQuery query(m_db);

    //获取id
    QString strSqlCount = QString("SELECT count(*) FROM %1").arg(m_strTableName);
    query.exec(strSqlCount);
    query.next();
    int iStart= query.value(0).toInt();
    iStart++;

    //插入数据
    QString strSql = QString("INSERT INTO %1(id,posX,posY,event,time)  VALUES(:id,:posx,:posy,:event,:time)").arg(m_strTableName);
    query.prepare(strSql);

    int iRows = m_vecData.size();
    int iCols = m_vecData[0].size();

    //第一列id
    QVariantList lstVariant;
    for (int id = iStart; id < iStart + iRows; ++id)
    {
        lstVariant.push_back(id);
    }
    query.addBindValue(lstVariant);

    //其它列
    for (int iCol = 0; iCol < iCols; ++iCol)
    {
        lstVariant.clear();
        for (int iRow = 0; iRow < iRows; ++iRow)
        {
            lstVariant.push_back(m_vecData[iRow][iCol]);
        }
        query.addBindValue(lstVariant);
    }

    //开启事务
    if (m_db.transaction())
    {
        if (!query.execBatch())
        {
            m_db.rollback();
            strErr = query.lastError().text();
            return false;
        }
        if (!m_db.commit())
        {
            m_db.rollback();
            strErr = m_db.lastError().text();
            return false;
        }
    }else{
        if (!query.execBatch())
        {
            strErr =  query.lastError().text();
            return false;
        }
    }
    query.finish();
    return true;
}

void WorkThread::addToTask(const QVector<QVector<QVariant>> &vecData)
{
    m_queData.append(vecData);
}