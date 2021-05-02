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
uniform vec2 uvOffset;
uniform sampler2D mainTex;

void main()
{
    vec3 N = normalize(EyespaceNormal);
    vec3 L = LightDirection;
    vec3 W = normalize(CameraPosition - WorldPosition);
    vec3 E = reflect(L, N);
    vec3 En = reflect(L, -N);
    
    float df = max(0.0, max(dot(N, -L), dot(-N, -L)));
    float sf = max(0.0, min(dot(E, W), dot(E, W)));
    sf = pow(sf, Shininess);

    vec2 uv = (UV + uvOffset) * uvScale;
    vec3 texColor = texture(mainTex, uv).rgb ;
    vec3 color = texColor * (Ambient + df * Diffuse) + sf * Specular;

    FragColor = vec4(color, 1.0);
}