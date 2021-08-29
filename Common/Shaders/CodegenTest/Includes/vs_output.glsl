struct VsOutput
{
  vec3 EyespaceNormal;
  vec3 WorldPosition;
  #ifdef USE_UV
  vec2 UV;
  #endif
};