#define instance instances[gl_InstanceID]

mat3x4 tm = get_transform(gl_InstanceID);
#ifdef BONES
mat4 BoneTransform = mat4(0);
for (int i = 0; i < 4; i++)
  BoneTransform += instance.Bones[BoneIndex[i]] * BoneWeights[i];
vec3 VertexPosition = mul(tm, BoneTransform * vec4(vertex_position, 1));
vsOutput.EyespaceNormal = mul(tm, mat3(BoneTransform) * Normal);
#else
vec3 VertexPosition = mul(tm, vec4(vertex_position, 1));
vsOutput.EyespaceNormal = mul(tm, Normal);
#endif
gl_Position = ViewProjection * vec4(VertexPosition, 1);
vsOutput.WorldPosition = VertexPosition;

#ifdef USE_UV
vsOutput.UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
#endif

instanceID = gl_InstanceID;