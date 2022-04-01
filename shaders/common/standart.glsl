#shader standart_shader

#include standart_material
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input

void main() 
{
  vec3 vertex_position = Position;
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

#include lambert_lighting
void main()
{
  vec3 color = LightedColor(vec3(1), material_inst, vsOutput.WorldPosition, vsOutput.EyespaceNormal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}

#shader standart_uv_shader

#define USE_UV

#include standart_material
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input
void main()
{
  vec3 vertex_position = Position;
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D mainTex;
#include lambert_lighting
void main()
{
  vec4 tex = texture(mainTex, vsOutput.UV);
  if (tex.a < 0.3)
    discard;
  vec3 texColor = tex.rgb;
  vec3 color = LightedColor(texColor, material_inst, vsOutput.WorldPosition, vsOutput.EyespaceNormal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}


#shader diffuse_normal_shader

#define USE_UV

#include standart_material
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input
void main()
{
  vec3 vertex_position = Position;
  #include common_vs
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
  vec3 texColor = tex.rgb;
  vec3 normalMap = texture(normalMap, vsOutput.UV).xyz * 2.0 - 1.0;
  vec3 normal = apply_normal_map(normalMap, vsOutput.EyespaceNormal, vsOutput.WorldPosition, vsOutput.UV);
  vec3 color = LightedColor(texColor, material_inst, vsOutput.WorldPosition, normal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}