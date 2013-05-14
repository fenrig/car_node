#include "linefollowingthread.h"
#include "spi.h"
#include "FindLine.h"
#include "QDebug"
#include "uartcamera.h"
#include "vector"
#include "QMutex"

#include "QFile"

linefollowingthread::linefollowingthread(SPI* x, QObject *parent) :
    QThread(parent), s(x)
{
    mutex = new QMutex();
    blstop = false;
}

void linefollowingthread::stop(void){
    mutex->lock();
    blstop = true;
    mutex->unlock();
}

void linefollowingthread::run(){
    FindLine fl;
    offsets os;
    UartCamera cam;
    cam.changeCompression(255);
    int i = 0;
    //for(int i=0;i<11;i++)
    std::vector<char> *data;
    forever
    {
        data = cam.GetPicture();
        /*Debug
        //std::vector<char> data;
        QFile pic("/home/dries/Documenten/Project2/PI code/picsred/origineel" + QString::number(i) + ".jpg");
        if (!pic.open(QIODevice::ReadOnly)){
            return;
        }
        char incomingbyte;
        while (!pic.atEnd()) {

            pic.read(&incomingbyte,1);
            data.push_back(incomingbyte);
        }
        Einde
        */
        //
        /*
        QFile pic("/root/pics/matthias" + QString::number(i) + ".jpg");
        if(!pic.open(QIODevice::WriteOnly)){
            qDebug() << "ohnoes pic no open";
            return;
        }
        for(std::vector<char>::iterator x = data.begin(); x != data.end(); ++x){
            pic.write(&(*x));
        }
        pic.close();
        */
        if(! data->empty() ){
            os = fl.FindOffset(*data,i);
            int size = 2;
            __u8 msg[size];
            msg[0]= os.left;
            msg[1]= os.right;
           // msg[0] = 125;
           // msg[1] = 125;
            qDebug() << i << ": " << msg[0] << ":" << msg[1];
            i++;
            //pic.close();

            //s->send(msg,2);
            //data.clear();

        }
        qDebug() << "-------------";

        mutex->lock();
        if(blstop == true) return;
        mutex->unlock();
    }
    /*
    forever{
        s->send(msg);
        QTest::qSleep(50);
        qDebug() << "send";
    }
    */
}

linefollowingthread::~linefollowingthread(){
    free(mutex);
}
