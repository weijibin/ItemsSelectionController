#include "itemsselection.h"
#include <QPainter>
#include <QGraphicsScene>
#include <QDebug>
#include "hhtoolhelper.h"

CHandleItem::CHandleItem(float fWidth, float fHeight)
{
    QPointF ptnTopLeft(-fWidth*0.5,-fHeight*0.5);
    m_rectBox.setTopLeft(ptnTopLeft);
    m_rectBox.setWidth(fWidth);
    m_rectBox.setHeight(fHeight);
    QVariant variant = QString("handleItem");
    this->setData(0, variant);

    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

void CHandleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath path;
    path.addEllipse(m_rectBox);

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPen pen;
    pen.setWidth(1);
    pen.setStyle(Qt::SolidLine);
    pen.setColor(QColor(0x666666));
    painter->setPen(pen);

    painter->fillPath(path, QBrush(QColor(0x00b266)));
    painter->drawPath(path);

    painter->restore();
}


CLineItem::CLineItem() : QGraphicsLineItem()
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

CLineItem::CLineItem(QLineF line) : QGraphicsLineItem(line)
{
    setFlag(QGraphicsItem::ItemIgnoresTransformations);
}

ItemsSelection::ItemsSelection(QGraphicsScene *scene,QGraphicsView *view)
{
    m_scene = scene;
    m_view = view;

    for (int i = 0; i < 9; ++i)
    {
        CHandleItem* pItem = new CHandleItem(16, 16);
        m_vecHandleItems.push_back(pItem);
    }
    for (int i = 0; i < 9; ++i)
    {
        CLineItem* pItem = new CLineItem();
        m_vecLineItems.push_back(pItem);
    }
}

ItemsSelection::~ItemsSelection()
{

}

bool ItemsSelection::isPickHandler(QPointF point)
{
    bool ret = false;
    m_handler = -1;
    for(int i =0; i<m_vecHandleItems.size(); i++)
    {
        CHandleItem*item = m_vecHandleItems.at(i);
        QRectF rect_Scene = item->mapRectToScene(item->boundingRect());
        if(rect_Scene.contains(point.x(),point.y()))
        {
            m_handler = i;
            ret = true;
            break;
        }
    }
    if(!ret)
    {
        for(int i = 0; i < m_selectedItems.size(); i++)
        {
            QGraphicsItem *item = m_selectedItems.at(i);
            QRectF rect_Scene = item->mapRectToScene(item->boundingRect());
            if(rect_Scene.contains(point.x(),point.y()))
            {
                m_handler = 10;
                ret = true;
                break;
            }
        }
    }
    return ret;
}


void ItemsSelection::down(QPointF pos)
{
    qDebug()<<"current Handler::"<< m_handler;
    m_ptDown = pos;
    m_fPreHeight = m_totalRect.height();
    m_fPreWidth = m_totalRect.width();

    m_itemsMatrixDown.clear();
    foreach (QGraphicsItem *item, m_selectedItems)
    {
        m_itemsMatrixDown.push_back(item->matrix());
    }

    if(m_handler == 8)
    {
        m_ptRef = m_totalRect.center();
    }
    else if(m_handler == 0 || m_handler == 2 || m_handler == 4 || m_handler == 6)
    {
        if(m_handler == 4)
        {
            m_ptRef = m_totalRect.topLeft();
        }
    }
    else if(m_handler == 1 || m_handler == 3 || m_handler == 5 || m_handler == 7)
    {

    }
    else if(m_handler == 10)
    {
        m_ptRef = pos;
    }
}

void ItemsSelection::move(QPointF pos)
{
    if(m_handler == 8)
    {
        float angle1 = HHToolHelper::angleOfLine( m_ptRef.x(), m_ptRef.y(), m_ptDown.x(), m_ptDown.y(), true );
        float angle2 = HHToolHelper::angleOfLine( m_ptRef.x(), m_ptRef.y(), pos.x(), pos.y(), true );
        float StepAngle = angle1 - angle2;

        QPointF  ref;
        for(int i = 0; i < m_selectedItems.count(); i++)
        {
            QGraphicsItem * item = m_selectedItems.at(i);
            ref = m_ptRef;
            QMatrix mat = HHToolHelper::rotate2( StepAngle * 180 / PI, ref );
            QMatrix t = m_itemsMatrixDown.at(i) * mat;
            item->setMatrix(t);
        }
    }
    else if(m_handler == 0 || m_handler == 2 || m_handler == 4 || m_handler == 6)
    {
        if(m_handler == 4)
        {
            float xNewWidth = pos.x() - m_ptRef.x();
            float yNewHeight = pos.y() - m_ptRef.y();
            if( 5 <= xNewWidth && 5 <= yNewHeight )
            {
                float xRate = xNewWidth / m_fPreWidth;
                float yRate = yNewHeight / m_fPreHeight;
                float rate = fmin( xRate, yRate );
                QPointF  ref;
                for(int i = 0; i < m_selectedItems.count(); i++)
                {
                    QGraphicsItem * item = m_selectedItems.at(i);
                    ref =  m_ptRef;
                    QMatrix mat  = HHToolHelper::scale2( rate, rate, ref.x(), ref.y());
                    QMatrix t = m_itemsMatrixDown.at(i) * mat;
                    item->setMatrix(t);
                    qDebug()<<item->matrix();
                }
            }
        }
    }
    else if(m_handler == 1 || m_handler == 3 || m_handler == 5 || m_handler == 7)
    {

    }
    else if(m_handler == 10)
    {
        float xOffset = pos.x() - m_ptRef.x();
        float yOffset = pos.y() - m_ptRef.y();

        QPointF  ref;
        for(int i = 0; i < m_selectedItems.count(); i++)
        {
            QGraphicsItem * item = m_selectedItems.at(i);
            ref =  m_ptRef;
            QMatrix mat;
            mat.translate(xOffset,yOffset);
            QMatrix t = m_itemsMatrixDown.at(i) * mat;
            item->setMatrix(t);
        }
    }

    updateSelection();
}

