#ifndef LINKLINE_H
#define LINKLINE_H

#include <QGraphicsItem>
#include <QRectF>

struct SLineInfo
{
    double m_dPosXSatrt;
    double m_dPosYStart;
    double m_dPosXEnd;
    double m_dPosYEnd;
};

class Node;
class linkLine
        : public QObject
        , public QGraphicsItem
{
public:
    linkLine(Node *startNode,Node *endNode);
    linkLine(const QLineF &line);
    ~linkLine();
    QPointF getStartPos();
    QPointF getEndPos();


    void setConfigInfo();
    void getConfigInfo();
    void updatePos();


    enum
    {
        Type = UserType + 1
    };
    int type() const override
    {
        return Type;
    }

public slots:
    void slotNodeMoving();//拖动节点的时候，画线

protected:
    virtual QRectF boundingRect() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QLineF m_line;
    Node *m_pStartNode;
    Node *m_pEndNode;
    QPointF m_posStart;//记录画线的两个端点
    QPointF m_posEnd;
};

#endif // LINKLINE_H
