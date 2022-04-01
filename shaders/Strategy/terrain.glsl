#shader terrain_shader

#define USE_UV

struct Material
{
  #define LAMBERT
  #include material

};
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input
uniform sampler2D heightMap;
void main()
{
  vec3 vertex_position = Position;
  float heigth = texture(heightMap, TexCoord).y;
  vertex_position.y += heigth * 1.0;
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D heightMap;
uniform sampler2D provincesMap;
uniform sampler2D normalMap;
uniform sampler2D treesMap;
uniform sampler2D riversMap;
uniform sampler2D terrainMap;

#include lambert_lighting
#include normal_map
void main()
{
  vec4 heigth = texture(heightMap, vsOutput.UV);
  vec4 tex = texture(provincesMap, vsOutput.UV);
  vec4 n = texture(normalMap, vsOutput.UV);
  vec4 trees = texture(treesMap, vsOutput.UV);
  vec4 rivers = texture(riversMap, vsOutput.UV);
  vec4 terrain = texture(terrainMap, vsOutput.UV);

  vec3 texColor = terrain.rgb;
  vec3 normalMap = texture(normalMap, vsOutput.UV).xyz * 2.0 - 1.0;
  vec3 normal = vsOutput.EyespaceNormal;
  normal = apply_normal_map(normalMap, vsOutput.EyespaceNormal, vsOutput.WorldPosition, vsOutput.UV);
  vec3 color = LightedColor(texColor, material_inst, vsOutput.WorldPosition, normal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}