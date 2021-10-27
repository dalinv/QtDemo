#include "clientwidget.h"
#include "ui_clientwidget.h"

#include <QFileDialog>
#include <QFileInfo>
#include <QHostAddress>
#include <QCryptographicHash>
#include <QDebug>
#include <QMessageBox>

#define PACKET_MIN_SIZE (sizeof (qint64)*2)

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ClientWidget)
    ,m_pSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    connect(m_pSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));
    connect(m_pSocket, SIGNAL(disconnect()), m_pSocket, SLOT(deleteLater()));
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

//连接服务器
void ClientWidget::on_pushButtonConnect_clicked()
{
    uint iPort=ui->lineEditPort->text().toUInt();
    QHostAddress ipAddress;
    ipAddress.setAddress(ui->lineEditIP->text());

    m_pSocket->connectToHost(ipAddress,iPort);
}

//发送文件
void ClientWidget::on_pushBtnSelectFile_clicked()
{
    on_pushButtonConnect_clicked();
    ui->progressBar->setValue(0);
    ui->lineEditFile->clear();
    m_strFile =QFileDialog::getOpenFileName(this,"Select File");
    if(m_strFile.isEmpty())
    {
        return;
    }
    QFileInfo fileInfo(m_strFile);
    m_iFileSize=fileInfo.size();
    int iFileNameSize=fileInfo.fileName().size();
    int iTotalSize =m_iFileSize + iFileNameSize;

    QFile *pFile=new QFile(m_strFile);
    if(!pFile->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Warning", QStringLiteral("读文件失败"));
        return;
    }
    pFile->close();

    QString strFileInfo = QString("%1##%2##%3").arg(iTotalSize).arg(iFileNameSize).arg(fileInfo.fileName());
    m_pSocket->write(strFileInfo.toUtf8());
    m_pSocket->waitForBytesWritten();
    sendFile();
}

//发送文件数据
void ClientWidget::sendFile()
{
    QFile *pFile = new QFile(m_strFile);
    if (!pFile->open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Warning", QStringLiteral("读文件失败"));
        return;
    }

    //分片发送数据
    QDataStream buffer(&m_outBlock,QIODevice::WriteOnly);
    buffer.setVersion(QDataStream::Qt_5_12);
    buffer.setByteOrder(QDataStream::LittleEndian);

    qint64 len=0;
    qint64 sendLen=0;
    do
    {
        char *buf=new char[4*1024*1024];
        len= pFile->read(buf, 4 * 1024 * 1024);
        len =m_pSocket->write(buf,len);
        m_pSocket->waitForBytesWritten();

        sendLen += len;
        int value=sendLen*100/m_iFileSize;
        ui->progressBar->setValue(value);

    }while(len>0);

    if(m_iFileSize==sendLen)
    {
        pFile->close();
        ui->progressBar->setValue(100);
        ui->lineEditFile->setText(m_strFile);
    }
}

//出错处理
void ClientWidget::slotError(QAbstractSocket::SocketError socketErr)
{
    switch (socketErr) {
    case QAbstractSocket::HostNotFoundError:
        qDebug()<<"主机不可达！";
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<"连接被拒绝！";
        break;
    case QAbstractSocket::RemoteHostClosedError:
    {
        qDebug() << "远程主机已经关闭！";
        m_pSocket->disconnectFromHost();
        break;
    }
    default:
        qDebug()<<tr("发生错误：").arg(m_pSocket->errorString());
        break;
    }
}
