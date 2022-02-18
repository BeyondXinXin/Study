#include "glwidget_1_1.h"

GLWidget_1_1::GLWidget_1_1(QWidget *parent)
  : QOpenGLWidget(parent)
{
}

void GLWidget_1_1::initializeGL()
{
    initializeOpenGLFunctions();
}

void GLWidget_1_1::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void GLWidget_1_1::paintGL()
{
    glClearColor(.2f, .3f, .3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
