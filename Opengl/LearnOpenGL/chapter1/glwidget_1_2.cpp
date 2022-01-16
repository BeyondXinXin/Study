#include "glwidget_1_2.h"

static float vertices[] = {
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    0.0f, 0.5f, 0.0f
};

const char *vertes_shader_source =
  "#version 450 core\n\
    layout(location=0)in vec3 a_pos;\n\
    void main(){\n\
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);\n\
    }\n\
    \0";

const char *fragmen_shader_source =
  "#version 450 core\n\
    out vec4 frag_color;\n\
    void main(){\n\
    frag_color = vec4(1.0f,0.5f,0.2f,1.0);\n\
    }\n\
    \0";

GLWidget_1_2::GLWidget_1_2(QWidget *parent)
    : QOpenGLWidget(parent)
{
}

void GLWidget_1_2::initializeGL()
{
    initializeOpenGLFunctions();

    // 顶点缓冲对象、顶点数组对象
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);

    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

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
}

void GLWidget_1_2::resizeGL(int w, int h)
{
}

void GLWidget_1_2::paintGL()
{
    glClearColor(.2, .3, .3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
