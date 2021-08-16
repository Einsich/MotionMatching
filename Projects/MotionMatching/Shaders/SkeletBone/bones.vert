#version 440
layout(location = 0)in vec3 Position;
layout(location = 1)in vec3 Normal;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};

struct Instance
{
    mat4 BoneTransform;
    vec3 Color;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};

out vec3 EyespaceNormal;
out vec3 WorldPosition;
out vec3 Color;
flat out int instanceID;
void main()
{
    #define instance instances[gl_InstanceID]

    vec4 worldPos = instance.BoneTransform * vec4(Position, 1);
    WorldPosition = worldPos.xyz;
    gl_Position = ViewProjection * worldPos;
    mat3 ModelNorm = mat3(instance.BoneTransform);
    ModelNorm[0] = normalize(ModelNorm[0]);
    ModelNorm[1] = normalize(ModelNorm[1]);
    ModelNorm[2] = normalize(ModelNorm[2]);
    EyespaceNormal = normalize(ModelNorm * Normal);
    Color = instance.Color;
    instanceID = gl_InstanceID;
}