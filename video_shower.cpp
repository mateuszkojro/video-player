#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

//hide the local functions in an anon namespace
namespace {

    int process(VideoCapture& capture) {
        int n = 0;
        char filename[200];


        namedWindow("window", WINDOW_KEEPRATIO); //resizable window;
        Mat frame;

        for (;;) {
            capture >> frame;
            if (frame.empty())
                break;

            imshow("window", frame);

        }
        return 0;
    }
}

int main(int ac, char** av) {
       std::string arg = "C:\\Users\\pc\\Documents\\nice.mp4";

    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file or image sequence
    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open the video device, video file or image sequence!\n" << endl;

        return 1;
    }
    return process(capture);
}