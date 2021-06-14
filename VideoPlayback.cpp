//
// Created by mateu on 5/10/2021.
//

#include <thread>
#include <iostream>
#include "VideoPlayback.h"

std::string VideoPlayback::last_error = "Video stream offline";

static QImage *mat2Image(cv::Mat &mat);

void VideoPlayback::change_camera() {

    close();

    video_capture_ = new cv::VideoCapture(0);


    assert(video_capture_->isOpened());

    if (!video_capture_->isOpened()) {
        last_error = "The camera capture is malformed";
        return;
    }

    /// clear frame counter
    current_completed_frame_ = 0;


    /// reboot threads
    read_thread_ = new std::thread(&VideoPlayback::th_frame_reader, this);
    effect_thread_ = new std::thread(&VideoPlayback::th_effect_adder, this);
    last_error = "Video stream online";

}

bool VideoPlayback::change_file(const std::string &path) {
    close();

      video_capture_ = new cv::VideoCapture(path);

    video_capture_->set(cv::CAP_PROP_FPS,30);
    /// look into it:
    //video_capture_ = new cv::VideoCapture(path,cv::VIDEO_ACCELERATION_ANY);
    assert(video_capture_->isOpened());

    if (!video_capture_->isOpened()) {
        last_error = "The video file is malformed";
        return false;
    }
    // video_capture_->set(cv::VIDEO_ACCELERATION_ANY)
    /// clear frame counter
    current_completed_frame_ = 0;


    /// reboot threads
    read_thread_ = new std::thread(&VideoPlayback::th_frame_reader, this);
    effect_thread_ = new std::thread(&VideoPlayback::th_effect_adder, this);
    last_error = "Video stream online";
    return true;
}

bool VideoPlayback::read_next_frame() {
    cv::Mat *input_buffer = new cv::Mat;
    {
        // std::lock_guard<std::mutex> lock(video_capture_mutex_);
        *video_capture_ >> *input_buffer;

    }
    if (input_buffer->empty()) {
        raw_frames_.push(nullptr);
        last_error = "The video file is malformed";
        // video_capture_->set(cv::CAP_PROP_POS_FRAMES, 0);???
        /// if frame is empty, function returns false
        /// it will stop thread
        return false;
    }

    //std::lock_guard<std::mutex> lock(raw_frames_mutex_);
    raw_frames_.push(input_buffer);

    return true;
}

void VideoPlayback::th_frame_reader() {

    while (2 > 1) {
        /// check whether the thread should be stopped
        if (disable_r_thread_) return;
        {
            //     std::lock_guard<std::mutex> lock(raw_frames_mutex_);
            /// if the buffer is filled with next 300 frames that is 10s video 30fps
            /// wait a bit, stop, get some help
            if (raw_frames_.size() > 50) {

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                continue;
            }
        }

        /// if  read next frame returns false, the file ended
        if (!read_next_frame()) return;
    }

}

void VideoPlayback::add_effect() {
    /// we .clone() frames anyway so we dont need to use move
    /// if i'm not mistaken ofc
    cv::Mat* temp_frame ;

    /// check if raw_frames_.front() is nullptr
    if(!raw_frames_.front()){
        analyzed_frames_.push(nullptr);
        return ;
    }


    /// get raw frame form queue
    {
        // std::lock_guard<std::mutex> lock(raw_frames_mutex_);

        temp_frame = raw_frames_.front();
        //  raw_frames_.front()->deallocate();
        //  delete raw_frames_.front();
        raw_frames_.pop();

    }
    /// analyze it using effect table
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        for (Effect *effect : effects_) {
            if (effect != nullptr) {
                effect->operator()(*temp_frame);
            }
        }
    }

    /// place analyzed therefore changed frame on top of analyzed_queue
    {
        //  std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
        QImage *temp_image = mat2Image(*temp_frame);

        // todo Think about that
        if (!temp_image) {
            last_error = "Bad format";
            analyzed_frames_.push(nullptr);
            return;
        }
        analyzed_frames_.push(new QPixmap(QPixmap::fromImage(*temp_image)));
        delete temp_image;
//        delete temp_frame;
    }

}

void VideoPlayback::th_effect_adder() {
    while (2 > 1) {
        {
            /// check whether we need to stop thread
            if (disable_e_thread_) return;
            /// we declare
            bool wait_for_frames = false;
            {
                //  std::lock_guard<std::mutex> lock(raw_frames_mutex_);

                /// if there are no incoming frames we wait
                if (raw_frames_.empty()) wait_for_frames = true;
                else wait_for_frames = false;
            }

            if (wait_for_frames) {
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                /// if we waited there's possibility that we need to wait some more,
                /// so we skip add_effect part
                continue;
            }

            add_effect();
        }
    }
}

