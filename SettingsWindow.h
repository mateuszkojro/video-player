//
// Created by mateu on 4/19/2021.
//

#ifndef VIDEO_PLAYER_QT_SETTINGSWINDOW_H
#define VIDEO_PLAYER_QT_SETTINGSWINDOW_H

enum class EffectNr {

    effect_grey_cale_,
    effect_hsv_,
    effect_blur_r_,
    effect_sobel_,
    effect_canny_,
    effect_noise_,
    effect_gauss_

};


#include <QWindow>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
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

    QLayout *layout_;

    void flip_effect_grey_scale();

    void flip_effect_hsv();

    void flip_effect_blur_r();

    void flip_effect_sobel();

    void flip_effect_canny();

    void flip_effect_noise();

    void flip_effect_gauss();

    void flip_effect_blue_shift();


    /// 0 in effect array
    Switch *effect_grey_cale_;
    /// 1 in effect array
    Switch *effect_hsv_;
    /// 2 in effect array
    Switch *effect_blur_r_;
    /// 3 in effect array
    Switch *effect_sobel_;
    /// 4 in effect array
    Switch *effect_canny_;
    /// 5 in effect array
    Switch *effect_noise_;
    /// 6 in effect array
    Switch *effect_gauss_;
    /// there is no need for it to be in effect array
    /// i's kinda a joke anyway
    Switch *effect_blue_shift_;
};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
