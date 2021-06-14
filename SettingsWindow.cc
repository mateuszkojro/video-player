#include "SettingsWindow.h"
#include "EffectsImplementations.h"
#include "playback/VideoConvert.h"


#include <exception>

SettingsWindow::SettingsWindow(QWidget *parent, GLWidget *opengl_widget) {

    parent_ = parent;

//    this->setWindowFlags(Qt::WindowStaysOnTopHint);

    opengl_widget_ = opengl_widget;

    /// Create buttons assign it to current window ans set its text
    change_script_button_ = new QPushButton("Change script", this);
    change_script_button_->show();
    connect(change_script_button_, &QPushButton::released, this, &SettingsWindow::open_file_lua_handler);
    file_name = new QLabel("None", this);
    file_name->show();


    always_on_top_ = new Switch("Always on top");

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
    //noise_level_slider_->show();


    sobel_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    sobel_level_slider_->setMaximum(100);
    sobel_level_slider_->setMinimum(1);
    sobel_level_slider_->setTickInterval(1);
    //sobel_level_slider_->show();

    blur_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    blur_level_slider_->setMaximum(19);
    blur_level_slider_->setMinimum(1);
    blur_level_slider_->setTickInterval(1);
    //blur_level_slider_->show();

    gauss_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    gauss_level_slider_->setMaximum(10);
    gauss_level_slider_->setMinimum(0.5);
    gauss_level_slider_->setTickInterval(0.5);
    //gauss_level_slider_->show();

    neon_b_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_b_level_slider_->setMaximum(256);
    neon_b_level_slider_->setMinimum(0);
    neon_b_level_slider_->setTickInterval(1);
    //neon_b_level_slider_->show();

    neon_g_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_g_level_slider_->setMaximum(256);
    neon_g_level_slider_->setMinimum(0);
    neon_g_level_slider_->setTickInterval(1);
    ///neon_g_level_slider_->show();

    neon_r_level_slider_ = new QSlider(Qt::Orientation::Horizontal, this);
    neon_r_level_slider_->setMaximum(256);
    neon_r_level_slider_->setMinimum(0);
    neon_r_level_slider_->setTickInterval(1);
    //neon_r_level_slider_->show();


    /// Show the buttons

    //always_on_top_->show();
    //effect_script_->show();
    //effect_hsv_->show();
    //effect_blur_r_->show();
    //effect_sobel_->show();
    //effect_canny_->show();
    //effect_neon_->show();
    //effect_noise_->show();
    //effect_grey_scale_->show();

    //text_noise->show();
    //text_sobel->show();
    //text_gauss->show();
    // text_r->show();
    // text_g->show();
    //text_b->show();
    //text_blur->show();

    pause_ = new QPushButton("Pause");
    skip_10_l_ = new QPushButton("Skip -10");
    skip_10_r_ = new QPushButton("Skip +10");
    camera_ = new QPushButton("Camera");
    save_ = new QPushButton("Save");
    //pause_->show();
    //skip_10_r_->show();
    //skip_10_l_->show();

    bottom_filler_ = new QWidget;
    bottom_filler_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    line = new QFrame;
    line->setObjectName(QString::fromUtf8("line"));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);


    /// Create a vertical layout
    layout_ = new QVBoxLayout;
    setSizeGripEnabled(layout_);
    setWindowTitle(tr("Settings"));


    layout_horizontal_up = new QGroupBox(tr("Script layout"));
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(effect_script_);
    layout->addWidget(change_script_button_);
    layout->addWidget(file_name);
    layout_horizontal_up->setLayout(layout);


    layout_vertical_1 = new QGroupBox(tr("First"));
    QBoxLayout *layout_1 = new QVBoxLayout;
    layout_1->addWidget(effect_grey_scale_);
    layout_1->addWidget(effect_hsv_);
    layout_vertical_1->setLayout(layout_1);

    layout_vertical_2 = new QGroupBox(tr("Second"));
    QBoxLayout *layout_2 = new QVBoxLayout;
    layout_2->addWidget(effect_blur_r_);
    layout_2->addWidget(blur_level_slider_);
    layout_2->addWidget(text_blur);
    layout_2->addWidget(effect_gauss_);
    layout_2->addWidget(gauss_level_slider_);
    layout_2->addWidget(text_gauss);
    layout_2->addWidget(effect_noise_);
    layout_2->addWidget(noise_level_slider_);
    layout_2->addWidget(text_noise);
    layout_vertical_2->setLayout(layout_2);

    layout_vertical_3 = new QGroupBox(tr("Third"));
    QBoxLayout *layout_3 = new QVBoxLayout;
    layout_3->addWidget(effect_sobel_);
    layout_3->addWidget(sobel_level_slider_);
    layout_3->addWidget(text_sobel);
    layout_3->addWidget(effect_canny_);
    layout_3->addWidget(effect_neon_);
    layout_3->addWidget(neon_b_level_slider_);
    layout_3->addWidget(text_b);
    layout_3->addWidget(neon_g_level_slider_);
    layout_3->addWidget(text_g);
    layout_3->addWidget(neon_r_level_slider_);
    layout_3->addWidget(text_r);
    layout_vertical_3->setLayout(layout_3);

    layout_horizontal_cent = new QGroupBox(tr("Central layout"));
    QHBoxLayout *layout_central = new QHBoxLayout;
    layout_central->addWidget(layout_vertical_1);
    layout_central->addWidget(layout_vertical_2);
    layout_central->addWidget(layout_vertical_3);
    layout_horizontal_cent->setLayout(layout_central);

    layout_horizontal_down = new QGroupBox(tr("Skip and Pause"));
    QHBoxLayout *layout_d = new QHBoxLayout;
    layout_d->addWidget(skip_10_l_);
    layout_d->addWidget(pause_);
    layout_d->addWidget(skip_10_r_);
    layout_horizontal_down->setLayout(layout_d);

    layout_horizontal_settings = new QGroupBox(tr("Settings"));
    QHBoxLayout *layout_s = new QHBoxLayout;
    layout_s->addWidget(always_on_top_);
    layout_s->addWidget(camera_);
    layout_s->addWidget(save_);
    layout_horizontal_settings->setLayout(layout_s);


    /// Set the margins
    layout_->setContentsMargins(5, 5, 15, 5);
    layout_->addWidget(layout_horizontal_up);
    layout_->addWidget(layout_horizontal_cent);
    layout_->addWidget(layout_horizontal_down);
    layout_->addWidget(layout_horizontal_settings);

    /// Add buttons to layout
    //layout_->addWidget(file_name);
    ///layout_->addWidget(change_script_button_);
    //layout_->addWidget(layout_horizontal_up);
    //layout_->addWidget(bottom_filler_);
    //layout_->addWidget(line);
    //layout_->addWidget(skip_10_l_);
    //layout_->addWidget(pause_);
    //layout_->addWidget(skip_10_r_);
    //layout_->addWidget(line);
    //layout_->addWidget(always_on_top_);
    ///layout_->addWidget(effect_script_);
    //layout_->addWidget(effect_grey_scale_);
    //layout_->addWidget(effect_hsv_);
    //layout_->addWidget(effect_blur_r_);
    //layout_->addWidget(blur_level_slider_);
    //layout_->addWidget(text_blur);
    //layout_->addWidget(effect_gauss_);
    //layout_->addWidget(gauss_level_slider_);
    //layout_->addWidget(text_gauss);
    //layout_->addWidget(effect_sobel_);
    ///layout_->addWidget(sobel_level_slider_);
    //layout_->addWidget(text_sobel);
    //layout_->addWidget(effect_canny_);
    //layout_->addWidget(effect_neon_);
    //layout_->addWidget(neon_r_level_slider_);
    //layout_->addWidget(text_r);
    // layout_->addWidget(neon_g_level_slider_);
    //layout_->addWidget(text_g);
    //layout_->addWidget(neon_b_level_slider_);
    //layout_->addWidget(text_b);
    //layout_->addWidget(effect_noise_);
    //layout_->addWidget(noise_level_slider_);
    //layout_->addWidget(text_noise);

    /// Add handlers to the buttons
    connect(always_on_top_, &QPushButton::released, this, &SettingsWindow::flip_always_on_top);
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
    connect(pause_, &QPushButton::released, this, &SettingsWindow::flip_pause_);
    connect(skip_10_l_, &QPushButton::released, this, &SettingsWindow::flip_skip_10_l);
    connect(skip_10_r_, &QPushButton::released, this, &SettingsWindow::flip_skip_10_r);
    connect(camera_, &QPushButton::released, this, &SettingsWindow::open_camera);
    connect(save_, &QPushButton::released, this, &SettingsWindow::save_file);

    connect(noise_level_slider_, &QSlider::valueChanged, this, &SettingsWindow::change_noise_level);

    connect(effect_grey_scale_, &QPushButton::released, this, &SettingsWindow::flip_effect_grey_scale);

    /// Create the window, set the layout and show it
    this->create();
    // this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setFixedSize(800, 600);
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

