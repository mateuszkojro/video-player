#ifndef GLWIDGET_H
#define GLWIDGET_H


#include <QOpenGLWidget>
#include <mutex>
#include <array>
#include <opencv2/highgui.hpp>
#include "Effect.h"

#define NEW_PIPELINE true

#if NEW_PIPELINE

#include "playback/VideoPlayback.h"

#endif

#define LOCK(item) std::lock_guard<std::mutex> item_lock(item)

class GLWidget : public QOpenGLWidget {
public:
    enum Mode {
        Video,
        Image
    };

    enum Action {
        Pause,
        SkipRight,
        SkipLeft,
        UseCamera,
    };

    GLWidget(QWidget *parent);

    void animate();

    void request_change_file(const std::string &path, Mode mode);

#if !NEW_PIPELINE
    void request_apply_effects(cv::Mat frame);
#endif

    void request_change_effect(int idx, Effect *effect);

    void request_action(Action action) {
        switch (action) {
            case UseCamera:
                current_mode_ = Video;
                playback_->change_camera();
                break;
            case SkipLeft:
                playback_->skip_10s();
                break;
            case SkipRight:
                playback_->back_10s();
                break;
            case Pause:
                playing_ = !playing_;
                break;
            default:
                throw "Bad action";
        }
    }

protected:


    void paintEvent(QPaintEvent *event) override;

    void paint_image(QPainter *painter);



    void set_image(const std::string &);

#if NEW_PIPELINE

    void change_current_pixmap(QPixmap *new_pixmap) {
        LOCK(current_pixmap_mutex_);
        delete current_pixmap_;
        current_pixmap_ = new_pixmap;
    }

#endif

private:
    Mode current_mode_;
    int elapsed_;

    bool playing_;

#if NEW_PIPELINE
    std::mutex current_pixmap_mutex_;
    QPixmap *current_pixmap_;

    VideoPlayback *playback_;
#else

    cv::VideoCapture *video_capture_;

    std::mutex in_mat_mutex_;
    cv::Mat input_matrix_;

    std::mutex out_mat_mutex_;
    cv::Mat output_matrix_;

    std::mutex image_mutex_;
    QImage *image_;

    std::mutex effects_mutex_;

    std::array<Effect *, 9> effects_;

#endif
};

#endif // GLWIDGET_H
