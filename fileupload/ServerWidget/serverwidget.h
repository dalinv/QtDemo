#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWidget; }
QT_END_NAMESPACE

class MyTcpServer;
class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

signals:
    void sigFileSavePath(const QString &strPath);

public slots:
    void on_pushBtnListening_clicked();
    void on_pushBtnSelectPath_clicked();
    void slotConnected();
    void slotSendStatus(const QString &strText,bool bFinished);

private:
    Ui::ServerWidget *ui;
    MyTcpServer *m_pTcpServer;
    int m_iCounter;

};
#endif // SERVERWIDGET_H
