#version 460 core

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D Texture0;

uniform vec2 Tiling;


out vec4 FinalColor;


void main() 
{
    vec4 mainCol = texture(Texture0, FragTexCoords * Tiling);

    if(mainCol.a < 0.5)
        discard;

    FinalColor = mainCol;
}