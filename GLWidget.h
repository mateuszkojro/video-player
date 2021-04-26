#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>

class Helper;

class GLWidget : public QOpenGLWidget
{
   public:
       GLWidget(QWidget *parent);

   public slots:
       void animate();

   protected:
       void paintEvent(QPaintEvent *event) override;

   private:
       int elapsed_;
       QImage* image_;
};

#endif // GLWIDGET_H
