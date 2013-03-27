#include "linefollowingthread.h"
#include "spi.h"
#include "FindLine.h"
#include "QDebug"
linefollowingthread::linefollowingthread(SPI* x, QObject *parent) :
    QThread(parent)
{
    s = x;
}

void linefollowingthread::run(){
    FindLine fl;
    offsets os;
    os = fl.FindOffset();
    int size = 2;
    __u8 msg[size];
    msg[0]= os.right;
    msg[1]=os.left;
    qDebug() << msg[0] << ":" << msg[1];
    s->send(msg,2);
    /*
    forever{
        s->send(msg);
        QTest::qSleep(50);
        qDebug() << "send";
    }
    */
}
