#ifndef GLWIDGET_1_4_H
#define GLWIDGET_1_4_H

#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLWidget>

class GLWidget_1_4 : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    explicit GLWidget_1_4(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    quint32 vaos_[2], vbos_[2], ebo_, shader_programs_[2];
};

#endif // GLWIDGET_1_4_H
