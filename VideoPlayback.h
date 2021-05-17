//
// Created by mateu on 5/10/2021.
//

#ifndef VIDEO_PLAYER_VIDEOPLAYBACK_H
#define VIDEO_PLAYER_VIDEOPLAYBACK_H

#include <opencv2/core.hpp>
#include <QPixmap>
#include <queue>
#include "Effect.h"


class VideoPlayback {
public:
    VideoPlayback(const std::string &path);

    /// @brief Get the next frame in the queue
    /// @return
    const QPixmap &next_frame();

    // but like yt  so we need to clean our buffor
    // move in file in reverse
    // read all of the frames again
    void change_effect(int index, Effect *effect);


    // clean buffor
    // open new file
    // read new file
    void change_file(const std::string &path);


    /// @brief Jump to specified frame clean queue (maybe not all of it if we want to be fast)
    /// @param index the frame to skip to
    /// @return
    void change_position(int index);

    void skip_10s();

    void back_10s();

    /// @brief Index of the current frame
    /// @return
    unsigned current_frame();

    // in frames i guess
    unsigned file_size();

    /// Later poza kolejka
    cv::Mat get_frame_raw(int index);

};


#endif //VIDEO_PLAYER_VIDEOPLAYBACK_H
