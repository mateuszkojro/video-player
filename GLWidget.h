#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <mutex>

class Helper;

class GLWidget : public QOpenGLWidget {
public:
    GLWidget(QWidget *parent);

public slots:

    void animate();

    void change_image(const std::string &path);

protected:
    void paintEvent(QPaintEvent *event) override;


private:
    int elapsed_;
    std::mutex image_mutex_;
    QImage *image_;
};

#endif // GLWIDGET_H
