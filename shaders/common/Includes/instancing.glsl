

layout(std430, binding = 1) readonly buffer InstanceData 
{
  Material instances[];
};
#ifdef VS
  #define material_inst instances[gl_InstanceID]
#elif PS
  #define material_inst instances[instanceID]
#endif

#include transforms