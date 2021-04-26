#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <mutex>
#include <array>
#include "Effect.h"

class Helper;

class GLWidget : public QOpenGLWidget {
public:
    GLWidget(QWidget *parent);

public slots:

    void animate();

    void change_image(const std::string &path);

    void apply_effects(cv::Mat& frame);

    void change_effect(int idx, Effect* effect);

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    int elapsed_;

    std::mutex image_mutex_;
    QImage *image_;

    std::mutex effects_mutex_;
    std::array<Effect*, 8> effects_;
};

#endif // GLWIDGET_H
