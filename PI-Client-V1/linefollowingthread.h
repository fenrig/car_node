#ifndef LINEFOLLOWINGTHREAD_H
#define LINEFOLLOWINGTHREAD_H

#include <QThread>
#include <QStringList>
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
    void setRoad(QString roadmap);
private:
    SPI* s;
    bool blstop;
    QMutex *mutex;
    QString road;
    QStringList instruction;
signals:
    
public slots:
    
};

#endif // LINEFOLLOWINGTHREAD_H
