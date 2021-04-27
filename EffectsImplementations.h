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

/// apply gray scale effect
class GrayScaleEffect : public Effect {
public:
    /// \return the matrix under frame will be overwritten with it's gray scaled image
    /// \param frame the pixel matrix that will be replaced with it's gray scaled form
    /// \note the type of pixel inside frame will be changed to 8UC1
    void operator()(cv::Mat &frame) override {
// todo test if it changes 8UC1 to 8UC3
 // coz it's easy to do down but not to go up
    //    cv::Mat img_src = frame.clone();
        cv::cvtColor(frame, frame, CV_RGB2GRAY); // heavy optimisations

    }
};
/// apply HSV effect
class HSVEffect : public Effect {
public:
    /// \return the matrix under frame will be overwritten with it's HSV scaled image
    /// \param frame the pixel matrix that will be replaced with it's HSV scaled form
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3
    void operator()(cv::Mat &frame) override {

      //  cv::Mat frame_copy = frame.clone();
        cvtColor(frame, frame, CV_BGR2HSV); // heavy optimisations

    }
};
/// apply Sobel effect
class SobelEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3
    void operator()(cv::Mat &frame) override {

        cv::Mat img_1, img_2;
        Sobel(frame, img_1, -1, 1, 0);
        Sobel(frame, img_2, -1, 0, 1);
        frame = abs(img_1) + abs(img_2);

    }
};


#endif //VIDEO_PLAYER_EFFECTSIMPLEMENTATIONS_H
