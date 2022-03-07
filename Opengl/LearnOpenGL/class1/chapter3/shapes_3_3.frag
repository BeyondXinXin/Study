#version 450 core

in vec2 text_cord;

out vec4 frag_color;

uniform sampler2D texture_wall_;
uniform sampler2D texture_smile_;

uniform float mix_value;

void main(void)
{
    vec4 img1 = texture(texture_wall_,text_cord);
    vec4 img2 = texture(texture_smile_,text_cord);

    frag_color = mix(img1,img2,mix_value);
}
