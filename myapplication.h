#ifndef MYAPPLICATION_H
#define MYAPPLICATION_H

#include <QApplication>

class MyApplication : public QApplication
{
public:
//    QApplication(int &argc, char **argv, int = ApplicationFlags);
    MyApplication(int &argc, char **argv);
    virtual bool notify(QObject *receiver, QEvent *event);
protected:
    virtual bool event(QEvent *);
};

#endif // MYAPPLICATION_H
