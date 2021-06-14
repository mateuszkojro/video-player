//
// Created by mateu on 4/19/2021.
//

#ifndef VIDEO_PLAYER_QT_SETTINGSWINDOW_H
#define VIDEO_PLAYER_QT_SETTINGSWINDOW_H

enum class EffectNr {
    effect_script_,
    effect_hsv_,
    effect_blur_r_,
    effect_sobel_,
    effect_noise_,
    effect_gauss_,
    effect_neon_,
    effect_grey_scale_,
    effect_canny_
};


#include <QWindow>
#include <QWidget>
#include <QPushButton>
#include <QDialog>
#include <QVBoxLayout>
#include <QAction>
#include <QMessageBox>
#include <QSlider>
#include <QLabel>


#include "custom_buttons/Switch.h"
#include "GLWidget.h"

class SettingsWindow : public QDialog {
public:
    explicit SettingsWindow(QWidget *parent, GLWidget *opengl_widget);

    ~SettingsWindow() override;

private:

    GLWidget *opengl_widget_;

    QWidget *parent_;

    QLayout *layout_;

    QGroupBox *layout_horizontal_up;

    QGroupBox *layout_vertical_1;

    QGroupBox *layout_vertical_2;

    QGroupBox *layout_vertical_3;

    QGroupBox *layout_horizontal_cent;

    QGroupBox *layout_horizontal_down;

    QGroupBox *layout_horizontal_settings;

    void flip_effect_script();

    void flip_effect_grey_scale();

    void flip_effect_hsv();

    void flip_effect_blur_r();

    void flip_effect_sobel();

    void flip_effect_canny();

    void flip_effect_noise();

    void flip_effect_gauss();

    void flip_effect_neon();

    void change_noise_level();

    void change_sobel_level();

    void change_blur_level();

    void change_gauss_level();

    void change_neon_r_level();

    void change_neon_g_level();

    void change_neon_b_level();

    void flip_always_on_top();

    void createMenus();

    void createActions();

    void open_camera();

    void save_file();

    void open_file_lua_handler();

    bool is_always_on_top_ = false;

    Switch *always_on_top_;
    Switch *effect_script_;
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

    QSlider *noise_level_slider_;
    QSlider *sobel_level_slider_;
    QSlider *blur_level_slider_;
    QSlider *gauss_level_slider_;
    QSlider *neon_r_level_slider_;
    QSlider *neon_g_level_slider_;
    QSlider *neon_b_level_slider_;

    QLabel *file_name;
    QLabel *text_noise;
    QLabel *text_sobel;
    QLabel *text_gauss;
    QLabel *text_blur;
    QLabel *text_r;
    QLabel *text_g;
    QLabel *text_b;

    QMenuBar *action_menu_;
    QAction *open_script_action_;
    QAction *new_action_;
    QWidget *bottom_filler_;
    QFrame *line;

    QPushButton* change_script_button_;

    QPushButton* camera_;

    QPushButton* save_;

    std::string filename_Lua;


    void flip_pause_();

    void flip_skip_10_l();

    void flip_skip_10_r();

    QPushButton *pause_;
    QPushButton *skip_10_l_;
    QPushButton *skip_10_r_;

};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
