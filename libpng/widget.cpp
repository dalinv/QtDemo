#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>
#include <QFileInfo>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    setWindowTitle("libpng tool");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnSelectFloder_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this, "选择png文件夹");
    if(!path.isEmpty())
    {
        ui->lineEditFolder->setText(path);
    }
}

void Widget::on_btnExec_clicked()
{
    ui->textEdit->clear();

    QStringList imagePathList;
    QString path = ui->lineEditFolder->text().trimmed();
    if(!path.isEmpty())
    {
        QDir dir(path);
        QStringList imgList = dir.entryList(QStringList("*.png"));

        foreach(auto img, imgList)
        {
            imagePathList.append(path + "/" + img);
        }
    }

    int count = 0;
    foreach(auto img, imagePathList)
    {
        QFileInfo info(img);
        ui->textEdit->append(info.fileName());

        QImage image(img);
        image.save(img, "png");
        count++;
        qApp->processEvents();//在需要执行这个耗时程序时不断的刷新界面
    }

    ui->textEdit->append(QString("共处理 %1 个图片").arg(count));
}
