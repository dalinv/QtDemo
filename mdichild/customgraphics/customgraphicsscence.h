#ifndef CUSTOMGRAPHICSSCENCE_H
#define CUSTOMGRAPHICSSCENCE_H

#include <QGraphicsScene>
#include <QListWidgetItem>

class linkLine;
class Node;
struct SNodeInfo;
struct SLineInfo;
class CustomGraphicsScence : public QGraphicsScene
{
    Q_OBJECT
public:
    CustomGraphicsScence(QWidget *parent = nullptr);
    void initSceneInfo(const QJsonObject &rootJson);
    void createNode(const QString &strName,const QIcon &icon, const QPointF& point);
    void createLine(const QPointF &posStart,const QPointF &posEnd);
    void updateView();
    void saveItems();
    void serialize(const QJsonObject &obj);
    QImage stringToImage(const QString &strImgData);

public slots:
    void slotModeChange(bool mode);
    void saveToJson(QJsonObject &obj);

signals:
    void sigModeChange(bool mode);

protected:
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) override;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) override;
    void dropEvent(QGraphicsSceneDragDropEvent *event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool m_bMouseMode;//true：默认模式 false：插入边模式
    bool m_bDrawLine;
    linkLine *m_pLastLineItem;
    Node *m_pNodeStart;
    Node *m_pNodeEnd;
    QPointF m_posStart;
    std::vector<SNodeInfo> m_vecNode;
    std::vector<SLineInfo> m_vecLine;
    bool m_bModify;
};

#endif // CUSTOMGRAPHICSSCENCE_H
