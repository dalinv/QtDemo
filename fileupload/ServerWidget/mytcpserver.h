#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QTcpServer>
#include <QWidget>

class WorkThread;
class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent=nullptr);

protected:
    void incomingConnection(qintptr handle) override;

public slots:
    void slotFileSvePath(const QString &strPath);

private:
    QWidget *m_pWidget;
    QString m_strSavePath;
};

#endif // MYTCPSERVER_H
