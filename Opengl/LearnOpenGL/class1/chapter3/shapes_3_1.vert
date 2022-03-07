#version 450 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec2 a_text_cord;

out vec2 text_cord;


void main(void)
{
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0f);
    text_cord = vec2(1-a_text_cord.x,a_text_cord.y);
}
