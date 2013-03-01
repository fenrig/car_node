#include "spi.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

// https://www.kernel.org/doc/Documentation/spi/spidev
// https://www.kernel.org/doc/Documentation/spi/spidev_fdx.c

SPI::SPI(){
    int spidev = open("/dev/spidev0.1", O_RDWR);
    dumpstat("/dev/spidev0.1",spidev);
}

void SPI::dumpstat(const char *name, int fd)
{
    __u8	mode, lsb, bits;
    __u32	speed;

    if (ioctl(fd, SPI_IOC_RD_MODE, &mode) < 0) {
        perror("SPI rd_mode");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
        perror("SPI rd_lsb_fist");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        perror("SPI bits_per_word");
        return;
    }
    if (ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
        perror("SPI max_speed_hz");
        return;
    }

    printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",
        name, mode, bits, lsb ? "(lsb first) " : "", speed);
}
