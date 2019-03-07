#include "myitem.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include "hhtoolhelper.h"

MyItem::MyItem(QGraphicsItem *parent) : QGraphicsItem(parent)
{
    m_rectBox.setRect(50,50,100,100);
    setFlags(QGraphicsItem::ItemIsSelectable);
    m_path.addRect(m_rectBox);
    m_angle = 0;
//    m_path.addEllipse(m_rectBox);
}

QRectF MyItem::boundingRect() const
{
    return m_rectBox;
}

QPainterPath MyItem::shape() const
{
    return m_path;
}

void MyItem::itemTranslate(QMatrix mat)
{

    QStringList lst = m_StringList;
    lst.clear();

//    qDebug()<<"SceneMatrix:::"<<this->sceneMatrix();

    // 方法一
//    QMatrix mat1= matrix();
//    setMatrix(mat1*mat);

//    setMatrix(mat,true);

//    // 方法二
//    // 旋转 回正
//    QPointF center = boundingRect().center();
//    QMatrix matR;
//    matR = HHToolHelper::rotate2(-m_angle,mapToScene(center));
//    QMatrix mat1= matrix();
//    this->setMatrix(mat1 * matR);

//     // 进行 平移操作
//    qDebug()<<m_angle;
//    qDebug()<<"pre::boundingRect::"<<boundingRect();
//    setItemPathMatrix(mat);
//    qDebug()<<"end::boundingRect::"<<boundingRect();

//    // 恢复 旋转
//    QPointF newCenter = boundingRect().center();
//    QMatrix matR1;
//    matR1 = HHToolHelper::rotate2(m_angle,mapToScene(newCenter));
//    QMatrix mat2= matrix();
//    this->setMatrix(mat2 * matR1);

// 方法三
    // 进行 平移操作
   qDebug()<<m_angle;

   QMatrix mat1 ;
   mat1.rotate(-m_angle);

   QPointF point(10,10);
   point = mat1.map(point);     // 向量根据 矩阵旋转一下
   QMatrix matTrans;
   matTrans.translate(point.x(),point.y());

   qDebug()<<"pre::boundingRect::"<<boundingRect();
   setItemPathMatrix(matTrans);
   qDebug()<<"end::boundingRect::"<<boundingRect();

   m_StringList.clear();

}

void MyItem::itemScale(QMatrix mat)
{

    qDebug()<<"pre::boundingRect::"<<boundingRect();
    setItemPathMatrix(mat);
    qDebug()<<"end::boundingRect::"<<boundingRect();
}

void MyItem::itemRotate(int angle, QPointF ref)
{

    float offset = angle - m_angle;
    m_angle = angle;

    qDebug()<<"rotateRect::"<<boundingRect();
    qDebug()<<"rotaeMatrix::"<<matrix();

    QPointF center = boundingRect().center();
    QPointF pref = ref;

    float dis = HHToolHelper::twoPointDistance(center,pref);

    if(dis < 1)
    {
        QMatrix matR;
        matR = HHToolHelper::rotate2(offset,mapToScene(pref));
        QMatrix mat1= matrix();

        setMatrix(mat1*matR);
    }
    else
    {
        //方法一
//        QMatrix mat;
//        mat = HHToolHelper::rotate2(offset,pref);   // 根据angle offset 计算偏移量
//        QPointF des = mat.map(center);

//        QMatrix matTranslate;
//        qreal xoffset = des.x() - center.x();
//        qreal yoffset = des.y() - center.y();
//        matTranslate.translate(xoffset,yoffset);
//        setItemPathMatrix(matTranslate);

        //方法二
        QMatrix matR;
        matR = HHToolHelper::rotate2(offset,QPointF(0,0));   // 根据angle offset 计算偏移量  0,0为scene的坐标
        QMatrix mat1= matrix();
        setMatrix(mat1 * matR);
    }
    qDebug()<<"pos:::"<<this->pos();
    qDebug()<<sceneMatrix();
    qDebug()<<matrix();
    qDebug()<<boundingRect();
}

void MyItem::setRotateMatrix(QMatrix mat)
{
    setMatrix(mat);
}

void MyItem::setItemPathMatrix(QMatrix mat)
{
    m_matrix = mat;
    m_path  = m_matrix.map(m_path);
    prepareGeometryChange();
    m_rectBox = m_matrix.mapRect(m_rectBox);
}

void MyItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);
    if(isSelected())
    {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(10);
        painter->setPen(pen);
    }
    else
    {
        QPen pen;
        pen.setColor(Qt::blue);
        pen.setWidth(5);
        painter->setPen(pen);
    }

    painter->setBrush(QBrush(QColor(100,100,100,150)));
    painter->drawPath(m_path);

    painter->drawLine(m_rectBox.center(),mapFromScene(QPointF(0,0)));

    painter->drawText(m_rectBox.topLeft()+QPointF(10,10),"Hello");
}

