#version 450 core
layout(location=0) in vec3 a_pos;

out vec4 vertex_colour;

void main(void)
{
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);
    vertex_colour=vec4(gl_Position.xyz,1.0);
}
