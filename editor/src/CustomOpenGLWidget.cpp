#include "include/CustomOpenGLWidget.h"

#include <QOpenGLContext>
#include <QOpenGLPaintDevice>
#include <QPainter>
#include <QMatrix4x4>
//#include <GL/GLU.h>

CustomOpenGLWidget::CustomOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    // m_background = QColor(100, 149, 237, 1); // non transparent
    m_background = QColor(100, 149, 237, 0);

    // todo init cube
    m_cube = Cube{};
}

CustomOpenGLWidget::~CustomOpenGLWidget()
{
}

void CustomOpenGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), m_background.alphaF());

    // todo ...
}

void CustomOpenGLWidget::paintGL()
{
    QPainter painter;
    painter.begin(this);

    painter.beginNativePainting();

    glClearColor(m_background.redF(), m_background.greenF(), m_background.blueF(), m_transparent ? 0.0f : 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    QMatrix4x4 modelview;
    modelview.rotate(m_fAngle, 0.0f, 1.0f, 0.0f);
    modelview.rotate(m_fAngle, 1.0f, 0.0f, 0.0f);
    modelview.rotate(m_fAngle, 0.0f, 0.0f, 1.0f);
    modelview.scale(m_fScale);
    modelview.translate(0.0f, -0.2f, 0.0f);

    if (m_qtLogo) {
        m_program1->bind();
        m_program1->setUniformValue(m_matrixUniform1, modelview);
        paintQtLogo();
        m_program1->release();
    }

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    painter.endNativePainting();

    for (const auto &layer: m_cube.leds)
    {
        for (const auto &line: layer)
        {
            for (const auto &led: line)
            {
                led.draw(&painter);
            }
        }
    }

    painter.end();
}

void CustomOpenGLWidget::resizeGL(int w, int h)
{
    QOpenGLWidget::resizeGL(w, h);
}
