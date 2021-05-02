#version 440
layout(location = 0)in vec3 Position ;
layout(location = 1)in vec3 Normal  ;
layout(location = 2)in vec2 TexCoord  ;

uniform mat4 ViewProjection;
uniform mat4 Model;

out vec3 EyespaceNormal;
out vec2 UV;
out vec3 WorldPosition;

uniform float Time;
void main()
{
    vec3 worldPos = Model[3].xyz;
    float freeStrength = clamp(Position.x*0.1, 0,0.1);
    float xScale = 2;
    float yScale = 5;
    float phase = Time*6 + Position.y*yScale + Position.x*xScale + worldPos.x + worldPos.z;
    vec3 pos = vec3(0, 0, cos(phase)) * freeStrength + Position;
    float dpdx = 1-sin(phase) * xScale * freeStrength;
    float dpdy = 1-sin(phase) * yScale * freeStrength;
    vec3 n = cross(vec3(1,0,dpdx), vec3(0,1, dpdy));
    EyespaceNormal = mat3(Model) * normalize(n);
    gl_Position = ViewProjection * Model * vec4(pos, 1);
    WorldPosition = (Model * vec4(pos, 1)).xyz;
    UV = TexCoord;
}