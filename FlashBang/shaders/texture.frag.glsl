#version 450
#

in vec2 TexCoordsFS;

layout (binding = 0) uniform sampler2D tex;

out vec4 color;

void main(void)
{
  color = texture(tex, TexCoordsFS);
  if (color.x == 1 && color.y == 1 && color.z == 1)
      color.w = .85;
}
