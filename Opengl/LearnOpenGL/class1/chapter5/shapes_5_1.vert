#version 450 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_text_cord;

out vec2 text_cord;

uniform mat4 mat_model;
uniform mat4 mat_view;
uniform mat4 mat_projection;

void main(void)
{
    gl_Position = mat_projection*mat_view*mat_model*vec4(a_pos,1.0f);
    text_cord = a_text_cord;
}
