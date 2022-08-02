#define instance instances[gl_InstanceID]

mat3x4 tm = get_transform(gl_InstanceID);
#ifdef BONES

mat3x4 BoneTransform =
  get_bone(gl_InstanceID, BoneIndex.x) * BoneWeights.x +
  get_bone(gl_InstanceID, BoneIndex.y) * BoneWeights.y +
  get_bone(gl_InstanceID, BoneIndex.z) * BoneWeights.z +
  get_bone(gl_InstanceID, BoneIndex.w) * BoneWeights.w;

vec3 VertexPosition = mulp(tm, mulp(BoneTransform, vertex_position));
vsOutput.EyespaceNormal = mul(tm, mul(BoneTransform, Normal));
#else
vec3 VertexPosition = mul(tm, vec4(vertex_position, 1));
vsOutput.EyespaceNormal = mul(tm, Normal);
#endif
gl_Position = ViewProjection * vec4(VertexPosition, 1);
vsOutput.WorldPosition = VertexPosition;

#ifdef USE_UV
vsOutput.UV = TexCoord * instance.uvScale + instance.uvOffset;
#endif

instanceID = gl_InstanceID;