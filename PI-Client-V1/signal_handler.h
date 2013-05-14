// http://doc.qt.digia.com/4.7/unix-signals.html
#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include <QObject>

#include <QSocketNotifier>
#include <linefollowingthread.h>

//static int setup_unix_signal_handlers(void);

class signal_handler : public QObject
{
    Q_OBJECT
public:
    explicit signal_handler(linefollowingthread *thread, QObject *parent = 0);
    // Unix signal handlers
    static void termSignalHandler(int unused);

public slots:
    // QT signal handlers
    void handleSigTerm();

private:
    static int sigtermFd[2];
    QSocketNotifier *snTerm;
    //bool* ptrthreadstop;
    linefollowingthread *threadptr;
    
};

#endif // SIGNAL_HANDLER_H
