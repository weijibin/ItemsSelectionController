#include "graphicsview.h"
#include "itemsselection.h"
#include <QDebug>
#include <QMouseEvent>
#include "myitem.h"
#include "hhtoolhelper.h"

GraphicsView::GraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    m_selection = NULL;
    isFilterBySelection = false;

    m_item1 = NULL;
    m_item2 = NULL;
}

void GraphicsView::setSelection(ItemsSelection *selection)
{
    m_selection = selection;
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(m_selection)
    {
        QPointF pos = mapToScene(event->pos());
        isFilterBySelection = m_selection->isPickHandler(pos);
        if(isFilterBySelection)
        {
            m_selection->down(pos);
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_selection)
    {
        QPointF pos = mapToScene(event->pos());
        if(isFilterBySelection)
        {
            m_selection->move(pos);
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(m_selection)
    {
        QPointF pos = mapToScene(event->pos());
        if(isFilterBySelection)
        {
            m_selection->up(pos);
        }
        isFilterBySelection = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    qDebug()<<"GraphicsView::KeyPressEvent";
    static int angle = 0;
    if(event->key() == Qt::Key_Up)
    {
        QMatrix mat;
//        QPointF point = m_item1->boundingRect().topLeft();
//        QPointF pref = m_item1->mapToScene(point);
        QPointF pref = m_item1->boundingRect().topLeft();
        mat = HHToolHelper::scale2(1.1,1.1,pref.x(),pref.y());
        m_item1->itemScale(mat);

//        QPointF ref = m_item1->boundingRect().center();
//        m_item1->itemRotate(angle,ref);
    }
    else if(event->key() == Qt::Key_Down)
    {
        QMatrix mat;
//        QPointF point = m_item1->boundingRect().topLeft();
//        QPointF pref = m_item1->mapToScene(point);
        QPointF pref = m_item1->boundingRect().topLeft();
        mat = HHToolHelper::scale2(0.8,0.8,pref.x(),pref.y());
        m_item1->itemScale(mat);

//        QPointF ref = m_item1->boundingRect().center();
//        m_item1->itemRotate(angle,ref);

    }
    else if(event->key() == Qt::Key_Left)
    {
        QPointF pref = m_item1->boundingRect().center();
//        QPointF pref = m_item1->mapFromScene(QPointF(0,0));
        angle += 10;

        m_item1->itemRotate(angle,pref);
    }
    else if(event->key() == Qt::Key_Right)
    {
//        QPointF pref = m_item1->boundingRect().center()+QPointF(100,100);
        QPointF pref = m_item1->mapFromScene(QPointF(0,0));
        angle -= 45;
        m_item1->itemRotate(angle,pref);
    }
    else if(event->key() == Qt::Key_M)
    {
        QMatrix mat1;
        mat1 = HHToolHelper::translation(10,10);
        m_item1->itemTranslate(mat1);

//        static int pos = 10;
//        m_item1->setPos(pos,pos);
//        pos++;
    }

    scene()->update();
}
