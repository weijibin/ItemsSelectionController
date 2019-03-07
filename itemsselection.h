#ifndef ITEMSSELECTION_H
#define ITEMSSELECTION_H

#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QLineF>

class CHandleItem : public QGraphicsItem
{
public:
    CHandleItem(float fWidth, float fHeight);

    QRectF boundingRect() const
    {
        return m_rectBox;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
    QRectF m_rectBox;
};

class CLineItem : public QGraphicsLineItem
{
public:
    CLineItem();
    CLineItem(QLineF line);
};

class ItemsSelection
{
public:
    ItemsSelection(QGraphicsScene * scene,QGraphicsView *view);
    ~ItemsSelection();

    QList<QGraphicsItem*> getSelectedItems(){ return m_selectedItems;}
    void setSelectedItems(QList<QGraphicsItem*> items);

    void selectionShow();
    void selectionHide();

    bool isPickHandler(QPointF point);
    void down(QPointF pos);
    void move(QPointF pos);
    void up(QPointF pos);

private:
    void updateSelection();

private:
    QList<CHandleItem* >  m_vecHandleItems;
    QList<CLineItem* > m_vecLineItems;
    QList<QGraphicsItem* > m_selectedItems;

    QGraphicsScene* m_scene;
    QGraphicsView* m_view;

    QRectF m_totalRect;
    int m_handler;
    QPointF     m_ptDown;
    QList<QMatrix> m_itemsMatrixDown;

    QPointF     m_ptRef;
    float           m_fPreWidth;
    float           m_fPreHeight;
};

#endif // ITEMSSELECTION_H
