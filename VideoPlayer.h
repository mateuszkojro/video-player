#ifndef VIDEO_PLAYER_H
#define VIDEO_PLAYER_H

#include <QMainWindow>
#include "GLWidget.h"

QT_BEGIN_NAMESPACE
class QAction;

class QActionGroup;

class QLabel;

class QMenu;

QT_END_NAMESPACE

class VideoPlayer : public QMainWindow {
Q_OBJECT

public:
    VideoPlayer();

    virtual ~VideoPlayer();

protected:
#ifndef QT_NO_CONTEXTMENU

    void contextMenuEvent(QContextMenuEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

#endif // QT_NO_CONTEXTMENU

private slots:

    void new_file_handler();

    void open_file_handler();

    void open_script_handler();

    void close_handler();

    void show_info_handler();

private:
    void second_window_create();

    void createActions();

    void createMenus();

    QMenu *action_menu_;
    QMenu *info_menu_;

    QAction *show_info_action_;
    QAction *new_action_;
    QAction *open_action_;
    QAction *open_script_action_;
    QAction *quit_action_;
    QAction *open_settings_action_;

    QDialog *second_window_;

    /// Widgets
    QWidget *central_widget_;
    QWidget *top_filler_;
    QWidget *bottom_filler_;
    GLWidget *opengl_widget_;

    QLayout *layout_;

    QTimer *timer_;

};

#endif
