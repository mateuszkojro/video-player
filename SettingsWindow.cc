#include "SettingsWindow.h"
#include "EffectsImplementations.h"

SettingsWindow::SettingsWindow(QWidget *parent, GLWidget *opengl_widget) {

    parent_ = parent;

    opengl_widget_ = opengl_widget;

    /// Create buttons assign it to current window ans set its text

    effect_script_ = new Switch("Script");

    effect_hsv_ = new Switch("Hsv");
    effect_blur_r_ = new Switch("Blur");
    effect_sobel_ = new Switch("Sobel");
    effect_canny_ = new Switch("Canny");
    effect_neon_ = new Switch("Neon");
    effect_noise_ = new Switch("Noise");
    effect_gauss_ = new Switch("Gauss");
    effect_grey_scale_ = new Switch("Grey Sale");

    text_noise = new QLabel("Noise percentage", this);
    ///text_noise->set;
    text_sobel = new QLabel("Scale factor for the derivative values", this);
    text_gauss = new QLabel("Gauss standard deviation", this);
    text_r = new QLabel("Saturation of the red color", this);
    text_g = new QLabel("Saturation of the green color", this);
    text_b = new QLabel("Saturation of the blue color", this);
    text_blur = new QLabel("Value specifying the size of kernel", this);

    noise_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    noise_level_slider_->setMaximum(99);
    noise_level_slider_->setMinimum(1);
    noise_level_slider_->setTickInterval(5);
    noise_level_slider_->show();


    sobel_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    sobel_level_slider_->setMaximum(100);
    sobel_level_slider_->setMinimum(1);
    sobel_level_slider_->setTickInterval(1);
    sobel_level_slider_->show();

    blur_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    blur_level_slider_->setMaximum(19);
    blur_level_slider_->setMinimum(1);
    blur_level_slider_->setTickInterval(1);
    blur_level_slider_->show();

    gauss_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    gauss_level_slider_->setMaximum(10);
    gauss_level_slider_->setMinimum(0.5);
    gauss_level_slider_->setTickInterval(0.5);
    gauss_level_slider_->show();

    neon_b_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_b_level_slider_->setMaximum(256);
    neon_b_level_slider_->setMinimum(0);
    neon_b_level_slider_->setTickInterval(1);
    neon_b_level_slider_->show();

    neon_g_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_g_level_slider_->setMaximum(256);
    neon_g_level_slider_->setMinimum(0);
    neon_g_level_slider_->setTickInterval(1);
    neon_g_level_slider_->show();

    neon_r_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_r_level_slider_->setMaximum(256);
    neon_r_level_slider_->setMinimum(0);
    neon_r_level_slider_->setTickInterval(1);
    neon_r_level_slider_->show();


    /// Show the buttons

    effect_script_->show();
    effect_hsv_->show();
    effect_blur_r_->show();
    effect_sobel_->show();
    effect_canny_->show();
    effect_neon_->show();
    effect_noise_->show();
    effect_gauss_->show();
    effect_grey_scale_->show();

    text_noise->show();
    text_sobel->show();
    text_gauss->show();
    text_r->show();
    text_g->show();
    text_b->show();
    text_blur->show();



    /// Create a vertical layout
    layout_ = new QVBoxLayout;
    setSizeGripEnabled(layout_);

    /// Set the margins
    layout_->setContentsMargins(5, 5, 15, 5);

    /// Add buttons to layout

    layout_->addWidget(effect_script_);
    layout_->addWidget(effect_hsv_);
    layout_->addWidget(effect_blur_r_);
    layout_->addWidget(blur_level_slider_);
    layout_->addWidget(text_blur);
    layout_->addWidget(effect_sobel_);
    layout_->addWidget(sobel_level_slider_);
    layout_->addWidget(text_sobel);
    layout_->addWidget(effect_neon_);
    layout_->addWidget(neon_r_level_slider_);
    layout_->addWidget(text_r);
    layout_->addWidget(neon_g_level_slider_);
    layout_->addWidget(text_g);
    layout_->addWidget(neon_b_level_slider_);
    layout_->addWidget(text_b);
    layout_->addWidget(effect_canny_);
    layout_->addWidget(effect_noise_);
    layout_->addWidget(noise_level_slider_);

    layout_->addWidget(text_noise);

    layout_->addWidget(effect_gauss_);
    layout_->addWidget(gauss_level_slider_);
    layout_->addWidget(text_gauss);
    layout_->addWidget(effect_grey_scale_);

    /// Add handlers to the buttons
    connect(effect_script_, &QPushButton::released, this, &SettingsWindow::flip_effect_script);
    connect(effect_grey_scale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);
    connect(effect_hsv_, &QPushButton::released, this, &SettingsWindow::flip_effect_hsv);
    connect(effect_blur_r_, &QPushButton::released, this, &SettingsWindow::flip_effect_blur_r);
    connect(blur_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_blur_level);
    connect(effect_sobel_, &QPushButton::released, this, &SettingsWindow::flip_effect_sobel);
    connect(sobel_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_sobel_level);
    connect(effect_canny_, &QPushButton::released, this, &SettingsWindow::flip_effect_canny);
    connect(effect_noise_, &QPushButton::released, this, &SettingsWindow::flip_effect_noise);
    connect(noise_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_noise_level);
    connect(effect_neon_, &QPushButton::released, this, &SettingsWindow::flip_effect_neon);
    connect(neon_r_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_neon_r_level);
    connect(neon_g_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_neon_g_level);
    connect(neon_b_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_neon_b_level);
    connect(effect_gauss_, &QPushButton::released, this, &SettingsWindow::flip_effect_gauss);
    connect(gauss_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_gauss_level);

    connect(noise_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_noise_level);

    connect(effect_grey_scale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);



    /// Create the window, set the layout and show it
    this->create();
    // this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setFixedSize(220, 700);
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
    delete layout_;
}

