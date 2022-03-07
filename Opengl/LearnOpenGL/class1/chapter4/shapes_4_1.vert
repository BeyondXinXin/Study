#version 450 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_text_cord;

out vec2 text_cord;

uniform mat4 rotation_mat;

void main(void)
{
    gl_Position = rotation_mat*vec4(a_pos.x,a_pos.y,a_pos.z,1.0f);
    text_cord = a_text_cord;
}
