#version 450 core

in vec3 a_pos;
//layout(location=2) in vec3 a_pos;

out vec4 vertex_colour;

void main(void)
{
    gl_Position = vec4(a_pos.x,a_pos.y,a_pos.z,1.0);
    vertex_colour=vec4(.8,.0,.0,1.0);
}
