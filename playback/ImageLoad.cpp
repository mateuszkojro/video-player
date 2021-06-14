//
// Created by studio25 on 14.06.2021.
//

#include <opencv2/imgcodecs.hpp>
#include <iostream>
#include "ImageLoad.h"

std::string ImageLoad::last_error = "Path not specified";

static QImage *mat2Image(cv::Mat &mat);

void ImageLoad::change_effect(int index, Effect *effect) {

        if (index >= effects_.size()) {
            last_error = "Adding effect failed";
            return;
        }
        effects_[index] = effect;
    add_effect();
}

bool ImageLoad::change_file(const std::string &path) {
    raw_image = cv::imread(path);
    add_effect();
    return true;
}

QPixmap *ImageLoad::get_frame() {
    return analyzed_image;
}

void ImageLoad::add_effect() {
    auto temp_image = raw_image.clone();
    for (Effect *effect : effects_) {
        if (effect != nullptr) {
            effect->operator()(temp_image);
        }
    }

    QImage *temp_frame = mat2Image(temp_image);
    analyzed_image = new QPixmap(QPixmap::fromImage(*temp_frame));

}


static QImage *mat2Image(cv::Mat &mat) {

    int type = mat.type();

//    std::clog << "Matrix type: " << type2str(type) << std::endl;

    const unsigned size = mat.rows * mat.cols * mat.channels();

    // todo there is the famous memory leak

    auto now = std::chrono::high_resolution_clock::now();

    auto buffer = new uchar[size];

    int i = 0;



    // OpenCV Mat gives us access only to beginnings of the rows sow we need to
    // go around that
    for (int r = 0; r < mat.rows; ++r) {
        uchar *ptr = mat.ptr(r, 0);
        uchar *ptr_end = ptr + ((mat.cols) * mat.channels()); // I dont wanna know why that is but it is
        for (; ptr != ptr_end; ++ptr) {
            buffer[i++] = *ptr;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "mat2im: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count() << "(ms)"
              << std::endl;

    auto cleanup_func = [](void *ptr) {
        std::cout << "usuwam image" << std::endl;
        delete (uchar *) ptr;
    };

    QImage *image;
    switch (type) {
        case CV_8UC3: {
            image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_BGR888, cleanup_func, buffer);

            break;
        }
        case CV_8UC1: {
            image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_Grayscale8, cleanup_func, buffer);

            break;
        }
        default:
            std::cout << "format not suported" << std::endl;
            return nullptr;
    }

    return image;
}