#version 450

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 VertexColor;
layout(location=2) in vec2 TexCoords;

uniform vec2 Translation;
uniform float RotationY;
uniform float ObjectWidth;
uniform float ZoomFactor;

out vec2 TexCoordsFS;

void main()
{
    vec3 newPosition = vec3(VertexPosition.x + 1.0 - ObjectWidth / 2, VertexPosition.y, VertexPosition.z);
    mat3 rot = mat3(
                 vec3(cos(RotationY), 0.0, sin(RotationY)),
                 vec3(0.0, 1.0, 0.0),
                 vec3(-sin(RotationY), 0.0, cos(RotationY)) 
               );

    newPosition = rot * newPosition;
    newPosition = vec3(newPosition.x - 1.0 + ObjectWidth / 2, newPosition.y, newPosition.z);
    gl_Position = vec4(newPosition + vec3(Translation, 0.0), ZoomFactor);
    TexCoordsFS = TexCoords;
}
