#include "binder.h"
#include <tcp_client.h>
#include <spi.h>
#include <cstdio>
#include <QMap>

binder::binder(tcp_client *tcp_x, SPI *spi_x, QObject *parent) :
    QObject(parent), tcp(tcp_x), spi(spi_x)
{
    instructions["forward"] = (unsigned char)1;
    instructions["backward"] = (unsigned char)2;
    instructions["fb-stop"] = (unsigned char)3;
    instructions["left"] = (unsigned char)4;
    instructions["right"] = (unsigned char)5;
    instructions["lr-stop"] = (unsigned char)6;
    // -------------
    connect(tcp, SIGNAL(readyRead(QString)), this, SLOT(parseTCPmsg(QString)));
}

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
        spi->send(sendvalue);
    }
    /*
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
     */
    //spi->send((unsigned char*)"x");
    //printf("[ERROR] Could not parse TCP message: '%s'\n",msg.toUtf8().constData());
}
