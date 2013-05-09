#ifndef LINEFOLLOWINGTHREAD_H
#define LINEFOLLOWINGTHREAD_H

#include <QThread>

class QMutex;

class SPI;

class linefollowingthread : public QThread
{
    Q_OBJECT
public:
    explicit linefollowingthread(SPI* x,QObject *parent = 0);
    ~linefollowingthread();
    void run();
    void stop();
private:
    SPI* s;
    bool blstop;
    QMutex *mutex;
signals:
    
public slots:
    
};

#endif // LINEFOLLOWINGTHREAD_H
