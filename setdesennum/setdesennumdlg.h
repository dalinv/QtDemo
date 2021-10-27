#ifndef SETDESENNUMDLG_H
#define SETDESENNUMDLG_H

#include <QDialog>
#include <QLabel>

enum RULER_ID
{
    leftRuler,
    rightRuler
};

class CustomRuler;

QT_BEGIN_NAMESPACE
namespace Ui { class SetDesenNumDlg; }
QT_END_NAMESPACE

class SetDesenNumDlg : public QDialog
{
    Q_OBJECT

public:
    SetDesenNumDlg(const QString& strPhone,QWidget *parent = nullptr);
    ~SetDesenNumDlg() override;
    void init();
    void updateDesennumText();
    void updateRulerPos(const int& start,const int& end);

public slots:
    void slotRulerMoving(int id,QPoint pos);//标尺移动过程中要处理的事情
    int judgePos(int pos);//判断在哪两个数之间

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Ui::SetDesenNumDlg *ui;
    QString m_strPhone;
    QString m_strNewPhone;
    CustomRuler *m_leftRuler;
    CustomRuler *m_rightRuler;

    int m_iStart,m_iEnd;
    int m_iWidth;
    int m_leftXPos,m_rightXPos;//光标x轴的最大范围
    int m_currentLeftRuler,m_currentRightRuler;
    QLabel *m_lbPhone;
};
#endif // SETDESENNUMDLG_H
