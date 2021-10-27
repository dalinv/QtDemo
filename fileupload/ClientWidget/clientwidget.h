#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();
    void sendFile();

private slots:
    void on_pushBtnSelectFile_clicked();
    void on_pushButtonConnect_clicked();
    void slotError(QAbstractSocket::SocketError socketErr);

private:
    Ui::ClientWidget *ui;
    QTcpSocket *m_pSocket;
    QByteArray m_outBlock;

    QString m_strFile;
    int m_iFileSize;
};
#endif // CLIENTWIDGET_H
