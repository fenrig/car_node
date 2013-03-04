#ifndef BINDER_H
#define BINDER_H

#include <QObject>

class tcp_client;
class SPI;

class binder : public QObject
{
private:
    tcp_client *tcp;
    SPI *spi;
public:
    binder(tcp_client *tcp_x, SPI *spi_x);
signals:
public slots:
    void parseTCPmsg(QString msg);
};

#endif // BINDER_H
