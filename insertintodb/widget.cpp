#include "widget.h"
#include "workthread.h"

#include <QMouseEvent>
#include <QMoveEvent>
#include <QDateTime>
#include <QPainter>
#include <QLine>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , m_pWorkThread(new WorkThread())
{
    this->setMouseTracking(true);
}

Widget::~Widget()
{
    m_pWorkThread->terminate();
    m_pWorkThread->wait();
}

void Widget::paintEvent(QPaintEvent *)
{
    if (m_quePoints.size() <= 0)
    {
        return;
    }

    QPainter painter(this);
    painter.setPen(QPen(Qt::green, 10));
    painter.setRenderHint(QPainter::Antialiasing);

    //画线 长度100
    QQueue<QPoint>::iterator it;
    for (it = m_quePoints.begin(); it != m_quePoints.end()-1; ++it)
    {
        painter.drawLine(*it, *(it + 1));
    }
}

//画线的点
void Widget::addToQueue(QPoint point)
{
    if (m_quePoints.count() > 100)
    {
        m_quePoints.removeFirst();
    }
    m_quePoints.append(point);
    update();
}


//启动线程将数据插入到数据库
void Widget::addDataToList(const QString &strEventName, QPoint point)
{
    QVector<QVariant> vecRow;
    vecRow.push_back(point.x());
    vecRow.push_back(point.y());
    vecRow.push_back(strEventName);
    vecRow.push_back(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));

    m_vecData.push_back(vecRow);

    //批量插入数据库
    if (m_vecData.size() % 1000 == 0)
    {
        m_pWorkThread->addToTask(m_vecData);
        m_vecData.clear();
        if (!m_pWorkThread->isRunning())
        {
            m_pWorkThread->start();
        }
    }
}

//鼠标事件
void Widget::mousePressEvent(QMouseEvent *event)
{
    QString strEventName;
    if(event->button()==Qt::LeftButton)
    {
        strEventName =QString("mousePressLeftButton");
    }
    else if(event->button()==Qt::RightButton)
    {
        strEventName =QString("mousePressRightButton");
    }
    else if(event->button()==Qt::MiddleButton)
    {
        strEventName =QString("mousePressMiddleButton");
    }
    addToQueue(event->pos());
    addDataToList(strEventName, event->pos());
}
void Widget::mouseMoveEvent(QMouseEvent *event)
{
    addToQueue(event->pos());
    addDataToList("move", event->pos());
}

void Widget::mouseReleaseEvent(QMouseEvent *event)
{
    addToQueue(event->pos());
    addDataToList("mouseRelease", event->pos());
}

void Widget::mouseDoubleClickEvent(QMouseEvent *event)
{
    addToQueue(event->pos());
    addDataToList("mouseDoubleClick", event->pos());
}
