#version 450 core

out vec4 frag_color;
uniform vec4 out_color;

void main(void)
{
    frag_color = out_color;
}
