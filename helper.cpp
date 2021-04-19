#include "helper.h"

#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <iostream>
#include <climits>

Helper::Helper() {
    width_ = 200;
    height_ = 200;
    buffer = new uchar[size() * 4];
    for (int i = 0; i < size() * 4; i++) {
        buffer[i] = 255;
    }
    image_ = new QImage(buffer, width_, height_, QImage::Format_ARGB32);
}

void Helper::paint(QPainter *painter, QPaintEvent *event, int elapsed) {
    image_->load("C:\\Users\\mateu\\Documents\\work\\video_player\\cmake-build-debug-mingw\\file.png");
    // image_->loadFromData(buffer, size() * 4); // <-- this does not work
    if (image_->isNull()){
        std::clog << "Image is for some reason null" << std::endl;
        exit(1);
    }
    *image_ = image_->scaledToHeight(height_);
    image_->scaledToHeight(width_);
//    std::clog << image_->width() << "x" << image_->height() << std::endl;
    painter->drawImage(QRect(0, 0, image_->width(), image_->height()), *image_);
}
