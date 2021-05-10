//
// Created by mateu on 5/10/2021.
//

#ifndef VIDEO_PLAYER_VIDEOPLAYBACK_H
#define VIDEO_PLAYER_VIDEOPLAYBACK_H

#include <opencv2/core.hpp>
#include <queue>

class VideoPlayback {
public:
    VideoPlayback(const std::string &path);

    /// @brief Get the next frame in the queue
    /// @return
    cv::Mat next_frame();


    /// @brief Jump to specified frame clean queue (maybe not all of it if we want to be fast)
    /// @param index the frame to skip to
    /// @return
    cv::Mat operator()(int index);

    /// @brief Index of the current frame
    /// @return
    unsigned current_frame();



private:

    std::queue<cv::Mat> queue_;

};


#endif //VIDEO_PLAYER_VIDEOPLAYBACK_H
