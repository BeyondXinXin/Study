#include "glwidget_2_1.h"

static const float vertices[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f
};

static const quint32 indices[] = {
    0, 1, 3,
    0, 2, 3
};

static const char *vertes_shader_source =
  "#version 450 core\n\
    layout(location=0)in vec3 a_pos;\n\
    void main(){\n\
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);\n\
    }\n\
    \0";

static const char *fragmen_shader_source =
  "#version 450 core\n\
    out vec4 frag_color;\n\
    void main(){\n\
    frag_color = vec4(1.0f,0.5f,0.2f,1.0);\n\
    }\n\
    \0";

GLWidget_2_1::GLWidget_2_1(QWidget *parent)
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
}

void GLWidget_2_1::initializeGL()
{
    initializeOpenGLFunctions();

    // 顶点缓冲对象、顶点数组对象、索引缓冲对象
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &ebo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    // 着色器
    quint32 vertex_shaer = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shaer, 1, &vertes_shader_source, nullptr);
    glCompileShader(vertex_shaer);
    quint32 fragment_shaer = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shaer, 1, &fragmen_shader_source, nullptr);
    glCompileShader(fragment_shaer);

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shaer);
    glAttachShader(shader_program_, fragment_shaer);
    glLinkProgram(shader_program_);

    glDeleteShader(vertex_shaer);
    glDeleteShader(fragment_shaer);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void GLWidget_2_1::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void GLWidget_2_1::paintGL()
{
    glClearColor(.2f, .3f, .3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);

    switch (shape_) {
    case NoShape: {

    } break;
    case Rectangle: {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, static_cast<void *>(nullptr));
    } break;
    }
}

void GLWidget_2_1::SetShape(const GLWidget_2_1::Shape &shape)
{
    shape_ = shape;
    update();
}

void GLWidget_2_1::SetWireFrame(const bool &shape)
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
