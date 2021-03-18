#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;
layout(location = 2)in vec2 TexCoord  ;

uniform mat4 ViewProjection;
uniform mat4 Model;

out vec3 EyespaceNormal;
out vec2 UV;
out vec3 WorldPosition;
void main()
{
    EyespaceNormal = mat3(Model) * Normal;
    gl_Position = ViewProjection * Model * vec4(Position, 1);
    WorldPosition = (Model * vec4(Position, 1)).xyz;
    UV = TexCoord;
}