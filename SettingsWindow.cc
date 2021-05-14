#include "SettingsWindow.h"
#include "EffectsImplementations.h"

SettingsWindow::SettingsWindow(QWidget *parent, GLWidget *opengl_widget) {

    parent_ = parent;

    opengl_widget_ = opengl_widget;

    /// Create buttons assign it to current window ans set its text

    effect_hsv_ = new Switch("Hsv");
    effect_blur_r_ = new Switch("Blur");
    effect_sobel_ = new Switch("Sobel");
    effect_canny_ = new Switch("Canny");
    effect_neon_ = new Switch("Neon");
    effect_noise_ = new Switch("Noise");
    effect_gauss_ = new Switch("Gauss");
    effect_grey_scale_ = new Switch("Grey Sale");

    pause_ = new QPushButton("Pause");
    skip_10_l_ = new QPushButton("Skip -10");
    skip_10_r_ = new QPushButton("Skip +10");

    /// Show the buttons

    effect_hsv_->show();
    effect_blur_r_->show();
    effect_sobel_->show();
    effect_canny_->show();
    effect_neon_->show();
    effect_noise_->show();
    effect_gauss_->show();
    effect_grey_scale_->show();
    pause_->show();
    skip_10_r_->show();
    skip_10_l_->show();

    /// Create a vertical layout
    /// layout_ = new QVBoxLayout;
    ///setSizeGripEnabled(layout_);

    layout_ = new QGridLayout(parent_);

    /// Set the margins
    layout_->setContentsMargins(5, 5, 5, 5);

    /// Add buttons to layout
    layout_->addWidget(skip_10_l_, 0, 0);
    layout_->addWidget(pause_, 0, 1);
    layout_->addWidget(skip_10_r_, 0, 2);
    layout_->addWidget(effect_hsv_, 1, 0, 1, 2);
    layout_->addWidget(effect_blur_r_, 2, 0, 1, 2);
    layout_->addWidget(effect_sobel_, 3, 0, 1, 2);
    layout_->addWidget(effect_neon_, 4, 0, 1, 2);
    layout_->addWidget(effect_canny_, 5, 0, 1, 2);
    layout_->addWidget(effect_noise_, 6, 0, 1, 2);
    layout_->addWidget(effect_gauss_, 7, 0, 1, 2);
    layout_->addWidget(effect_grey_scale_, 8, 0, 1, 2);
    /*
    layout_->addWidget(skip_10_l_);
    layout_->addWidget(pause_);
    layout_->addWidget(skip_10_r_);
    layout_->addWidget(effect_hsv_);
    layout_->addWidget(effect_blur_r_);
    layout_->addWidget(effect_sobel_);
    layout_->addWidget(effect_neon_);
    layout_->addWidget(effect_canny_);
    layout_->addWidget(effect_noise_);
    layout_->addWidget(effect_gauss_);
    layout_->addWidget(effect_grey_scale_); */


    /// Add handlers to the buttons
    connect(effect_grey_scale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);
    connect(effect_hsv_, &QPushButton::released, this, &SettingsWindow::flip_effect_hsv);
    connect(effect_blur_r_, &QPushButton::released, this, &SettingsWindow::flip_effect_blur_r);
    connect(effect_sobel_, &QPushButton::released, this, &SettingsWindow::flip_effect_sobel);
    connect(effect_canny_, &QPushButton::released, this, &SettingsWindow::flip_effect_canny);
    connect(effect_noise_, &QPushButton::released, this, &SettingsWindow::flip_effect_noise);
    connect(effect_neon_, &QPushButton::released, this, &SettingsWindow::flip_effect_neon);
    connect(effect_gauss_, &QPushButton::released, this, &SettingsWindow::flip_effect_gauss);
    connect(effect_grey_scale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);

    connect(pause_, &QPushButton::released, this, &SettingsWindow::flip_pause_);
    connect(skip_10_l_, &QPushButton::released, this, &SettingsWindow::flip_skip_10_l);
    connect(skip_10_r_, &QPushButton::released, this, &SettingsWindow::flip_skip_10_r);



    /// Create the window, set the layout and show it
    this->create();
    // this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setFixedSize(220, 500);
    this->setLayout(layout_);
    this->show();
}

SettingsWindow::~SettingsWindow() {

    delete effect_hsv_;
    delete effect_blur_r_;
    delete effect_sobel_;
    delete effect_canny_;
    delete effect_noise_;
    delete effect_gauss_;
    delete effect_neon_;
    delete effect_grey_scale_;
    delete pause_;
    delete skip_10_l_;
    delete skip_10_r_;
    delete layout_;
}

void SettingsWindow::flip_effect_grey_scale() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_grey_scale_->isChecked() ? new GrayScaleEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_grey_scale_), setting);
}


void SettingsWindow::flip_effect_hsv() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_hsv_->isChecked() ? new HSVEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_hsv_), setting);
}

void SettingsWindow::flip_effect_blur_r() {
    auto setting = effect_blur_r_->isChecked() ? new Blur_rEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_blur_r_), setting);

}

void SettingsWindow::flip_effect_sobel() {
    auto setting = effect_sobel_->isChecked() ? new SobelEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_sobel_), setting);
}

void SettingsWindow::flip_effect_canny() {
    auto setting = effect_canny_->isChecked() ? new CannyEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_canny_), setting);

}

void SettingsWindow::flip_effect_noise() {
    auto setting = effect_noise_->isChecked() ? new NoiseEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_noise_), setting);

}

void SettingsWindow::flip_effect_gauss() {
    auto setting = effect_gauss_->isChecked() ? new GaussianBlurEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_gauss_), setting);

}


void SettingsWindow::flip_effect_neon() {
    auto setting = effect_neon_->isChecked() ? new NeonEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_neon_), setting);

}

void SettingsWindow::flip_pause_() {
    //auto setting = pause_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i returned only some info now
    auto setting = pause_->isChecked() ? "clicked" : "no";
    opengl_widget_->change_state(static_cast<int>(StateNr::pause_), setting);

}

void SettingsWindow::flip_skip_10_l() {
    //auto setting = skip_10_l_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i return only some info now
    auto setting = skip_10_l_->isChecked() ? "clicked" : "no";
    opengl_widget_->change_state(static_cast<int>(StateNr::skip_10_l_), setting);

}

void SettingsWindow::flip_skip_10_r() {
    //auto setting = skip_10_r_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i return only some info now
    auto setting = skip_10_r_->isChecked() ? "clicked" : "no";
    opengl_widget_->change_state(static_cast<int>(StateNr::skip_10_r_), setting);

}


