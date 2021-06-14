//
// Created by studio25 on 08.06.2021.
//

#ifndef VIDEO_PLAYER_VIDEOCONVERT_H
#define VIDEO_PLAYER_VIDEOCONVERT_H

#include "VideoPlayback.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class VideoConvert {
protected:

    enum playback_source {
        file,
        camera
    };

    std::mutex video_capture_mutex_;
    cv::VideoCapture *video_capture_;

    std::mutex effects_mutex_;
    std::array<Effect *, 8> effects_;

    std::string source_file_path_;

    std::string destination_file_path_;

    cv::VideoWriter *oVideoWriter;

    playback_source video_source_;

    std::thread *analyze_thread;

    bool disable_a_thread_;

public:
    void close();


    void setSourceFilePath(const std::string &sourceFilePath) {
        source_file_path_ = sourceFilePath;
    }

    void setDestinationFilePath(const std::string &destinationFilePath) {
        destination_file_path_ = destinationFilePath;
    }

    static std::string last_error;

    VideoConvert() {
        video_capture_ = nullptr;
        oVideoWriter = nullptr;
        disable_a_thread_ = true;
        analyze_thread = nullptr;
        source_file_path_ = "";
        destination_file_path_ = "";
        effects_.fill(nullptr);
    };
protected:


    bool complete_frame();

public:

    void th_main_loop();

    bool change_file(const std::string &path = "none");

    bool change_camera();

    ~VideoConvert() {
        close();
    }
};


#endif //VIDEO_PLAYER_VIDEOCONVERT_H
