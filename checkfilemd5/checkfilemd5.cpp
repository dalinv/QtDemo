#include "checkfilemd5.h"
#include "ui_checkfilemd5.h"
#include <QStringLiteral>
#include <QFile>
#include <QCryptographicHash>
#include <QFileDialog>

CheckFileMd5::CheckFileMd5(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckFileMd5)
{
    ui->setupUi(this);
    ui->textBrowser->append(QStringLiteral("Md5值"));
}

CheckFileMd5::~CheckFileMd5()
{
    delete ui;
}

QString CheckFileMd5::getFilePath(int index)
{
    ui->labelResult->clear();
    ui->textBrowser->clear();
    ui->textBrowser->append(QStringLiteral("Md5值"));
    QString strFile =QFileDialog::getOpenFileName(this,"Select File");
    if(strFile.isEmpty())
    {
        return QString();
    }

    //获取Md5值
    QFile *pFile=new QFile(strFile);
    if(!pFile->open(QIODevice::ReadOnly))
    {
        return QString();
    }
    QString strMd5 =QCryptographicHash::hash(pFile->readAll(),QCryptographicHash::Md5).toHex();
    pFile->close();

    //显示Md5值
    QString strFileName;
    if(index==0)
    {
        m_strMd5ValueA=strMd5;
    }
    else if(index==1)
    {
        m_strMd5ValueB=strMd5;
    }
    return strFile;
}

void CheckFileMd5::on_pushButtonSelectA_clicked()
{
   ui->lineEditA->setText(getFilePath(0));
}

void CheckFileMd5::on_pushButtonSelectB_clicked()
{
    ui->lineEditB->setText(getFilePath(1));
}

void CheckFileMd5::on_pushButtonCheckMd5_clicked()
{
    if(m_strMd5ValueA==m_strMd5ValueB)
    {
        ui->labelResult->setText(QStringLiteral("文件相同"));
    }
    else
    {
        ui->labelResult->setText(QStringLiteral("文件不同"));
    }
    ui->textBrowser->append(QStringLiteral("文件A:")+m_strMd5ValueA);
    ui->textBrowser->append(QStringLiteral("文件B:")+m_strMd5ValueB);
}
