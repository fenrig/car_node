#include "binder.h"
#include <tcp_client.h>
#include <spi.h>

binder::binder(tcp_client *tcp_x, SPI *spi_x) :
    tcp(tcp_x), spi(spi_x)
{
    connect(tcp, SIGNAL(readyRead(QString)), this, SLOT(parseTCPmsg(QString)));
}

void binder::parseTCPmsg(QString msg){
    if(msg == "forward"){
        spi->send((unsigned char*)"f");
        return;
     }
    if(msg == "backward"){
        spi->send((unsigned char*)"b");
        return;
     }
}
