#ifndef SPI_H
#define SPI_H

#include <linux/types.h>

class SPI
{
    int dev;
    __u8 *tx;
    __u8 *rx;
public:
    SPI();
    void dumpstat(int channel = 0);
    int setSPImode(void);
    void setupSPI(int channel = 0, int speed = 7800000);
    void send(__u8 msg[],int channel = 0);
};

#endif // SPI_H
