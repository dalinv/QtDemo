#pragma comment(lib,"libxl.lib")
#include "mytableview.h"
#include "ui_mytableview.h"
#include "core/include/include_libxl/libxl.h"
#include "mymodel.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTableWidgetItem>
#include <QMetaType>
#include <QStringLiteral>
#include <QElapsedTimer>

using namespace libxl;

MyTableView::MyTableView(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MyTableView)
    , m_pModel(new MyModel())
{
    ui->setupUi(this);
    ui->tableView->setModel(m_pModel);
    connect(this, &MyTableView::sigGetRowData, m_pModel, &MyModel::slotGetRowData);
    connect(this, &MyTableView::sigGetMaxMiniValue, m_pModel, &MyModel::slotGetMaxMiniValue);
}

MyTableView::~MyTableView()
{
    delete ui;
}

void MyTableView::on_pushButton_clicked()
{
    QString strFileName = QFileDialog::getOpenFileName(this, tr("select file"),
        "", "*.xlsx");
    if(strFileName.isEmpty())
    {
        return;
    }
    ui->lineEdit->setText(strFileName); 
    //m_pModel->slotReadExcel(strFileName);
    loadExcel(strFileName);
}

void MyTableView::loadExcel(const QString& strFileName)
{
    QElapsedTimer time;
    time.start();

    Book *pBook=xlCreateXMLBookA();  //获取book对象
    pBook->setKey("Smartware Cloud (Nanjing) Information & Technology Co., Ltd.", "windows-2c27220207ceee076ab26a6badp3f2s1");

    bool bLoad = pBook->load(strFileName.toLocal8Bit());//加载excel文件
    if (!bLoad)
    {
        //QMessageBox::warning(this, QString("Tips"), pBook->errorMessage());
        return;
    }

    int sheetCount = pBook->sheetCount();
    for (int i = 0; i < sheetCount; ++i)
    {
        Sheet *sheet = pBook->getSheet(i);//获取sheet对象
        if (sheet)
        {
            m_iRows = sheet->lastRow();
            m_iCols = sheet->lastCol();

            m_pModel->setRowCount(m_iRows);
            m_pModel->setColumnCount(m_iCols);

            //设置水平表头
            QStringList lsField;
            for (int iCol = 0; iCol < m_iCols; ++iCol)
            {
                QString strField=QString::fromLocal8Bit(sheet->readStr(0, iCol));
                lsField.append(strField);
                if(strField==QStringLiteral("交易金额"))
                {
                    m_iColumnMoney=iCol;
                }
            }
            m_pModel->setHorizontalHeaderLabels(lsField);

            //设置表格内容
            for (int iRow = 1; iRow < m_iRows; ++iRow)
            {
                QStringList lStrRow;
                for (int iCol = 0; iCol < m_iCols; ++iCol)
                {
                    //先判断单元格的数据类型，再使用对应的函数来读取
                    CellType cellType = sheet->cellType(iRow, iCol);

                    switch (cellType) {
                    case CellType::CELLTYPE_NUMBER://数值型
                    {
                        lStrRow.append(QString::number(sheet->readNum(iRow, iCol)));
                        //m_pModel->setData(m_pModel->index(iRow - 1, iCol), QString::number(sheet->readNum(iRow, iCol)), Qt::DisplayRole);
                        //qDebug() << QString::number(sheet->readNum(iRow, iCol));
                        break;
                    }

                    case CellType::CELLTYPE_STRING://字符型
                    {
                        QString str = QString::fromLocal8Bit(sheet->readStr(iRow, iCol));
                        lStrRow.append(str);
                        //m_pModel->setData(m_pModel->index(iRow - 1, iCol), str, Qt::DisplayRole);
                        //qDebug() << QString::fromLocal8Bit(sheet->readStr(iRow, iCol));
                        break;
                    }

                    case CellType::CELLTYPE_BOOLEAN://布尔
                        break;

                    case CellType::CELLTYPE_EMPTY:
                        break;

                    case CellType::CELLTYPE_BLANK:
                        break;

                    case CellType::CELLTYPE_ERROR:
                        break;
                    }

                    //获取交易金额列的最大值和最小值
                    if (iCol == m_iColumnMoney)
                    {
                        QString strNum=sheet->readStr(iRow, iCol);
                        double dNum=strNum.toDouble();
                        m_maxValue = std::max(dNum, m_maxValue);
                        m_minValue = std::min(dNum, m_minValue);
                    }
                }
                emit sigGetRowData(lStrRow);
            }
        }
    }
    emit sigGetMaxMiniValue(m_iColumnMoney,m_maxValue,m_minValue);
    pBook->release();

    qDebug() << "spend times: " << time.elapsed() / 1000 << " s";
}
