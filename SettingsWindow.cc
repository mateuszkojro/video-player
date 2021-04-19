//
// Created by mateu on 4/19/2021.
//

#include <QVBoxLayout>
#include <QAction>
#include <QMessageBox>

#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) {

    parent_ = parent;

    effect_one_button_ = new QPushButton(this);
    effect_one_button_->setText(QString("Add effect \"One\""));
    effect_one_button_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    effect_two_button_ = new QPushButton(this);
    effect_two_button_->setText(QString("Add effect \"Two\""));
    effect_two_button_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    effect_one_button_->show();
    effect_two_button_->show();


    QVBoxLayout *layout = new QVBoxLayout;
    setSizeGripEnabled(layout);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->addWidget(effect_one_button_);
    layout->addWidget(effect_two_button_);

    connect(effect_one_button_, &QPushButton::released, this, &SettingsWindow::effect1_handler);
    connect(effect_two_button_, &QPushButton::released, this, &SettingsWindow::effect2_handler);

    this->create();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setLayout(layout);
    this->show();
}

SettingsWindow::~SettingsWindow() {
    delete effect_one_button_;
    delete effect_two_button_;
}

void SettingsWindow::effect1_handler() {
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("Now apply 1st effect"));
}

void SettingsWindow::effect2_handler() {
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("Now apply 2nd effect"));
}
