layout(std140, binding = 0) uniform Commondata 
{
    mat4 ViewProjection;
    mat4 ViewProjectionSkybox;
    vec3 CameraPosition;
    vec3 LightDirection;
    vec3 AmbientLight;
    vec3 SunLight;
    vec4 Time;
};
#include common_functions