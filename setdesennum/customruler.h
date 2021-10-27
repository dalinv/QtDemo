#ifndef CUSTOMRULER_H
#define CUSTOMRULER_H

#include <QWidget>
#include <QLabel>
class CustomRuler:public QWidget
{
    Q_OBJECT
public:
    CustomRuler(QWidget *parent = nullptr);
    void setRulerId(const int& id);
    int getRulerId();

signals:
    void rulerMoving(int id,QPoint pos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QLabel *m_lbRuler;
    int m_rulerId;
    QPoint m_posPress;
};

#endif // CUSTOMRULER_H
