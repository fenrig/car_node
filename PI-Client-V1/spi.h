#ifndef SPI_H
#define SPI_H

#include <linux/types.h>

class SPI
{
    int dev;
public:
    SPI();
    void dumpstat(const char *name);
    int setSPImode(void);
    void setupSPI(int channel = 0, int speed = 500000);
};

#endif // SPI_H
