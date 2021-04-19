#include <QApplication>

#include "VideoPlayer.h"

#if WITH_OPENCV

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <string>

using namespace cv;

int main() {

    std::string file_in_name = "sunflower.jpg";
    std::string file_out_name = "wynik.jpg";

    bool grey = 0;
    bool hsv = 0;
    bool blur_r = 0;
    bool sobel = 0;
    bool canny = 0;
    bool szum = 0;
    bool gauss = 0;

    Mat img_src, img_dst;
    std::vector<int> compression_params;					//element przechowuj�cy dane o sposobie zapisu
    //compression_params.push_back(100);
    img_src = imread(file_in_name);

    if (!img_src.data)										//Sprawdzenie poprawnosci odczytu
    {
        std::cout << "Nie znaleziono pliku " << file_in_name << ".";
        return -1;
    }

    grey = 1;
    //hsv = 1;
    //blur_r = 1;
    //sobel = 1;
    //canny = 1;
    //szum = 1;
    //gauss = 1;

    if (grey) {
        cvtColor(img_src, img_dst, CV_RGB2GRAY);
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (hsv) {
        cvtColor(img_src, img_dst, CV_BGR2HSV);
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (blur_r) {
        int sigma = 2;
        int ksize = (sigma * 5) | 1;
        blur(img_src, img_dst, Size(ksize, ksize));
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (sobel) {
        Mat img_1, img_2;
        Sobel(img_src, img_1, -1, 1, 0);
        Sobel(img_src, img_2, -1, 0, 1);
        img_dst = abs(img_1) + abs(img_2);
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (canny) {												//wykrywanie krawedzi
        Canny(img_src, img_dst, 100, 200);
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (szum) {
        img_dst = img_src.clone();
        double noise_percentage = 10.0;
        int img_rows = img_dst.rows;
        int img_columns = img_dst.cols;
        int img_channels = img_dst.channels();
        int noise_points = (int)(((double)img_rows * img_columns * img_channels) * noise_percentage / 100.0);

        for (int count = 0; count < noise_points; count++) {
            int row = rand() % img_rows;
            int column = rand() % img_columns;
            int channel = rand() % img_channels;
            uchar* pixel = img_dst.ptr(row) + (column * img_channels) + channel;
            *pixel = (rand() % 2 == 1) ? 255 : 0;
        }
        imwrite(file_out_name, img_dst, compression_params);
    }
    if (gauss) {
        for (int i = 1; i < 10; i++) {
            img_dst = img_src.clone();
            GaussianBlur(img_src, img_dst, Size(2 * i + 1, 2 * i + 1), 1.5);
            imwrite(file_out_name, img_dst, compression_params);
        }
    }

    return 0;
}

#else

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    VideoPlayer window;
    window.show();
    return app.exec();
}

#endif