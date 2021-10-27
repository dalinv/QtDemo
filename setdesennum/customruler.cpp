#include "customruler.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
CustomRuler::CustomRuler(QWidget *parent)
    :QWidget(parent)
{
    resize(32,32);
    m_lbRuler=new QLabel(this);
    m_lbRuler->setPixmap(QPixmap(":/new/res/icon.png"));
}

void CustomRuler::setRulerId(const int& id)
{
    m_rulerId=id;
}

int CustomRuler::getRulerId()
{
    return  m_rulerId;
}

void CustomRuler::mousePressEvent(QMouseEvent *event)
{
    //记录点击时候的位置
    m_posPress = event->pos();
}

void CustomRuler::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point=this->mapToParent(event->pos()- m_posPress);
    this->move(point);

    emit rulerMoving(m_rulerId, point);
}
