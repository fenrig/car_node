#include "uartcamera.h"
#include <QDebug>
#include <unistd.h>
#include <vector>

#include <QFile>

// Serial library
//  https://code.google.com/p/qextserialport/downloads/detail?name=qextserialport-1.2rc.zip&can=1&q=
//  Werkt vermoedelijk op de pi (aangezien standaard linux serial programma's er ook mee werken)
//  http://docs.qextserialport.googlecode.com/git/1.2/qextserialport.html

// Betere Datasheet
//  http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/LightImaging/UserManual.pdf

#include "qextserialport.h"

#define DATARATE_high 0x10
#define DATARATE_low 0x00

#define DATARATE (((DATARATE_high)<<8) + DATARATE_low)

UartCamera::UartCamera(QObject *parent) :
    QObject(parent), picpointer(0x0000)
{
    port = new QextSerialPort("/dev/ttyAMA0");//"/dev/ttyUSB0");
    port->open(QIODevice::ReadWrite);
    if(port->isOpen()) qDebug() << "Opened Serial";
    else qDebug() << "Failed opening Serial";
    port->setBaudRate(BAUD38400);
    init();
    changeSizeRST(320*240);
    changeTransferSpeed(BAUD115200);
    usleep(25000);
    //takePic();
}

bool UartCamera::init(void){
    reset();
    int i;
    char data[15];
    char incomingbyte;
    bool initflag = true;
    while(initflag){
        /*
        if(port->bytesAvailable() > 0) qDebug() << "Bytes available";
        else qDebug() << "No Bytes";
        */
        while(port->bytesAvailable() > 0){
            port->read(&incomingbyte,1);
            for(i = 0;i < 14;i++){
                data[i] = data[i+1];
            }
            data[14] = incomingbyte;

            if(data[0] == (char)0x36 && data[1] == (char)0x32 &&
                    data[2] == (char)0x35 && data[3] == (char)0x0D &&
                    data[4] == (char)0x0A && data[5] == (char)0x49 &&
                    data[6] == (char)0x6E && data[7] == (char)0x69 &&
                    data[8] == (char)0x74 && data[9] == (char)0x20 &&
                    data[10] == (char)0x65 && data[11] == (char)0x6E &&
                    data[12] == (char)0x64 && data[13] == (char)0x0D &&
                    data[14] == (char)0x0A){
                 initflag = false;
                 break;
            }
        }
    }
    usleep(3 * 1000 * 1000); // Wait 3 seconds
    return true;
}

void UartCamera::reset(void){
    QByteArray resetcommand;
    resetcommand.resize(4);
    resetcommand[0] = 0x56;
    resetcommand[1] = 0x00;
    resetcommand[2] = 0x26;
    resetcommand[3] = 0x00;
    port->write(resetcommand,4);
    port->flush();
    while(port->bytesAvailable() < 4){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x26 && answer[3] == (char)0x00){
        qDebug() << "Camera Reset succeeded";
    }else{
        qDebug() << "Camera Reset failed, reattempting";
        reset();
    }
    //qDebug() << QString::number(answer[0], 16) << QString::number(answer[1], 16) << QString::number(answer[2], 16) << QString::number(answer[3], 16);
}

void UartCamera::takePic(void){
    QByteArray takepiccmd;
    takepiccmd.resize(5);
    takepiccmd[0] = 0x56;
    takepiccmd[1] = 0x00;
    takepiccmd[2] = 0x36;
    takepiccmd[3] = 0x01;
    takepiccmd[4] = 0x00;
    port->write(takepiccmd,5);
    port->flush();
    while(port->bytesAvailable() < 5){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x36 && answer[3] == (char)0x00
            && answer[4] == (char)0x00){
        qDebug() << "Camera took pic";
    }else{
        qDebug() << "Camera pic failed";
    }
    //qDebug() << QString::number(answer[0], 16) << QString::number(answer[1], 16) << QString::number(answer[2], 16) << QString::number(answer[3], 16) << QString::number(answer[4], 16);
}

