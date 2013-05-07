#ifndef UARTCAMERA_H
#define UARTCAMERA_H

#include <QObject>
#include <qextserialport.h>
#include <vector>

class UartCamera : public QObject
{
    Q_OBJECT
    QextSerialPort *port;
    unsigned char MH,ML;
    int picpointer;
public:
    explicit UartCamera(QObject *parent = 0);
    ~UartCamera();
    std::vector<char> GetPicture();
    void changeSize(int size);
    void changeSizeRST(int size);
    void changeCompression(int size);
private:
        bool init(void);
        void reset(void);
        void takePic(void);
        void ReadData(void);
        unsigned short int FileSize(void);
        void changeTransferSpeed(BaudRateType baud);
        void stopTakingPic(void);
signals:
    
public slots:
    
};

#endif // UARTCAMERA_H
