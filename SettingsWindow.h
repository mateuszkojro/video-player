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

    void effect1_handler();

    void effect2_handler();

    Switch  *effect_one_button_;
    Switch *effect_two_button_;
};


#endif //VIDEO_PLAYER_QT_SETTINGSWINDOW_H
