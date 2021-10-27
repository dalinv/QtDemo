#include "customgraphicsscence.h"
#include "node.h"
#include "linkline.h"

#include <QListWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsItem>
#include <QGraphicsView>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QPixmap>

CustomGraphicsScence::CustomGraphicsScence(QWidget *parent)
    : QGraphicsScene(parent)
    , m_bMouseMode(true)
    , m_bDrawLine(false)
    , m_pLastLineItem(nullptr)
    , m_bModify(false)
{
    setSceneRect(0,0,1440,1024);//设置场景大小
    connect(parent,SIGNAL(signalModeChange(bool)),this,SLOT(slotModeChange(bool)));
}

void CustomGraphicsScence::initSceneInfo(const QJsonObject &rootJson)
{
    serialize(rootJson);
    if(m_vecLine.size()==0 && m_vecNode.size()==0)
        return;
    for(auto nodeItor:m_vecNode)
    {
        QString strNodeName=nodeItor.m_strName;

        QString strImg=nodeItor.m_strImgPath;
        QImage nodeImg=stringToImage(strImg);
        QPixmap pixmap=QPixmap::fromImage(nodeImg);
        QIcon nodeIcon(pixmap);

        QPointF posNode(nodeItor.m_dPosX,nodeItor.m_dPosY);

        createNode(strNodeName,nodeIcon, posNode);
    }
    for(auto lineItor:m_vecLine)
    {
        QPointF posLineStart(lineItor.m_dPosXSatrt,lineItor.m_dPosYStart);
        QPointF posLineEnd(lineItor.m_dPosXEnd,lineItor.m_dPosYEnd);
        createLine(posLineStart,posLineEnd);
    }
    saveItems();
    updateView();
}

void CustomGraphicsScence::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
    if(m_bMouseMode)
    {
        QObject *pObj = event->source();
        QString strClassName = pObj->metaObject()->className();
        if(strClassName == "QListWidget")
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->ignore();
        }
    }
}

void CustomGraphicsScence::dropEvent(QGraphicsSceneDragDropEvent *event)
{
    if(m_bMouseMode)
    {
        QObject *obj=event->source();
        QListWidget *listNode = dynamic_cast<QListWidget *>(obj);
        QListWidgetItem *itemNode = listNode->currentItem();
        QPointF scenePos=event->scenePos();
        QIcon icon=itemNode->icon();
        QString nodeName = itemNode->text();

        createNode(nodeName,icon, scenePos);
        saveItems();
        event->acceptProposedAction();//表明用户可以在场景上拖放对象，默认是不接受的
        updateView();
    }
}

void CustomGraphicsScence::createNode(const QString &nodeName,const QIcon &icon, const QPointF& point)
{
    Node *scenceItem=new Node(nodeName,icon,point);
    scenceItem->setPos(point);
    scenceItem->setZValue(1);
    addItem(scenceItem);
}

void CustomGraphicsScence::createLine(const QPointF &posStart,const QPointF &posEnd)
{
    QTransform transForm;
    Node *pNodeStart=dynamic_cast<Node *>(itemAt(posStart,transForm));
    Node *pNodeEnd=dynamic_cast<Node *>(itemAt(posEnd,transForm));
    if(pNodeStart==nullptr&&pNodeEnd==nullptr)
    {
        return;
    }
    linkLine *pLineItem=new linkLine(pNodeStart,pNodeEnd);
    addItem(pLineItem);
}

void CustomGraphicsScence::dragMoveEvent(QGraphicsSceneDragDropEvent *event)
{
    if(m_bMouseMode)
    {
        event->accept();
    }
}

void CustomGraphicsScence::slotModeChange(bool mode)
{
    m_bMouseMode=mode;
    emit sigModeChange(mode);
}

void CustomGraphicsScence::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(!m_bMouseMode)//插入边模式
        {
            //判断当前点击区域是否有节点，没有就不画线
            QTransform transForm;
            m_pNodeStart=dynamic_cast<Node *>(itemAt(event->scenePos(),transForm));
            if(m_pNodeStart==nullptr)
            {
                m_bDrawLine = false;
                return;
            }

            m_bDrawLine = true;

            QPointF posLineStart=m_pNodeStart->getDrawLinePos();
            QLine line(posLineStart.rx(),posLineStart.ry(),posLineStart.rx(),posLineStart.ry());

            linkLine *pLineItem=new linkLine(line);
            addItem(pLineItem);
            m_pLastLineItem=pLineItem;
        }
        else {
            event->ignore();
            QGraphicsScene::mousePressEvent(event);
        }
    }
}

void CustomGraphicsScence::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bMouseMode && m_bDrawLine)
    {
        removeItem(m_pLastLineItem);
        QPointF posMove=event->scenePos();

        QPointF posLineStart=m_pNodeStart->getDrawLinePos();
        QLine line(posLineStart.rx(),posLineStart.ry(),posMove.rx(),posMove.ry());
        linkLine *pLineItem=new linkLine(line);

        addItem(pLineItem);
        m_pLastLineItem=pLineItem;
    }
    else
    {
        event->ignore();
        QGraphicsScene::mouseMoveEvent(event);
    }
    
    updateView();
}

