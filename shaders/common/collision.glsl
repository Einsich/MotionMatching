#shader collision_shader

#vertex_shader

layout(location = 0)in vec3 Position ;

#include transforms



#include common_data

void main()
{
  gl_Position = ViewProjection * vec4(mul(get_transform(gl_InstanceID), vec4(Position, 1)), 1);
}

#pixel_shader
out vec4 FragColor;

void main()
{    
  FragColor = vec4(0, 1, 0, 1);
}