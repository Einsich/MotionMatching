#shader section_plane

#define USE_UV

struct Material
{
  #define LAMBERT
#include material
  float sectionScale;
};

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
  float toCameraDist = length(CameraPosition - vsOutput.WorldPosition);
  vec2 section = vsOutput.WorldPosition.xz / material_inst.sectionScale;
  section = fract(section);
  section = min(section, abs(section - vec2(1,1)));
  float t = min(section.x, section.y);
  bool addSection = toCameraDist < 45 && t * material_inst.sectionScale < 0.05;
  vec3 texColor = addSection ? vec3(1, 1, 1) : texture(mainTex, vsOutput.UV).rgb;
  float shininess = material_inst.Shininess;
  float metallness = material_inst.Metallness;
  vec3 color = LightedColor(texColor, shininess, metallness, vsOutput.WorldPosition, vsOutput.EyespaceNormal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}