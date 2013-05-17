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
    /*
     *PC Dries
     *Lcation where the images are
     */
    //Path = "/home/dries/Afbeeldingen/20130311_184718 (another copy).jpg"; //path where image is located
    //Path = "/home/dries/Afbeeldingen/20130311_184718 (copy).jpg"; //path where image is located
    //Path = "/home/dries/Afbeeldingen/Lines/20130311_184718.jpg"; //path where image is located
    /*
     * Raspberry PI Dries
     * Location where the images are
     */
    //Path = "/root/Lines/20130311_184718 (another copy).jpg";
    //Path = "/root/Lines/20130311_184718 (copy).jpg";
    //Path = "/root/Lines/20130311_184718.jpg";

    QByteArray ba = Path.toLocal8Bit();
    const char *PathChar = ba.data();
    Mat img = imread(PathChar); //read image+
    return img;
}
Mat FindLine::WhiteFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat whiteOnly;
    Mat src2;
    cvtColor(src,src,CV_BGR2HSV);
    //inRange(src,Scalar(0,0,255), Scalar(0,0,255), whiteOnly);
    //inRange(src,Scalar(0,0,100), Scalar(100,50,255), whiteOnly);
    src2 = src + cv::Scalar(-255, -255,0);
    inRange(src2,Scalar(0,0,120), Scalar(0,0,255),whiteOnly);
    //inRange(src2,Scalar(0,0,120), Scalar(100,100,255),whiteOnly); //working decently

    return whiteOnly;
}
Mat FindLine::RedFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat redOnly;
    cvtColor(src,src,CV_BGR2HSV);
    inRange(src,Scalar(0,50,100),Scalar(255,255,255),redOnly);

    return redOnly;
}
Mat FindLine::BlueFilter(const Mat& src)
{
    assert(src.type() == CV_8UC3);
    Mat blueOnly;
    cvtColor(src,src,CV_BGR2HSV);
    inRange(src,Scalar(255,255,0),Scalar(255,255,255),blueOnly);

    return blueOnly;
}
offsets FindLine::FindOffset(std::vector<char>* data, int teller, QString instructie, QString nextInstr)
{
    offsets offset;
    Mat img = Mat(*data);
    img = imdecode(img,1);
    //read img from path
    //img = FindLine::ReadImage();
    if(img.data)
    {
        vector<int> compression_params;
        compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
        compression_params.push_back(95);
        /*
         *Debug
         */
        //QString Path = "/home/dries/Documenten/Project2/PI code/picsred/origineel" + QString::number(teller) + ".jpg";
        QString Path = "/root/pics/origineel" + QString::number(teller) + ".jpg";
        QByteArray ba = Path.toLocal8Bit();
        const char *PathChar = ba.data();
        imwrite(PathChar,img, compression_params);
        /*
         *End
         */
        //crop image
        cv::Rect myROI(0, 100, 320, 10); //start position x:0 y:250 || size x:320 y:10
        img = img(myROI);
        //filteren op witte kleur
        //img = FindLine::WhiteFilter(img);
        //filteren op rode kleur
        /*
         *Debug
         */
        /*QString Path = "/home/dries/Documenten/Project2/PI code/DebuggingCarCode/TestPictures/filtered2-" + QString::number(teller) + ".jpg";
        QString Path = "/root/pics/origineel" + QString::number(teller) + ".jpg";
        QByteArray ba = Path.toLocal8Bit();
        const char *PathChar = ba.data();
        imwrite(PathChar,img, compression_params); */
        /*
         *End
         */

        /*
            N->E, S->W: rood, wit, links
            N->S, S->N: rood, wit
            N->W, E->N: rood, wit, rechts

            E->S, W->N: rood, blauw, links
            E->W, W->E: rood, blauw
            E->N, W->S: rood, blauw, rechts
        */
        qDebug("Voor alles");
        if((instructie=="n" && nextInstr=="e") || (instructie=="s" && nextInstr=="w"))
        {
            qDebug("nesw");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    wit=true;
                    qDebug("nesw rood");
                }
            }
            else if(wit==true)
            {
                img=WhiteFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    wit=false;
                    links=true;
                    qDebug("nesw wit");
                }
            }
            else if(links==true)
            {
                offset.left=0;
                offset.right=250;
                links=false;
                rood=true;
                status=false;
                qDebug("nesw links");
            }
        }
        else if((instructie=="n" && nextInstr=="s") || (instructie=="s" && nextInstr=="n"))
        {
            qDebug("nssn");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    wit=true;
                    qDebug("nssn rood");
                }
            }
            else if(wit==true)
            {
                img=WhiteFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    wit=false;
                    rood=true;
                    status=false;
                    qDebug("nssn wit");
                }
            }

        }
        else if((instructie=="n" && nextInstr=="w") || (instructie=="e" && nextInstr=="n"))
        {
            qDebug("nwen");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    wit=true;
                    qDebug("nwen rood");
                }
            }
            else if(wit==true)
            {
                img=WhiteFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    wit=false;
                    rechts=true;
                    qDebug("nwen wit");
                }
            }
            else if(rechts==true)
            {
                offset.left=250;
                offset.right=0;
                rechts=false;
                rood=true;
                status=false;
                qDebug("nwen rechts");
            }
        }
        else if((instructie=="e" && nextInstr=="s") || (instructie=="w" && nextInstr=="n"))
        {
            qDebug("eswn");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    blauw=true;
                    qDebug("eswn rood");
                }
            }
            else if(blauw==true)
            {
                img=BlueFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    blauw=false;
                    links=true;
                    qDebug("eswn blauw");
                }
            }
            else if(links==true)
            {
                offset.left=0;
                offset.right=250;
                links=false;
                blauw=true;
                status=false;
                qDebug("eswn links");
            }
        }
        else if((instructie=="e" && nextInstr=="w") || (instructie=="w" && nextInstr=="e"))
        {
            qDebug("ewwe");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    blauw=true;
                    qDebug("ewwe rood");
                }
            }
            else if(blauw==true)
            {
                img=BlueFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    blauw=false;
                    rood=true;
                    status=false;
                    qDebug("eswn blauw");
                }
            }
        }
        else if((instructie=="e" && nextInstr=="n") || (instructie=="w" && nextInstr=="s"))
        {
            qDebug("enws");
            if(rood==true)
            {
                img = RedFilter(img);
                offset = searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    rood=false;
                    blauw=true;
                    qDebug("enws rood");
                }
            }
            else if(blauw==true)
            {
                img=BlueFilter(img);
                offset=searchOffset(img);
                if(offset.left==250 && offset.right==250)
                {
                    blauw=false;
                    rechts=true;
                    qDebug("eswn blauw");
                }
            }
            else if(rechts==true)
            {
                offset.left=0;
                offset.right=250;
                rechts=false;
                blauw=true;
                status=false;
                qDebug("eswn rechts");
            }
        }

        // nsnwneswewsen
        /*
         *Debug
         */
        //QString Path2 = "/home/dries/Documenten/Project2/PI code/picsred/filtered2-" + QString::number(teller) + ".jpg";
        QString Path2 = "/root/pics/test" + QString::number(teller) + ".jpg";
        QByteArray ba2 = Path2.toLocal8Bit();
        const char *PathChar2 = ba2.data();
        imwrite(PathChar2,img, compression_params);
        /*
         *End
         */
    }
    else
    {
        printf("Invalid image! \n");
    }
    qDebug() << "No offset!";
    offset.left = 250;
    offset.right = 250;
    return offset;
}
offsets FindLine::searchOffset(const Mat& img)
 {
        offsets offset;
        //find offsets
        uint8_t* pixelPtr = (uint8_t*)img.data;
        int cn = img.channels();
        int whitePX,whitePX2,i,j,x;
        int roadwith = 250;
        int counterL=0;
        for(i=0;i<img.rows;i++)
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
                    if(whitePX==9) //if there is a row of white px (10 wide)
                    {
                        //check for the other side of the road, road is 250 px wide
                        //qDebug() << "10 witte pixels!";
                        if(pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 0]==255 && pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 1]==255 && pixelPtr[i*img.cols*cn + (j+roadwith)*cn + 2]==255)
                        {   //two white tracks?
                            for(x=0;x<10;x++) //check the next 10 pxs to find the other side
                            {
                                Scalar_<uint8_t> bgrPixelRight;
                                int position;
                                //position = j+roadwith-10+x; //check other side of the road
                                position = j+roadwith-5+x;
                                bgrPixelRight.val[0] = pixelPtr[i*img.cols*cn + (position)*cn + 0];
                                bgrPixelRight.val[1] = pixelPtr[i*img.cols*cn + (position)*cn + 1];
                                bgrPixelRight.val[2] = pixelPtr[i*img.cols*cn + (position)*cn + 2];
                                if(bgrPixelRight.val[0]==255 && bgrPixelRight.val[1]==255 && bgrPixelRight[2]==255)
                                {
                                    whitePX2=whitePX2+1;
                                }
                                /*
                                else
                                {
                                    whitePX2=0;
                                }
                                */
                            }
                            //if(whitePX2==10)
                            if(whitePX2>5)
                            {
                                //return offsets
                                qDebug()<< "Found Tracks!";
                                offset.left = 160-j; //160= mid of the picture , j = position of the track
                                offset.right= j+roadwith-160; //j+roadwith = position of the track , 160=mid of the picture
                                return offset;
                            }
                            else
                            {
                                whitePX2=0;
                            }
                        }
                        else //if no white pxl is found on the other side
                        {
                            counterL = counterL+1; //count how many times there is no white pixel found on the other side
                            //qDebug() << "CounterL:" << counterL;
                            if(counterL==5) //if it happened 5 times that there was no white pxl at the other side, there is no road on the other side
                            {
                                offset.left = 160-j;
                                offset.right = 250;
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
        }
        //right side only
        for(i=0;i<img.rows;i++)
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
                    if(whitePX==9) //if there is a row of white px (10 wide)
                    {
                        whitePX=0;
                        qDebug() << "Found right track!";
                        offset.left = 250; //default value for it there is no left track
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
    return offset;
}
