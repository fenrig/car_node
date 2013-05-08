#include "linefollowingthread.h"
#include "spi.h"
#include "FindLine.h"
#include "QDebug"
#include "uartcamera.h"
#include "vector"

#include "QFile"
linefollowingthread::linefollowingthread(SPI* x, QObject *parent) :
    QThread(parent), stop(false), s(x)
{

}

void linefollowingthread::run(){
    FindLine fl;
    offsets os;
    UartCamera cam;
    cam.changeCompression(255);
    for(int i=0;i<11;i++)
    {
        std::vector<char> data = cam.GetPicture();
        /*Debug
         *std::vector<char> data;
        QFile pic("/home/dries/Documenten/Project2/PI code/picsred/origineel" + QString::number(i) + ".jpg");
        if (!pic.open(QIODevice::ReadOnly)){
            return;
        }
        char incomingbyte;
        while (!pic.atEnd()) {

            pic.read(&incomingbyte,1);
            data.push_back(incomingbyte);
        }
        *Einde
        */
        //
        os = fl.FindOffset(data,i);
        int size = 2;
        __u8 msg[size];
        msg[0]= os.left;
        msg[1]= os.right;
        qDebug() << i << ": " << msg[0] << ":" << msg[1];
        //pic.close();

        s->send(msg,2);

        if(stop) return;
    }
    /*
    forever{
        s->send(msg);
        QTest::qSleep(50);
        qDebug() << "send";
    }
    */
}
