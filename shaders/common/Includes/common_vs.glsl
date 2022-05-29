#define instance instances[gl_InstanceID]

mat3x4 tm = get_transform(gl_InstanceID);
#ifdef BONES

mat4 BoneTransform =
  instance.Bones[BoneIndex.x] * BoneWeights.x +
  instance.Bones[BoneIndex.y] * BoneWeights.y +
  instance.Bones[BoneIndex.z] * BoneWeights.z +
  instance.Bones[BoneIndex.w] * BoneWeights.w;

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