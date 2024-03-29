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
#include <QWidget>

#include <utility>

#include "ScriptHandler.h"

#include <iostream>
#include <QWidget>

class LuaEffect : public Effect {
public:

    explicit LuaEffect(std::string path) : path_(std::move(path)) {}

    std::string path_;

    void operator()(cv::Mat &frame) override {
        ScriptHandler::update_current_frame(frame);
        if (ScriptHandler::run_script(path_) == ScriptHandler::Error) {
            auto mat = cv::imread("../example-scripts/black.jpg");
            ScriptHandler::update_current_frame(mat);
            ScriptHandler::update_error(ScriptHandler::get_last_error());
            ScriptHandler::run_script("../example-scripts/show_error.lua");
            frame = ScriptHandler::get_current_frame();
            return;
        }
        frame = ScriptHandler::get_current_frame();
    }

    ~LuaEffect() override = default;

    void show_error(const std::string &error);

};

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

    ~GrayScaleEffect() override = default;
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

    ~HSVEffect() override = default;
};

/// apply Sobel effect
class SobelEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3

    explicit SobelEffect(int value = 1)
            : scale(value) {};

    double scale;   //skalowanie obliczonych wartości pochodnych

    void operator()(cv::Mat &frame) override {

        cv::Mat img_1, img_2;
        Sobel(frame, img_1, -1, 1, 0, 3, scale);
        Sobel(frame, img_2, -1, 0, 1, 3, scale);
        frame = img_1 + img_2;
        //frame = abs(img_1) + abs(img_2);

    }

    ~SobelEffect() override = default;
};

/// apply Blur_r effect
class Blur_rEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3

    explicit Blur_rEffect(int value = 2)
            : sigma(value) {};

    int sigma;

    void operator()(cv::Mat &frame) override {

        //int sigma = 2;
        int KernelSize = (sigma * 5) | 1;
        blur(frame, frame, cv::Size(KernelSize, KernelSize));

    }
};

/// apply Canny effect
class CannyEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3
    void operator()(cv::Mat &frame) override {
        cv::Mat frame_copy = frame.clone();
        Canny(frame, frame_copy, 100, 200);
        frame = frame_copy.clone();
    }
};

/// apply Noise effect
class NoiseEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3
    explicit NoiseEffect(int value = 50)
            : noise_percentage(value) {};

    double noise_percentage;

    void operator()(cv::Mat &frame) override {

        int img_rows = frame.rows;
        int img_columns = frame.cols;
        int img_channels = frame.channels();
        int noise_points = (int) (((double) img_rows * img_columns * img_channels) * noise_percentage / 100.0);

        for (int count = 0; count < noise_points; count++) {

            int row = rand() % img_rows;
            int column = rand() % img_columns;
            int channel = rand() % img_channels;

            uchar *pixel = frame.ptr(row) + (column * img_channels) + channel;
            *pixel = (rand() % 2 == 1) ? 255 : 0;
        }

    }

};

/// apply GaussianBlur effect
class GaussianBlurEffect : public Effect {
public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3

    explicit GaussianBlurEffect(double value = 0.5)
            : n(value) {};

    double n;   //odchylenie standardowe gaussa w kierunku X

    void operator()(cv::Mat &frame) override {

        for (int i = 1; i < 10; i++) {
            GaussianBlur(frame, frame, cv::Size(2 * i + 1, 2 * i + 1), n);
        }
    }
};

/// apply Neon <3 effect
class NeonEffect : public Effect {
private:

    struct Color {
        Color(uchar r, uchar g, uchar b) : R_(r), G_(g), B_(b) {}

        uchar R_;
        uchar G_;
        uchar B_;
    };

    Color gen_rainbow(unsigned height, unsigned max_height) {
        // sine wave algorithm
        // 3 parts
        // 1 :
        // r = cos(i) , g = sin(i), b =0
        // 2 :
        // r = 0 , g = cos(i), b = sin(x)
        // 3:
        // r = sin(i) , g = 0, b = cos(x)
        // every part is max_height / 3 translates into 0, PI/2

        // so for example in point 1/3 * max_height
        // r = cos(PI/2) = 0, g = sin(PI/2) = 1, b = 0

        char witch_third = height / (max_height / 3);

        double height_in_radians;
        switch (witch_third) {
            case 0:
                height_in_radians = height * M_PI / (max_height / 3) / 2;

                return Color(cos(height_in_radians) * 255, sin(height_in_radians) * 255, 0);
            case 1:
                height -= max_height / 3;
                height_in_radians = height * M_PI / (max_height / 3) / 2;
                return Color(0, cos(height_in_radians) * 255, sin(height_in_radians) * 255);

            case 2:
                height -= 2 * max_height / 3;
                height_in_radians = height * M_PI / (max_height / 3) / 2;
                return Color(sin(height_in_radians) * 255, 0, cos(height_in_radians) * 255);

        }

        return {255, 0, 0};
    }


    void apply_color(int x, int y, cv::Mat &frame) {

        frame.at<cv::Vec3b>(x, y)[0] = effect_color.R_;
        frame.at<cv::Vec3b>(x, y)[1] = effect_color.G_;
        frame.at<cv::Vec3b>(x, y)[2] = effect_color.B_;
    }

public:

    /// \return the matrix under frame will be overwritten with contours of itself (cool)
    /// \param frame the pixel matrix that will be replaced with contours of itself (cool)
    /// \note the type of pixel inside frame will be changed to ??? <- something i belive it's 8UC3

    explicit NeonEffect(uchar r = 178, uchar g = 250, uchar b = 40)
            : r_(r), g_(g), b_(b) {};

    uchar r_;
    uchar g_;
    uchar b_;

    Color effect_color{r_, g_, b_};

    void operator()(cv::Mat &frame) override {

        cv::Mat output;

        Canny(frame, output, 50, 200);
        for (int x = 0; x < output.rows; x++) {
            for (int y = 0; y < output.cols; y++) {
                if ((int) output.at<uchar>(x, y) == 255) {

                    apply_color(x, y, frame);

                }
            }
        }

    }

};

#endif //VIDEO_PLAYER_EFFECTSIMPLEMENTATIONS_H
