//
// Created by mateu on 5/10/2021.
//

#include <thread>
#include "VideoPlayback.h"

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
            if (raw_frames_.size() > 30 * 10) std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        /// if  read next frame returns false, the file ended
        /// so we
        if (!read_next_frame()) return;

    }
}
