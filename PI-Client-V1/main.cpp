#include <QCoreApplication>
#include "tcp_client.h"
#include "spi.h"
#include "binder.h"
#include "linefollowingthread.h"
#include "signal_handler.h"

#include <QtTest/QTest>
#include <QDebug>
#include <QTime>
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

static int setup_unix_signal_handlers(){
    struct sigaction hup, term;

    /*
    hup.sa_handler = MyDaemon::hupSignalHandler;
    sigemptyset(&hup.sa_mask);
    hup.sa_flags = 0;
    hup.sa_flags |= SA_RESTART;

    if (sigaction(SIGHUP, &hup, 0) > 0)
       return 1;
    */

    term.sa_handler = MyDaemon::termSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0) > 0)
       return 2;

    return 0;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    setup_unix_signal_handlers();
    //tcp_client c;
    //c.connectToServer();
    SPI s;

    linefollowingthread lft(&s);
    //linefollowingthread lft(NULL);
    lft.run();

  //  binder b(&c,&s);

    /*
    unsigned char message[] = "zbcd";
    forever{
        s.send(message);
        QTest::qSleep(50);
    }
    */

    return a.exec();
}
