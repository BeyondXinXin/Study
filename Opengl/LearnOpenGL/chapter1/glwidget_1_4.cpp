#include "glwidget_1_4.h"

static float vertices1[] = {
    -.9f, -.5f, .0f,
    .0f, -.5f, .0f,
    -.45f, .5f, .0f
};

static float vertices2[] = {
    .0f, -.5f, .0f,
    .9f, -.5f, .0f,
    .45f, .5f, .0f
};

static const char *vertes_shader_source =
  "#version 450 core\n\
    layout(location=0)in vec3 a_pos;\n\
    void main(){\n\
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);\n\
    }\n\
    \0";

static const char *fragmen_shader1_source =
  "#version 450 core\n\
    out vec4 frag_color;\n\
    void main(){\n\
    frag_color = vec4(1.0f,0.5f,0.2f,1.0);\n\
    }\n\
    \0";

static const char *fragmen_shader2_source =
  "#version 450 core\n\
    out vec4 frag_color;\n\
    void main(){\n\
    frag_color = vec4(1.0f,1.0f,0.2f,1.0);\n\
    }\n\
    \0";

GLWidget_1_4::GLWidget_1_4(QWidget *parent)
  : QOpenGLWidget(parent)
{
}

void GLWidget_1_4::initializeGL()
{
    initializeOpenGLFunctions();

    // 顶点缓冲对象、顶点数组对象
    glGenVertexArrays(2, vaos_);
    glGenBuffers(2, vbos_);

    glBindVertexArray(vaos_[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindVertexArray(vaos_[1]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos_[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 着色器
    quint32 vertex_shaer = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shaer, 1, &vertes_shader_source, nullptr);
    glCompileShader(vertex_shaer);

    quint32 fragment_shaer1 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shaer1, 1, &fragmen_shader1_source, nullptr);
    glCompileShader(fragment_shaer1);

    quint32 fragment_shaer2 = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shaer2, 1, &fragmen_shader2_source, nullptr);
    glCompileShader(fragment_shaer2);

    shader_programs_[0] = glCreateProgram();
    glAttachShader(shader_programs_[0], vertex_shaer);
    glAttachShader(shader_programs_[0], fragment_shaer1);
    glLinkProgram(shader_programs_[0]);

    shader_programs_[1] = glCreateProgram();
    glAttachShader(shader_programs_[1], vertex_shaer);
    glAttachShader(shader_programs_[1], fragment_shaer2);
    glLinkProgram(shader_programs_[1]);

    glDeleteShader(vertex_shaer);
    glDeleteShader(fragment_shaer1);
    glDeleteShader(fragment_shaer2);
}

void GLWidget_1_4::resizeGL(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
}

void GLWidget_1_4::paintGL()
{
    glClearColor(.2f, .3f, .3f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_programs_[0]);
    glBindVertexArray(vaos_[0]);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glUseProgram(shader_programs_[1]);
    glBindVertexArray(vaos_[1]);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
