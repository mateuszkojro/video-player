#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QMainWindow>
#include "helper.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QLabel;
class QMenu;
QT_END_NAMESPACE

class VideoPlayer : public QMainWindow
{
    Q_OBJECT

public:
    VideoPlayer();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private slots:
    void newFile();
    void open();
    void close();
    void showInfo();

private:
    void createActions();
    void createMenus();

    QMenu *actionMenu;
    QMenu *infoMenu;

    QAction *showInfoAct;

    QAction *newAct;
    QAction *openAct;
    QAction *quitAct;
private:
    Helper helper;
    //QLabel *infoLabel;
};

#endif
