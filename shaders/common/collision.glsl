#shader collision_shader

#vertex_shader

layout(location = 0)in vec3 Position ;

struct Instance
{
  mat4 Model;
};

layout(std430, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};



#include common_data

void main()
{
  gl_Position = ViewProjection * instances[gl_InstanceID].Model * vec4(Position, 1);
}

#pixel_shader
out vec4 FragColor;

void main()
{    
  FragColor = vec4(0, 1, 0, 1);
}