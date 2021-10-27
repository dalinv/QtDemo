#include "serverwidget.h"
#include "ui_serverwidget.h"
#include "mytcpserver.h"

#include <QHostAddress>
#include <QStringLiteral>
#include <QFileDialog>
#include <QMetaEnum>

ServerWidget::ServerWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ServerWidget)
    , m_pTcpServer(new MyTcpServer(this))
    , m_iCounter(0)
{
    ui->setupUi(this);
    ui->lineEditSavePath->setReadOnly(true);
    //更新文件保存位置
    connect(this, &ServerWidget::sigFileSavePath, m_pTcpServer, &MyTcpServer::slotFileSvePath);
}

ServerWidget::~ServerWidget()
{
    delete ui;
}

//服务器开启监听
void ServerWidget::on_pushBtnListening_clicked()
{
    uint iPort=ui->lineEditPort->text().toUInt();
    QHostAddress ipAddress;
    ipAddress.setAddress(ui->lineEditIP->text());

    if(!m_pTcpServer->listen(ipAddress,iPort))
    {
        ui->textEdit->append(QStringLiteral("无法启动服务器：%1").arg(m_pTcpServer->errorString()));
    }
    else {
        ui->pushBtnListening->setEnabled(false);
        ui->textEdit->append(QStringLiteral("开始监听..."));
    }
}

//设置文件保存位置
void ServerWidget::on_pushBtnSelectPath_clicked()
{
    QString strFilePath=QFileDialog::getExistingDirectory(this,"Select Directory");
    if(!strFilePath.isEmpty())
    {
        ui->lineEditSavePath->setText(strFilePath);
        emit sigFileSavePath(ui->lineEditSavePath->text());//更新文件保存位置
    }
}

void ServerWidget::slotConnected()
{
    ui->textEdit->append(QStringLiteral("客户端%1连接成功").arg(++m_iCounter));
}

void ServerWidget::slotSendStatus(const QString &strText, bool bFinished)
{
    if (bFinished)
    {
        ui->textEdit->append(QStringLiteral("文件：%1接收成功").arg(strText));
    }
    else
    {
        ui->textEdit->append(QStringLiteral("文件：%1接收失败").arg(strText));
    }
}