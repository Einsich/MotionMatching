#version 440
in vec3 EyespaceNormal;
in vec3 WorldPosition;
in vec3 Color;
flat in int instanceID;
out vec4 FragColor;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};


struct Instance
{
    mat4 BoneTransform;
    vec3 Color;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};
void main()
{
    #define instance instances[instanceID]

    vec3 Diffuse = instance.Color;
    vec3 Ambient = Diffuse * 0.2;
    vec3 Specular = vec3(1,1,1);
    vec3 N = EyespaceNormal;
    vec3 L = LightDirection;
    vec3 W = normalize(CameraPosition - WorldPosition);
    vec3 E = reflect(L, N);
    
    float df = max(0.0, dot(N, -L));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, 50);
    vec3 color = Ambient + df * Diffuse + sf * Specular;

    FragColor = vec4(color, 1.0);
}