QPixmap *VideoPlayback::next_frame() {
    /// let's check whether threads are running
    assert(!disable_e_thread_);
    assert(!disable_r_thread_);

    bool wait_for_frames = false;
    do {
        {
            //   std::lock_guard<std::mutex> lock(analyzed_frames_mutex_);
            /// check whether we should wait for frames
            if (analyzed_frames_.empty()) wait_for_frames = true;
            else wait_for_frames = false;
        }
        /// here we will probably spend the most of the time
        /// here we wait if lag accrues
        if (wait_for_frames)
            std::this_thread::sleep_for(std::chrono::milliseconds(32)); /// 16 milliseconds == 1/60 s

    } while (wait_for_frames);

    /// we need to make copy here, well we could simply pass the pointer
    /// but in that case we would need rewrite some code,
    /// so for now it's a copy and fixme next_frame should return QPixmap*
    /// for now i leave it as is

    auto temp = analyzed_frames_.front();
    /// delete frame from
    analyzed_frames_.pop();

    current_completed_frame_++;
    return temp;
}

void VideoPlayback::change_effect(int index, Effect *effect) {

    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        if (index >= effects_.size()) {
            last_error = "Adding effect failed";
            return;
        }
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
    video_source_ = file;
    video_capture_ = nullptr;
    raw_frames_ = {};
    analyzed_frames_ = {};
    effects_.fill(nullptr);
    disable_r_thread_ = true;
    disable_e_thread_ = true;
    read_thread_ = nullptr;
    effect_thread_ = nullptr;
}

unsigned VideoPlayback::current_frame() {

    return current_completed_frame_;
}

void VideoPlayback::buck_up_reading(int number_of_frames) {
    // video_capture_->set(CV_CAP_PROP_POS_MSEC, number_of_frames);
}

std::string VideoPlayback::get_last_error() {
    return last_error;
}

void VideoPlayback::close() {

    /// if read thread is active
    if (read_thread_ != nullptr) {
        /// tell him to shut off
        disable_r_thread_ = true;
        /// and join thread
        read_thread_->join();
        /// clean up, probably not necessary but hey
        delete read_thread_;
    }
    /// as above so below
    if (effect_thread_ != nullptr) {
        disable_e_thread_ = true;
        effect_thread_->join();
        delete effect_thread_;
    }
    /// all of those steps are always made with threads offline, so we don't need locks

    /// for some reason queue does not have .clear() but I may be dumb
    /// this looks fine tho
    while (!raw_frames_.empty()) raw_frames_.pop();
    while (!analyzed_frames_.empty()) analyzed_frames_.pop();

    /// prepare threads to run again, but with different file,
    /// again we probably could skip whole "stop threads step" but kojro insisted
    disable_e_thread_ = false;
    disable_r_thread_ = false;

    /// delete last video handle
    /// i thing we could simply override the existing one,
    /// but this whole function will be run fev times in our video player live spam so there's no need to be fast here
    delete video_capture_;

    last_error = "Video stream offline";

}

bool VideoPlayback::change_position(int index) {
    if(video_source_ == camera) return false;
    last_error  = "input from camera does not support this operation";

    if (index < 0)
        index = 0;
    video_capture_->set(cv::CAP_PROP_POS_FRAMES, index);
    return true;
}

bool VideoPlayback::skip_10s() {
    if(video_source_ == camera) return false;
    last_error  = "input from camera does not support this operation";

    /// current_position is in milliseconds
    double current_position = video_capture_->get(cv::CAP_PROP_POS_MSEC);
    video_capture_->set(cv::CAP_PROP_POS_MSEC, current_position + 10 * 1000);


    /// if we went past the file
    /// snap back to the end
    /// but this is a guessing game
    if (video_capture_->get(cv::CAP_PROP_POS_AVI_RATIO) >= 1)
        video_capture_->set(cv::CAP_PROP_POS_AVI_RATIO, 1);

return true;
}

bool VideoPlayback::back_10s() {
    if(video_source_ == camera) return false;
    last_error  = "input from camera does not support this operation";
/// current_position is in milliseconds
    double current_position = video_capture_->get(cv::CAP_PROP_POS_MSEC);

    current_position -= 10 * 1000;

    if (current_position < 0)
        current_position = 0;

    video_capture_->set(cv::CAP_PROP_POS_MSEC, current_position);
    return true;
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

//    std::cout << "mat2im: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - now).count() << "(ms)"
//              << std::endl;

    auto cleanup_func = [](void *ptr) {
//        std::cout << "usuwam image" << std::endl;
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