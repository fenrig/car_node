#include "tcp_client.h"
#include <QTcpSocket>
#include <QHostInfo>
#include <cstdio>

tcp_client::tcp_client(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(on_connected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(read_data()));
}

void tcp_client::read_data(){
    //char read_buffer[56] = "";
    char *read_buffer = (socket->read(56)).data();
    printf("%s\n",read_buffer);
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
    char buffer[56];
    /*
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
    socket->connectToHost("fenrig-N73SV", 666);
}
