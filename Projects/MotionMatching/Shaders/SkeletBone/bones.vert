#version 440
layout(location = 0)in vec3 Position;
layout(location = 1)in vec3 Normal;

#define ARROW_DRAWCALL_LIMIT 150

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};
uniform mat4 BoneTransforms[ARROW_DRAWCALL_LIMIT];
uniform vec3 Colors[ARROW_DRAWCALL_LIMIT];

out vec3 EyespaceNormal;
out vec3 WorldPosition;
out vec3 Color;
void main()
{
    mat4 Model = BoneTransforms[gl_InstanceID];
    vec4 worldPos = Model * vec4(Position, 1);
    WorldPosition = worldPos.xyz;
    gl_Position = ViewProjection * worldPos;
    mat3 ModelNorm = mat3(Model);
    ModelNorm[0] = normalize(ModelNorm[0]);
    ModelNorm[1] = normalize(ModelNorm[1]);
    ModelNorm[2] = normalize(ModelNorm[2]);
    EyespaceNormal = ModelNorm * Normal;
    Color = Colors[gl_InstanceID];
}