#include "GLWidget.h"

#include <QPainter>
#include <QTimer>
#include <QPaintEvent>
#include <exception>
#include <stdexcept>
#include <QMessageBox>

#define EMPTY_IMAGE "empty.png"

// from: https://stackoverflow.com/questions/10167534/how-to-find-out-what-type-of-a-mat-object-is-with-mattype-in-opencv
std::string type2str(int type) {
    std::string r;

    uchar depth = type & CV_MAT_DEPTH_MASK;
    uchar chanels = 1 + (type >> CV_CN_SHIFT);

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
    r += (chanels + '0');

    return r;
}

static QImage *mat2Image(cv::Mat &mat) {

    auto type = mat.type();

//    std::clog << "Matrix type: " << type2str(type) << std::endl;

    const unsigned size = mat.rows * mat.cols * mat.channels();
    // todo there is the famous memory leak
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

    QImage *image;
    switch (type) {
        case CV_8UC3: {
            image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_BGR888);
            break;;
        }
        case CV_8UC1: {
            image = new QImage(buffer, mat.cols, mat.rows, QImage::Format_Grayscale8);
            break;
        }
        default:
            throw "cv::Mat format not supported";
    }

    return image;
}

void GLWidget::set_image(const std::string &path) {
#if NEW_PIPELINE
    current_mode_ = Mode::Image;
    cv::Mat input_image(0, 0, CV_8UC3);
    // todo - this may fail we need to inform about that
    input_image = cv::imread(path);
    QImage *image = mat2Image(input_image);
    change_current_pixmap(new QPixmap(QPixmap::fromImage(*image)));
#else
    current_mode_ = Mode::Image;

    cv::Mat input_image(0, 0, CV_8UC3);
    // todo - this may fail we need to inform about that
    input_image = cv::imread(path);
    {
        std::lock_guard<std::mutex> lock(in_mat_mutex_);
        input_matrix_ = input_image.clone();
        output_matrix_ = input_image.clone();
    }
    std::lock_guard<std::mutex> lock(image_mutex_);
    // todo this delete segfaults
    //delete image_;
    image_ = mat2Image(input_image);
#endif
}

GLWidget::GLWidget(QWidget *parent)
        : QOpenGLWidget(parent) {

#if NEW_PIPELINE

    playback_ = new VideoPlayback();
    current_pixmap_ = nullptr;

#else

    effects_.fill(nullptr);
#endif
    playing_ = true;

    setFixedSize(parent->width(), parent->height());

    set_image(EMPTY_IMAGE);
    // That should make it resizable
    // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GLWidget::animate() {
    elapsed_ = (elapsed_ + qobject_cast<QTimer *>(sender())->interval()) % 1000;
    update();
}

#if NEW_PIPELINE

void GLWidget::paint_image(QPainter *painter) {
    LOCK(current_pixmap_mutex_);
    painter->drawPixmap(QPoint(0, 0), *current_pixmap_);
}

#else

void GLWidget::paint_image(QPainter *painter) {

    // todo that should be done only once not on every render
    std::lock_guard<std::mutex> lock(image_mutex_);
    // todo here! the SIEGSEGV happens date: 27.04
    QPixmap pixmap = QPixmap::fromImage(*this->image_);
    pixmap = pixmap.scaled(width(), height());
    painter->drawPixmap(QPoint(0, 0), pixmap);

}

#endif

void GLWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::LosslessImageRendering);

#if NEW_PIPELINE
    if (current_mode_ == Mode::Video) {
        if (!playing_)
            return;
        QPixmap *frame = playback_->next_frame();
        if (frame) {
            *frame = frame->scaled(width(), height());
            change_current_pixmap(frame);
        } else {
            set_image(EMPTY_IMAGE);
            playback_->close();
            QMessageBox::warning(
                    this,
                    QString("Video error"),
                    QString(VideoPlayback::get_last_error().c_str()));
        }

    }
#else
    switch (current_mode_) {
        case Mode::Image: {
            break;
        }
        case Mode::Video: {
            // This needs to go out
            (*video_capture_) >> input_matrix_;
            output_matrix_ = input_matrix_.clone();
            if (!input_matrix_.empty()) {
                request_apply_effects(output_matrix_);
                std::lock_guard<std::mutex> lock(image_mutex_);
                delete image_;
//                input_matrix_ = frame.clone();
                this->image_ = mat2Image(output_matrix_);
            } else {
                video_capture_->set(cv::CAP_PROP_POS_FRAMES, 0);
            }
            break;
        }
    }
#endif
    paint_image(&painter);
    painter.end();
}

void GLWidget::request_change_file(const std::string &path, Mode mode) {
#if NEW_PIPELINE
    current_mode_ = mode;
    if (path != "") {
        switch (mode) {
            case Mode::Image:
                set_image(path);
                break;
            case Mode::Video:
                if (!playback_->change_file(path)) {
                    set_image(EMPTY_IMAGE);
                    QMessageBox::warning(
                            this,
                            QString("Video error"),
                            QString(VideoPlayback::get_last_error().c_str()));
                    playback_->close();
                }
                break;
            default:
                throw "Bad mode";
        }
    }

#else
    current_mode_ = mode;
    switch (mode) {
        case Mode::Image: {
            set_image(path);
            break;
        }
        case Mode::Video: {
            delete video_capture_;
            video_capture_ = new cv::VideoCapture(path);
        }
    }
#endif
}

#if !NEW_PIPELINE
void GLWidget::request_apply_effects(cv::Mat frame) {

    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        for (Effect *effect : effects_) {
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

#endif

void GLWidget::request_change_effect(int idx, Effect *effect) {
#if NEW_PIPELINE
    playback_->change_effect(idx, effect);
#else
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        delete effects_.at(idx);
        effects_.at(idx) = effect;
    }
    output_matrix_ = input_matrix_.clone();
    request_apply_effects(output_matrix_);
#endif
}
