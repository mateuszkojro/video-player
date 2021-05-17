//
// Created by mateu on 5/10/2021.
//

#include <thread>
#include "VideoPlayback.h"
static QImage *mat2Image(cv::Mat &mat);
void VideoPlayback::change_file(const std::string &path) {

    delete video_capture_;
    video_capture_ = new cv::VideoCapture(path);
    std::thread(th_frame_reader);
}

bool VideoPlayback::read_next_frame() {

    cv::Mat one_frame_buffor;
    {
        std::lock_guard<std::mutex> lock(video_capture_mutex_);
        (*video_capture_) >> one_frame_buffor;

    }
    if (one_frame_buffor.empty()) {

        video_capture_->set(cv::CAP_PROP_POS_FRAMES, 0);
        /// if frame is empty return false
        /// it stops everything
        return false;
    }

    std::lock_guard<std::mutex> lock(raw_frames_mutex_);
    raw_frames_.push(new cv::Mat(one_frame_buffor));
    return true;
}

void VideoPlayback::th_frame_reader() {
    while (2 > 1) {
        {
            std::lock_guard<std::mutex> lock(raw_frames_mutex_);
            /// if the buffor is filled with next 300 frames that is 10s video 30fps
            /// wait a bit, stop, get some help
            if (raw_frames_.size() > 30 * 10) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
        }

        /// if  read next frame returns false, the file ended
        /// so we
        if (!read_next_frame()) return;

    }
}

void VideoPlayback::add_effect() {

    // we .clone() frames anyway so we dont need to use move
    // if i'm not mistaken ofc
    cv::Mat temp_frame;
    {

        std::lock_guard<std::mutex> lock(raw_frames_mutex_);

        int assert_size = raw_frames_.size();

        temp_frame = raw_frames_.front()->clone();

        assert(raw_frames_.size() == assert_size - 1);

    }

    std::lock_guard<std::mutex> lock(effects_mutex_);
    for (Effect *effect : effects_) {
        if (effect != nullptr) {
            effect->operator()(temp_frame);
        }
    }

    {
        std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
        analyzed_frames_.push(mat2Image(temp_frame));

    }


}

void VideoPlayback::th_effect_adder() {
while(2>1){
    {
        std::lock_guard<std::mutex> lock(raw_frames_mutex_);
        /// if the buffor is filled with next 300 frames that is 10s video 30fps
        /// wait a bit, stop, get some help
        if (raw_frames_.size() <=0 ){

            std::this_thread::sleep_for(std::chrono::seconds(1));
            continue;
        }
        add_effect();
    }


}


}

static QImage *mat2Image(cv::Mat &mat) {

    int type = mat.type();

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
            break;
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