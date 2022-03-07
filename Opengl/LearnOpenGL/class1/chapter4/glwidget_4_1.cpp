#include "glwidget_4_1.h"

#include <QKeyEvent>

static const float vertices[] = {
    // positions      // colors        // texture coords
    0.3f, 0.3f, 0.0f, 1.0f, 1.0f, // top right
    0.3f, -0.3f, 0.0f, 1.0f, 0.0f, // bottom right
    -0.3f, -0.3f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.3f, 0.3f, 0.0f, 0.0f, 1.0f // top left
};

static const quint32 indices[] = {
    0, 1, 3,
    1, 2, 3
};

GLWidget_4_1::GLWidget_4_1(QWidget *parent)
  : QOpenGLWidget(parent)
{
    btn_add_rectangle_ = new QPushButton(u8"增加矩形", this);
    btn_clear_rectangle_ = new QPushButton(u8"清空", this);
    btn_wire_frame_ = new QPushButton(u8"线框模式", this);

    btn_add_rectangle_->setFixedSize(100, 30);
    btn_clear_rectangle_->setFixedSize(100, 30);
    btn_wire_frame_->setFixedSize(100, 30);

    btn_wire_frame_->setCheckable(true);

    btn_add_rectangle_->move(10, 10);
    btn_clear_rectangle_->move(10, 50);
    btn_wire_frame_->move(10, 90);

    connect(btn_add_rectangle_, &QPushButton::clicked, this, [&]() {
        SetShape(Rectangle);
    });
    connect(btn_clear_rectangle_, &QPushButton::clicked, this, [&]() {
        SetShape(NoShape);
    });
    connect(btn_wire_frame_, &QPushButton::clicked, this, [&]() {
        SetWireFrame(btn_wire_frame_->isChecked());
    });

    connect(&timer_, &QTimer::timeout, this, &GLWidget_4_1::AutoChange);

    setFocusPolicy(Qt::StrongFocus);
}

void GLWidget_4_1::initializeGL()
{
    initializeOpenGLFunctions();

    // 着色器
    gl_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shapes_4_1.vert");
    gl_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shapes_4_1.frag");
    if (!gl_shader_.link()) {
        qDebug() << gl_shader_.log();
    }
    gl_shader_.bind();
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // 着色器 uniform
    gl_shader_.setUniformValue("mix_value", 0.3f);
    gl_shader_.setUniformValue("texture_wall_", 1);
    gl_shader_.setUniformValue("texture_smile_", 2);

    // 纹理
    gl_texture_wall_ = new QOpenGLTexture(QImage(":/wall.jpg").mirrored());
    gl_texture_smile_ = new QOpenGLTexture(QImage(":/awesomeface.png").mirrored());

    // 顶点缓冲对象、顶点数组对象、索引缓冲对象
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), reinterpret_cast<GLvoid *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void GLWidget_4_1::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void GLWidget_4_1::paintGL()
{
    glClearColor(.2f, .3f, .3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_shader_.bind();
    glBindVertexArray(vao_);

    switch (shape_) {
    case NoShape: {

    } break;
    case Rectangle: {
        gl_texture_wall_->bind(1);
        gl_texture_smile_->bind(2);

        auto data = QTime::currentTime().msec();
        QMatrix4x4 mat;

        mat.setToIdentity();
        mat.translate(0.3f, -0.3f, 1);
        mat.rotate(data, .0f, .0f, 1.0f);
        gl_shader_.setUniformValue("rotation_mat", mat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void *>(nullptr));

        mat.setToIdentity();
        mat.translate(-0.3f, 0.3f, 1);
        mat.scale(static_cast<float>(fabs(sin(data))));
        gl_shader_.setUniformValue("rotation_mat", mat);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void *>(nullptr));

    } break;
    }
}

void GLWidget_4_1::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Up) {
        mix_vlaue_ = mix_vlaue_ + 0.1f;
        ChangeMixValue();

    } else if (event->key() == Qt::Key_Down) {
        mix_vlaue_ = mix_vlaue_ - 0.1f;
        ChangeMixValue();
    }
}

void GLWidget_4_1::showEvent(QShowEvent *event)
{
    timer_.start(200);
    QOpenGLWidget::showEvent(event);
}

void GLWidget_4_1::SetShape(const GLWidget_4_1::Shape &shape)
{
    shape_ = shape;
    update();
}

void GLWidget_4_1::SetWireFrame(const bool &shape)
{
    makeCurrent();
    if (shape) {
        // 线框填充方式
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    } else { // 多边形填充方式
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}

void GLWidget_4_1::ChangeMixValue()
{
    if (mix_vlaue_ > 1.0f) {
        mix_vlaue_ = 1.0f;
    }
    if (mix_vlaue_ < .0f) {
        mix_vlaue_ = .0f;
    }

    gl_shader_.bind();
    gl_shader_.setUniformValue("mix_value", mix_vlaue_);
    update();
}

void GLWidget_4_1::AutoChange()
{
    update();
}
