#include "signal_handler.h"
#include <QSocketNotifier>
#include <QUdpSocket>
#include "signal.h"
#include <netinet/in.h>
#include <QCoreApplication>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <QDebug>

int signal_handler::sigtermFd[2];

signal_handler::signal_handler(QObject *parent, const char *name) :
    QObject(parent)
{
    //if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
     //  qFatal("Couldn't create HUP socketpair");

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    //snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    //connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
}

void signal_handler::termSignalHandler(int){
    char a = 1;
    qDebug() << "termsignalhandler";

    write(sigtermFd[0], &a, sizeof(a));
}

void signal_handler::handleSigTerm(){
    printf("handlesigterm\n");
    snTerm->setEnabled(false);
    char tmp;

    read(sigtermFd[1], &tmp, sizeof(tmp));

    // ----- activate end
    QCoreApplication::quit();
    //
    snTerm->setEnabled(true);
}
