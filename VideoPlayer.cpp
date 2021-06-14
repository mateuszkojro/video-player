#include <QtWidgets>

#include "VideoPlayer.h"
#include "SettingsWindow.h"
#include <iostream>


VideoPlayer::VideoPlayer() {

    /// Create the central widget
    central_widget_ = new QWidget;
    setCentralWidget(central_widget_);

    /// Make filler on top
//    top_filler_ = new QWidget;
//    top_filler_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /// Make a filler on the bottom
    bottom_filler_ = new QWidget;
    bottom_filler_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /// Create the widget that is going to contain actual video frame
    opengl_widget_ = new GLWidget(this);
    opengl_widget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    /// Make a timer and run function on every tick - I am not sure tho
    timer_ = new QTimer(this);
    connect(timer_, &QTimer::timeout, opengl_widget_, &GLWidget::animate);
    timer_->start(50);

    /// Make a vertical layout
    layout_ = new QVBoxLayout;
    layout_->setContentsMargins(5, 5, 5, 5);

    /// Configure widgets in the layout
//    layout_->addWidget(top_filler_);
    layout_->addWidget(bottom_filler_);
    layout_->addWidget(opengl_widget_);

    /// Set layout as the central widget
    central_widget_->setLayout(layout_);

    this->setMinimumSize(500, 500);

    /// Run helpers for the actions and menus
    createActions();
    createMenus();

    /// Show message on the status bar
    statusBar()->showMessage(tr("Hello!"));

    setWindowTitle(tr("Video Player"));

    second_window_create();
}

VideoPlayer::~VideoPlayer() {
    second_window_->close();
    delete action_menu_;
    delete info_menu_;
    delete show_info_action_;
    delete new_action_;
    delete open_action_;
    delete quit_action_;
    delete open_settings_action_;
    delete second_window_;
    delete central_widget_;
//    delete top_filler_;
//    delete bottom_filler_;
//    delete opengl_widget_;
//    delete layout_;
//    delete timer_;
}

/// \brief helpers to make actions
void VideoPlayer::createActions() {

    /// Create the action
    new_action_ = new QAction(tr("&New"), this);
    /// Set the shortuct for the action
    new_action_->setShortcuts(QKeySequence::New);

    new_action_->setStatusTip(tr("Create a new file"));
    /// Add function taht is goin to be handling what happens
    connect(new_action_, &QAction::triggered, this, &VideoPlayer::new_file_handler);

    /// The same goes for all other actions...

    open_action_ = new QAction(tr("&Open..."), this);
    open_action_->setShortcuts(QKeySequence::Open);
    open_action_->setStatusTip(tr("Open an existing file"));
    connect(open_action_, &QAction::triggered, this, &VideoPlayer::open_file_handler);

    use_camera_action_ = new QAction(tr("&Camera..."), this);
    connect(use_camera_action_, &QAction::triggered, this, &VideoPlayer::use_camera_handler);

    open_script_action_ = new QAction(tr("&Open script file"), this);
//    open_script_action_->setShortcut(QKeySequence::)

    open_script_action_->setStatusTip(tr("Open file with lua script"));
    connect(open_script_action_, &QAction::triggered, this, &VideoPlayer::open_script_handler);

    quit_action_ = new QAction(tr("Close application"), this);
    quit_action_->setShortcut(QKeySequence::Close);
    quit_action_->setStatusTip("Quit application");
    connect(quit_action_, &QAction::triggered, this, &VideoPlayer::close_handler);

    show_info_action_ = new QAction(tr("Info"), this);
    show_info_action_->setShortcut(QKeySequence::HelpContents);
    show_info_action_->setStatusTip("Info about application");
    connect(show_info_action_, &QAction::triggered, this, &VideoPlayer::show_info_handler);

    open_settings_action_ = new QAction(tr("Open settings"), this);
    open_settings_action_->setStatusTip("Open a window with more settings");
    connect(open_settings_action_, &QAction::triggered, this, &VideoPlayer::second_window_create);
}

