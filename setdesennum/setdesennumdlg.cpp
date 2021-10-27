#include "setdesennumdlg.h"
#include "ui_setdesennumdlg.h"
#include "customruler.h"

#include <QPainter>
#include <QDrag>
#include <QMimeData>
#include <QMouseEvent>

#define RULER_Y 50

SetDesenNumDlg::SetDesenNumDlg(const QString& strPhone,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SetDesenNumDlg)
{
    ui->setupUi(this);
    this->resize(500, 300);
    this->setAcceptDrops(true);
    setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);

    m_strPhone=strPhone;
    m_iWidth=24;
    m_leftXPos=100-6;
    m_rightXPos= m_leftXPos +m_iWidth*11;


    init();
    connect(m_leftRuler,SIGNAL(rulerMoving(int,QPoint)),this,SLOT(slotRulerMoving(int,QPoint)));
    connect(m_rightRuler,SIGNAL(rulerMoving(int,QPoint)),this,SLOT(slotRulerMoving(int,QPoint)));
}

SetDesenNumDlg::~SetDesenNumDlg()
{
    delete ui;
}
void SetDesenNumDlg::init()
{
    //默认脱敏位数4-7
    m_iStart=4;
    m_iEnd=7;
    updateDesennumText();//获取脱敏后的文本

    m_leftRuler=new CustomRuler(this);
    m_leftRuler->setRulerId(RULER_ID::leftRuler);
    m_rightRuler=new CustomRuler(this);
    m_rightRuler->setRulerId(RULER_ID::rightRuler);
    updateRulerPos(m_iStart,m_iEnd);
}

void SetDesenNumDlg::updateRulerPos(const int& start,const int& end)
{
    m_currentLeftRuler=m_leftXPos+m_iWidth*(start-1);
    m_currentRightRuler=m_leftXPos+m_iWidth*end;
    m_leftRuler->setGeometry(m_currentLeftRuler-16,RULER_Y,32,32);
    m_rightRuler->setGeometry(m_currentRightRuler-16,RULER_Y,32,32);
}

void SetDesenNumDlg::updateDesennumText()
{
    QString strNewPhone;
    for(int i=0;i<11;++i)
    {
        if(i>=m_iStart-1&&i<m_iEnd)
        {
            strNewPhone[i]='*';
        }
        else
        {
            strNewPhone[i]=m_strPhone[i];
        }
    }
    m_strNewPhone=strNewPhone;
}

void SetDesenNumDlg::paintEvent(QPaintEvent *)
{
    /***********************************
    ** 左标尺最大范围(100-6 , 100-6+24*(11-1))
    ** 右标尺最大范围(100-6+24*1 , 100-6+24*11)
    ** 且标尺的x轴坐标只能落在 100-6+24*n
    ** 若移动超出范围 则落在实际范围的边界
    ************************************/
    QFont font("Microsoft YaHei", 23);
    QPainter painter(this);
    painter.setFont(font);

    //画背景
    painter.setPen(QColor("#AFEEEE"));
    painter.setBrush(QColor("#AFEEEE"));
    QRect rect(QPoint(m_currentLeftRuler,100),QPoint(m_currentRightRuler,130));
    painter.drawRect(rect);

    //画光标
    painter.setPen(QColor("#999999"));
    painter.setBrush(Qt::yellow);
    painter.drawLine(m_currentLeftRuler,70,m_currentLeftRuler,130);
    painter.drawLine(m_currentRightRuler,70,m_currentRightRuler,130);

    //画号码
    updateDesennumText();
    int iCount=0;
    QPoint pos(100,130);
    for(int i=0;i<11;i++)
    {
        pos.rx()=100+m_iWidth*iCount;
        QString strChar=m_strNewPhone.at(i);

        painter.drawText(pos,strChar);
        iCount++;
    }
}

void SetDesenNumDlg::slotRulerMoving(int id,QPoint pos)
{
    int xRuler=pos.rx();//标尺左x轴坐标
    int xPos;
    if(id==RULER_ID::leftRuler)
    {
        int left=m_leftXPos;
        int right= m_currentRightRuler -m_iWidth;
        if(xRuler>=left && xRuler<=right)
        {
            xPos=judgePos(pos.rx());
        }
        else if(xRuler<left) //移动到边界
        {
            xPos=left;
        }
        else if (xRuler> right)
        {
            xPos=right;
        }
        m_leftRuler->move(xPos-16,RULER_Y);
        m_currentLeftRuler = xPos;
        m_iStart=(m_currentLeftRuler-m_leftXPos)/m_iWidth+1;
    }
    if(id==RULER_ID::rightRuler)
    {
        int left=m_currentLeftRuler+m_iWidth;
        int right=m_rightXPos;
        if(xRuler>=left && xRuler<=right)
        {
            xPos=judgePos(pos.rx());
        }
        else if(xRuler<left)
        {
            xPos=left;
        }
        else if (xRuler>right)
        {
            xPos=right;
        }
        m_rightRuler->move(xPos-16,RULER_Y);
        m_currentRightRuler = xPos;
        m_iEnd=(m_currentRightRuler-m_leftXPos)/m_iWidth;
    }
    update();
}

int SetDesenNumDlg::judgePos(int pos)
{
    int resPos;
    int i=0,start=0,end=0;
    while(i<12)
    {
        start=m_leftXPos+i*m_iWidth;
        end=start+m_iWidth;
        if(pos>=start&&pos<=end)
        {
            resPos=end;
            break;
        }
        i++;
    }
    return resPos;
}
