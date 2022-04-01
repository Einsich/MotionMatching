#version 450
#define VS 1


#define USE_UV

struct Material
{
  #define LAMBERT
  #ifdef LAMBERT
  float Metallness;
  float Shininess;
#endif

#ifdef USE_UV
  vec2 uvScale;
  vec2 uvOffset;
#endif

};
struct VsOutput
{
  vec3 EyespaceNormal;
  vec3 WorldPosition;
#ifdef USE_UV
  vec2 UV;
#endif
};
struct Instance
{
    Material material;
    #if BONES
    mat4 Bones[BONES];
    #endif
    mat4 Model;
};
layout(std430, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};
#ifdef VS
  #define material_inst instances[gl_InstanceID].material
#elif PS
  #define material_inst instances[instanceID].material
#endif
layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    mat4 ViewProjectionSkybox;
    vec3 CameraPosition;
    vec3 LightDirection;
    vec3 AmbientLight;
    vec3 SunLight;
    vec4 Time;
};



layout(location = 0)in vec3 Position;
layout(location = 1)in vec3 Normal;
#ifdef USE_UV
layout(location = 2)in vec2 TexCoord;
#endif

out VsOutput vsOutput;
flat out int instanceID;
uniform sampler2D heightMap;
void main()
{
  vec3 vertex_position = Position;
  float heigth = texture(heightMap, TexCoord).y;
  vertex_position.y += heigth * 1.0;
  #define instance instances[gl_InstanceID]

#ifdef BONES
mat4 BoneTransform = mat4(0);
for (int i = 0; i < 4; i++)
  BoneTransform += instance.Bones[BoneIndex[i]] * BoneWeights[i];
vec4 VertexPosition = instance.Model * BoneTransform * vec4(vertex_position, 1);
vsOutput.EyespaceNormal = mat3(instance.Model) * mat3(BoneTransform) * Normal;
#else
vec4 VertexPosition = instance.Model * vec4(vertex_position, 1);
vsOutput.EyespaceNormal = mat3(instance.Model) * Normal;
#endif
gl_Position = ViewProjection * VertexPosition;
vsOutput.WorldPosition = VertexPosition.xyz;

#ifdef USE_UV
vsOutput.UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
#endif

instanceID = gl_InstanceID;
}

