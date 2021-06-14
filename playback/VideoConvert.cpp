//
// Created by studio25 on 08.06.2021.
//

#include <iostream>
#include "VideoConvert.h"

std::string VideoConvert::last_error;

bool VideoConvert::complete_frame() {
    /// read next frame
    cv::Mat *input_buffer = new cv::Mat;
    {
        // std::lock_guard<std::mutex> lock(video_capture_mutex_);
        *video_capture_ >> *input_buffer;

    }
    /// check whether file is valid
    if (input_buffer->empty()) {

        last_error = "The video file is malformed";
        return false;
    }


    /// analyze it using effect table
    {
        std::lock_guard<std::mutex> lock(effects_mutex_);
        for (Effect *effect : effects_) {
            if (effect != nullptr) {
                effect->operator()(*input_buffer);
            }
        }
    }

    /// save to file
    oVideoWriter->write(*input_buffer);
    delete input_buffer;
    return true;
}

void VideoConvert::th_main_loop() {
    while (2 > 1) {

        if (!complete_frame() || disable_a_thread_) break;

    }
}

bool VideoConvert::change_file(const std::string &path) {
    close();
    if(path != "none")
        source_file_path_ = path;


    video_capture_ = new cv::VideoCapture(path);

    video_capture_->set(cv::CAP_PROP_FPS, 30);

    assert(video_capture_->isOpened());

    if (!video_capture_->isOpened()) {
        last_error = "The video file is malformed";
        return false;
    }

    int frame_width = video_capture_->get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = video_capture_->get(cv::CAP_PROP_FRAME_HEIGHT);

    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    std::cout<<destination_file_path_<<" <destin\n";
    oVideoWriter = new cv::VideoWriter(destination_file_path_, codec, video_capture_->get(cv::CAP_PROP_FPS), cv::Size(frame_width, frame_height), true);

    last_error = "Video stream online";


    analyze_thread = new std::thread(&VideoConvert::th_main_loop, this);
    return true;
}

bool VideoConvert::change_camera() {
    close();
    video_capture_ = new cv::VideoCapture(0);

    video_capture_->set(cv::CAP_PROP_FPS, 30);



    if (!video_capture_->isOpened()) {
        assert(false);
        last_error = "The video file is malformed";
        return false;
    }

    int frame_width = video_capture_->get(cv::CAP_PROP_FRAME_WIDTH);
    int frame_height = video_capture_->get(cv::CAP_PROP_FRAME_HEIGHT);

    int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

    oVideoWriter = new cv::VideoWriter(destination_file_path_, codec, video_capture_->get(cv::CAP_PROP_FPS), cv::Size(frame_width, frame_height), true);

    last_error = "Video stream online";

    analyze_thread = new std::thread(&VideoConvert::th_main_loop, this);
    return true;
}

void VideoConvert::close() {

    if (analyze_thread != nullptr) {
        /// tell him to shut off
        disable_a_thread_ = true;
        /// and join thread
        analyze_thread->join();
        /// clean up, probably not necessary but hey
        delete analyze_thread;
    }
    disable_a_thread_ = false;

    delete video_capture_;
    delete oVideoWriter;

    last_error = "Video stream offline";

}
