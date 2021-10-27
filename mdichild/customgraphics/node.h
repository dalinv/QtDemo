#ifndef NODE_H
#define NODE_H

#include <QGraphicsItem>
#include <QFont>
#include <QRectF>

struct SNodeInfo
{
    QString m_strImgPath;
    QString m_strName;
    double m_dPosX;
    double m_dPosY;
};

class Node
        : public QObject
        , public QGraphicsItem
{
    Q_OBJECT
public:
    Node(QString strName,QIcon icon,QPointF point,int z=1);
    ~Node();
    QPointF getDrawLinePos();
    QString getName();
    QString getStrImg();
    QPointF getPos();


    void readJson(const QJsonObject &obj);


    QString imageToString(const QImage &image);


    enum
    {
        Type = UserType + 2
    };
    int type() const override
    {
        return Type;
    }

public slots:
    void slotModeChanged(bool mode);

signals:
    void sigNodeMoving();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool m_bPressed;
    bool  m_bMouseMode;

    SNodeInfo m_nodeInfo;
    QString m_strName;
    QPointF m_posStart;
    QPointF m_nodePos;
    QFont m_font;
    QPixmap m_pixmap;
};

#endif // NODE_H
