//
// Created by mateu on 4/19/2021.
//

#ifndef VIDEO_PLAYER_QT_SETTINGSWINDOW_H
#define VIDEO_PLAYER_QT_SETTINGSWINDOW_H

#include <QWindow>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QAction>
#include <QMessageBox>

#include "Switch.h"

class SettingsWindow : public QDialog {
public:
    explicit SettingsWindow(QWidget *parent);

    ~SettingsWindow() override;

private:

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

    Switch *effect_grey_cale_;
    Switch *effect_hsv_;
    Switch *effect_blur_r_;
    Switch *effect_sobel_;
    Switch *effect_canny_;
    Switch *effect_noise_;
    Switch *effect_gauss_;
    Switch *effect_blue_shift_;
};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
