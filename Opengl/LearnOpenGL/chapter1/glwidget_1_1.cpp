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
}

void GLWidget_1_1::paintGL()
{
    glClearColor(.2, .3, .3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}
