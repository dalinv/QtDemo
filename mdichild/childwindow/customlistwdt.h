#ifndef CUSTOMLISTWDT_H
#define CUSTOMLISTWDT_H

#include <QListWidget>

class CustomListWdt : public QListWidget
{
public:
    CustomListWdt();

    static QString sNodeType() { return QStringLiteral("item/node"); }
protected:
//    void mouseMoveEvent(QMouseEvent *event);
//    void mousePressEvent(QMouseEvent *event);
//    void dragEnterEvent(QDragEnterEvent *event) override;
//    void dragMoveEvent(QDragMoveEvent *event) override;//鼠标离开控件
//    void dropEvent(QDropEvent *event) override;
//    void startDrag(Qt::DropActions supportedActions) override;//开始拖拽


private:
    QPoint m_dragPos; // 拖放起点
    QListWidgetItem *m_dragItem; // 被拖放的item
};

#endif // CUSTOMLISTWDT_H
