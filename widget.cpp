#include "widget.h"
#include "ui_widget.h"
#include "myitem.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QDebug>
#include <QKeyEvent>
#include <QPushButton>
#include <QGraphicsRectItem>

#include "itemsselection.h"
#include "graphicsview.h"
#include "hhtoolhelper.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    m_scene = new QGraphicsScene;
    m_view = new GraphicsView;
    m_view->setScene(m_scene);
    this->layout()->addWidget(m_view);

    this->layout()->addWidget(new QPushButton("hello"));

    m_scene->setSceneRect(-400,-400,800,800);



    m_item2 = new QGraphicsRectItem(0,0,150,100);
    m_scene->addItem(m_item2);


    ItemsSelection *selection = new ItemsSelection(m_scene,m_view);
    QList<QGraphicsItem*> items;
    QGraphicsEllipseItem * item = new QGraphicsEllipseItem(QRectF(200,120,60,100));
    m_scene->addItem(item);
    items.append(m_item2);
    items.append(item);
    selection->setSelectedItems(items);
    selection->selectionShow();

    m_view->setSelection(selection);

//    m_view->setDragMode(QGraphicsView::RubberBandDrag);
}

Widget::~Widget()
{
    delete ui;
}

