#version 450 core

in vec2 text_cord;

out vec4 frag_color;

uniform sampler2D texture_smile_;

void main(void)
{
    vec4 img1 = texture(texture_smile_,text_cord);

    frag_color = img1;
}
