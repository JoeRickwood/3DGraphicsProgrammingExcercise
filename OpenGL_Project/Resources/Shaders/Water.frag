#version 460 core

#define DRAG_MULT 0.38 // changes how much waves pull on the water
#define WATER_DEPTH 1.0 // how deep is the water
#define CAMERA_HEIGHT 1.5 // how high the camera should be
#define ITERATIONS_RAYMARCH 12 // waves iterations of raymarching
#define ITERATIONS_NORMAL 36 // waves iterations when calculating normals

in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D Texture0;
uniform vec2 Tiling;

uniform float AmbientStrength            = 0.5f;
uniform vec3 AmbientColor                = vec3(1.f, 1.f, 1.f);

uniform vec3 LightColor                  = vec3(1.f, 1.f, 1.f);
uniform vec3 LightPos                    = vec3(-300.f, 0.f, 100.f);

uniform vec3 CameraPos;
uniform float LightSpecularStrength     = 0.5f;
uniform float ObjectShininess           = 35.0f;


out vec4 FinalColor;


void main() 
{
    FinalColor = vec4(vec3(1f, 1f, 1f) * dot(FragNormal, vec3(0f, 1f, 0f)), 1f);
}