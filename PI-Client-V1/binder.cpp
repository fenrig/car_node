#include "binder.h"
#include <tcp_client.h>
//#include <spi.h>
//#include <cstdio>
#include <QMap>
#include <linefollowingthread.h>
#include <QTest>
#include <QDebug>

binder::binder(tcp_client *tcp_x/*, SPI *spi_x*/, linefollowingthread *lft, QObject *parent) :
    QObject(parent), tcp(tcp_x), lft(lft)//, spi(spi_x)
{
    connect(tcp, SIGNAL(readyRead(QString)), this, SLOT(parseTCPmsg(QString)));
}

void binder::parseTCPmsg(QString msg){
    QByteArray* val = NULL;
    if(read==true)
    {
        qDebug("read==true");
        if(lft->isRunning())
        {
            qDebug("isRunning: if");
            lft->stop();
        }
        while (lft->isRunning())
        {
            qDebug("isRunning: while");
            QTest::qSleep(50);
        }

        lft->setRoad(msg);
        lft->start();
        read=false;
        qDebug("stop");
    }
    if(msg == "instructionlist_changed"){
        val = new QByteArray("get_instructions");
        read = true;
    }
    if(val != NULL) tcp->write_data(val);
    qDebug("Einde");
}


/*
// CAR-V1
void binder::parseTCPmsg(QString msg){
    printf("[CONNECT] Data: '%s'\n",msg.toUtf8().constData());
    //QByteArray iArray = msg.toAscii().toHex();
    //printf("HEX: '%s'\n",iArray.data());
    unsigned char value = instructions[msg];
    if(value == (unsigned char)(0))
        printf("[ERROR] Could not parse TCP message: '%s'\n",msg.toUtf8().constData());
    else{
        unsigned char sendvalue[2];
        sendvalue[0] = value;
        sendvalue[1] = (unsigned char)255;
        spi->send(sendvalue,2);
    }

    if(msg == QString("forward")){
        spi->send((unsigned char*)1);
        return;
     }
    if(msg == QString("backward")){
        spi->send((unsigned char*)2);
        return;
     }
    if(msg == QString("fb-stop")){
        spi->send((unsigned char*)3);
        return;
     }
    if(msg == QString("left")){
        spi->send((unsigned char*)4);
        return;
     }
     *//*
    //spi->send((unsigned char*)"x");
    //printf("[ERROR] Could not parse TCP message: '%s'\n",msg.toUtf8().constData());
}
*/
