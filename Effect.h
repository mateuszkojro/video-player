//
// Created by mateu on 4/27/2021.
//

#ifndef VIDEO_PLAYER_EFFECT_H
#define VIDEO_PLAYER_EFFECT_H

#include <opencv2/core.hpp>

class Effect {
public:
    virtual void operator()(const cv::Mat &frame) = 0;
};

#endif //VIDEO_PLAYER_EFFECT_H
