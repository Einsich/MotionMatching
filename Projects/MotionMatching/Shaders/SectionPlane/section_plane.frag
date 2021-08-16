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
    float sectionScale;
    vec2 uvScale;
    vec2 uvOffset;
};
struct Instance
{
    TexturedMaterial material;
    mat4 Model;
};
layout(packed, binding = 1) readonly buffer InstanceData 
{
    Instance instances[];
};

uniform sampler2D mainTex;
void main()
{
    #define material instances[instanceID].material

    vec3 N = normalize(EyespaceNormal);
    vec3 L = LightDirection;
    vec3 toCamera = CameraPosition - WorldPosition;
    float toCameraDist = length(toCamera);
    vec3 W = toCamera / toCameraDist;
    vec3 E = reflect(L, N);
    
    float df = max(0.0, dot(N, -L));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, material.Shininess);

    vec2 section = WorldPosition.xz / material.sectionScale;
    section = fract(section);
    section = min(section, abs(section - vec2(1,1)));
    float t = min(section.x, section.y);
    bool addSection = toCameraDist < 45 && t * material.sectionScale < 0.05;
    vec3 texColor = addSection ? vec3(1,1,1) : texture(mainTex, UV).rgb;
    vec3 color = texColor * (material.Ambient + df * material.Diffuse) + sf * material.Specular;

    FragColor = vec4(color, 1.0);
}