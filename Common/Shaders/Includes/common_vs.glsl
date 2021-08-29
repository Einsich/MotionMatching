#define instance instances[gl_InstanceID]

#ifdef BONES
mat4 BoneTransform = mat4(0);
for (int i = 0; i < 4; i++)
  BoneTransform += instance.Bones[BoneIndex[i]] * BoneWeights[i];
vec4 VertexPosition = instance.Model * BoneTransform * vec4(Position, 1);
vsOutput.EyespaceNormal = mat3(instance.Model) * mat3(BoneTransform) * Normal;
#else
vec4 VertexPosition = instance.Model * vec4(Position, 1);
vsOutput.EyespaceNormal = mat3(instance.Model) * Normal;
#endif
gl_Position = ViewProjection * VertexPosition;
vsOutput.WorldPosition = VertexPosition.xyz;

#ifdef USE_UV
vsOutput.UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
#endif

instanceID = gl_InstanceID;