#shader bones

#include common_data

#include vs_output

struct Instance
{
    mat4 BoneTransform;
    vec4 Color;
};
layout(std430, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};


#vertex_shader

#include vs_input

void main()
{
  instanceID = gl_InstanceID;
  vec4 worldPos = instances[instanceID].BoneTransform * vec4(Position, 1);
  vsOutput.WorldPosition = worldPos.xyz;
  gl_Position = ViewProjection * worldPos;
  mat3 ModelNorm = mat3(instances[instanceID].BoneTransform);
  ModelNorm[0] = normalize(ModelNorm[0]);
  ModelNorm[1] = normalize(ModelNorm[1]);
  ModelNorm[2] = normalize(ModelNorm[2]);
  vsOutput.EyespaceNormal = normalize(ModelNorm * Normal);

}

#pixel_shader

#include ps_input
out vec4 FragColor;

#include lambert_lighting
void main()
{
  float shininess = 40;
  float metallness = 0;
  vec3 color = LightedColor(instances[instanceID].Color.rgb, shininess, metallness, 
    vsOutput.WorldPosition, vsOutput.EyespaceNormal, LightDirection, CameraPosition);
  FragColor = vec4(color, 1.0);
}