#version 330 core
in vec3 Color;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture1;

uniform float Alpha;

void main()
{
    FragColor = texture(Texture1, vec2(TexCoord.x, TexCoord.y));
    FragColor.a = Alpha;	
}