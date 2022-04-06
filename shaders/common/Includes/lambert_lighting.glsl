
vec3 LightedColor(
  vec3 color,
  float shininess,
  float metallness,
  vec3 world_position,
  vec3 world_normal,
  vec3 light_dir,
  vec3 camera_pos)
{
  vec3 W = normalize(camera_pos - world_position);
  vec3 E = reflect(light_dir, world_normal);
  float df = max(0.0, dot(world_normal, -light_dir));
  float sf = max(0.0, dot(E, W));
  sf = pow(sf, shininess);
  return color * (AmbientLight + df * SunLight) + vec3(1,1,1) * sf * metallness;
}

vec3 CalculateLighting(
  vec3 vColor,
  vec3 vNormal,
  vec3 vLightDirection,
  float vAmbient,
  vec3 vLightDiffuse,
  float vLightIntensity)
{
	float NdotL = dot(vNormal, -vLightDirection);

	float vHalfLambert = NdotL * 0.5f + 0.5f;
	vHalfLambert *= vHalfLambert;

	vHalfLambert = vAmbient + (1.0f - vAmbient) * vHalfLambert;
	return saturate(vHalfLambert * vColor * vLightDiffuse * vLightIntensity);
}
