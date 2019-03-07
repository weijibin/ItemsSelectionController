#ifndef MYITEM_H
#define MYITEM_H

#include <QGraphicsItem>
#include <QPainterPath>

class MyItem : public QGraphicsItem
{
public:
    MyItem(QGraphicsItem *parent = Q_NULLPTR);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter,
                       const QStyleOptionGraphicsItem *option,
                       QWidget *widget);
    virtual QPainterPath shape() const;

    void itemScale(QMatrix mat);
    void itemTranslate(QMatrix mat);
    void itemRotate(int angle , QPointF ref);

    QMatrix getItemMatrix(){return m_matrix;}
private:
    void setRotateMatrix(QMatrix mat);
    void setItemPathMatrix(QMatrix mat);
private:
    QRectF m_rectBox;
    QMatrix m_matrix;
    QPainterPath m_path;
    float m_angle;
    QStringList m_StringList;
};

#endif // MYITEM_H
