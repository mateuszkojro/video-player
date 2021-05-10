#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <mutex>
#include <array>
#include <opencv2/highgui.hpp>
#include "Effect.h"


class GLWidget : public QOpenGLWidget {
public:
    GLWidget(QWidget *parent);

public slots:

    void animate();

    void change_image(const std::string &path);

    void apply_effects(cv::Mat frame);

    void change_effect(int idx, Effect* effect);

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    int elapsed_;

    cv::VideoCapture* video_capture_;

    std::mutex mat_mutex_;
    cv::Mat current_image_;

    std::mutex image_mutex_;
    QImage *image_;

    std::mutex effects_mutex_;
    std::array<Effect*, 8> effects_;
};

#endif // GLWIDGET_H
