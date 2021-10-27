#include "node.h"

#include <QIcon>
#include <QPainter>
#include <QFontMetrics>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#define ITEM_SIZE 34
#define ICON_SIZE 32

Node::Node(QString strName,QIcon icon,QPointF point,int z)
    : QGraphicsItem()
    ,m_bPressed(false)
    ,m_bMouseMode(true)
{
    m_pixmap=QPixmap(icon.pixmap(32,32));
    m_strName=strName;
    m_font.setPixelSize(12);
    m_nodePos=point;

    this->setFlag(QGraphicsItem::ItemIsMovable, true);
    this->setAcceptDrops(true);
    connect(this->scene(),SIGNAL(sigModeChange(bool)),this,SLOT(slotModeChanged(bool)));
}

Node::~Node()
{
}

QRectF Node::boundingRect() const
{
    QFontMetrics fm = QFontMetrics(m_font);
    QRect rectText = fm.boundingRect(m_strName);
    int iTextWidth = rectText.width();
    qreal qIconWidth = 34,qWidth=2;
    if(qIconWidth < iTextWidth)
    {
        qIconWidth = iTextWidth;
    }
    QRectF rect(-qIconWidth / 2-qWidth, -qIconWidth / 2-qWidth,qIconWidth+2*qWidth,qIconWidth+2*qWidth);
    return rect;
}

void Node::paint(QPainter *painter,const QStyleOptionGraphicsItem *, QWidget *)
{
    //画一个item的全部内容，最好在boundingRect返回的区域内画图
    painter->setRenderHint(QPainter::Antialiasing, true);

    //图标
    painter->drawPixmap(boundingRect().topLeft(),m_pixmap);

    //名称
    QFontMetrics metrics = QFontMetrics(m_font);
    QRect textRect = metrics.boundingRect(m_strName);
    int m_iTextWidth = textRect.width();
    painter->setFont(m_font);

    QRect paintTextRect(-m_iTextWidth / 2, 32 / 2, m_iTextWidth, textRect.height());
    painter->drawText(paintTextRect, Qt::AlignHCenter | Qt::AlignVCenter, m_strName);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_bMouseMode && event->button()==Qt::LeftButton)
    {
        m_bPressed=true;
        m_posStart=event->scenePos();
    }
}

void Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_bPressed && m_bMouseMode)
    {
        qreal qx=event->scenePos().rx()-m_posStart.rx();
        qreal qy=event->scenePos().ry()-m_posStart.ry();
        this->moveBy(qx,qy);

        emit sigNodeMoving();
        update();
    }
    m_posStart = event->scenePos();
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_bPressed && m_bMouseMode)
    {
        qreal qx=event->scenePos().rx()-m_posStart.rx();
        qreal qy=event->scenePos().ry()-m_posStart.ry();
        m_nodePos.rx()=event->scenePos().rx();
        m_nodePos.ry()=event->scenePos().ry();
        this->moveBy(qx,qy);

        update();
    }
}

void Node::slotModeChanged(bool mode)
{
    m_bMouseMode=mode;
}

QString Node::getName()
{
    return m_strName;
}

QString Node::getStrImg()
{
    return imageToString(m_pixmap.toImage());
}

QPointF Node::getPos()
{
    return m_nodePos;
}

QPointF Node::getDrawLinePos()
{
    return mapToScene(boundingRect().center());
}

QString Node::imageToString(const QImage &image)
{
    QByteArray byteArray;
    QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
    dataStream << image;
    QString strImgData = QString::fromLocal8Bit(byteArray.toBase64());
    return strImgData;
}