void ItemsSelection::up(QPointF pos)
{
    if(m_handler == 8)
    {
        m_scene->update();
    }
    else if(m_handler == 0 || m_handler == 2 || m_handler == 4 || m_handler == 6)
    {
        if(m_handler == 4)
        {
            m_scene->update();
        }
    }
    else if(m_handler == 1 || m_handler == 3 || m_handler == 5 || m_handler == 7)
    {

    }
    else if(m_handler == 10)
    {
        m_scene->update();
    }

    updateSelection();
}


void ItemsSelection::selectionHide()
{
    foreach(CHandleItem*item,m_vecHandleItems)
    {
        if(m_scene->items().contains(item))
            m_scene->removeItem(item);
    }
    foreach(CLineItem*item,m_vecLineItems)
    {
        if(m_scene->items().contains(item))
            m_scene->removeItem(item);
    }
}

void ItemsSelection::selectionShow()
{
    if(m_selectedItems.count())
    {
        foreach(CLineItem*item,m_vecLineItems)
        {
            if(!m_scene->items().contains(item))
                m_scene->addItem(item);
        }
        foreach(CHandleItem*item,m_vecHandleItems)
        {
            if(!m_scene->items().contains(item))
                m_scene->addItem(item);
        }
        updateSelection();

    }
}

void ItemsSelection::setSelectedItems(QList<QGraphicsItem *> items)
{
    m_selectedItems = items;
}

void ItemsSelection::updateSelection()
{

    if(m_selectedItems.count())
    {
        QRectF rect;
        foreach (QGraphicsItem *item, m_selectedItems)
        {
//            rect |= item->mapRectToScene(item->boundingRect()) ;
            QPainterPath path = item->mapToScene(item->shape()) ;
            rect |= path.boundingRect();
        }

        m_totalRect  = rect;

        m_vecHandleItems.at(0)->setPos(rect.topLeft());
        m_vecHandleItems.at(1)->setPos(rect.center().x(),rect.topLeft().y());
        m_vecHandleItems.at(2)->setPos(rect.topRight());
        m_vecHandleItems.at(3)->setPos(rect.topRight().x(),rect.center().y());
        m_vecHandleItems.at(4)->setPos(rect.bottomRight());
        m_vecHandleItems.at(5)->setPos(rect.center().x(),rect.bottomRight().y());
        m_vecHandleItems.at(6)->setPos(rect.bottomLeft());
        m_vecHandleItems.at(7)->setPos(rect.topLeft().x(),rect.center().y());
        m_vecHandleItems.at(8)->setPos(rect.center().x(),rect.topLeft().y()-30);

        m_vecLineItems.at(0)->setLine(QLineF(m_vecHandleItems.at(0)->pos(),m_vecHandleItems.at(1)->pos()));
        m_vecLineItems.at(1)->setLine(QLineF(m_vecHandleItems.at(1)->pos(),m_vecHandleItems.at(2)->pos()));
        m_vecLineItems.at(2)->setLine(QLineF(m_vecHandleItems.at(2)->pos(),m_vecHandleItems.at(3)->pos()));
        m_vecLineItems.at(3)->setLine(QLineF(m_vecHandleItems.at(3)->pos(),m_vecHandleItems.at(4)->pos()));
        m_vecLineItems.at(4)->setLine(QLineF(m_vecHandleItems.at(4)->pos(),m_vecHandleItems.at(5)->pos()));
        m_vecLineItems.at(5)->setLine(QLineF(m_vecHandleItems.at(5)->pos(),m_vecHandleItems.at(6)->pos()));
        m_vecLineItems.at(6)->setLine(QLineF(m_vecHandleItems.at(6)->pos(),m_vecHandleItems.at(7)->pos()));
        m_vecLineItems.at(7)->setLine(QLineF(m_vecHandleItems.at(7)->pos(),m_vecHandleItems.at(0)->pos()));
        m_vecLineItems.at(8)->setLine(QLineF(m_vecHandleItems.at(8)->pos(),m_vecHandleItems.at(1)->pos()));
    }
}