void SettingsWindow::flip_effect_script() {
    auto setting = effect_script_->isChecked() ? new LuaEffect("../example-scripts/script_bgr.lua") : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_script_), setting);
}

void SettingsWindow::flip_effect_grey_scale() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_grey_scale_->isChecked() ? new GrayScaleEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_grey_scale_), setting);
}

void SettingsWindow::flip_effect_hsv() {
    /// Function called after effect 1 button was pressed
    auto setting = effect_hsv_->isChecked() ? new HSVEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_hsv_), setting);
}     

void SettingsWindow::flip_effect_blur_r() {
    auto setting = effect_blur_r_->isChecked() ? new Blur_rEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_blur_r_), setting);

}

void SettingsWindow::flip_effect_sobel() {
    auto setting = effect_sobel_->isChecked() ? new SobelEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_sobel_), setting);
}

void SettingsWindow::flip_effect_canny() {
    auto setting = effect_canny_->isChecked() ? new CannyEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_canny_), setting);

}

void SettingsWindow::flip_effect_noise() {
//    auto setting = effect_noise_->isChecked() ? new NoiseEffect() : nullptr;
//    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_noise_), setting);
    change_noise_level();
}

void SettingsWindow::flip_effect_gauss() {
    auto setting = effect_gauss_->isChecked() ? new GaussianBlurEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_gauss_), setting);

}

void SettingsWindow::flip_effect_neon() {
    auto setting = effect_neon_->isChecked() ? new NeonEffect() : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);

}

void SettingsWindow::change_noise_level() {
    int level = noise_level_slider_->value();
    auto setting = effect_noise_->isChecked() ? new NoiseEffect(level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_noise_), setting);
}

void SettingsWindow::change_sobel_level() {
    int level = sobel_level_slider_->value();
    auto setting = effect_sobel_->isChecked() ? new SobelEffect(level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_sobel_), setting);
}

void SettingsWindow::change_blur_level() {
    int level = blur_level_slider_->value();
    auto setting = effect_blur_r_->isChecked() ? new Blur_rEffect(level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_blur_r_), setting);
}

void SettingsWindow::change_gauss_level() {
    int level = gauss_level_slider_->value();
    auto setting = effect_gauss_->isChecked() ? new GaussianBlurEffect(level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_gauss_), setting);
}

void SettingsWindow::change_neon_b_level() {
    int level = neon_b_level_slider_->value();
    auto setting = effect_neon_->isChecked() ? new NeonEffect(level,0,0) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}

void SettingsWindow::change_neon_g_level() {
    int level = neon_g_level_slider_->value();
    auto setting = effect_neon_->isChecked() ? new NeonEffect(0,level,0) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}

void SettingsWindow::change_neon_r_level() {
    int level = neon_r_level_slider_->value();
    auto setting = effect_neon_->isChecked() ? new NeonEffect(0, 0, level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}