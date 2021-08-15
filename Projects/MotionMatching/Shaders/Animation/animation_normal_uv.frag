#version 440
in vec2 UV;
in vec3 EyespaceNormal;
in vec3 WorldPosition;
flat in int instanceID;
out vec4 FragColor;

layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    vec3 CameraPosition;
    vec3 LightDirection;
};
struct TexturedMaterial
{
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
    float Shininess;
    vec2 uvScale;
    vec2 uvOffset;
};
struct Instance
{
    TexturedMaterial material;
    mat4 Model;
    mat4 Bones[60];
};
layout(packed, binding = 1) buffer InstanceData 
{
    Instance instances[];
};
uniform sampler2D mainTex;
void main()
{
    TexturedMaterial material = instances[instanceID].material;
    vec3 N = EyespaceNormal;
    vec3 L = LightDirection;
    vec3 W = normalize(CameraPosition - WorldPosition);
    vec3 E = reflect(L, N);
    
    float df = max(0.0, dot(N, -L));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, material.Shininess);

    vec3 texColor = texture(mainTex, UV).rgb ;
    vec3 color = texColor * (material.Ambient + df * material.Diffuse) + sf * material.Specular;
    //color = Weight;
    FragColor = vec4(color, 1.0);
}