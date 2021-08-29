

#define instance instances[gl_InstanceID]

vsOutput.EyespaceNormal = mat3(instance.Model) * Normal;
vec4 position = instance.Model * vec4(Position, 1);
gl_Position = ViewProjection * position;
vsOutput.WorldPosition = position.xyz;

#ifdef USE_UV
vsOutput.UV = TexCoord * instance.material.uvScale + instance.material.uvOffset;
#endif

instanceID = gl_InstanceID;