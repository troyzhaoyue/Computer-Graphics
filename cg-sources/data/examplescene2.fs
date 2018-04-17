#version 330 core
uniform sampler2D texture0;
in vec2 f_TexCoord0;
layout (location=0) out vec4 fragColor;

void main(void)
{
    fragColor = texture2D(texture0, vec2(f_TexCoord0.x, f_TexCoord0.y));
}
