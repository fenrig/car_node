#include "signal_handler.h"
#include <QSocketNotifier>

signal_handler::signal_handler(QObject *parent = 0, const char *name = 0) :
    QObject(parent)
{
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sighupFd))
       qFatal("Couldn't create HUP socketpair");

    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
}

void signal_handler::termSignalHandler(int){
    char a = 1;
    ::write(sigtermFd[0], &a, sizeof(a));
}

void signal_handler::handleSigTerm(){
    snTerm->setEnabled(false);
    char tmp;
    ::read(sigtermFd[1], &tmp, sizeof(tmp));

    // do QT stuff

    snTerm->setEnabled(true);
}