void SettingsWindow::open_camera() {
    opengl_widget_->request_action(GLWidget::UseCamera);
}

void SettingsWindow::save_file() {
    auto* video_convet = new VideoConvert();

    auto path = QFileDialog::getSaveFileName(this,"Create new file").toStdString();
    video_convet->setDestinationFilePath(path);



    /// if( file is input )
    auto in = QFileDialog::getOpenFileName(this,"Chose file to be converted").toStdString();
    video_convet->change_file(in);
    /// else
    ///video_convet->change_camera();



    std::cout << "it worked?" << std::endl;
}

void SettingsWindow::createActions() {

    open_script_action_ = new QAction(tr("Open Lua"), this);
    connect(open_script_action_, &QAction::triggered, this, &SettingsWindow::open_file_lua_handler);
}

void SettingsWindow::createMenus() {
    action_menu_ = new QMenuBar(this);

    action_menu_->addAction(open_script_action_);
}

void SettingsWindow::open_file_lua_handler() {
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Lua File"));
    std::cout << "filename: " << fileName.toStdString() << std::endl;

    filename_Lua = fileName.toStdString();
    if (fileName.isEmpty()) {
        file_name->setText("None");
        return;
    }

    file_name->setText(fileName);
}

void SettingsWindow::flip_effect_script() {

    if (filename_Lua.empty())
        return;

    auto setting = effect_script_->isChecked() ? new LuaEffect(filename_Lua) : nullptr;
    try {
        opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_script_), setting);
    } catch (ScriptHandler::Exception &e) {
        QMessageBox::warning(
                nullptr,
                QString("Script error"),
                QString(e.what()));
    }
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
    auto setting = effect_neon_->isChecked() ? new NeonEffect(level, 0, 0) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}

