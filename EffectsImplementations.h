//
// Created by mateu on 4/27/2021.
//

#ifndef VIDEO_PLAYER_EFFECTSIMPLEMENTATIONS_H
#define VIDEO_PLAYER_EFFECTSIMPLEMENTATIONS_H

#include "Effect.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core.hpp>

class GrayScaleEffect : public Effect {
public:
    void operator()(cv::Mat &frame) override {
//        auto source = frame;
        cv::Mat img_src = frame.clone(), img_dst;
        cvtColor(img_src, img_dst, CV_RGB2GRAY);
        frame = img_dst.clone();
    }
};

#endif //VIDEO_PLAYER_EFFECTSIMPLEMENTATIONS_H
