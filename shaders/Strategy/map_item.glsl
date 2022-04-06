#shader map_item_shader
#define USE_UV
struct Material
{
  vec2 mapTexelSize;
  vec3 additionalColorMultiplier;
};
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input
uniform sampler2D heightMap;
void main()
{
  #define instance instances[gl_InstanceID]
  vec3 vertex_position = Position;
  mat4 tm = instance.Model;
  tm[3].y = 0;
  vec4 VertexPosition = tm * vec4(vertex_position, 1);
  vsOutput.EyespaceNormal = mat3(instance.Model) * Normal;

  float height = texture(heightMap, material_inst.mapTexelSize*VertexPosition.xz).y;
  VertexPosition.y += height * 1.0;

  gl_Position = ViewProjection * VertexPosition;
  vsOutput.WorldPosition = VertexPosition.xyz;

  vsOutput.UV = TexCoord;

}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D mainTex;
uniform sampler2D normalMap;
#include lambert_lighting
#include normal_map

void main()
{
  vec4 tex = texture(mainTex, vsOutput.UV);
  if (tex.a < 0.3)
    discard;
  
  vec3 texColor = tex.rgb * (1 + material_inst.additionalColorMultiplier);
  vec3 normalMap = texture(normalMap, vsOutput.UV).xyz * 2.0 - 1.0;
  vec3 normal = apply_normal_map(normalMap, vsOutput.EyespaceNormal, vsOutput.WorldPosition, vsOutput.UV);
  float shininess = 40;
  float metallness = 0;
  vec3 color = LightedColor(texColor, shininess, metallness, vsOutput.WorldPosition, normal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}