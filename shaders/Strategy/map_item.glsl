#shader map_item_shader
#define USE_UV
struct Material
{
  vec3 additionalColorMultiplier;
};
#include vs_output
#include instancing
#include common_data
#include map_uniforms

#vertex_shader

#include vs_input
uniform sampler2D heightMap;
void main()
{
  #define instance instances[gl_InstanceID]
  vec3 vertex_position = Position;

  mat3x4 tm = get_transform(gl_InstanceID);
  vec2 mapPos = vec2(tm[0].w, tm[2].w);
  float height = texture(heightMap, mapSize.zw*mapPos).y;
  tm[1].w = height * 1.0;
  vec3 VertexPosition = mul(tm, vec4(vertex_position, 1));
  vsOutput.EyespaceNormal = mul(tm, Normal);


  gl_Position = ViewProjection * vec4(VertexPosition, 1);
  vsOutput.WorldPosition = VertexPosition;

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