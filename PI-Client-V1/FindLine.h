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
public:
    explicit FindLine(QObject *parent = 0);
    Mat ReadImage(void);
    Mat WhiteFilter(const Mat& src);
    offsets FindOffset(std::vector<char> data);
};

#endif // FINDLINE_H
