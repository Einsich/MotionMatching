#shader standart_shader

#include standart_material
#include vs_output
#include instancing
#include common_data

#vertex_shader

#include vs_input

void main() 
{
  #include common_vs
}

#pixel_shader

#include ps_input
out vec4 FragColor;

#include lambert_lighting
void main()
{
    vec3 color = LightedColor(vec3(1), material_inst, vsOutput, LightDirection, CameraPosition);
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
  vec3 color = LightedColor(texColor, material_inst, vsOutput, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}