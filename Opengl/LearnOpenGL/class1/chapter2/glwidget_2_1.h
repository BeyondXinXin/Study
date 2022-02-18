#ifndef GLWIDGET_2_1_H
#define GLWIDGET_2_1_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>
#include <QPushButton>

class GLWidget_2_1 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

private:
    enum Shape
    {
        NoShape,
        Rectangle
    };

public:
    explicit GLWidget_2_1(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    void SetShape(const Shape &shape);
    void SetWireFrame(const bool &shape);

private:
    quint32 vao_, vbo_, ebo_, shader_program_;

    QPushButton *btn_add_rectangle_;
    QPushButton *btn_clear_rectangle_;
    QPushButton *btn_wire_frame_;

    Shape shape_;
};

#endif // GLWIDGET_1_3_H
