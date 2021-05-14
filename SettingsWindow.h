//
// Created by mateu on 4/19/2021.
//

#ifndef VIDEO_PLAYER_QT_SETTINGSWINDOW_H
#define VIDEO_PLAYER_QT_SETTINGSWINDOW_H

enum class EffectNr {


    effect_hsv_,
    effect_blur_r_,
    effect_sobel_,
    effect_noise_,
    effect_gauss_,
    effect_neon_,
    effect_grey_scale_,
    effect_canny_
};

enum class StateNr {
    pause_,
    skip_10_l_,
    skip_10_r_
};


#include <QWindow>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QAction>
#include <QMessageBox>

#include "Switch.h"
#include "GLWidget.h"

class SettingsWindow : public QDialog {
public:
    explicit SettingsWindow(QWidget *parent, GLWidget *opengl_widget);

    ~SettingsWindow() override;

private:

    GLWidget *opengl_widget_;

    QWidget *parent_;

    ///QLayout *layout_;
    /// i change typ of layout (QLayout -> QGridLayout) to add buttons in horizontal and vertical way - if it's wrong - my apologize
    QGridLayout *layout_;

    void flip_effect_grey_scale();

    void flip_effect_hsv();

    void flip_effect_blur_r();

    void flip_effect_sobel();

    void flip_effect_canny();

    void flip_effect_noise();

    void flip_effect_gauss();

    void flip_effect_neon();

    void flip_pause_();

    void flip_skip_10_l();

    void flip_skip_10_r();

    /// 0 in effect array
    Switch *effect_gauss_;
    /// 1 in effect array
    Switch *effect_hsv_;
    /// 2 in effect array
    Switch *effect_blur_r_;
    /// 3 in effect array
    Switch *effect_sobel_;
    /// 4 in effect array
    Switch *effect_noise_;
    /// 5
    Switch *effect_neon_;
    /// 6 in effect array
    Switch *effect_canny_;
    /// 7 in effect array
    Switch *effect_grey_scale_;

    QPushButton *pause_;
    QPushButton *skip_10_l_;
    QPushButton *skip_10_r_;

};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
