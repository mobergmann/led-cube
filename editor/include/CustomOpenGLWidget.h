#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>

#include "Models/Led.h"
#include "Models/Cube.h"


class CustomOpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT

private:
    QColor m_background;
    Cube m_cube;

public:
    explicit CustomOpenGLWidget(QWidget *parent = nullptr);
    ~CustomOpenGLWidget() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};

#endif //CUSTOMOPENGLWIDGET_H