void UartCamera::ReadData(void){
    MH = picpointer / 0x100;
    ML = picpointer % 0x100;

    QByteArray readdatacmd;
    readdatacmd.resize(16);
    readdatacmd[0] = 0x56;
    readdatacmd[1] = 0x00;
    readdatacmd[2] = 0x32;
    readdatacmd[3] = 0x0c;
    readdatacmd[4] = 0x00;
    readdatacmd[5] = 0x0a;
    readdatacmd[6] = 0x00;
    readdatacmd[7] = 0x00;
    readdatacmd[8] = MH;   // MM 1
    readdatacmd[9] = ML;   // MM 2
    readdatacmd[10]= 0x00;
    readdatacmd[11]= 0x00;
    readdatacmd[12] = DATARATE_high;
    readdatacmd[13] = DATARATE_low;

    /*
    readdatacmd[12]= 0x00; // KK 1
    readdatacmd[13]= 0x20; // KK 2
    */
    readdatacmd[14]= 0x00; // XX 1
    readdatacmd[15]= 0x0a; // XX 2
    port->write(readdatacmd,16);
    port->flush();

    picpointer += DATARATE; // kk1kk2
}

std::vector<char> UartCamera::GetPicture(){

    picpointer = 0;
    bool EndFlag = false;
    int k,j;
    char incomingbyte;
    unsigned char data[DATARATE];

    takePic();

    std::vector<char> *vect = new std::vector<char>;

    //test
    QFile file("/root/pics/out.jpg");
    if (!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "krijgt file niet open";
    }
    //test

    while(!EndFlag){
        k = j = 0;
        ReadData();
        while((k < (10 + DATARATE)) /*&& !EndFlag */){
            //while(port->bytesAvailable() == 0 && !EndFlag){;}
            while(port->bytesAvailable() > 0){
                port->read(&incomingbyte,1);
                k++;

                if((k>5) && (j<DATARATE) && (!EndFlag)){
                    vect->push_back(incomingbyte);
                    file.write(&incomingbyte);
                    data[j] = incomingbyte;
                    if((data[j-1] == 0xFF) && (data[j] == 0xD9)) EndFlag = true;
                    j++;
                }
            }
        }
    }

    stopTakingPic();
    while(port->bytesAvailable() > 0){
        port->read(&incomingbyte, 1);
    }
    file.close();
    return *vect;
}

unsigned short int UartCamera::FileSize(void){
    QByteArray filesizecmd;
    filesizecmd.resize(5);
    filesizecmd[0] = 0x56;
    filesizecmd[1] = 0x00;
    filesizecmd[2] = 0x34;
    filesizecmd[3] = 0x01;
    filesizecmd[4] = 0x00;
    port->write(filesizecmd,5);
    port->flush();
    while(port->bytesAvailable() < 9){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x34 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x04 && answer[5] == (char)0x00 &&
            answer[6] == (char)0x00){
        unsigned short int size = 0;
        size += ((unsigned char)answer[7]);
        size = size << 2;
        size += ((unsigned char)answer[8]);
        qDebug() << "Reading jpeg size succeeded: " << QString::number(size);
        return ((answer[7] << 8) + answer[8]);
    }else{
        qDebug() << "Reading jpeg size failed";
    }
    //qDebug() << QString::number(answer[0], 16) << QString::number(answer[1], 16) << QString::number(answer[2], 16) << QString::number(answer[3], 16) << QString::number(answer[4], 16) << QString::number(answer[5], 16) << QString::number(answer[6], 16);
    return 0x00;
}

void UartCamera::changeTransferSpeed(BaudRateType baud){
    QByteArray filesizecmd;
    filesizecmd.resize(7);
    filesizecmd[0] = 0x56;
    filesizecmd[1] = 0x00;
    filesizecmd[2] = 0x24;
    filesizecmd[3] = 0x03;
    filesizecmd[4] = 0x01;

    switch (baud){
        case BAUD9600:
            filesizecmd[5] = 0xAE;
            filesizecmd[6] = 0xC8;
            break;
        case BAUD19200:
            filesizecmd[5] = 0x56;
            filesizecmd[6] = 0xE4;
            break;
        case BAUD38400:
            filesizecmd[5] = 0x2A;
            filesizecmd[6] = 0xF2;
            break;
        case BAUD57600:
            filesizecmd[5] = 0x1C;
            filesizecmd[6] = 0x4C;
            break;
        case BAUD115200:
            filesizecmd[5] = 0x0D;
            filesizecmd[6] = 0xA6;
            break;
        default:
            return;
            break;
    }
    port->write(filesizecmd,7);
    port->flush();

    while(port->bytesAvailable() < 5){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x24 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x00){
        qDebug() << "Baud change succeeded";
        port->setBaudRate(baud);
    }else{
        qDebug() << "Baud change failed";
    }
    // qDebug() << QString::number(answer[0], 16) << QString::number(answer[1], 16) << QString::number(answer[2], 16) << QString::number(answer[3], 16) << QString::number(answer[4], 16);
}

