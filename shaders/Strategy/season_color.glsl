
uniform sampler2DArray terrainColormapArray;

vec3 season_color(vec2 uv, float year_time)
{
  int season = int(year_time) & 3;
  int next = (season+1) & 3;
  float progress = fract(year_time);
   
  return mix(
    texture(terrainColormapArray, vec3(uv, season)).rgb,
    texture(terrainColormapArray, vec3(uv, next)).rgb,
    progress);
}