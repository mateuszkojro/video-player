//
// Created by mateu on 5/10/2021.
//

#include <thread>
#include "VideoPlayback.h"

static QImage *mat2Image(cv::Mat &mat);

void VideoPlayback::change_file(const std::string &path) {
    {
        std::lock_guard<std::mutex> lock(video_capture_mutex_);
        delete video_capture_;

        video_capture_ = new cv::VideoCapture(path);
        assert(video_capture_->isOpened());
    }

    read_thread = new std::thread(&VideoPlayback::th_frame_reader, this);
    effect_thread = new std::thread(&VideoPlayback::th_effect_adder, this);

}

bool VideoPlayback::read_next_frame() {

    cv::Mat one_frame_buffor;
    {
        std::lock_guard<std::mutex> lock(video_capture_mutex_);
        (*video_capture_) >> one_frame_buffor;

    }
    if (one_frame_buffor.empty()) {

        // video_capture_->set(cv::CAP_PROP_POS_FRAMES, 0);???
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
        /// so we fill war_frames with nullptrs than we quit thread
        if (!read_next_frame()) {
            std::lock_guard<std::mutex> lock(raw_frames_mutex_);
            for (int i = 0; i < 5; ++i)raw_frames_.push(nullptr);
            return;
        }

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

        raw_frames_.pop();
        assert(raw_frames_.size() == assert_size - 1);

    }
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        for (Effect *effect : effects_) {
            if (effect != nullptr) {
                effect->operator()(temp_frame);
            }
        }
    }
    {
        std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
        analyzed_frames_.push(new QPixmap(QPixmap::fromImage(*mat2Image(temp_frame))));

    }

}

void VideoPlayback::th_effect_adder() {
    while (2 > 1) {
        {
            bool wait_for_frames = false;
            {
                std::lock_guard<std::mutex> lock(raw_frames_mutex_);
                /// if pushed frames are nullptrs that means wideo capture closed
                /// so we shut down effect adding thread as well
                if (raw_frames_.front() == nullptr) return;
                /// if there are no incoming frames we wait
                if (raw_frames_.empty()) wait_for_frames = true;
                else wait_for_frames = false;
            }

            if (wait_for_frames) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
                continue;
            }
            add_effect();
        }
    }
}

QPixmap &VideoPlayback::next_frame() {

    bool wait_for_frames = false;
    do {
        {
            std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
            if (analyzed_frames_.empty()) wait_for_frames = true;
        }
        // here we will probably spend the most of the time, because if i'm not mistaken
        // here we wait if lag accrues
        if (wait_for_frames) std::this_thread::sleep_for(std::chrono::milliseconds(16));

    } while (wait_for_frames);
    //todo std::move? here but i'll play safe for now
    QPixmap temp = *analyzed_frames_.front();
    analyzed_frames_.pop();
    current_completed_frame++;
    return temp;
}

void VideoPlayback::change_effect(int index, Effect *effect) {

    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        effects_[index] = effect;
    }

    int number_of_loaded_frames = 0;
    {
        std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
        number_of_loaded_frames += analyzed_frames_.size();
        while (!analyzed_frames_.empty()) analyzed_frames_.pop();
    }
    {
        std::lock_guard<std::mutex> lock(raw_frames_mutex_);
        number_of_loaded_frames += raw_frames_.size();
        while (!raw_frames_.empty()) raw_frames_.pop();
    }
    //void buck_up_reading(number_of_loaded_frames);
}

VideoPlayback::VideoPlayback() {
    video_capture_ = nullptr;
    raw_frames_ = {};
    analyzed_frames_ = {};
    effects_.fill(nullptr);

}

unsigned VideoPlayback::current_frame() {
    return current_completed_frame;
}

void VideoPlayback::buck_up_reading(int number_of_frames) {
   // video_capture_->set(CV_CAP_PROP_POS_MSEC, number_of_frames);
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