#include <QCoreApplication>
#include "tcp_client.h"
#include "spi.h"
#include "binder.h"
#include "linefollowingthread.h"

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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

  //  tcp_client c;
  //  c.connectToServer();
    SPI s;

    linefollowingthread lft(&s);
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
