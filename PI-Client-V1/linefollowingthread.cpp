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
    instruction = road.split(";", QString::SkipEmptyParts);
    qDebug() << instruction;
    fl.rood=true;
    fl.blauw=false;
    fl.wit=false;
    fl.links=false;
    fl.rechts=false;
    for(int teller=0;teller<instruction.length()-1;teller++)
    {
        fl.status=true;
        qDebug("Teller:");
        qDebug() << teller;
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
                qDebug() << i << ": " << msg[0] << ":" << msg[1];
                i++;
                s->send(msg,2);
            }
            qDebug() << "-------------";

            data->clear();
            delete(data);

            mutex->lock();
            if(blstop == true) return;
            mutex->unlock();
        }
    }
    qDebug("Bestemming bereikt!");
}

void linefollowingthread::setRoad(QString roadmap)
{
    road = roadmap;
    qDebug("TEST2");
}

linefollowingthread::~linefollowingthread(){
    delete(mutex);
    s = NULL;
}
