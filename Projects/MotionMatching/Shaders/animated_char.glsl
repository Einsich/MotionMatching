#shader animated_character

#define USE_UV

#include standart_material
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
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

uniform sampler2D mainTex;
#include lambert_lighting
void main()
{
  vec3 texColor = texture(mainTex, vsOutput.UV).rgb;
  vec3 color = LightedColor(texColor, material_inst, vsOutput, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}