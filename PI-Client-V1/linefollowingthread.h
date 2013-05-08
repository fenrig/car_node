#ifndef LINEFOLLOWINGTHREAD_H
#define LINEFOLLOWINGTHREAD_H

#include <QThread>

class SPI;

class linefollowingthread : public QThread
{
    Q_OBJECT
public:
    explicit linefollowingthread(SPI* x,QObject *parent = 0);
    void run();

    //bool stop;
private:
    SPI* s;
signals:
    
public slots:
    
};

#endif // LINEFOLLOWINGTHREAD_H
