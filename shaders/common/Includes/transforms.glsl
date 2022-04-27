
layout(std430, binding = 2) readonly buffer DynamicTransforms 
{
  mat3x4 dynamicTransforms[];
};

layout(std430, binding = 3) readonly buffer StaticTransforms 
{
  mat3x4 staticTransforms[];
};
uniform int dynamicTransformsOffset;

mat3x4 get_transform(int instance_id)
{
  return dynamicTransforms[dynamicTransformsOffset + instance_id];
}

vec3 mul(in mat3x4 m, vec4 p)
{
  return vec3(dot(m[0], p), dot(m[1], p), dot(m[2], p));
}

vec3 mul(in mat3x4 m, vec3 p)
{
  return vec3(dot(m[0].xyz, p), dot(m[1].xyz, p), dot(m[2].xyz, p));
}