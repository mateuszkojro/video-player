//
// Created by studio25 on 14.06.2021.
//

#ifndef VIDEO_PLAYER_IMAGELOAD_H
#define VIDEO_PLAYER_IMAGELOAD_H


#include <mutex>
#include <opencv2/core/mat.hpp>
#include <QPixmap>
#include "../Effect.h"

class ImageLoad {

protected:
    std::array<Effect *, 8> effects_;

    cv::Mat raw_image;
    QPixmap * analyzed_image;

    void add_effect();


public:

    static std::string last_error;

    // but like yt  so we need to clean our buffer
    // move in file in reverse
    // read all of the frames again
    void change_effect(int index, Effect *effect);

    // clean buffer
    // open new file
    // read new file
    bool change_file(const std::string &path);

    /// @brief Get the analyzed frame
    /// @return
    QPixmap *get_frame();

};


#endif //VIDEO_PLAYER_IMAGELOAD_H
