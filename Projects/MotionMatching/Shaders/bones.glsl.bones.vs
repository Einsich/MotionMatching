#version 440
#define VS 1






struct Instance
{
    mat4 BoneTransform;
    vec3 Color;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};






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

