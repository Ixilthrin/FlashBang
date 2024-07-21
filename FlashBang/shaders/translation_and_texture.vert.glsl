#version 450

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 VertexColor;
layout(location=2) in vec2 TexCoords;

uniform vec2 Translation;

out vec2 TexCoordsFS;

void main()
{
    gl_Position = vec4(VertexPosition + vec3(Translation, 0.0), 1.0);
    TexCoordsFS = TexCoords;
}
