#include "glwidget.h"
#include "helper.h"

#include <QPainter>
#include <QTimer>

GLWidget::GLWidget(Helper *helper, QWidget *parent)
    : QOpenGLWidget(parent), helper(helper)
{
    elapsed = 0;
    setFixedSize(parent->width(), parent->height());
    setBackgroundRole(QPalette::Base);
    // That should make it resizable
    // setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void GLWidget::animate()
{
    elapsed = (elapsed + qobject_cast<QTimer*>(sender())->interval()) % 1000;
    update();
}

void GLWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::LosslessImageRendering);
    helper->paint(&painter, event, elapsed);
    painter.end();
}
