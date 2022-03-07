#ifndef GLWIDGET_5_1_H
#define GLWIDGET_5_1_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QOpenGLWidget>
#include <QPushButton>
#include <QTime>
#include <QTimer>

class GLWidget_5_1 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

private:
    enum Shape
    {
        NoShape,
        Rectangle
    };

public:
    explicit GLWidget_5_1(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    void SetShape(const Shape &shape);
    void SetWireFrame(const bool &shape);
    void ChangeMixValue();
    void AutoChange();

private:
    quint32 vao_, vbo_;

    QPushButton *btn_add_rectangle_;
    QPushButton *btn_clear_rectangle_;
    QPushButton *btn_wire_frame_;

    Shape shape_ = Rectangle;

    QOpenGLShaderProgram gl_shader_;
    QOpenGLTexture *gl_texture_wall_, *gl_texture_smile_;

    float mix_vlaue_ = 0.5;

    QTimer timer_;
};

#endif
