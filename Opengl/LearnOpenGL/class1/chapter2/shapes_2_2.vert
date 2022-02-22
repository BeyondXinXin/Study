#version 450 core
layout(location=0) in vec3 a_pos;

void main(void)
{
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);
}
