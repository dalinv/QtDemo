#ifndef CHECKFILEMD5_H
#define CHECKFILEMD5_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class CheckFileMd5; }
QT_END_NAMESPACE

class CheckFileMd5 : public QWidget
{
    Q_OBJECT

public:
    CheckFileMd5(QWidget *parent = nullptr);
    ~CheckFileMd5();

private:
    QString getFilePath(int index);

private slots:
    void on_pushButtonSelectA_clicked();
    void on_pushButtonSelectB_clicked();
    void on_pushButtonCheckMd5_clicked();

private:
    Ui::CheckFileMd5 *ui;
    QString m_strMd5ValueA;
    QString m_strMd5ValueB;
};
#endif // CHECKFILEMD5_H
