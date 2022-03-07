#include "glwidget_3_2.h"

static const float vertices[] = {
    // positions      // colors        // texture coords
    0.75f, 0.75f, 0.0f, 2.0f, 2.0f, // top right
    0.75f, -0.75f, 0.0f, 2.0f, -1.0f, // bottom right
    -0.75f, -0.75f, 0.0f, -1.0f, -1.0f, // bottom left
    -0.75f, 0.75f, 0.0f, -1.0f, 2.0f // top left
};

static const quint32 indices[] = {
    0, 1, 3,
    1, 2, 3
};

GLWidget_3_2::GLWidget_3_2(QWidget *parent)
  : QOpenGLWidget(parent)
{
    btn_add_rectangle_ = new QPushButton(u8"���Ӿ���", this);
    btn_clear_rectangle_ = new QPushButton(u8"���", this);
    btn_wire_frame_ = new QPushButton(u8"�߿�ģʽ", this);

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
}

void GLWidget_3_2::initializeGL()
{
    initializeOpenGLFunctions();

    // ��ɫ��
    gl_shader_.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shapes_3_2.vert");
    gl_shader_.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shapes_3_2.frag");
    if (!gl_shader_.link()) {
        qDebug() << gl_shader_.log();
    }
    gl_shader_.bind();

    gl_texture_ = new QOpenGLTexture(QImage(":/awesomeface.png").mirrored());
    gl_shader_.setUniformValue("texture_smile_", 0);

    // ���㻺����󡢶���������������������
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
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void GLWidget_3_2::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void GLWidget_3_2::paintGL()
{
    glClearColor(.2f, .3f, .3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    gl_shader_.bind();
    glBindVertexArray(vao_);

    switch (shape_) {
    case NoShape: {

    } break;
    case Rectangle: {
        gl_texture_->bind();

        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        //        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

        const static float border_color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void *>(nullptr));
    } break;
    }
}

void GLWidget_3_2::SetShape(const GLWidget_3_2::Shape &shape)
{
    shape_ = shape;
    update();
}

void GLWidget_3_2::SetWireFrame(const bool &shape)
{
    makeCurrent();
    if (shape) {
        // �߿���䷽ʽ
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    } else { // �������䷽ʽ
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    doneCurrent();
    update();
}
