#ifndef GLWIDGET_1_3_H
#define GLWIDGET_1_3_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>

class GLWidget_1_3 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit GLWidget_1_3(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    quint32 vao_, vbo_, ebo_, shader_program_;
};

#endif // GLWIDGET_1_3_H
