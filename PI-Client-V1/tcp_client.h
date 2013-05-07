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
    ~tcp_client();
    void connectToServer();
    void write_data(QByteArray *buffer);
signals:
    void readyRead(QString msg);
public slots:
private slots:
    void on_connected();
    void read_data();
    void on_disconnected();
private:
    QTcpSocket* socket;
    QString UDP_Server_Discovery();
};

#endif // TCP_CLIENT_H
