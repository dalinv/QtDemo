#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QFile>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    explicit WorkThread(qintptr socketDescriptor,const QString &strPath,QObject *parent=nullptr);
    ~WorkThread() override;
    void tcpClose(bool);

protected:
    void run() override;

signals:
    void sigError(QTcpSocket::SocketError socketErr);
    void sigRecvFileStatus(const QString &strFileName,bool bSuccessed);
    void sigConnected();

public slots:
    void slotReadyRead();
    void slotError(QTcpSocket::SocketError);

private:
    qintptr m_socketDescriptor;
    QString m_strPath;
    QTcpSocket *m_pSocket;


    qint64 m_fileNameSize;//文件名的大小
    qint64 m_recvSize;//已接收的文件大小
    qint64 m_totalSize;//接收数据的总大小 = 16字节+文件名大小+文件大小

    QString m_fileName;
    QByteArray m_buffer;
    QFile *m_pFile;
};

#endif // WORKTHREAD_H
