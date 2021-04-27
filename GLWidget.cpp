#include "GLWidget.h"

#include <QPainter>
#include <QTimer>
#include <QPainter>
#include <QPaintEvent>
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>

// from: https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
std::string type2str(int type) {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (type >> CV_CN_SHIFT);

    switch (depth) {
        case CV_8U:
            r = "8U";
            break;
        case CV_8S:
            r = "8S";
            break;
        case CV_16U:
            r = "16U";
            break;
        case CV_16S:
            r = "16S";
            break;
        case CV_32S:
            r = "32S";
            break;
        case CV_32F:
            r = "32F";
            break;
        case CV_64F:
            r = "64F";
            break;
        default:
            r = "User";
            break;
    }

    r += "C";
    r += (chans + '0');

    return r;
}

QImage *mat2Image(cv::Mat &mat) {

    // todo We should act differently for different cv::Mat formats

    auto type = mat.type();

    std::clog << "Matrix type: " << type2str(type) << std::endl;

    const unsigned size = mat.rows * mat.cols * mat.channels();
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

    auto *image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_RGB888);
    return image;
}

GLWidget::GLWidget(QWidget *parent)
        : QOpenGLWidget(parent) {

    effects_.fill(nullptr);
    elapsed_ = 0;
    setFixedSize(parent->width(), parent->height());
    std::string input_file_path = "jpg.jpg";
    change_image(input_file_path);
    // That should make it resizable
    // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GLWidget::animate() {
    elapsed_ = (elapsed_ + qobject_cast<QTimer *>(sender())->interval()) % 1000;
    update();
}

void GLWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::LosslessImageRendering);
    /// Paint on widget from QImage
    auto paint = [this](QPainter *painter, QPaintEvent *event, int elapsed) {
        // todo that should be done only once not on every render
        std::lock_guard<std::mutex> lock(image_mutex_);
        QPixmap pixmap = QPixmap::fromImage(*this->image_);
        pixmap.scaled(1000, 1000);
        painter->drawPixmap(QPoint(0, 0), pixmap);
    };

    paint(&painter, event, elapsed_);
    painter.end();
}

void GLWidget::change_image(const std::string &path) {
    cv::Mat input_image(0, 0, CV_8UC3);
    input_image = cv::imread(path);
    {
        std::lock_guard<std::mutex> lock(mat_mutex_);
        current_image_ = input_image.clone();
    }
    std::lock_guard<std::mutex> lock(image_mutex_);
    // todo this delete segfaults
    //delete image_;
    image_ = mat2Image(input_image);
}

void GLWidget::apply_effects(cv::Mat frame) {
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        for (Effect* effect : effects_) {
            if (effect != nullptr) {
                effect->operator()(frame);
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(image_mutex_);
        image_ = mat2Image(frame);
    }
}

void GLWidget::change_effect(int idx, Effect *new_effect) {
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        // todo we are leaking memory here
        effects_.at(idx) = new_effect;
    }
    apply_effects(current_image_);
}
