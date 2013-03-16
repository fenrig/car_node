
#include "FindLine.h"
#include <opencv/cv.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/highgui.h>

#include "QString"
#include "QDebug"

/*
 *C includes
 */
#include <stdio.h>
#include <iostream>
#include <math.h>

using namespace cv;
FindLine::FindLine(QObject *parent) :
    QObject(parent)
{
}

Mat FindLine::ReadImage()
{
    QString Path;
    //Path = "/home/dries/Afbeeldingen/20130311_184718 (another copy).jpg"; //path where image is located
    //Path = "/home/dries/Afbeeldingen/20130311_184718 (copy).jpg"; //path where image is located
    //Path = "/home/dries/Afbeeldingen/Lines/20130311_184718.jpg"; //path where image is located
    /*
     * Raspberry PI Dries
     * Location where the images are
     */
    //Path = "~/Lines/20130311_184718 (another copy).jpg";
    //Path = "~/Lines/20130311_184718 (copy).jpg";
    Path = "~/Lines/20130311_184718.jpg";

    QByteArray ba = Path.toLocal8Bit();
    const char *PathChar = ba.data();
    Mat img = imread(PathChar); //read image+
    return img;
}

Mat FindLine::WhiteFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat whiteOnly;
    cvtColor(src,src,CV_BGR2HSV);
    //inRange(src,Scalar(0,0,255), Scalar(0,0,255), whiteOnly);
    //inRange(src,Scalar(0,0,100), Scalar(100,50,255), whiteOnly);
    inRange(src,Scalar(0,0,90), Scalar(40,40,255),whiteOnly);
    return whiteOnly;
}
offsets FindLine::FindOffset()
{
    offsets offset;
    Mat img;
    //read img from path
    img = FindLine::ReadImage();
    //crop image
    printf("Test before myROI \n");
    cv::Rect myROI(0, 230, 320, 10); //start position x:0 y:230 || size x:320 y:10
    printf("Test after myROI \n");
    img = img(myROI);
    //filteren op witte kleur
    img = FindLine::WhiteFilter(img);
    //imshow("hallo",img);


    //find offsets
    uint8_t* pixelPtr = (uint8_t*)img.data;
    int cn = img.channels();
    int whitePX,blackPX,i,j,x,counter=0;
    int roadwith = 220;
    int counterL=0;
    for(i=0;i<img.rows;i++)
    {
        if(counter<5)
        {
            for(j=0;j<img.cols/2; j+= cn)
            {
                //get color from image
                Scalar_<uint8_t> bgrPixel;
                bgrPixel.val[0] = pixelPtr[i*img.cols*cn + j*cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[i*img.cols*cn + j*cn + 1]; // G
                bgrPixel.val[2] = pixelPtr[i*img.cols*cn + j*cn + 2]; // R
                //check if color is white or not
                if(bgrPixel.val[0]==255 && bgrPixel.val[1]==255 && bgrPixel.val[2]==255)
                {
                    whitePX = whitePX +1; //count the amount of white pixels
                    //qDebug() << "witte pixel!" << whitePX;
                    if(whitePX==9) //if there is a row of white px (10 wide)
                    {
                        //check for the other side of the road, road is 220 px wide
                        qDebug() << "10 witte pixels!";
                        if(pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 0]==255 && pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 1]==255 && pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 2]==255)
                        {   //two white tracks?
                            qDebug() << "TEST";
                            for(x=0;x<10;x++)
                            {
                                Scalar_<uint8_t> bgrPixelRight;
                                int position;
                                position = j+roadwith-10+x; //check other side of the road
                                bgrPixelRight.val[0] = pixelPtr[i*img.cols*cn + (position)*cn + 0];
                                bgrPixelRight.val[1] = pixelPtr[i*img.cols*cn + (position)*cn + 1];
                                bgrPixelRight.val[2] = pixelPtr[i*img.cols*cn + (position)*cn + 2];
                                if(bgrPixelRight.val[0]==255 && bgrPixelRight.val[1]==255 && bgrPixelRight[2]==255)
                                {
                                   blackPX=blackPX+1;
                                }
                                else
                                {
                                    blackPX=0;
                                }
                            }
                            if(blackPX==10)
                            {
                                //return offsets
                                qDebug()<< "Found Tracks!";
                                offset.left = j; //j is inside coord from the left track
                                offset.right= j+roadwith-160; //j+210 is the inside coord from the left track
                                return offset;
                            }
                            else
                            {
                                blackPX=0;
                            }
                        }
                        else
                        {
                            counterL = counterL+1;
                            qDebug() << "CounterL:" << counterL;
                            if(counterL==5)
                            {
                                qDebug() << "Testing";
                                offset.left = 160-j;
                                offset.right = 220;
                                return offset;
                            }
                        }
                        whitePX=0;
                    }
                }
                else
                {
                    whitePX=0; //if there is a black px in the row, put counter on 0
                    //qDebug() << "zwarte pixel!";
                }
            }
            counter=counter+1;
        }
        if(counter==5) //if this is true , there is no track on the left side => follow the right side
        {
            for(j=img.cols/2;j<img.cols && j>=img.cols/2; j+= cn)
            {
                //get color from image
                Scalar_<uint8_t> bgrPixel;
                bgrPixel.val[0] = pixelPtr[i*img.cols*cn + j*cn + 0]; // B
                bgrPixel.val[1] = pixelPtr[i*img.cols*cn + j*cn + 1]; // G
                bgrPixel.val[2] = pixelPtr[i*img.cols*cn + j*cn + 2]; // R
                //check if color is white or not
                if(bgrPixel.val[0]==255 && bgrPixel.val[1]==255 && bgrPixel.val[2]==255)
                {
                    whitePX = whitePX +1; //count the amount of white pixels
                    //qDebug() << "witte pixel!" << whitePX;
                    if(whitePX==9) //if there is a row of white px (10 wide)
                    {
                        //check for the other side of the road, road is 220 px wide
                        qDebug() << "10 witte pixels! rechts";
                        whitePX=0;
                        qDebug() << "Found right track!";
                        offset.left = 220; //default value for it there is no left track
                        offset.right = j-160;
                        return offset;
                        //returnen van offsets
                    }
                }
                else
                {
                    whitePX=0; //if there is a black px in the row, put counter on 0
                    //qDebug() << "zwarte pixel!";
                }
            }
        }
    }
    //imshow("cropped",img);

    return offset;
}
