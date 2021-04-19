#include <QtWidgets>
#include "glwidget.h"

#include "VideoPlayer.h"
#include "SettingsWindow.h"

VideoPlayer::VideoPlayer() {

    /// i think thtat is the main widget on the window
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    GLWidget *openGL = new GLWidget(&helper_, this);
    openGL->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(50);

    /// layout is containing elements in some configuration
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
    /// layout->addWidget(topFiller);
    layout->addWidget(bottomFiller);
    layout->addWidget(openGL);
    widget->setLayout(layout);

    createActions();
    createMenus();

    QString message = tr("Hello!");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Video Player"));
    /// setMinimumSize(520, 510);

    /// set the window size
    /// resize(520, 520);
    second_window_create();
    second_window_->setFocus();
}

VideoPlayer::~VideoPlayer() {
    delete second_window_;
}

/// \brief helpers to make actions
void VideoPlayer::createActions() {

    new_action_ = new QAction(tr("&New"), this);
    new_action_->setShortcuts(QKeySequence::New);
    new_action_->setStatusTip(tr("Create a new file"));
    connect(new_action_, &QAction::triggered, this, &VideoPlayer::new_file_handler);

    open_action_ = new QAction(tr("&Open..."), this);
    open_action_->setShortcuts(QKeySequence::Open);
    open_action_->setStatusTip(tr("Open an existing file"));
    connect(open_action_, &QAction::triggered, this, &VideoPlayer::open_handler);

    quit_action_ = new QAction(tr("Close application"), this);
    quit_action_->setShortcut(QKeySequence::Close);
    quit_action_->setStatusTip("Quit application");
    connect(quit_action_, &QAction::triggered, this, &VideoPlayer::close);

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

    action_menu_ = menuBar()->addMenu(tr("&Action"));
    action_menu_->addAction(new_action_);
    action_menu_->addAction(open_action_);
    action_menu_->addAction(quit_action_);
    action_menu_->addAction(open_settings_action_);

    info_menu_ = menuBar()->addMenu(tr("&Info"));
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

    statusBar()->showMessage(QString("File->NewFile hit"));
}

/// \brief What happens when open is clicked
void VideoPlayer::open_handler() {
    statusBar()->showMessage(QString("File->OpenFile hit"));
}

/// show dialog confirming a window closing then act accordingly
void VideoPlayer::close_handler() {
    switch (QMessageBox::question(
            this,
            tr("Video Player"),
            tr("Do you want to quit?"),
            QMessageBox::Yes |
            QMessageBox::Cancel,
            QMessageBox::Cancel)) {
        case QMessageBox::Yes:
            qDebug("yes");
            QCoreApplication::quit();
            break;
        case QMessageBox::Cancel:
            qDebug("cancel");
            break;
        default:
            qDebug("close");
            break;
    }
}

/// show info dialog
void VideoPlayer::show_info_handler() {
    statusBar()->showMessage(QString("Info->ShowInfo hit"));
    QMessageBox::information(
            this,
            tr("Video Player"),
            tr("It plays video files duh ... well sometimes <br> well not yet"));
}

void VideoPlayer::second_window_create() {
    second_window_ = new SettingsWindow(this);
}

