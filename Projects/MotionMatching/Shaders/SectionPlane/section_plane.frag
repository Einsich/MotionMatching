#version 440
in vec2 UV;
in vec3 EyespaceNormal;
in vec3 WorldPosition;
out vec4 FragColor;

uniform vec3 CameraPosition;

uniform vec3 LightDirection;

uniform vec3 Ambient;
uniform vec3 Diffuse;
uniform vec3 Specular;
uniform float Shininess;

uniform vec2 uvScale;
uniform float sectionScale;
uniform vec2 uvOffset;
uniform sampler2D mainTex;
void main()
{
    vec3 N = normalize(EyespaceNormal);
    vec3 L = LightDirection;
    vec3 toCamera = CameraPosition - WorldPosition;
    float toCameraDist = length(toCamera);
    vec3 W = toCamera / toCameraDist;
    vec3 E = reflect(L, N);
    
    float df = max(0.0, dot(N, -L));
    float sf = max(0.0, dot(E, W));
    sf = pow(sf, Shininess);

    vec2 uv = (UV + uvOffset) * uvScale;
    vec2 section = WorldPosition.xz / sectionScale;
    section = fract(section);
    section = min(section, abs(section - vec2(1,1)));
    float t = min(section.x, section.y);
    bool addSection = toCameraDist < 45 && t * sectionScale < 0.03;
    vec3 texColor = addSection ? vec3(1,1,1) : texture(mainTex, uv).rgb;
    vec3 color = texColor * (Ambient + df * Diffuse) + sf * Specular;

    FragColor = vec4(color, 1.0);
}