/// \brief helpers to make menus
void VideoPlayer::createMenus() {
    /// Add "Action" menu to the menu bar
    action_menu_ = menuBar()->addMenu(tr("&Action"));

    /// Add diferent actions to the menu bar
    action_menu_->addAction(new_action_);
    action_menu_->addAction(open_action_);
    action_menu_->addAction(quit_action_);
    action_menu_->addAction(use_camera_action_);
    action_menu_->addAction(open_settings_action_);

    /// Add "Info" menu to the menu bar
    info_menu_ = menuBar()->addMenu(tr("&Info"));
    /// Add show info action to the menu bar
    info_menu_->addAction(show_info_action_);
}


#ifndef QT_NO_CONTEXTMENU

/// \brief I think these are context menus on the right click
void VideoPlayer::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.exec(event->globalPos());
}

#endif // QT_NO_CONTEXTMENU


/// \brief What happens when newFile is clicked
void VideoPlayer::new_file_handler() {
    /// Show message in status bar
    statusBar()->showMessage(QString("File->NewFile hit"));
}

/// \brief What happens when open is clicked
void VideoPlayer::open_file_handler() {
    /// Show message in status bar
    statusBar()->showMessage(QString("File->OpenFile hit"));
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open File"));
    std::cout << "filename: " << fileName.toStdString() << std::endl;

    /// Ask the user a question and handle the responses
    switch (QMessageBox::question(
            this,
            tr("Video Player"),
            tr("Is this file a video?"),
            QMessageBox::Yes |
            QMessageBox::No |
            QMessageBox::Cancel,
            QMessageBox::Cancel)) {
        case QMessageBox::Yes: {
            /// Print to console
            qDebug("Video");
            /// Exit the application
            opengl_widget_->request_change_file(fileName.toStdString(), GLWidget::Video);
            break;
        }
        case QMessageBox::No: {
            /// Print to console
            qDebug("Image");
            /// Exit the application
            opengl_widget_->request_change_file(fileName.toStdString(), GLWidget::Image);
            break;
        }
        case QMessageBox::Cancel: {
            /// Print to the console
            qDebug("cancel");
            break;
        }
        default:
            /// Print to the console
            qDebug("close");
            break;
    }


}

/// show dialog confirming a window closing then act accordingly
void VideoPlayer::close_handler() {

    /// Ask the user a question and handle the responses
    switch (QMessageBox::question(
            this,
            tr("Video Player"),
            tr("Do you want to quit?"),
            QMessageBox::Yes |
            QMessageBox::Cancel,
            QMessageBox::Cancel)) {

        case QMessageBox::Yes: {
            /// Print to console
            qDebug("yes");
            /// Exit the application
            QCoreApplication::exit(0);
            break;
        }
        case QMessageBox::Cancel: {
            /// Print to the console
            qDebug("cancel");
            break;
        }
        default:
            /// Print to the console
            qDebug("close");
            break;
    }
}

/// show info dialog
void VideoPlayer::show_info_handler() {
    /// Show message in the status bar
    statusBar()->showMessage(QString("Info->ShowInfo hit"));
    /// Show dialog with text
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("It plays video files duh ... well sometimes <br> well not yet"));
}

/// Create the second window
void VideoPlayer::second_window_create() {
    second_window_ = new SettingsWindow(this, opengl_widget_);
}

void VideoPlayer::open_script_handler() {
    statusBar()->showMessage(QString("File->OpenScript hit"));
    auto fileName = QFileDialog::getOpenFileName(this,
                                                 tr("Open Image"), "/home/jana", tr("All files"));
    std::cout << "Script name: " << fileName.toStdString() << std::endl;
    throw "Not implemented";
//    opengl_widget_->request_change_file(fileName.toStdString(), GLWidget::Mode::Script);
}

void VideoPlayer::closeEvent(QCloseEvent *event) {
    VideoPlayer::close_handler();
    event->ignore();
}

void VideoPlayer::resizeEvent(QResizeEvent *event) {
    std::cout << "resize main window" << std::endl;
    opengl_widget_->setFixedSize(event->size().width() - 10, event->size().height());
    QWidget::resizeEvent(event);
}

void VideoPlayer::use_camera_handler() {
    opengl_widget_->request_action(GLWidget::UseCamera);
}
