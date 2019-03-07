#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class ItemsSelection;
class QGraphicsScene;
class GraphicsView;
class MyItem;
class QGraphicsRectItem;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private:
    QGraphicsScene *m_scene;
    GraphicsView * m_view;
    Ui::Widget *ui;

    MyItem *m_item1;
    QGraphicsRectItem *m_item2;
};

#endif // WIDGET_H
