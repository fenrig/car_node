#ifndef FINDLINE_H
#define FINDLINE_H

#include <QString>
#include <QObject>
#include "vector"

#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>

typedef struct {
    int left;
    int right;
} offsets;

using namespace cv;
class FindLine : public QObject
{
private:
    VideoCapture cam;
public:
    explicit FindLine(QObject *parent = 0);
    ~FindLine();
    Mat ReadImage(void);
    Mat WhiteFilter(const Mat& src);
    Mat RedFilter(const Mat& src);
    Mat BlueFilter(const Mat& src);
    offsets FindOffset(int teller, QString instructie, QString nextInstr);
    offsets searchOffset(const Mat& img);
    offsets TurnLeft();
    offsets TurnRight();
    bool blauw,rood,wit,links,rechts,status;
};

#endif // FINDLINE_H
