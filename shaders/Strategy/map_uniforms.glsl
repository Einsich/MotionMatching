struct Border
{
  vec4 color0, color1;
};
#define MAX_BORDERS 2048

layout(std140, binding = 1) uniform MapData 
{
  vec4 mapSize;
  Border borders[];
};