void SettingsWindow::change_neon_g_level() {
    int level = neon_g_level_slider_->value();
    auto setting = effect_neon_->isChecked() ? new NeonEffect(0, level, 0) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}

void SettingsWindow::change_neon_r_level() {
    int level = neon_r_level_slider_->value();
    auto setting = effect_neon_->isChecked() ? new NeonEffect(0, 0, level) : nullptr;
    opengl_widget_->request_change_effect(static_cast<int>(EffectNr::effect_neon_), setting);
}

void SettingsWindow::flip_always_on_top() {
    is_always_on_top_ = always_on_top_->isChecked();
    std::cout << "is always on top is: " << is_always_on_top_ << std::endl;
    this->setWindowFlag(Qt::WindowStaysOnTopHint, is_always_on_top_);
}

void SettingsWindow::flip_skip_10_l() {
    //auto setting = skip_10_l_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i return only some info now
//    auto setting = skip_10_l_->isChecked() ? "clicked" : "no";
    opengl_widget_->request_action(GLWidget::SkipLeft);
}

void SettingsWindow::flip_skip_10_r() {
    //auto setting = skip_10_r_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i return only some info now
//    auto setting = skip_10_r_->isChecked() ? "clicked" : "no";
    opengl_widget_->request_action(GLWidget::SkipRight);

}

void SettingsWindow::flip_pause_() {
    //auto setting = pause_->isChecked() ? new NeonEffect() : nullptr;
    /// idk what type of message i have to return so for checking i returned only some info now
//    auto setting = pause_->isChecked() ? "clicked" : "no";
    opengl_widget_->request_action(GLWidget::Pause);
}