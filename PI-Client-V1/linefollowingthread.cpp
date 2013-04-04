#include "linefollowingthread.h"
#include "spi.h"
#include "FindLine.h"
#include "QDebug"
#include "uartcamera.h"
#include "vector"
linefollowingthread::linefollowingthread(SPI* x, QObject *parent) :
    QThread(parent)
{
    s = x;
}

void linefollowingthread::run(){
    FindLine fl;
    offsets os;
    UartCamera cam;
    cam.changeCompression(255);
    std::vector<char> data = cam.GetPicture();
    //for(int i=0;i<10;i++)
    //{
        os = fl.FindOffset(data);
        int size = 2;
        __u8 msg[size];
        msg[0]= os.right;
        msg[1]=os.left;
        qDebug() << msg[0] << ":" << msg[1];
        s->send(msg,2);
    //}
    /*
    forever{
        s->send(msg);
        QTest::qSleep(50);
        qDebug() << "send";
    }
    */
}
