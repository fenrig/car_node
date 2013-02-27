#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <QObject>
//#include <QTcpSocket>
class QTcpSocket;

class tcp_client : public QObject
{
    Q_OBJECT
public:
    tcp_client(QObject *parent = 0);
    void connectToServer();
signals:
public slots:
    void on_connected();
    void read_data();
private:
    QTcpSocket* socket;
};

#endif // TCP_CLIENT_H
