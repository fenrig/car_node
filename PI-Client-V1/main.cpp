#include <QCoreApplication>
#include "tcp_client.h"
#include "spi.h"
#include "binder.h"
#include "linefollowingthread.h"
#include "signal_handler.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QTime>
#include <signal.h>
//#include <QThread>

/* Socket:
 *  http://www.youtube.com/watch?v=u5OdR46542M
 *  http://qt-project.org/doc/qt-4.8/qtcpsocket.html
 *  http://qt.developpez.com/doc/4.6/network-programming/
 *  http://www.qtforum.org/article/9/qt-network-tutorial.html
 * SPI:
 *  http://www.haifux.org/lectures/258/gpio_spi_i2c_userspace.pdf
 *  http://armbedded.eu/node/318
 *  http://www.raspberrypi.org/phpBB3/viewtopic.php?p=132128
 */

int setup_unix_signal_handlers(void){
    struct sigaction term;

    term.sa_handler = signal_handler::termSignalHandler;
    //term.sa_handler = QCoreApplication::exit;

    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if(sigaction(SIGINT, &term, 0) > 0)
        return 2;

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //tcp_client c;
    //c.connectToServer();
    //SPI s;
    SPI *s = NULL;
    linefollowingthread lft(NULL);
    //linefollowingthread lft(NULL);
    signal_handler sh(&lft.stop);
    setup_unix_signal_handlers();
    lft.start();

  //  binder b(&c,&s);

    /*
    unsigned char message[] = "zbcd";
    forever{
        s.send(message);
        QTest::qSleep(50);
    }
    */
    qDebug() << "events?";
    return a.exec();
}
