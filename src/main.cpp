#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <condition_variable>
#include <detect/detect.h>

using namespace std;
using namespace cv;

Scalar AimHSVColorBlue(215, 0.5, 0.5);
Scalar BlueRange(15, 0.5, 0.5);

Scalar AimHSVColorGreen(160.0, 0.5, 0.5);
Scalar GreenRange(20, 0.5, 0.5);

condition_variable CapedOrRecord;
bool ISCaped;
mutex m;

Mat CapImg;

VideoCapture RecordCap(0);
VideoWriter OutPut;

double fps, VideoWidth, VideoHeight;

const int FrameCount = 100;
int Frame = 0;

void cap()
{
    while(true)
    {
        unique_lock<mutex> lk(m);
        CapedOrRecord.wait(lk, [&]{ return !ISCaped; } );

        Frame++;

        cout << "Cap " << Frame << endl;

        RecordCap >> CapImg;

        detect(CapImg, AimHSVColorBlue, BlueRange);

        imshow("RecordCap", CapImg);

        int c = waitKey(1000 / fps);
        if((c & 255) == 27)
            return;

        ISCaped = true;

        CapedOrRecord.notify_all();

        if(Frame > FrameCount)
            return ;

    }

}

void record()
{
    while(true)
    {
        if(!CapImg.empty())
        {
            unique_lock<mutex> lk(m);
            CapedOrRecord.wait(lk, [&]{ return ISCaped; } );

            cout << "Record " << Frame << endl;

            if(!CapImg.empty())
                OutPut << CapImg;

        }

        ISCaped = false;

        CapedOrRecord.notify_all();

        if(Frame > FrameCount)
            return ;

    }

}

void recordInitialize()
{

    fps = RecordCap.get(CAP_PROP_FPS);
    VideoWidth = RecordCap.get(CAP_PROP_FRAME_WIDTH);
    VideoHeight = RecordCap.get(CAP_PROP_FRAME_HEIGHT);
    cout << "FPS : " << fps << endl;
    cout << "VideoWidth : " << VideoWidth << endl;
    cout << "VideoHeight : " << VideoHeight << endl;

    int fourcc = VideoWriter::fourcc('m','p','4','v');
    Size size(VideoWidth, VideoHeight);
    OutPut = {"Cap.mp4", fourcc, fps, size};

}

int main()
{
    recordInitialize();

    thread proc1(cap);

    waitKey(1);

    thread proc2(record);

    proc1.join();
    proc2.join();



    return 0;
}