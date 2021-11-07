#shader sky_box

#vertex_shader

layout(location = 0)in vec3 Position ;

out vec3 TexCoords;

#include common_data

void main()
{
  TexCoords = Position;
  gl_Position = (ViewProjectionSkybox * vec4(Position, 1.0)).xyww;
}

#pixel_shader
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
  FragColor = textureLod(skybox, TexCoords, 0);
}