#shader border_shader

struct Instance
{
  mat4 Model;
};
layout(std430, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};

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
  mat4 tm = instances[gl_InstanceID].Model;
  vec4 VertexPosition = tm * vec4(Position.x, 0, Position.y, 1);
  vsOutput.EyespaceNormal = mat3(tm) * vec3(0,1,0);

  float height = texture(heightMap, mapSize.zw*VertexPosition.xz).y;
  VertexPosition.y += max(height, 94./255.) * 1.0+0.01;

  gl_Position = ViewProjection * VertexPosition;
  vsOutput.WorldPosition = VertexPosition.xyz;

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
  if (c0.a > 0)
  {
    vec4 tex = textureLod(thickBorder, vsOutput.UV * vec2(1, 2), lod);
    tex.a /= 0.75;
    tex.rgb = ((vsOutput.UV.y < 0.5 ? c0.rgb : c1.rgb) + tex.rgb*0.7) * tex.a;
    FragColor = tex;
  }
  else
  {
    vec2 uv = vsOutput.UV;
    uv.y = (uv.y - 0.5) * 1.5 + 0.5;
    FragColor = textureLod(thinBorder, uv, lod-1);
  }
}