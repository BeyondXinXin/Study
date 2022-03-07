#ifndef GLWIDGET_3_5_H
#define GLWIDGET_3_5_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QTime>
#include <QTimer>

class GLWidget_3_5 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

private:
    enum Shape
    {
        NoShape,
        Rectangle
    };

public:
    explicit GLWidget_3_5(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void showEvent(QShowEvent *event) override;

private:
    void SetShape(const Shape &shape);
    void SetWireFrame(const bool &shape);

    void ColorAutoChange();

private:
    quint32 vao_, vbo_, ebo_;

    QPushButton *btn_add_rectangle_;
    QPushButton *btn_clear_rectangle_;
    QPushButton *btn_wire_frame_;

    Shape shape_;

    QOpenGLShaderProgram gl_shader_;

    QTimer timer;
};

#endif
