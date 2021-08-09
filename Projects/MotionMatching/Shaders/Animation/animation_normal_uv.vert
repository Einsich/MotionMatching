#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;
layout(location = 2)in vec2 TexCoord  ;
layout(location = 3)in vec4 BoneWeights  ;
layout(location = 4)in uvec4 BoneIndex  ;

uniform mat4 Model;

uniform mat4 Bones[100];
layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};
out vec3 EyespaceNormal;
out vec2 UV;
out vec3 WorldPosition;
void main()
{
    mat4 BoneTransform = mat4(0);
	for (int i = 0; i < 4; i++)
		BoneTransform += Bones[BoneIndex[i]] * BoneWeights[i];
    vec4 VertexPosition = Model * BoneTransform * vec4(Position, 1);
    EyespaceNormal = mat3(Model) * mat3(BoneTransform) * Normal;
    gl_Position = ViewProjection * VertexPosition;
    WorldPosition = VertexPosition.xyz;
    UV = TexCoord;
}