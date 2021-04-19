#ifndef HELPER_H
#define HELPER_H

#include <QBrush>
#include <QFont>
#include <QPen>
#include <QWidget>

class Helper {
public:
    Helper();

public:
    void paint(QPainter *painter, QPaintEvent *event, int elapsed);

private:
    QImage *image_;
    uchar *buffer;
    int width_;
    int height_;

    int size() const { return width_ * height_; };
};

#endif
