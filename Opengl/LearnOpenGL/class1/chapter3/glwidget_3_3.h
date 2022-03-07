#ifndef GLWIDGET_3_3_H
#define GLWIDGET_3_3_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QPushButton>

class GLWidget_3_3 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

private:
    enum Shape
    {
        NoShape,
        Rectangle
    };

public:
    explicit GLWidget_3_3(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void SetShape(const Shape &shape);
    void SetWireFrame(const bool &shape);

private:
    quint32 vao_, vbo_, ebo_;

    QPushButton *btn_add_rectangle_;
    QPushButton *btn_clear_rectangle_;
    QPushButton *btn_wire_frame_;

    Shape shape_;

    QOpenGLShaderProgram gl_shader_;
};

#endif
