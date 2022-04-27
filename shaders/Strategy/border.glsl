#shader border_shader

#include transforms

struct VsOutput
{
  vec3 EyespaceNormal;
  vec3 WorldPosition;
  vec2 UV;
};

#include common_data
#include map_uniforms


#vertex_shader

layout(location = 0)in vec2 Position;
layout(location = 1)in vec2 UV;
layout(location = 2)in uint BorderIndex;
out VsOutput vsOutput;
flat out uint borderIndex;
uniform sampler2D heightMap;
void main()
{
  mat3x4 tm = get_transform(gl_InstanceID);
  vec3 VertexPosition = mul(tm, vec4(Position.x, 0, Position.y, 1));
  vsOutput.EyespaceNormal = mul(tm, vec3(0,1,0));

  float height = texture(heightMap, mapSize.zw*VertexPosition.xz).y;
  VertexPosition.y += max(height, 94./255.) * 1.0+0.01;

  gl_Position = ViewProjection * vec4(VertexPosition, 1);
  vsOutput.WorldPosition = VertexPosition;

  vsOutput.UV = UV;
  borderIndex = BorderIndex;

}

#pixel_shader

in VsOutput vsOutput;
flat in uint borderIndex;

out vec4 FragColor;
uniform sampler2D thinBorder;
uniform sampler2D thickBorder;

void main()
{
  
  float distToCamera = length(vsOutput.WorldPosition - CameraPosition);
  float lod = log2(1+distToCamera)+1;
  vec4 c0 = borders[borderIndex].color0;
  vec4 c1 = borders[borderIndex].color1;
  vec4 result;
  if (c0.a > 0)
  {
    result = textureLod(thickBorder, vsOutput.UV * vec2(1, 2), lod);
    result.a /= 0.75;
    result.rgb = ((vsOutput.UV.y < 0.5 ? c0.rgb : c1.rgb) + result.rgb*0.7) * result.a;
  }
  else
  {
    vec2 uv = vsOutput.UV;
    uv.y = (uv.y - 0.5) * 1.5 + 0.5;
    result = textureLod(thinBorder, uv, lod-1);
  }
  if (c1.a > 0)
  {
    float pulsationTime = c1.a - Time.x;
    float t = cos(pulsationTime);
    t = t*t + 1.0;
    vec3 pulsationColor = vec3(1, 0.84, 0) * t;
    float p = 1.0 - abs(vsOutput.UV.y - 0.5);
    p = pow(p, 7);
    float pulsationAlpha = t * p;
    result.rgb = mix(result.rgb, pulsationColor, pulsationAlpha);
    result.a = max(result.a, pulsationAlpha);
  }
  FragColor = result;
}