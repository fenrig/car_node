#ifndef THREADING_H
#define THREADING_H

#include <QThread>

class Threading : public QThread
{
public:
    Threading();
    void run();
};

void Threading::run()
{

}

#endif // THREADING_H
