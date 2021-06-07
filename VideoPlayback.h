//
// Created by mateu on 5/10/2021.
// but written by piotr233 @piotr233
//

#ifndef VIDEO_PLAYER_VIDEOPLAYBACK_H
#define VIDEO_PLAYER_VIDEOPLAYBACK_H

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <QPixmap>
#include <queue>
#include <thread>
#include "Effect.h"




class VideoPlayback {
    std::mutex video_capture_mutex_;
    cv::VideoCapture *video_capture_;

    std::mutex effects_mutex_;
    std::array<Effect *, 8> effects_;

    std::mutex raw_frames_mutex_;
    std::queue<cv::Mat *> raw_frames_;


    std::mutex analyzed_frames_mutex_;
    std::queue<QPixmap *> analyzed_frames_;

    unsigned current_completed_frame_ = 0;

    bool video_source;

    /// last action that has occurred
    /// list of already defined
    /// <ol>
    /// <li> "The video file is malformed"
    /// <li> "Adding effect failed"
    /// <li> "Video stream offline"
    /// <li> "Bad format"
    /// <li> "Video stream online"
    /// </ol>
    static std::string last_error;


    /// disable thread booleans
    /// if set true threads will turn inactive
    /// if false they remain active
    bool disable_r_thread_;

    std::thread *read_thread_;

    /// disable thread booleans
    /// if set true threads will turn inactive
    /// if false they remain active
    bool disable_e_thread_;

    std::thread *effect_thread_;

    bool read_next_frame();

    void add_effect();

    void buck_up_reading(int number_of_frames);


public:


    VideoPlayback();

    void th_frame_reader();

    void th_effect_adder();


    /// @brief Get the next frame in the queue
    /// @return
    QPixmap *next_frame();

    // but like yt  so we need to clean our buffor
    // move in file in reverse
    // read all of the frames again
    void change_effect(int index, Effect *effect);


    // clean buffor
    // open new file
    // read new file
    void change_file(const std::string &path);

    void redirect_input_from_camera();


    static std::string get_last_error();

    ~VideoPlayback() {
        close();
    }

    /// @brief Jump to specified frame clean queue (maybe not all of it if we want to be fast)
    /// @param index the frame to skip to
    /// @return
    void change_position(int index);

    void skip_10s();

    void back_10s();

    /// @brief Index of the current frame
    /// @return
    unsigned current_frame();

    /// \brief a.k.a. shut me up<br>
    /// closes every thread opened for VideoPlayback purposes<br>
    /// cleans whole data stream
    void close();

    // in frames i guess
    unsigned file_size();

    /// Later poza kolejka
    cv::Mat get_frame_raw(int index);

};


#endif //VIDEO_PLAYER_VIDEOPLAYBACK_H
