#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QQueue>
#include <deque>

class WorkThread;
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void addToQueue(QPoint point);
    void addDataToList(const QString &strEventName,QPoint point);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    WorkThread *m_pWorkThread;
    QQueue<QPoint> m_quePoints;
    QVector<QVector<QVariant>> m_vecData;
};
#endif // WIDGET_H