void UartCamera::changeSizeRST(int size){
    QByteArray picsizerstcmd;
    picsizerstcmd.resize(9);
    picsizerstcmd[0] = 0x56;
    picsizerstcmd[1] = 0x00;
    picsizerstcmd[2] = 0x31;
    picsizerstcmd[3] = 0x05;
    picsizerstcmd[4] = 0x04;
    picsizerstcmd[5] = 0x01;
    picsizerstcmd[6] = 0x00;
    picsizerstcmd[7] = 0x19;
    switch (size){
        case (640*480):
            picsizerstcmd[8] = 0x00;
            break;
        case (320*240):
            picsizerstcmd[8] = 0x11;
            break;
        case (160*120):
            picsizerstcmd[8] = 0x22;
            break;
        default:
            return;
            break;
    }
    port->write(picsizerstcmd,9);
    port->flush();
    while(port->bytesAvailable() < 5){;}
    QByteArray answer = port->readAll();
    //76 00 54 00 00 //aangepast
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x31 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x00){
        qDebug() << "Picture size changed";
        init();
    }else{
        qDebug() << "Picture size not changed";
    }
}

void UartCamera::changeSize(int size){
    QByteArray picsizecmd;
    picsizecmd.resize(5);
    picsizecmd[0] = 0x56;
    picsizecmd[1] = 0x00;
    picsizecmd[2] = 0x54;
    picsizecmd[3] = 0x01;
    switch (size){
        case (640*480):
            picsizecmd[4] = 0x00;
            break;
        case (320*240):
            picsizecmd[4] = 0x11;
            break;
        case (160*120):
            picsizecmd[4] = 0x22;
            break;
        default:
            return;
            break;
    }
    port->write(picsizecmd,5); //aangepast
    port->flush();
    while(port->bytesAvailable() < 5){;}
    QByteArray answer = port->readAll();
    //76 00 54 00 00 //aangepast
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x54 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x00){
        qDebug() << "Picture size changed";
    }else{
        qDebug() << "Picture size not changed";
    }
    // ----------------------------
    QString x = "";
    int i;
    for(i = 0;i < answer.length();i++){
        x = x + QString::number((unsigned char)answer[i], 16) + " ; ";
    }
    qDebug() << x;
    // ----------------------------
}

void UartCamera::stopTakingPic(void){
    QByteArray stoppiccmd;
    stoppiccmd.resize(5);
    stoppiccmd[0] = 0x56;
    stoppiccmd[1] = 0x00;
    stoppiccmd[2] = 0x36;
    stoppiccmd[3] = 0x01;
    stoppiccmd[4] = 0x03;
    port->write(stoppiccmd,5);
    port->flush();
    while(port->bytesAvailable() < 5){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x36 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x00){
        qDebug() << "Stop taking pics";
    }else{
        qDebug() << "Could not stop taking pics";
    }
    // ----------------------------
    QString x = "";
    int i;
    for(i = 0;i < answer.length();i++){
        x = x + QString::number((unsigned char)answer[i], 16) + " ; ";
    }
    // ----------------------------
}

void UartCamera::changeCompression(int size){
    QByteArray changecompressioncmd;
    changecompressioncmd.resize(9);
    changecompressioncmd[0] = 0x56;
    changecompressioncmd[1] = 0x00;
    changecompressioncmd[2] = 0x31;
    changecompressioncmd[3] = 0x05;
    changecompressioncmd[4] = 0x01;
    changecompressioncmd[5] = 0x01;
    changecompressioncmd[6] = 0x12;
    changecompressioncmd[7] = 0x04;
    changecompressioncmd[8] = (char)size;
    port->write(changecompressioncmd,9);
    port->flush();
    while((port->bytesAvailable()) < 5){;}
    QByteArray answer = port->readAll();
    if(answer[0] == (char)0x76 && answer[1] == (char)0x00 &&
            answer[2] == (char)0x31 && answer[3] == (char)0x00 &&
            answer[4] == (char)0x00){
        qDebug() << "Compression changed: " << size;
    }else{
        qDebug() << "Compression not changed";
    }
    // 56 00 31 0 01 01 12 04 XX
}
