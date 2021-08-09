#version 440
in vec3 EyespaceNormal;
in vec3 WorldPosition;
in vec3 Color;
out vec4 FragColor;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};

uniform vec3 Specular;
uniform float Shininess;
void main()
{
    vec3 Diffuse = Color;
    vec3 Ambient = Diffuse * 0.2;
    vec3 N = normalize(EyespaceNormal);
    vec3 L = LightDirection;
    vec3 W = normalize(CameraPosition - WorldPosition);
    vec3 E = reflect(L, N);
    
    float df = max(0.0, dot(N, -L));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, Shininess);
    vec3 color = Ambient + df * Diffuse + sf * Specular;

    FragColor = vec4(color, 1.0);
}