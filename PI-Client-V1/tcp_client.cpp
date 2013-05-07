#include "tcp_client.h"
#include <QTcpSocket>
#include <QUdpSocket>
#include <QHostInfo>
#include <cstdio>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

#define udpgetport 6666

tcp_client::tcp_client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
    connect(socket,SIGNAL(disconnected()), this, SLOT(on_disconnected()));
}

void tcp_client::read_data(){
    //char read_buffer[56] = "";
    QByteArray read_buffer = (socket->readAll());
    printf("[INFO] Received: %s\n",read_buffer.data());
    emit readyRead((QString(read_buffer).trimmed()));
}

void tcp_client::write_data(QByteArray *buffer){
    buffer->rightJustified(56,' ',true);
    socket->write(*buffer);
    socket->flush();
}

void tcp_client::on_connected(){
    //----------
    printf("[INFO] Authenticating to CAR-SERVER\n");
    // char auth_buffer[13] = "car_v1      ";
    char auth_buffer[13] = "car_v2      ";
    socket->write(auth_buffer);
    socket->flush();
    //----------
    printf("[INFO] Authentication passed & Connection established\n");
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

void tcp_client::on_disconnected(){
    printf("[ERROR] Disconnected with TCP Server\n");
    connectToServer();
}

void tcp_client::connectToServer(){
    printf("[INFO] Initiating connection procedure\n");
    //socket->connectToHost("fenrig-N73SV", 666);
    //socket->connectToHost("192.168.10.124", 666);
    socket->connectToHost(UDP_Server_Discovery(), 666);
}

tcp_client::~tcp_client(){
    socket->close();
}

QString tcp_client::UDP_Server_Discovery(){
    printf("[INFO] Initiating UDP Discover\n");
    // Om ip adressen te ontdekken
    QNetworkAddressEntry inter;
    // sockets aanmaken en verbinden met enerzijds broadcast en anderzijds een luister poort
    QUdpSocket udpSocketSend;
    QUdpSocket udpSocketGet;
    udpSocketSend.connectToHost(inter.broadcast(), 667);
    // udpSocketGet->bind(inter->ip(),667);
    // udpSocketGet->bind(QHostAddress::Any,667)
    if(udpSocketGet.bind(udpgetport,QUdpSocket::ShareAddress))
        printf("[INFO] Could properly bind udpSocketget to %d\n",udpgetport);
    else printf("[INFO] Couldn't properly bind udpSocketget to %d\n",udpgetport);
    // Pakket verzenden
    QByteArray send_datagram = "DISCOVER-CAR-SERVER";
    // Optimalisatie voor in de loop
    QByteArray receive_datagram;
    QHostAddress server;
    quint16 serverPort;
    forever{
        udpSocketSend.writeDatagram(send_datagram, QHostAddress::Broadcast, 667);
        if(udpSocketGet.waitForReadyRead(3000)){
            receive_datagram.resize(udpSocketGet.pendingDatagramSize());
            udpSocketGet.readDatagram(receive_datagram.data(),receive_datagram.size(),&server,&serverPort);
            printf("[INFO] udpSocketGet DATA: \'%s\'\n",receive_datagram.data());
            if(QString::fromUtf8(receive_datagram.data()) == "DISCOVERED-CAR-SERVER")
                printf("[INFO] Found CAR-SERVER on %s\n",server.toString().toUtf8().constData());
                return server.toString();
        }else printf("[INFO] UDP Discover TimeOut\n");
    }
    return "";
}
