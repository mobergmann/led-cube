#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H

#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLPaintDevice>


//QT_BEGIN_NAMESPACE
//class QPainter;
//class QOpenGLContext;
//class QOpenGLPaintDevice;
//QT_END_NAMESPACE

class CustomOpenGLWidget: public QOpenGLWidget, protected QOpenGLFunctions
{
Q_OBJECT
private:
    QOpenGLContext *m_context = nullptr;
    QOpenGLPaintDevice *m_device = nullptr;

public:
    explicit CustomOpenGLWidget(QWidget *parent = nullptr);
    ~CustomOpenGLWidget();

    virtual void render(QPainter *painter);
    virtual void render();

    virtual void initialize();

};

#endif //CUSTOMOPENGLWIDGET_H
