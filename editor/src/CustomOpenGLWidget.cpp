#include "include/CustomOpenGLWidget.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>


CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
//    setSurfaceType(QWindow::OpenGLSurface);
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
    delete m_device;
}

void CustomOpenGLWidget::render(QPainter *painter)
{
    Q_UNUSED(painter);
}

void CustomOpenGLWidget::initialize()
{
}

void CustomOpenGLWidget::render()
{
    if (!m_device)
        m_device = new QOpenGLPaintDevice;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_device->setSize(size() * devicePixelRatio());
    m_device->setDevicePixelRatio(devicePixelRatio());

    QPainter painter(m_device);
    render(&painter);
}
