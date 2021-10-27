#include "workthread.h"

#include <QByteArray>
#include <QDataStream>
#include <QFileInfo>
#include <QStringLiteral>
#include <QCryptographicHash>
#include <QDebug>

#define PACKET_MIN_SIZE (sizeof (qint64)*2)

WorkThread::WorkThread(qintptr socketDescriptor,const QString &strPath,QObject *parent)
    :QThread(parent)
    ,m_socketDescriptor(socketDescriptor)
    ,m_strPath(strPath)
    ,m_pSocket(new QTcpSocket())
    ,m_fileNameSize(0)
    ,m_recvSize(0)
    ,m_totalSize(0)
{
    m_pSocket->moveToThread(this);
    connect(m_pSocket, &QTcpSocket::readyRead, this, &WorkThread::slotReadyRead, Qt::DirectConnection);
    connect(m_pSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(slotError(QAbstractSocket::SocketError)));
    connect(m_pSocket, SIGNAL(disconnect()), m_pSocket, SLOT(deleteLater()));

    //连接方式：
    //如果信号在接收者所依附的线程内发射，使用直接连接
    //如果发射信号的信号和接收者所依附的线程不同，使用队列连接

    //QThread 是用来管理线程的，它所依附的线程和它管理的线程并不是同一个东西
    //QThread的对象依附在主线程中，所以他的slot函数会在主线程中执行，而不是次线程。除非：
    //QThread 对象依附到次线程中(通过movetoThread)
    //slot 和信号是直接连接，且信号在次线程中发射
}

WorkThread::~WorkThread()
{
}

void WorkThread::run()
{
    if(!m_pSocket->setSocketDescriptor(m_socketDescriptor))
    {
        emit sigError(m_pSocket->error());//出错信号
        return;
    }
    else
    {
        emit sigConnected();       
    }
    exec();//消息循环
}


void WorkThread::slotReadyRead()
{
    QByteArray byteData = m_pSocket->readAll();
   // m_pSocket->waitForReadyRead();

    if(m_recvSize==0)
    {
        if( m_fileNameSize == 0)
        {
            m_totalSize = QString(byteData).section("##", 0, 0).toInt();
            m_fileNameSize = QString(byteData).section("##", 1, 1).toInt();
            m_fileName = QString(byteData).section("##", 2, 2);
        }
        if(m_fileNameSize != 0)
        {
            m_recvSize += m_fileNameSize;

            QString strFile=m_strPath+"/"+m_fileName;
            m_pFile=new QFile(strFile);

            if(!m_pFile->open(QFile::WriteOnly))
            {
                emit sigRecvFileStatus(m_fileName,false);
                qDebug() << "write file error!";
                return;
            }
            else
            {
                m_pSocket->write("create file success");
            }
        }
        else
        {
            return;
        }
    }
    else
    {
        //当已接收数据小于文件总大小时，写入文件
        if (m_recvSize < m_totalSize)
        {
            m_recvSize += byteData.size();

            m_pFile->write(byteData);
        }

        if(m_recvSize == m_totalSize)
        {
            tcpClose(true);
        }
        else if(m_recvSize>m_totalSize)
        {
            tcpClose(false);
        }
    }    
}

void WorkThread::tcpClose(bool bState)
{
    m_pFile->close();
    emit sigRecvFileStatus(m_fileName, bState);
    finished({});
    m_pSocket->disconnectFromHost();
    quit();
    wait();
}

void WorkThread::slotError(QTcpSocket::SocketError)
{
    m_pSocket->disconnectFromHost();
}
