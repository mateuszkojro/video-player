//
// Created by studio25 on 08.06.2021.
//

#ifndef VIDEO_PLAYER_VIDEOCONVERT_H
#define VIDEO_PLAYER_VIDEOCONVERT_H

#include "VideoPlayback.h"

#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>



class VideoConvert : public VideoPlayback {
public:
    VideoConvert() : VideoPlayback() {

    };

    std::string file_path_;

    cv::VideoWriter *oVideoWriter;

protected:

public:
    void setFilePath(const std::string &filePath) {
        file_path_ = filePath;
    }

protected:
    void add_effect() override {
        /// we .clone() frames anyway so we dont need to use move
        /// if i'm not mistaken ofc
        cv::Mat *temp_frame;


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

        /// save to file
        oVideoWriter->write(*temp_frame);
        delete temp_frame;
    }

public:
    bool change_file(const std::string &path) {

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

        int frame_width = video_capture_->get(cv::CAP_PROP_FRAME_WIDTH);
        int frame_height = video_capture_->get(cv::CAP_PROP_FRAME_HEIGHT);
        int codec = cv::VideoWriter::fourcc('M', 'J', 'P', 'G');

       oVideoWriter = new  cv::VideoWriter("out.avi", codec, 10, cv::Size(frame_width, frame_height), true);


        // video_capture_->set(cv::VIDEO_ACCELERATION_ANY)
        /// clear frame counter
        current_completed_frame_ = 0;


        /// reboot threads
        read_thread_ = new std::thread(&VideoPlayback::th_frame_reader, this);
        effect_thread_ = new std::thread(&VideoPlayback::th_effect_adder, this);
        last_error = "Video stream online";
        return true;
    }

};


#endif //VIDEO_PLAYER_VIDEOCONVERT_H
