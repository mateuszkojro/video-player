#include "helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

QImage *mat2Image(cv::Mat &mat) {
    const unsigned size = mat.rows * (mat.cols + 1) * 3;
    auto buffer = new uchar[size];

    int i = 0;

    // OpenCV Mat gives us access only to beginnings of the rows sow we need to
    // go around that
    for (int r = 0; r < mat.rows; ++r) {
        uchar *ptr = mat.ptr(r, 0);
        uchar *ptr_end = ptr + (int) ((mat.cols + 1) * 3); // I dont wanna know why that is but it is
        for (; ptr != ptr_end; ++ptr) {
            buffer[i++] = *ptr;
        }
    }

    auto *image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_RGB888);
    return image;
}

Helper::Helper() {
    std::string file_in_name = "jpg.jpg";
    cv::Mat img_src(0, 0, CV_8UC3);
    img_src = cv::imread(file_in_name);
    image_ = mat2Image(img_src);
}

void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed) {
    QPixmap pixmap = QPixmap::fromImage(*image_);
    pixmap.scaled(1000, 1000);
    painter->drawPixmap(QPoint(0, 0), pixmap);
}
