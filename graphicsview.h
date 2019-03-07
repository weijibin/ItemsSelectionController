#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
class MyItem;
class ItemsSelection;
class GraphicsView : public QGraphicsView
{
public:
    GraphicsView(QWidget *parent = Q_NULLPTR);
    void setSelection(ItemsSelection *selection);

    MyItem *m_item1;
    MyItem *m_item2;
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
private:
    ItemsSelection *m_selection;
    bool isFilterBySelection;
};

#endif // GRAPHICSVIEW_H
