#ifndef CUSTOMBUTTON_H
#define CUSTOMBUTTON_H

#include <QToolButton>
#include <QWidget>

class CustomNode : public QToolButton
{
public:
    CustomNode(QWidget *parent = nullptr);

protected:
//    void mousePressEvent(QMouseEvent *) override;
//    void mouseReleaseEvent(QMouseEvent *) override;
};

#endif // CUSTOMBUTTON_H
