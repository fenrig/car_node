/* Based on https://github.com/WiringPi/WiringPi/blob/master/wiringPi/wiringPiSPI.c
 *
 */
#include "spi.h"

#include <stdio.h>
#include <unistd.h>
//#include <stdlib.h>
#include <fcntl.h>
//#include <string.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <linux/types.h>
#include <linux/spi/spidev.h>

// --------------
const static char       *spiDev0 = "/dev/spidev0.0" ;
const static char       *spiDev1 = "/dev/spidev0.1" ;
static __u8       spiMode   = 0 ;
static __u16      spiBPW    = 8 ;
static __u16      spiDelay  = 0;

static __u32    spiSpeeds [2] ;
static int         spiFds [2] ;


// https://www.kernel.org/doc/Documentation/spi/spidev
// https://www.kernel.org/doc/Documentation/spi/spidev_fdx.c
// https://github.com/WiringPi/WiringPi/blob/master/wiringPi/wiringPiSPI.c

SPI::SPI(){
    dumpstat();
    setupSPI();
    dumpstat();
}

void SPI::send(unsigned char *msg, int size, int channel){
    tx = (__u8*) msg;
    __u8 quirk[size];
    rx = quirk;
    struct spi_ioc_transfer tr;

    tr.tx_buf = (unsigned long)tx;
    tr.rx_buf = (unsigned long)rx;
    tr.len = size;
    tr.delay_usecs = spiDelay;
    tr.speed_hz = spiSpeeds[channel];
    tr.bits_per_word = spiBPW;
    if((ioctl(dev, SPI_IOC_MESSAGE(1), &tr)) < 1)
        printf("Failed sending\n");
}

void SPI::dumpstat(int channel){
    channel &= 1;

    const char *name = (channel == 0 ? spiDev0 : spiDev1);

    int localdev = open(name, O_RDWR);

    __u8	mode, lsb, bits;
    __u32	speed;

    if (ioctl(localdev, SPI_IOC_RD_MODE, &mode) < 0) {
        perror("SPI rd_mode");
        return;
    }
    if (ioctl(localdev, SPI_IOC_RD_LSB_FIRST, &lsb) < 0) {
        perror("SPI rd_lsb_fist");
        return;
    }
    if (ioctl(localdev, SPI_IOC_RD_BITS_PER_WORD, &bits) < 0) {
        perror("SPI bits_per_word");
        return;
    }
    if (ioctl(localdev, SPI_IOC_RD_MAX_SPEED_HZ, &speed) < 0) {
        perror("SPI max_speed_hz");
        return;
    }

    printf("%s: spi mode %d, %d bits %sper word, %d Hz max\n",
        name, mode, bits, lsb ? "(lsb first) " : "", speed);
}

int SPI::setSPImode(void){
    struct spi_ioc_transfer spi;
    //channel &= 1 ;

     //spi.tx_buf        = (unsigned long)data ;
     //spi.rx_buf        = (unsigned long)data ;
     //spi.len           = len ;
     //spi.delay_usecs   = spiDelay ;
     //spi.speed_hz      = spiSpeeds [channel] ;
     //spi.bits_per_word = spiBPW ;

     //return ioctl (spiFds [channel], SPI_IOC_MESSAGE(1), &spi) ;
    return 0;
}

void SPI::setupSPI(int channel, int speed){
    channel &= 1;

    dev = open(channel == 0 ? spiDev0 : spiDev1, O_RDWR);

    spiSpeeds[channel] = speed;

    if((ioctl(dev, SPI_IOC_WR_MODE, &spiMode)) < 0)
        printf("Failed setting spiMode (WR)\n");
    if((ioctl(dev, SPI_IOC_RD_MODE, &spiMode)) < 0)
        printf("Failed setting spiMode (RD)\n");
    if((ioctl(dev, SPI_IOC_WR_BITS_PER_WORD, &spiBPW)) < 0)
        printf("Failed setting Bits Per Word (WR)\n");
    if((ioctl(dev, SPI_IOC_RD_BITS_PER_WORD, &spiBPW)) < 0)
        printf("Failed setting Bits Per Word (RD)\n");
    if((ioctl(dev, SPI_IOC_WR_MAX_SPEED_HZ, &speed)) < 0)
        printf("Failed setting SPEED (WR)\n");
    if((ioctl(dev, SPI_IOC_RD_MAX_SPEED_HZ, &speed)) < 0)
        printf("Failed setting SPEED (RD)\n");
}

