#shader map_shader

uniform mat3 trasformViewProjection;
#vertex_shader

layout(location = 0)in vec2 Position;
out vec2 uv;

void main() 
{
  uv = (Position + vec2(1,1)) * 0.5;
  gl_Position = vec4(trasformViewProjection * vec3(Position, 1), 1);
}

#pixel_shader

in vec2 uv;
out vec4 outColor;

uniform usampler2D sprite;
uniform usampler2D border;

uniform vec4 color;
#define MAX_COLORS 1024
uniform vec3 colors[MAX_COLORS];
void main()
{
  uint center = texture(sprite, uv).r;
  vec2 resolution = textureSize(sprite, 0);
  const int N = 4;
  ivec2 d[N] = ivec2[](ivec2(1, 0), ivec2(0, 1), ivec2(-1, 0), ivec2(0, -1));
  float borderShadow = 1;
  bool playableLand = center != MAX_COLORS - 1;
  for (int i = 0; playableLand && i < N; i++)
  {
    uint cell = texture(sprite, uv + d[i] / resolution).r;
    if (cell != center)
    {
      borderShadow = 0.8;
      break;
    }
  }
  outColor = vec4(colors[center]*borderShadow, 1);
}