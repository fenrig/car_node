#ifndef SPI_H
#define SPI_H

class SPI
{
public:
    SPI();
    void dumpstat(const char *name, int fd);
    int setSPImode(void);
};

#endif // SPI_H
