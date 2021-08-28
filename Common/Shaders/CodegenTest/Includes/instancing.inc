

struct Instance
{
    Material material;
    #if BONES
    mat4 Bones[BONES];
    #endif
    mat4 Model;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};
#ifdef VS
  #define material_inst instances[gl_InstanceID].material
#elif PS
  #define material_inst instances[instanceID].material
#endif