void CustomGraphicsScence::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(!m_bMouseMode && m_bDrawLine)
    {
        QTransform transForm;
        m_pNodeEnd=dynamic_cast<Node *>(itemAt(event->scenePos(),transForm));
        if(m_pNodeEnd==nullptr)
        {
            removeItem(m_pLastLineItem);
            return;
        }

        removeItem(m_pLastLineItem);
        linkLine *pLineItem=new linkLine(m_pNodeStart,m_pNodeEnd);
        addItem(pLineItem);
        m_bDrawLine = false;
    }
    else
    {
        event->ignore();
        QGraphicsScene::mouseReleaseEvent(event);
    }

    saveItems();
    updateView();
}

void CustomGraphicsScence::updateView()
{
    QList<QGraphicsView *> lstViews = this->views();
    for(auto pView:lstViews)
    {
        pView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    }
}

//将场景中的所有图元保存到容器中
void CustomGraphicsScence::saveItems()
{
    QList<QGraphicsItem *> lsItem=this->items();
    if(lsItem.isEmpty())
    {
        return;
    }

    m_vecNode.clear();
    m_vecLine.clear();

    for(auto pItem: lsItem)
    {
        if(pItem->type() == Node::Type)
        {
            Node *pNode = dynamic_cast<Node *>(pItem);
            if(!pNode)
            {
                continue;
            }

            SNodeInfo nodeInfo;
            nodeInfo.m_strName=pNode->getName();
            nodeInfo.m_strImgPath=pNode->getStrImg();
            nodeInfo.m_dPosX=pNode->getPos().rx();
            nodeInfo.m_dPosY=pNode->getPos().ry();

            m_vecNode.push_back(nodeInfo);
        }

        if(pItem->type() == linkLine::Type)
        {
            linkLine *pNode = dynamic_cast<linkLine *>(pItem);
            if(!pNode)
            {
                continue;
            }

            SLineInfo lineInfo;
            lineInfo.m_dPosXSatrt=pNode->getStartPos().rx();
            lineInfo.m_dPosYStart=pNode->getStartPos().ry();
            lineInfo.m_dPosXEnd=pNode->getEndPos().rx();
            lineInfo.m_dPosYEnd=pNode->getEndPos().ry();
            m_vecLine.push_back(lineInfo);
        }
    }
}

//遍历容器，将所有图元的信息保存到json对象中
void CustomGraphicsScence::saveToJson(QJsonObject &obj)
{
    QJsonArray jsonArrNode;
    for(auto itor : m_vecNode)
    {
        QJsonObject jsonObj;
        jsonObj["nodeName"]=itor.m_strName;
        jsonObj["nodeImg"]=itor.m_strImgPath;

        QJsonObject pos;
        pos["nodePosX"] = itor.m_dPosX;
        pos["nodePosY"] = itor.m_dPosY;
        jsonObj["nodePos"] = pos;

        jsonArrNode.append(jsonObj);
    }
    obj.insert("nodeInfo", jsonArrNode);

    QJsonArray jsonArrLine;
    for(auto itor : m_vecLine)
    {
        QJsonObject jsonObj;

        QJsonObject posStart;
        posStart["linePosXStart"] = itor.m_dPosXSatrt;
        posStart["linePosYStart"] = itor.m_dPosYStart;
        jsonObj["linePosStart"] = posStart;

        QJsonObject posEnd;
        posEnd["linePosXEnd"] = itor.m_dPosXEnd;
        posEnd["linePosYEnd"] = itor.m_dPosYEnd;
        jsonObj["linePosEnd"] = posEnd;

        jsonArrLine.append(jsonObj);
    }
    obj.insert("lineInfo", jsonArrLine);

    //测试
    //QString strJson=QJsonDocument(obj).toJson();
}

void CustomGraphicsScence::serialize(const QJsonObject &obj)
{
    QJsonArray jsonArrNode = obj.value("nodeInfo").toArray();
    m_vecNode.reserve(jsonArrNode.count());

    for(auto itor : jsonArrNode)
    {
        SNodeInfo nodeInfo;

        QJsonObject jsonNode = itor.toObject();
        nodeInfo.m_strName=jsonNode.value("nodeName").toString();
        nodeInfo.m_strImgPath=jsonNode.value("nodeImg").toString();

        auto posObj=jsonNode.value("nodePos").toObject();
        nodeInfo.m_dPosX=posObj.value("nodePosX").toDouble();
        nodeInfo.m_dPosY=posObj.value("nodePosY").toDouble();
        m_vecNode.push_back(nodeInfo);
    }


    QJsonArray jsonArrLine = obj.value("lineInfo").toArray();
    m_vecNode.reserve(jsonArrLine.count());

    for(auto itor : jsonArrLine)
    {
        SLineInfo lineInfo;
        QJsonObject jsonLine = itor.toObject();

        auto posObjStart=jsonLine.value("linePosStart").toObject();
        lineInfo.m_dPosXSatrt=posObjStart.value("linePosXStart").toDouble();
        lineInfo.m_dPosYStart=posObjStart.value("linePosYStart").toDouble();

        auto posObjEnd=jsonLine.value("linePosEnd").toObject();
        lineInfo.m_dPosXEnd=posObjEnd.value("linePosXEnd").toDouble();
        lineInfo.m_dPosYEnd=posObjEnd.value("linePosYEnd").toDouble();

        m_vecLine.push_back(lineInfo);
    }
}

QImage CustomGraphicsScence::stringToImage(const QString &strImgData)
{
    QByteArray byteArray = QByteArray::fromBase64(strImgData.toLocal8Bit());
    QImage image;
    QDataStream dataStream(&byteArray, QIODevice::ReadOnly);
    dataStream >> image;
    return image;
}
