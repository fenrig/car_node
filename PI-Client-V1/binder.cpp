#include "binder.h"
#include <tcp_client.h>
#include <spi.h>
#include <cstdio>

binder::binder(tcp_client *tcp_x, SPI *spi_x, QObject *parent) :
    QObject(parent), tcp(tcp_x), spi(spi_x)
{
    connect(tcp, SIGNAL(readyRead(QString)), this, SLOT(parseTCPmsg(QString)));
}

void binder::parseTCPmsg(QString msg){
    printf("[CONNECT] Data: %s\n",msg.toUtf8().constData());
    if(msg == QString("forward")){
        spi->send((unsigned char*)"f");
        return;
     }
    if(msg == QString("backward")){
        spi->send((unsigned char*)"b");
        return;
     }
    spi->send((unsigned char*)"x");
}
