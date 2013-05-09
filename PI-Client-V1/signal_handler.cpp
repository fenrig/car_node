#include "signal_handler.h"
#include <QSocketNotifier>
#include "signal.h"
#include <QCoreApplication>

#include <unistd.h>
#include <sys/socket.h>

#include <linefollowingthread.h>
#include <QTest>

int signal_handler::sigtermFd[2];

signal_handler::signal_handler(linefollowingthread *thread, QObject *parent, const char *name) :
    QObject(parent), threadptr(thread) //, ptrthreadstop(stop)
{

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, sigtermFd))
       qFatal("Couldn't create TERM socketpair");
    //snHup = new QSocketNotifier(sighupFd[1], QSocketNotifier::Read, this);
    //connect(snHup, SIGNAL(activated(int)), this, SLOT(handleSigHup()));
    snTerm = new QSocketNotifier(sigtermFd[1], QSocketNotifier::Read, this);
    connect(snTerm, SIGNAL(activated(int)), this, SLOT(handleSigTerm()));
}

void signal_handler::termSignalHandler(int){
    char a = 1;
    write(sigtermFd[0], &a, sizeof(a));
    //QTimer::singleShot(50,this,SLOT(handleSigTerm()));
}

void signal_handler::handleSigTerm(){
    snTerm->setEnabled(false);
    char tmp;

    read(sigtermFd[1], &tmp, sizeof(tmp));
    threadptr->stop();

    // ----- activate end
    //*ptrthreadstop = true;
    int countertimeout = 0;
    while(threadptr->isRunning()){
        QTest::qSleep(50);
        countertimeout++;
        if(countertimeout == 40) break;
    }
    QCoreApplication::quit();
    //
    snTerm->setEnabled(true);
}
