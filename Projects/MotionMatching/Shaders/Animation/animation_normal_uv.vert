#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;
layout(location = 2)in vec2 TexCoord  ;
layout(location = 3)in vec4 BoneWeights  ;
layout(location = 4)in uvec4 BoneIndex  ;

layout(std140, binding = 0) readonly uniform Commondata 
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
    vec2 uvScale;
    vec2 uvOffset;
};
struct Instance
{
    TexturedMaterial material;
    mat4 Model;
    mat4 Bones[60];
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
    
    mat4 BoneTransform = mat4(0);
	for (int i = 0; i < 4; i++)
		BoneTransform += instance.Bones[BoneIndex[i]] * BoneWeights[i];
    vec4 VertexPosition = instance.Model * BoneTransform * vec4(Position, 1);
    EyespaceNormal = mat3(instance.Model) * mat3(BoneTransform) * Normal;
    gl_Position = ViewProjection * VertexPosition;
    WorldPosition = VertexPosition.xyz;
    UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
    instanceID = gl_InstanceID;
}