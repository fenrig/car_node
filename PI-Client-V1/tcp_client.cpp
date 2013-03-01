#include "tcp_client.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostInfo>
#include <cstdio>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

tcp_client::tcp_client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
}

void tcp_client::read_data(){
    //char read_buffer[56] = "";
    QByteArray read_buffer = (socket->read(56));
    printf("%s\n",read_buffer.data());

}

void tcp_client::write_data(QByteArray *buffer){
    buffer->rightJustified(56,' ',true);
    // socket->write(buffer);
    // socket->flush();

}

void tcp_client::on_connected(){
    //----------
    printf("Auth\n");
    char auth_buffer[13] = "car_v1      ";
    socket->write(auth_buffer);
    socket->flush();
    //----------
    printf("Connection established.\n");
    socket->setReadBufferSize(56);
    /*
    char buffer[56];
    forever{
        printf(">> ");
        gets(buffer);
        int len = strlen(buffer);
        buffer[len] = '\n';
        buffer[len+1] = '\0';
        socket->write(buffer);
        socket->flush();
    }
    */
}

void tcp_client::connectToServer(){
    printf("Connecting\n");
    //socket->connectToHost("fenrig-N73SV", 666);
    //socket->connectToHost("192.168.10.124", 666);
    socket->connectToHost(UDP_Server_Discovery(), 666);
    printf("connect?\n");
}

QString tcp_client::UDP_Server_Discovery(){
    // Om ip adressen te ontdekken
    QNetworkAddressEntry *inter = new QNetworkAddressEntry();
    // sockets aanmaken en verbinden met enerzijds broadcast en anderzijds een luister poort
    QUdpSocket *udpSocketSend = new QUdpSocket();
    QUdpSocket *udpSocketGet = new QUdpSocket();
    udpSocketSend->connectToHost(inter->broadcast(), 667);
    // udpSocketGet->bind(inter->ip(),667);
    // udpSocketGet->bind(QHostAddress::Any,667)
    // bind niet :/
    if(udpSocketGet->bind(6666,QUdpSocket::ShareAddress)) printf("TRUE\n");
    else printf("False\n");
    // Pakket verzenden
    QByteArray send_datagram = "DISCOVER-CAR-SERVER";
    // Optimalisatie voor in de loop
    QByteArray receive_datagram;
    QHostAddress server;
    quint16 serverPort;
    forever{
        udpSocketSend->writeDatagram(send_datagram, QHostAddress::Broadcast, 667);
        if(udpSocketGet->waitForReadyRead(30000)){
            receive_datagram.resize(udpSocketGet->pendingDatagramSize());
            udpSocketGet->readDatagram(receive_datagram.data(),receive_datagram.size(),&server,&serverPort);
            printf("DATA: %s\n",receive_datagram.data());
            if(QString::fromUtf8(receive_datagram.data()) == "DISCOVERED-CAR-SERVER"){
                printf("HUH\n");
                return server.toString();
            }
        }else printf("TimeOut\n");
    }
    return "";
}
