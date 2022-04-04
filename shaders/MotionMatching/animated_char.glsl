#shader animated_character

#define USE_UV

struct Material
{
  vec3 AdditionalColor;
  #define LAMBERT
  #include material
};
#include vs_output

#define BONES 60
#include instancing
#include common_data

#vertex_shader

#include vs_input
layout(location = 3)in vec4 BoneWeights  ;
layout(location = 4)in uvec4 BoneIndex  ;

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
  float shininess = material_inst.Shininess;
  float metallness = material_inst.Metallness;
  vec3 color = texture(mainTex, vsOutput.UV).rgb + material_inst.AdditionalColor;
  color = LightedColor(color, shininess, metallness, vsOutput, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}