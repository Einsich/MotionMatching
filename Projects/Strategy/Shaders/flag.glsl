#shader flag

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

void main()
{
  #define instance instances[gl_InstanceID]
  vec3 worldPos = instance.Model[3].xyz;
  float freeStrength = clamp(Position.x*0.1, 0.0,0.1);
  float xScale = 2;
  float yScale = 5;
  float phase = Time*6 + Position.y*yScale + Position.x*xScale + worldPos.x + worldPos.z;
  vec3 pos = vec3(0, 0, cos(phase)) * freeStrength + Position;
  float dpdx = 1-sin(phase) * xScale * freeStrength;
  float dpdy = 1-sin(phase) * yScale * freeStrength;
  vec3 n = cross(vec3(1,0,dpdx), vec3(0,1, dpdy));
  vsOutput.EyespaceNormal = mat3(instance.Model) * normalize(n);
  gl_Position = ViewProjection * instance.Model * vec4(pos, 1);
  vsOutput.WorldPosition = (instance.Model * vec4(pos, 1)).xyz;
  vsOutput.UV = TexCoord;
  instanceID = gl_InstanceID;
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