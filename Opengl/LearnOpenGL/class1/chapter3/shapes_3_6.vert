#version 450 core

layout(location=0) in vec3 a_pos;
layout(location=1) in vec3 a_color;

out vec4 vertex_colour;

uniform float off_set;

void main(void)
{
    gl_Position = vec4(
                off_set+a_pos.x,
                off_set+a_pos.y,
                off_set+a_pos.z,1.0);
    vertex_colour=vec4(a_color.xyz,1.0);

}
