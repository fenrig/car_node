#ifndef BINDER_H
#define BINDER_H

#include <QObject>

class tcp_client;
class SPI;

class binder : public QObject
{
    Q_OBJECT
//private:
    tcp_client *tcp;
    SPI *spi;
    QMap<QString, unsigned char> instructions;
public:
    binder(tcp_client *tcp_x, SPI *spi_x, QObject *parent = 0);
signals:
public slots:
    void parseTCPmsg(QString msg);
};

#endif // BINDER_H
