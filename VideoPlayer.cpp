#include <QtWidgets>
#include <glwidget.h>

#include "VideoPlayer.h"

VideoPlayer::VideoPlayer()
{
    /// i think thtat is the main widget on the window
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QWidget *topFiller = new QWidget;
    topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *bottomFiller = new QWidget;
    bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    GLWidget *openGL = new GLWidget(&helper, this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, openGL, &GLWidget::animate);
    timer->start(50);

    /// layout is containing elements in some configuration
    QVBoxLayout *layout = new QVBoxLayout;
    layout->setContentsMargins(5, 5, 5, 5);
//    layout->addWidget(topFiller);
    layout->addWidget(bottomFiller);
    layout->addWidget(openGL);
    widget->setLayout(layout);

    createActions();
    createMenus();

    QString message = tr("A context menu is available by right-clicking");
    statusBar()->showMessage(message);

    setWindowTitle(tr("Video Player"));
    setMinimumSize(520, 510);

    /// set the window size
    resize(520, 520);
}

/// \brief helpers to make actions
void VideoPlayer::createActions(){

    newAct = new QAction(tr("&New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, &QAction::triggered, this, &VideoPlayer::newFile);

    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, &QAction::triggered, this, &VideoPlayer::open);

    quitAct = new QAction(tr("Close application"), this);
    quitAct->setShortcut(QKeySequence::Close);
    quitAct->setStatusTip("Quit application");
    connect(quitAct, &QAction::triggered, this, &VideoPlayer::close);

    showInfoAct = new QAction(tr("Info"), this);
    showInfoAct->setShortcut(QKeySequence::HelpContents);
    showInfoAct->setStatusTip("Info about application");
    connect(showInfoAct,&QAction::triggered, this, &VideoPlayer::showInfo);
}

/// \brief helpers to make menus
void VideoPlayer::createMenus(){

    actionMenu = menuBar()->addMenu(tr("&Action"));
    actionMenu->addAction(newAct);
    actionMenu->addAction(openAct);
    actionMenu->addAction(quitAct);

    infoMenu = menuBar()->addMenu(tr("&Info"));
    infoMenu->addAction(showInfoAct);
}


#ifndef QT_NO_CONTEXTMENU
/// \brief I think these are context menus on the right click
void VideoPlayer::contextMenuEvent(QContextMenuEvent *event){

    QMenu menu(this);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU


/// \brief What happens when newFile is clicked
void VideoPlayer::newFile(){

    statusBar()->showMessage(QString("File->NewFile hit"));
}

/// \brief What happens when open is clicked
void VideoPlayer::open(){
    statusBar()->showMessage(QString("File->OpenFile hit"));
}

/// show dialog confirming a window closing then act accordingly
void VideoPlayer::close(){
    switch( QMessageBox::question(
                this,
                tr("Video Player"),
                tr("Do you want to quit?"),
                QMessageBox::Yes |
                QMessageBox::Cancel,
                QMessageBox::Cancel ) )
    {
      case QMessageBox::Yes:
        qDebug( "yes" );
        QCoreApplication::quit();
        break;
      case QMessageBox::Cancel:
        qDebug( "cancel" );
        break;
      default:
        qDebug( "close" );
        break;
    }
}

/// show info dialog
void VideoPlayer::showInfo(){
    statusBar()->showMessage(QString("Info->ShowInfo hit"));
    QMessageBox::information(
        this,
        tr("Video Player"),
        tr("It plays video files duh ... well sometimes <br> well not yet") );
}

