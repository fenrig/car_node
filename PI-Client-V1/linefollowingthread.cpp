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
    qDebug("TEST");
    FindLine fl;
    offsets os;
    UartCamera cam;
    cam.changeCompression(255);
    int i = 0;
    //for(int i=0;i<11;i++)
    std::vector<char> *data;
    instruction = road.split(";", QString::SkipEmptyParts);
    fl.rood=false;
    fl.blauw=false;
    fl.wit=false;
    fl.links=false;
    fl.rechts=false;
    for(int teller=0;teller<instruction.length()-1;teller++)
    {

        qDebug("debug");
        fl.status=true;
        while(fl.status==true)
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
            int size = 2;
            __u8 msg[size];
            if(! data->empty() ){
                os = fl.FindOffset(data,i,instruction.at(teller),instruction.at(teller+1));
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

            data->clear();
            delete(data);

            mutex->lock();
            if(blstop == true) return;
            mutex->unlock();
        }
    }
    /*
    forever{
        s->send(msg);
        QTest::qSleep(50);
        qDebug() << "send";
    }
    */
}

void linefollowingthread::setRoad(QString roadmap)
{
    road = roadmap;
}

linefollowingthread::~linefollowingthread(){
    delete(mutex);
    s = NULL;
}
