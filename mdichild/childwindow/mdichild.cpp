#include "mdichild.h"
#include "ui_mdichild.h"
#include "../customgraphics/customgraphicsscence.h"
#include "../mainwindow.h"
#include <QKeyEvent>
#include <QSaveFile>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

MdiChild::MdiChild(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MdiChild)
    , m_bMouseMode(true)
    ,m_bSave(false)
{
    ui->setupUi(this);

    ui->graphicsView->setAcceptDrops(true);
    m_scene=new CustomGraphicsScence(this);
    ui->graphicsView->setScene(m_scene);
}

MdiChild::~MdiChild()
{
    delete ui;
}

void MdiChild::initScene(const QJsonObject &rootObj)
{
    if(rootObj.isEmpty())
    {
        return;
    }
    m_scene->initSceneInfo(rootObj);
}

void MdiChild::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if(keyEvent->key()==Qt::Key_I)
    {
        m_bMouseMode=false;//false为插入边模式
        setCursor(Qt::CrossCursor);
        emit signalModeChange(m_bMouseMode);
    }

    if(keyEvent->key()==Qt::Key_Space)
    {
        m_bMouseMode=true;
        setCursor(Qt::ArrowCursor);
        emit signalModeChange(m_bMouseMode);
    }
}

void MdiChild::slotSaveAllInfo(const QString &strPath)
{
    QJsonObject jsonObjAll;
    m_scene->saveToJson(jsonObjAll);
    writeData(jsonObjAll,strPath);
    m_bSave=true;
    m_strSavePath=strPath;
}

void MdiChild::writeData(const QJsonObject &jsonObj,const QString &strPath)
{
    QSaveFile file(strPath);
    bool ret = file.open(QFile::WriteOnly);
    if(ret)
    {
        file.write(QJsonDocument(jsonObj).toJson());
        if(file.commit())
        {
            QMessageBox::information(this,QString("tips"),QString("Save Successed!"));
        }
    }
    else
    {
        if(!m_bSave)
        {
            QMessageBox::warning(this, tr("Hint"), tr("Saved to file failed!"));
        }
    }
}

void MdiChild::closeEvent(QCloseEvent *event)
{
    if(!m_bSave)
    {
        QMessageBox::StandardButton resButton=QMessageBox::information(this,QString("tips"),QString("The file not saved,are you want to save it?"),QMessageBox::Yes |QMessageBox::No,QMessageBox::Yes);
        switch (resButton) {
        case QMessageBox::Yes:
        {
            emit sigSaveFile();
            break;
        }
        case QMessageBox::No:
        {
            break;
        }
        default:
            break;
        }
    }
    QWidget::closeEvent(event);
}

bool MdiChild::getSaveFlag()
{
    return m_bSave;
}

QString MdiChild::getSavePath()
{
    return m_strSavePath;
}
