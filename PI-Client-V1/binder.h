#ifndef BINDER_H
#define BINDER_H

#include <QObject>
#include <QMap>

class tcp_client;
class linefollowingthread;
// class SPI;

class binder : public QObject
{
    Q_OBJECT
//private:
    tcp_client *tcp;
    linefollowingthread *lft;
    // SPI *spi;
    QMap<QString, unsigned char> instructions;
public:
    binder(tcp_client *tcp_x/*, SPI *spi_x*/,linefollowingthread *lft, QObject *parent = 0);
    bool read=false;
signals:
public slots:
    void parseTCPmsg(QString msg);
};

#endif // BINDER_H
