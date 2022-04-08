#shader terrain_shader

#variant MAP_MODE 0,1;

#define PHYSICAL_MODE 0
#define POLITICAL_MODE 1

#define USE_UV

struct Material
{
  #include material
  float atlasScale;
  float colorMapWeigth;
  float seasonTime;
  ivec2 mapSize;
  vec2 texelSize;
#if MAP_MODE != PHYSICAL_MODE
#define MAX_COLORS 255
#define MAX_PROVINCES 1024
  vec3  stateColor[MAX_COLORS];
  uvec2 provincesInfo[MAX_PROVINCES];
  //state, occupated by, 
#endif
};
struct VsOutput
{
  vec3 EyespaceNormal;
  vec3 WorldPosition;
#ifdef USE_UV
  vec2 UV;
#endif
  float underWater;
};

#include instancing
#include common_data

#vertex_shader

#include vs_input
uniform sampler2D heightMap;
void main()
{
  vec3 vertex_position = Position;
  float height = texture(heightMap, TexCoord).y;
  vertex_position.y += height * 1.0;
  #include common_vs
  vsOutput.underWater = height - (94.0 / 255.0);
}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D heightMap;
uniform usampler2D provincesMap;
uniform sampler2D normalMap;
uniform usampler2D terrainMap;
uniform sampler2DArray terrainDiffuseArray;
uniform sampler2DArray terrainNormalArray;

#include lambert_lighting
#include normal_map
#include season_color

uint terrain_type(vec2 physUV, vec2 detailedUV, ivec2 mapSize, out vec4 finalColor, out vec3 finalNormal)
{
  const int N = 4;
  vec4 color[N];
  vec3 normal[N];
  uvec4 terrain = textureGather(terrainMap, physUV, 0);
  for (int i = 0; i < N; i++)
  {
    uint type = terrain[i];
    vec3 atlasUV = vec3(detailedUV, type);
  #if MAP_MODE == PHYSICAL_MODE
    color[i] = vec4(texture(terrainDiffuseArray, atlasUV).rgb, 1);
  #endif
    normal[i] = texture(terrainNormalArray, atlasUV).xyz * 2.0 - 1.0;
  }
  vec2 uv = physUV * mapSize - 0.5;
  vec2 a = fract(uv);
  vec2 b = 1 - a;
  float w[N] = float[](b.x*a.y, a.x*a.y, a.x*b.y, b.x*b.y);

  for (int i = 0; i < N; i++)
  {
  #if MAP_MODE == PHYSICAL_MODE
    finalColor += color[i] * w[i];
  #endif
    finalNormal += normal[i] * w[i];
  }
  finalNormal = normalize(finalNormal);
  return textureLod(terrainMap, physUV, 0).x;
}

#if MAP_MODE != PHYSICAL_MODE
vec3 political_map(vec2 physUV, ivec2 mapSize, vec3 original_color)
{
  const int N = 4;
  uvec4 indexes = textureGather(provincesMap, physUV, 0);
  vec2 uv = physUV * mapSize - 0.5;
  vec2 a = fract(uv);
  vec2 b = 1 - a;
  float w[N] = float[](b.x*a.y, a.x*a.y, a.x*b.y, b.x*b.y);
  vec3 finalColor = vec3(0,0,0);

  for (int i = 0; i < N; i++)
  {
    uvec2 provInfo = material_inst.provincesInfo[indexes[i]];
    vec3 c = provInfo.x < MAX_COLORS ? material_inst.stateColor[provInfo.x] : original_color;
    finalColor += c * w[i];
  }
  return finalColor;
}
#endif

vec3 GetOverlay(vec3 vColor,vec3 vOverlay, float vOverlayPercent )
{
	vec3 res;
	res.r = vOverlay.r < .5 ? (2 * vOverlay.r * vColor.r) : (1 - 2 * (1 - vOverlay.r) * (1 - vColor.r));
	res.g = vOverlay.g < .5 ? (2 * vOverlay.g * vColor.g) : (1 - 2 * (1 - vOverlay.g) * (1 - vColor.g));
	res.b = vOverlay.b < .5 ? (2 * vOverlay.b * vColor.b) : (1 - 2 * (1 - vOverlay.b) * (1 - vColor.b));

	return mix(vColor, res, vOverlayPercent);
}

void main()
{

  float yearTime = Time.x * material_inst.seasonTime;
  vec3 colorMap = season_color(vsOutput.UV, yearTime);


  if (vsOutput.underWater < -0.02)
  {
    FragColor = vec4(colorMap, 1);
    return;
  }
#if MAP_MODE == POLITICAL_MODE
  colorMap = political_map(vsOutput.UV, material_inst.mapSize, colorMap).rgb;
#endif
  vec3 texColor = colorMap;
  vec3 normalMap = texture(normalMap, vsOutput.UV).xyz * 2.0 - 1.0;
  vec3 normal = vsOutput.EyespaceNormal;
  normal = apply_normal_map(normalMap, vsOutput.EyespaceNormal, vsOutput.WorldPosition, vsOutput.UV);

  vec4 detailColor = vec4(0,0,0,0);
  vec3 detailNormal = vec3(0,0,1);
  vec2 detailedUV = vsOutput.UV * material_inst.atlasScale;
  uint terrainType = terrain_type(vsOutput.UV, detailedUV, material_inst.mapSize, detailColor, detailNormal);

  normal = apply_normal_map(detailNormal, normal, vsOutput.WorldPosition, detailedUV);
#if MAP_MODE == PHYSICAL_MODE
  texColor = GetOverlay(detailColor.rgb, texColor, material_inst.colorMapWeigth);
#endif

  vec3 color = CalculateLighting(texColor, normal, LightDirection, 0, vec3(1,1,1), 1.5);

  FragColor = vec4(color, 1.0);
}