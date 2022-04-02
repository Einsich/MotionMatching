#shader terrain_shader

#define USE_UV

struct Material
{
  #define LAMBERT
  #include material
  float atlasScale;
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
uniform sampler2D terrainMap;
uniform sampler2DArray terrainDiffuseArray;
uniform sampler2DArray terrainNormalArray;

#include lambert_lighting
#include normal_map

int terrain_type(vec3 color)
{
  const float eps = 0.1;
  const int N = 5;
  int indices[N] = int[](0, 7, 8, 4, 6);
  vec3 colors[N] = vec3[](
    vec3(0.09326, 0.20117, 0.01099),
    vec3(0.07031, 0.29102, 0.42383),
    vec3(0.00000, 0.09326, 0.00183),
    vec3(0.05273, 0.02319, 0.00562),
    vec3(0.57813, 0.67188, 0.14746)
  );
  for (int i = 0; i < N; i++)

  if (length(color - pow(colors[i], vec3(1/2.2))) < eps)
    return indices[i];
  return -1;
}

void main()
{
  vec4 heigth = texture(heightMap, vsOutput.UV);
  vec4 tex = texture(provincesMap, vsOutput.UV);
  vec4 n = texture(normalMap, vsOutput.UV);
  vec4 terrain = texture(terrainMap, vsOutput.UV);

  
  vec3 texColor = terrain.rgb;
  vec3 normalMap = texture(normalMap, vsOutput.UV).xyz * 2.0 - 1.0;
  vec3 normal = vsOutput.EyespaceNormal;
  normal = apply_normal_map(normalMap, vsOutput.EyespaceNormal, vsOutput.WorldPosition, vsOutput.UV);

  float atlasScale = material_inst.atlasScale;
  int terrainType = terrain_type(terrain.rgb);//should be in texture
  if (terrainType >= 0)
  {
    vec2 cellUV = vsOutput.UV * atlasScale;
    vec3 detailDiffuse = texture(terrainDiffuseArray, vec3(cellUV, terrainType)).rgb;
    vec3 detailNormal = texture(terrainNormalArray, vec3(cellUV, terrainType)).xyz * 2.0 - 1.0;
    normal = apply_normal_map(detailNormal, normal, vsOutput.WorldPosition, cellUV);
    texColor = detailDiffuse.rgb;
  }
  vec3 color = LightedColor(texColor, material_inst, vsOutput.WorldPosition, normal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}