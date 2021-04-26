#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) {

    parent_ = parent;

    /// Create buttons assign it to current window ans set its text
    effect_one_button_ = new QPushButton(this);
    effect_one_button_->setText(QString("Add effect \"hsv\""));
    effect_one_button_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    effect_two_button_ = new QPushButton(this);
    effect_two_button_->setText(QString("Add effect \"sobel\""));
    effect_two_button_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);

    //effect_two_button_->setStyleSheet("border-image:url(:/Icons/images/myImage.png);");

    /// Show the buttons
    effect_one_button_->show();
    effect_two_button_->show();

    /// Create a vertical layout
    layout_ = new QVBoxLayout;
    setSizeGripEnabled(layout_);

    /// Set the margins
    layout_->setContentsMargins(5, 5, 5, 5);

    /// Add buttons to layout
    layout_->addWidget(effect_one_button_);
    layout_->addWidget(effect_two_button_);

    /// Add handlers to the buttons
    connect(effect_one_button_, &QPushButton::released, this, &SettingsWindow::effect1_handler);
    connect(effect_two_button_, &QPushButton::released, this, &SettingsWindow::effect2_handler);

    /// Create the window, set the layout and show it
    this->create();
    this->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
    this->setLayout(layout_);
    this->show();
}

SettingsWindow::~SettingsWindow() {
    delete effect_one_button_;
    delete effect_two_button_;
    delete layout_;
}


/// Function called after effect 1 button was pressed
void SettingsWindow::effect1_handler() {

    /// Popup the dialog with the text and the title
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("Now apply hsv effect"));
}

/// Function called after effect 2 button was pressed
void SettingsWindow::effect2_handler() {

    /// Popup the dialog with the text and the title
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("Now apply sobel effect"));


}
