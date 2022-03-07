#version 450 core
in vec4 vertex_colour;

out vec4 frag_color;

void main(void)
{
    frag_color = vertex_colour;
}
