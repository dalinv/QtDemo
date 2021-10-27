#include "linkline.h"
#include "node.h"
#include <QIcon>
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>

linkLine::linkLine(Node *startNode,Node *endNode)
    :m_pStartNode(startNode)
    ,m_pEndNode(endNode)
    ,m_posStart(startNode->getDrawLinePos())
    ,m_posEnd(endNode->getDrawLinePos())
{
    connect(m_pStartNode,&Node::sigNodeMoving,this,&linkLine::slotNodeMoving);
    connect(m_pEndNode,&Node::sigNodeMoving,this,&linkLine::slotNodeMoving);
    updatePos();
}

linkLine::linkLine(const QLineF &line)
    :m_line(line)
{
    m_posStart=m_line.p1();
    m_posEnd=m_line.p2();
    update();
}

linkLine::~linkLine()
{

}

void linkLine::paint(QPainter *painter,const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen(QColor("#666666"),2);
    painter->setPen(pen);
    painter->drawLine(m_posStart,m_posEnd);
}


void linkLine::slotNodeMoving()
{
    updatePos();
}

void linkLine::updatePos()
{
    m_posStart=m_pStartNode->getDrawLinePos();
    m_posEnd=m_pEndNode->getDrawLinePos();
    update();
}

QRectF linkLine::boundingRect() const
{
    qreal x1=m_posStart.x();
    qreal y1=m_posStart.y();
    qreal x2=m_posEnd.x();
    qreal y2=m_posEnd.y();
    qreal xLeftTop,yLeftTop,xRightBottom,yRightBottom;
    if(x1>=x2)
    {
        xLeftTop=x1;
        xRightBottom=x2;
    }
    else {
        xLeftTop=x2;
        xRightBottom=x1;
    }

    if(y1>=y2)
    {
        yLeftTop=x1;
        yRightBottom=x2;
    }
    else {
        yLeftTop=x2;
        yRightBottom=x1;
    }

    QRectF rect(xLeftTop,yLeftTop,xRightBottom,yRightBottom);
    return rect;
}

QPointF linkLine::getStartPos()
{
    return m_posStart;
}

QPointF linkLine::getEndPos()
{
    return m_posEnd;
}
