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
  mat3x4 tm = get_transform(gl_InstanceID);
  #define instance instances[gl_InstanceID]
  vec3 worldPos = vec3(tm[0].w, tm[1].w, tm[2].w);
  float freeStrength = clamp(Position.x*0.1, 0.0,0.1);
  float xScale = 2;
  float yScale = 5;
  float phase = Time.x*6 + Position.y*yScale + Position.x*xScale + worldPos.x + worldPos.z;
  vec3 pos = vec3(0, 0, cos(phase)) * freeStrength + Position;
  float dpdx = 1-sin(phase) * xScale * freeStrength;
  float dpdy = 1-sin(phase) * yScale * freeStrength;
  vec3 n = cross(vec3(1,0,dpdx), vec3(0,1, dpdy));
  vsOutput.EyespaceNormal = mul(tm, Normal);
  pos = mul(tm, vec4(pos, 1));
  gl_Position = ViewProjection * vec4(pos, 1);
  vsOutput.WorldPosition = pos;
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
  float shininess = material_inst.Shininess;
  float metallness = material_inst.Metallness;
  vec3 color = LightedColor(texColor, shininess, metallness, vsOutput.WorldPosition, vsOutput.EyespaceNormal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}