#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;
layout(location = 2)in vec2 TexCoord  ;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};
struct TexturedMaterial
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
    float sectionScale;
    vec2 uvScale;
    vec2 uvOffset;
};
struct Instance
{
    TexturedMaterial material;
    mat4 Model;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};

out vec3 EyespaceNormal;
out vec2 UV;
out vec3 WorldPosition;
flat out int instanceID;
void main()
{
    #define instance instances[gl_InstanceID]
    EyespaceNormal = mat3(instance.Model) * Normal;
    vec4 position = instance.Model * vec4(Position, 1);
    gl_Position = ViewProjection * position;
    WorldPosition = position.xyz;
    UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
    instanceID = gl_InstanceID;
}