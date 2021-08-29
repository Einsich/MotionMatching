#shader bones

#include common_data

#include vs_output

struct Instance
{
    mat4 BoneTransform;
    vec3 Color;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};


#vertex_shader

#include vs_input

void main()
{
  #define instance instances[gl_InstanceID]

  vec4 worldPos = instance.BoneTransform * vec4(Position, 1);
  vsOutput.WorldPosition = worldPos.xyz;
  gl_Position = ViewProjection * worldPos;
  mat3 ModelNorm = mat3(instance.BoneTransform);
  ModelNorm[0] = normalize(ModelNorm[0]);
  ModelNorm[1] = normalize(ModelNorm[1]);
  ModelNorm[2] = normalize(ModelNorm[2]);
  vsOutput.EyespaceNormal = normalize(ModelNorm * Normal);
  instanceID = gl_InstanceID;

}

#pixel_shader

#include ps_input
out vec4 FragColor;
struct Material
{
  float Shininess;
  float Metallness;
};
#include lambert_lighting
void main()
{
  #define instance instances[instanceID]
  Material boneMat = {40, 0};
  vec3 color = LightedColor(instance.Color, boneMat, vsOutput, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}