#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <childwindow/mdichild.h>

#include <QMdiSubWindow>
#include <QAction>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QStringList>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_titleNum(1)
{
    ui->setupUi(this);
    setListWdtStyle();
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);//设置子窗口的视口模式
    ui->mdiArea->setTabsClosable(true);//设置子窗口的关闭按钮

    connect(ui->actionNew,&QAction::triggered,this,&MainWindow::slotMenuNew);
    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::slotMenuSave);
    connect(ui->actionOpen,&QAction::triggered,this,&MainWindow::slotMenuOpen);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::slotExit);
    connect(ui->actionNodeType,&QAction::triggered,this,&MainWindow::slotNodeType);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//新建子页面
void MainWindow::slotMenuNew()
{
    QString strTitle="New"+QString::number(m_titleNum);
    QJsonObject jsonObj;
    newMdiChild(strTitle,jsonObj);
}

void MainWindow::newMdiChild(const QString &strName,const QJsonObject &jsonObj)
{
    MdiChild *mdiChild=new MdiChild();
    ui->mdiArea->addSubWindow(mdiChild);
    m_currentMdiChild = mdiChild;
    mdiChild->setWindowTitle(strName);
    mdiChild->show();
    mdiChild->initScene(jsonObj);

    connect(mdiChild,&MdiChild::sigSaveFile,this,&MainWindow::slotMenuSave);
    m_titleNum++;
}

void MainWindow::slotMenuOpen()
{
    QString strFilePath = QFileDialog::getOpenFileName(this, tr("Open File"),"", "*.form");
    if(!strFilePath.isEmpty())
    {
        QFile file(strFilePath);
        if(!file.open(QIODevice::ReadOnly))
        {
            QMessageBox::warning(this,tr("Tips"),tr("cannot open file:\n%1").arg(strFilePath));
            return;
        }


        QJsonParseError *err=new QJsonParseError;
        QJsonDocument jsonDoc=QJsonDocument::fromJson(file.readAll(),err);

        if(err->error!=QJsonParseError::NoError)//解析是否出现错误
        {
            qDebug()<<err->errorString();
            return;
        }

        QJsonObject rootObj=jsonDoc.object();//获取对象
        QFileInfo fileInfo(strFilePath);
        newMdiChild(fileInfo.fileName(),rootObj);//新建頁面

        file.close();
    }
}

void MainWindow::slotMenuSave()
{
    if(m_currentMdiChild->getSaveFlag())//已经保存过的文件 直接保存
    {
        m_currentMdiChild->slotSaveAllInfo(m_currentMdiChild->getSavePath());
    }
    else {
        QString strFilePath =
            QFileDialog::getSaveFileName(this, tr("Save File"), "", "*.form");
        if(!strFilePath.isEmpty())
        {
            m_currentMdiChild->slotSaveAllInfo(strFilePath);
        }
    }
}

void MainWindow::slotExit()
{
    QApplication::closeAllWindows();
}

void MainWindow::slotNodeType()
{
    if(ui->dockWidget->isHidden())
    {
        ui->dockWidget->show();
    }
}

void MainWindow::setListWdtStyle()
{
    ui->listWidget->setIconSize(QSize(32, 32));
    ui->listWidget->setGridSize(QSize(64, 64));
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setResizeMode(QListWidget::Adjust);

    ui->listWidget->setDragEnabled(true);
    ui->listWidget->setDragDropMode(QAbstractItemView::DragOnly);
    ui->listWidget->setDropIndicatorShown(true);

    ui->listWidget->setAcceptDrops(false);//是否接收drag和drop
    ui->listWidget->setDefaultDropAction(Qt::CopyAction);
    ui->listWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget->setMouseTracking(true);
}


void MainWindow::on_mdiArea_subWindowActivated(QMdiSubWindow *)
{
    QMdiSubWindow *mdiSubWindow=ui->mdiArea->activeSubWindow();
    if(!mdiSubWindow)
        return;

    QWidget *wdt=mdiSubWindow->widget();
    assert(wdt);
    m_currentMdiChild=dynamic_cast<MdiChild *>(wdt);
    assert(m_currentMdiChild);
}

