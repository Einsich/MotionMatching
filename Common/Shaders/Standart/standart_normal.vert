#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};
uniform mat4 Model;
struct VsOutput
{
    vec3 EyespaceNormal;
    vec3 WorldPosition;
};
out VsOutput vsOutput;
void main()
{
    vsOutput.EyespaceNormal = mat3(Model) * Normal;
    gl_Position = ViewProjection * Model * vec4(Position, 1);
    vsOutput.WorldPosition = (Model * vec4(Position, 1)).xyz;
}