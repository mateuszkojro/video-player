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
    enum Mode {
        Video,
        Image,
        Script
    } ;

public slots:

    void animate();

    void change_file(const std::string &, Mode);

    void apply_effects(cv::Mat frame);

    void change_effect(int idx, Effect* effect);
    

protected:


    void paintEvent(QPaintEvent *event) override;
    void paint_image(QPainter* painter);
    void set_image(const std::string&);


private:
    Mode current_mode_;

    int elapsed_;

    cv::VideoCapture* video_capture_;

    std::mutex in_mat_mutex_;
    cv::Mat input_matrix_;

    std::mutex out_mat_mutex_;
    cv::Mat output_matrix_;

    std::mutex image_mutex_;
    QImage *image_;

    std::mutex effects_mutex_;
    std::array<Effect*, 8> effects_;
};

#endif // GLWIDGET_H
