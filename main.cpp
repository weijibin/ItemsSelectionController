#include "widget.h"
#include <QApplication>
#include "myapplication.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
    MyApplication a(argc, argv);
    Widget w;
    w.show();

    return a.exec();
}
