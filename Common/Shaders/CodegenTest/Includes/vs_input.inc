
layout(location = 0)in vec3 Position;
layout(location = 1)in vec3 Normal;
#ifdef USE_UV
layout(location = 2)in vec2 TexCoord;
#endif


out VsOutput vsOutput;
flat out int instanceID;