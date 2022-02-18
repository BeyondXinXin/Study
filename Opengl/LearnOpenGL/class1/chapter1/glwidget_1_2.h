#ifndef GLWIDGET_1_2_H
#define GLWIDGET_1_2_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>

class GLWidget_1_2 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit GLWidget_1_2(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    quint32 vao_, vbo_, shader_program_;
};

#endif // GLWIDGET_1_2_H
