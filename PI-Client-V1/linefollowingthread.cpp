#include "linefollowingthread.h"
#include "spi.h"
#include "FindLine.h"

linefollowingthread::linefollowingthread(SPI* x, QObject *parent) :
    QThread(parent)
{
    s = x;
}

void linefollowingthread::run(){
    FindLine fl;
    //s->send(fl.FindOffset(fl.WhiteFilter()));
}
