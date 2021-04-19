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

class SettingsWindow : public QDialog {
public:
    explicit SettingsWindow(QWidget *parent);

    ~SettingsWindow() override;

private:

    QWidget *parent_;

    QLayout *layout_;

    void effect1_handler();

    void effect2_handler();

    QPushButton *effect_one_button_;
    QPushButton *effect_two_button_;
};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
