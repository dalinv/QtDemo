#include "mytcpserver.h"
#include "workthread.h"
#include <serverwidget.h>

MyTcpServer::MyTcpServer(QObject *parent)
    :QTcpServer(parent)
{
    m_pWidget=dynamic_cast<QWidget *>(parent);
    connect(m_pWidget,SIGNAL(sigFileSavePath(const QString&)),this,SLOT(slotFileSvePath(const QString&)));
}

//每当有新的客户端连接请求到达时 handle用于接受连接的本地套接字描述符
void MyTcpServer::incomingConnection(qintptr handle)
{
    //启动新线程处理
    WorkThread *thread=new WorkThread(handle,m_strSavePath,this);

    //延迟销毁 (信号在同一个线程中，使用直接连接的方式)
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()),Qt::DirectConnection);
    //显示处理
    connect(thread,SIGNAL(sigConnected()),m_pWidget,SLOT(slotConnected()));
    connect(thread, SIGNAL(sigRecvFileStatus(const QString&, bool)), m_pWidget, SLOT(slotSendStatus(const QString&, bool)));
    connect(thread, SIGNAL(sigError(QTcpSocket::SocketError)), m_pWidget, SLOT(slotError(QTcpSocket::SocketError)));

    thread->start();
}

void MyTcpServer::slotFileSvePath(const QString &strPath)
{
    m_strSavePath=strPath;
}
