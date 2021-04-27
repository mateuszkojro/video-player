#include "SettingsWindow.h"
#include "EffectsImplementations.h"

SettingsWindow::SettingsWindow(QWidget *parent, GLWidget *opengl_widget) {

    parent_ = parent;

    opengl_widget_ = opengl_widget;

    /// Create buttons assign it to current window ans set its text
    effect_grey_cale_ = new Switch("Grey cale");
    effect_hsv_ = new Switch("Hsv");
    effect_blur_r_ = new Switch("Blur r");
    effect_sobel_ = new Switch("Sobel");
    effect_blue_shift_ = new Switch("Blue Shift");
    effect_canny_ = new Switch("Canny");
    effect_noise_ = new Switch("Noise");
    effect_gauss_ = new Switch("Gauss");


    effect_blue_shift_->setChecked(true);

    /// Show the buttons

    effect_grey_cale_->show();
    effect_hsv_->show();
    effect_blur_r_->show();
    effect_sobel_->show();
    effect_blue_shift_->show();
    effect_canny_->show();
    effect_noise_->show();
    effect_gauss_->show();


    /// Create a vertical layout
    layout_ = new QVBoxLayout;
    setSizeGripEnabled(layout_);

    /// Set the margins
    layout_->setContentsMargins(5, 5, 15, 5);

    /// Add buttons to layout
    layout_->addWidget(effect_grey_cale_);
    layout_->addWidget(effect_hsv_);
    layout_->addWidget(effect_blur_r_);
    layout_->addWidget(effect_sobel_);
    layout_->addWidget(effect_blue_shift_);
    layout_->addWidget(effect_canny_);
    layout_->addWidget(effect_noise_);
    layout_->addWidget(effect_gauss_);


    /// Add handlers to the buttons
    connect(effect_grey_cale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);
    connect(effect_hsv_, &QPushButton::released, this, &SettingsWindow::flip_effect_hsv);
    connect(effect_blur_r_, &QPushButton::released, this, &SettingsWindow::flip_effect_blur_r);
    connect(effect_sobel_, &QPushButton::released, this, &SettingsWindow::flip_effect_sobel);
    connect(effect_canny_, &QPushButton::released, this, &SettingsWindow::flip_effect_canny);
    connect(effect_noise_, &QPushButton::released, this, &SettingsWindow::flip_effect_noise);
    connect(effect_gauss_, &QPushButton::released, this, &SettingsWindow::flip_effect_gauss);
    connect(effect_blue_shift_, &QPushButton::released, this, &SettingsWindow::flip_effect_blue_shift);




    /// Create the window, set the layout and show it
    this->create();
    // this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setFixedSize(220, 500);
    this->setLayout(layout_);
    this->show();
}

SettingsWindow::~SettingsWindow() {
    delete effect_grey_cale_;
    delete effect_hsv_;
    delete effect_blur_r_;
    delete effect_sobel_;
    delete effect_canny_;
    delete effect_noise_;
    delete effect_gauss_;
    delete effect_blue_shift_;
    delete layout_;
}

void SettingsWindow::flip_effect_grey_scale() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_grey_cale_->isChecked() ? new GrayScaleEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_grey_cale_), setting);
}


void SettingsWindow::flip_effect_hsv() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_hsv_->isChecked() ? new HSVEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_hsv_), setting);
}

void SettingsWindow::flip_effect_blur_r() {
    if (effect_blur_r_->isChecked()) {
        /// here add to vector or some shiet
        /// If i'm not mistaken ths is true after clicking meaning button is blue
        /// and effect should be applied
        ;
    } else {

        /// this one makes effect disappear

        ;
    }

}

void SettingsWindow::flip_effect_sobel() {
    auto setting = effect_sobel_->isChecked() ? new SobelEffect() : nullptr;
    opengl_widget_->change_effect(static_cast<int>(EffectNr::effect_sobel_), setting);
}

void SettingsWindow::flip_effect_canny() {
    if (effect_canny_->isChecked()) {
        /// here add to vector or some shiet
        /// If i'm not mistaken ths is true after clicking meaning button is blue
        /// and effect should be applied
        ;
    } else {

        /// this one makes effect disappear

        ;
    }

}

void SettingsWindow::flip_effect_noise() {
    if (effect_noise_->isChecked()) {
        /// here add to vector or some shiet
        /// If i'm not mistaken ths is true after clicking meaning button is blue
        /// and effect should be applied
        ;
    } else {

        /// this one makes effect disappear

        ;
    }

}

void SettingsWindow::flip_effect_gauss() {
    if (effect_gauss_->isChecked()) {
        /// here add to vector or some shiet
        /// If i'm not mistaken ths is true after clicking meaning button is blue
        /// and effect should be applied
        ;
    } else {

        /// this one makes effect disappear

        ;
    }

}

void SettingsWindow::flip_effect_blue_shift() {

    if (effect_blue_shift_->isChecked()) {

    } else {
        effect_blue_shift_->setChecked(true);

    }

}
