#include "myapplication.h"
#include <QDebug>
#include <QEvent>

MyApplication::MyApplication(int &argc, char **argv) : QApplication(argc,argv)
{

}

bool MyApplication::event(QEvent *event)
{
//    qDebug()<<"MyApplication event Type:::"<<event->type();
    return QApplication::event(event);
}

bool MyApplication::notify(QObject *receiver, QEvent *event)
{
//    qDebug()<<"MyApplication event Type notify:::"<<event->type();
    return QApplication::notify(receiver,event);
}
